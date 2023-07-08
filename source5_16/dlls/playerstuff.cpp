/*****************************************************\
*	Auto-OP Shodan
*
*	File: playerstuff.cpp
*	Description: Player think functions..
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	This program is free software; you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation; either version 2 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
************************************/

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "shake.h"
#include "autoop.h"
#include "commands.h"
#include "customweapons.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;

extern int team1_admins;
extern int team2_admins;
extern int team3_admins;
extern int team4_admins;

extern int message_StatusText;
extern int message_StatusValue;

int TFXPlayerPreThink(edict_t *pEntity)
{
	if(FStrEq(STRING(pEntity->v.netname),"") || (!FStrEq(STRING(pEntity->v.classname),"player")))
		return 0;

	//Prevent gagged players changing name
	if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 1)
	{
		if(strcmp(STRING(pEntity->v.netname),PlayerInfo[ENTINDEX(pEntity)].GaggedName) != 0)
		{
			char *infobuffer=g_engfuncs.pfnGetInfoKeyBuffer(pEntity);
			g_engfuncs.pfnSetClientKeyValue(ENTINDEX(pEntity), infobuffer, "name", PlayerInfo[ENTINDEX(pEntity)].GaggedName);
		}
	}

	//If frags jumped like 10 or more, then its not a kill, more likely a cap.
	if(pEntity->v.frags >= (PlayerInfo[ENTINDEX(pEntity)].LastKnownFrags + 10))
	{
		if(UTIL_CountPlayers() < 15)
			PlayerInfo[ENTINDEX(pEntity)].NeedFrags = pEntity->v.frags + (PlayerInfo[ENTINDEX(pEntity)].NeedFrags - PlayerInfo[ENTINDEX(pEntity)].LastKnownFrags);
	}

	PlayerInfo[ENTINDEX(pEntity)].LastKnownFrags = pEntity->v.frags;

	if(pEntity->v.playerclass == CLASS_ENGINEER)
		if(pEntity->v.button & IN_ATTACK)
			UTIL_PlayerRepairTracelines(pEntity);

	if(!(pEntity->v.button & IN_ATTACK))
		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_SCOUTSPEED)
			pEntity->v.maxspeed = 400;

	if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_LOWGRAV)
		pEntity->v.gravity = 0.15;

	//Status reset check
	if(PlayerInfo[ENTINDEX(pEntity)].StatusReset > 0)
	{
		if(PlayerInfo[ENTINDEX(pEntity)].StatusReset <= gpGlobals->time)
		{
			//Reset status bar
			MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, pEntity );
				WRITE_BYTE (1);
				WRITE_SHORT(0);
			MESSAGE_END();

			MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, pEntity );
				WRITE_BYTE (2);
				WRITE_SHORT(0);
			MESSAGE_END();

			MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, pEntity );
				WRITE_BYTE (3);
				WRITE_SHORT(0);
			MESSAGE_END();

			//Put back the text that it should be
			MESSAGE_BEGIN( MSG_ONE, message_StatusText, NULL, pEntity );
				WRITE_BYTE (1);
				WRITE_STRING( PlayerInfo[ENTINDEX(pEntity)].LastStatusText );
			MESSAGE_END();

			PlayerInfo[ENTINDEX(pEntity)].StatusReset = 0.0;
		}
	}

	//As a spy with a sniper rifle does not lose his disguise
	//lets help him out :)
	if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_sniper.mdl"))
	{
		if(PlayerInfo[ENTINDEX(pEntity)].RemoveDisguise < gpGlobals->time)
		{
			if(pEntity->v.button & IN_ATTACK)
			{
				PlayerInfo[ENTINDEX(pEntity)].RemoveDisguise = gpGlobals->time + 5;
				//Remove disguise
				CLIENT_COMMAND ( pEntity, UTIL_VarArgs("disguise_friendly 8\n") );
			}
		}
	}

	//Admin team checks
	if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin == 0)
	{
		if((pEntity->v.team == 1) && (team1_admins == 1))
		{
			CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 2\n") );
		}

		if((pEntity->v.team == 2) && (team2_admins == 1))
		{
			CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 1\n") );
		}

		if((pEntity->v.team == 3) && (team3_admins == 1))
		{
			CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 1\n") );
		}

		if((pEntity->v.team == 4) && (team4_admins == 1))
		{
			CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 1\n") );
		}
	}

	
	if(Features.CustomWeapons == 1)
	{
		//CUSTOM WEAPON FIRING DETECTION CODE!!!
		if(pEntity->v.button & IN_ATTACK)
		{
			if(FStrEq(STRING(pEntity->v.viewmodel),"models/autoop/v_tfc_energygun.mdl"))
			{
				//pEntity->v.button = 0;
				pEntity->v.button &= ~IN_ATTACK;

				if(PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[ENERGYGUN_ID] < gpGlobals->time)
				{
					FireEnergy(pEntity);	
				}
			}

			if(FStrEq(STRING(pEntity->v.viewmodel),"models/autoop/v_tfc_tazar.mdl"))
			{
				//pEntity->v.button = 0;
				pEntity->v.button &= ~IN_ATTACK;

				if(PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[TAZAR_ID] < gpGlobals->time)
				{
					FireTazar(pEntity);	
				}
			}

			if(FStrEq(STRING(pEntity->v.viewmodel),"models/autoop/v_tfc_ablaster.mdl"))
			{
				//pEntity->v.button = 0;
				pEntity->v.button &= ~IN_ATTACK;

				if(PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[ANTHRAX_BLASTER_ID] < gpGlobals->time)
				{
					FireBlaster(pEntity);	
				}
			}

			if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tripmine.mdl"))
			{
				//pEntity->v.button = 0;
				pEntity->v.button &= ~IN_ATTACK;

				if(PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[TRIPMINE_ID] < gpGlobals->time)
				{
					PlaceTripMine(pEntity);	
				}
			}
		}

		CustomGunModelSwitch(pEntity);
	}

	if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.Warnings >= 4)
	{
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\n\n\n******************************\n"));
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "You have been banned for 24 hours.\n"));
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "By Server(AutoBan), Reason: Warnings from admins exceeded 4.\n"));
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\nContact rob@planetfortress.com If you believe this to be in error.\n"));
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "Make sure you quote the admin and reason if you expect help.\n"));
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "******************************\n"));

		PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.Warnings = 0;

		char BanCommand[256];
		_snprintf(BanCommand,256,"banid 1440 %s\n",GETPLAYERAUTHID(pEntity));
		SERVER_COMMAND(BanCommand);
		SERVER_COMMAND("writeid\n");

		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("disconnect\n") );
	}


	int Index = ENTINDEX(pEntity);

	//Grapple Checks
	if(PlayerInfo[Index].GrapStat == 1)
	{
		MoveToDest(PlayerInfo[Index].GrapOrig.x,PlayerInfo[Index].GrapOrig.y,PlayerInfo[Index].GrapOrig.z,pEntity->v.maxspeed,INDEXENT(Index));
	}

	//CHECK TO SEE IF PLAYER IS HANGING
	if(PlayerInfo[Index].GrapStat == 2)
	{
		PlayerMovedtoDest(INDEXENT(Index));
	}

	//NAME CHECKS
	if(stristr(STRING(pEntity->v.netname),"") != 0) //Invalid char check
	{
		CLIENT_COMMAND(pEntity,"name Player\n");
	}

	//Players may not take the name of the responsebot
	if((FStrEq(STRING(pEntity->v.netname),BOT_NAME) || FStrEq(STRING(pEntity->v.netname),BOT_NAME2)))
	{
		CLIENT_COMMAND(pEntity,"name Player\n");
	}

	//END OF NAME CHECKS

	if(PlayerInfo[Index].PathBuild.Drawingpath > 0 && (gpGlobals->time >= PlayerInfo[Index].PathBuild.NextDraw))
	{
		DrawPath(pEntity);
	}
	
	if(pEntity->v.health <= 0)
	{
		PlayerInfo[Index].IsUsingTurret = 0;
		PlayerInfo[Index].FBAmmo = 2;
		PlayerInfo[Index].ProxMineAmmo = PROX_MINE_AMMO;
		PlayerInfo[Index].TripMineAmmo = TRIPMINE_AMMO;
		PlayerInfo[Index].C4Ammo = C4_AMMO;
		PlayerInfo[Index].SnarkAmmo = SNARK_AMMO;

		PlayerInfo[Index].AbilityOverride &= ~ABILITY_SCOUTSPEED;
	
		PlayerInfo[Index].AlreadyHealed = 0;
		PlayerInfo[Index].SwitchingToCustomGun = 0;
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("tfc_newmodels 0\n") );
		
		//Grapple Check
		if(PlayerInfo[Index].GrapStat > 0)
		{
			//If they died grappling, kill the grapple.
			PlayerDrops(pEntity,1);
		}
	}

	if(pEntity->v.deadflag != DEAD_NO) //Spy feign
	{
		if(PlayerInfo[Index].GrapStat > 0)
		{
			//If they feign, kill the grapple.
			PlayerDrops(pEntity,1);
		}
	}


	if(PlayerInfo[Index].MovingEnt == 1)
	{
		QuickMove(pEntity);
	}

	if(PlayerInfo[Index].Selected.LockX == 1)
	{
		QuickMoveX(pEntity);
	}

	if(PlayerInfo[Index].Selected.LockY == 1)
	{
		QuickMoveY(pEntity);
	}

	if(PlayerInfo[Index].Selected.LockZ == 1)
	{
		QuickMoveZ(pEntity);
	}

	if(PlayerInfo[Index].Selected.IsPulling == 1)
	{
		PullThink(pEntity);
	}

	if(Features.HWSword == 1)
	{
		//DualBar Stuff
		if(pEntity->v.playerclass == CLASS_HWGUY)
		{
			if(pEntity->v.viewmodel != 0)
			{
				if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_crowbar.mdl"))
				{
					pEntity->v.viewmodel = MAKE_STRING("models/autoop/v_tfc_dualbar.mdl");
					pEntity->v.weaponmodel = MAKE_STRING("models/autoop/p_tfc_dualbar.mdl");
				}
			}

			if(pEntity->v.button & IN_ATTACK)
			{
				if(FStrEq(STRING(pEntity->v.viewmodel),"models/autoop/v_tfc_dualbar.mdl"))
				{
					SwingDualBar(pEntity);
				}
			}
		}
	}

	if(Features.Vampirism == 1)
	{
		if(pEntity->v.health > 0)
			if(PlayerInfo[ENTINDEX(pEntity)].IsSucking == 1)
				SuckBlood(pEntity);
	}

	//Anti Spawn Camper stuff (makes player invuln 6 secs after spawn)
	if(pEntity->v.health <= 0)
		PlayerInfo[ENTINDEX(pEntity)].HasDied = 1;

	if(pEntity->v.health > 0 && PlayerInfo[ENTINDEX(pEntity)].HasDied == 1)
	{
		PlayerInfo[ENTINDEX(pEntity)].HasDied = 0;
		PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon = StartingWeapon(pEntity);

		if(PlayerInfo[Index].AbilityOverride & ABILITY_LOWGRAV2)
			PlayerInfo[Index].AbilityOverride &= ~ABILITY_LOWGRAV2;
		else
			PlayerInfo[Index].AbilityOverride &= ~ABILITY_LOWGRAV;
		
		//Custom spawn point
		if(PlayerInfo[ENTINDEX(pEntity)].CustomSpawnPoint != Vector(0,0,0))
			pEntity->v.origin = PlayerInfo[ENTINDEX(pEntity)].CustomSpawnPoint;

		//Make player invuln for a few seconds
		if(Features.AntiSpawnCamp == 1)
			AntiCamp(pEntity);

		//Give grenades if they should
		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_5GREN1)
			PatchpvPrivateInt(pEntity,56,5);

		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_5GREN2)
			PatchpvPrivateInt(pEntity,60,5);

		//Give weapons if we should
		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_SPAWNRPG)
		{
			PlayerInfo[Index].AbilityOverride &= ~ABILITY_SPAWNRPG;
			if(pEntity->v.playerclass != CLASS_SOLDIER)
				TFXGiveWeapon(1,pEntity);
		}

		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_SPAWNCHAINGUN)
		{
			PlayerInfo[Index].AbilityOverride &= ~ABILITY_SPAWNCHAINGUN;
			if(pEntity->v.playerclass != CLASS_HWGUY)
				TFXGiveWeapon(2,pEntity);
		}

		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_SPAWNSNIPERRIFLE)
		{
			PlayerInfo[Index].AbilityOverride &= ~ABILITY_SPAWNSNIPERRIFLE;
			if(pEntity->v.playerclass != CLASS_SNIPER)
				TFXGiveWeapon(12,pEntity);
		}

		if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_SPAWNSHOTGUN)
		{
			PlayerInfo[Index].AbilityOverride &= ~ABILITY_SPAWNSHOTGUN;
				TFXGiveWeapon(4,pEntity);
		}
	}

	//turn off anticamp when we reach end of its time
	if(gpGlobals->time > PlayerInfo[ENTINDEX(pEntity)].InvulnEnd && PlayerInfo[ENTINDEX(pEntity)].IsInvuln == 1)
		AntiCampOver(pEntity);

	//Visual Effects Check
	if(PlayerInfo[ENTINDEX(pEntity)].VisualEffect > 0)
	{
		if(PlayerInfo[ENTINDEX(pEntity)].EffectNextThink <= gpGlobals->time)
		{
			//Do effect code
			if(PlayerInfo[ENTINDEX(pEntity)].VisualEffect == 1)
			{					
				//Fire it
				int m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
				MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
					WRITE_BYTE( TE_BEAMTORUS);
					WRITE_COORD(pEntity->v.origin.x);
					WRITE_COORD(pEntity->v.origin.y);
					WRITE_COORD(pEntity->v.origin.z);
					WRITE_COORD(pEntity->v.origin.x);
					WRITE_COORD(pEntity->v.origin.y);
					WRITE_COORD(pEntity->v.origin.z + (int)RANDOM_LONG(50,80) * 2 / .2); // reach damage radius over .3 seconds
					WRITE_SHORT(m_spriteTexture );
					WRITE_BYTE( 0 ); // startframe
					WRITE_BYTE( 1 ); // framerate
					WRITE_BYTE( 2 ); // life
					WRITE_BYTE( (int)RANDOM_LONG(30,150) );  // width
					WRITE_BYTE( 0 );   // noise
					WRITE_BYTE( (int)RANDOM_LONG(50,255) );   // r, g, b
					WRITE_BYTE( (int)RANDOM_LONG(50,255) );   // r, g, b
					WRITE_BYTE( (int)RANDOM_LONG(50,255) );   // r, g, b
					WRITE_BYTE( 255 );	// brightness
					WRITE_BYTE( 0 );		// speed
				MESSAGE_END();

				//Set next effect think
				PlayerInfo[ENTINDEX(pEntity)].EffectNextThink = gpGlobals->time + 0.3;
			}
		}
	}

	return 0;
}

void AntiCamp(edict_t *pEntity)
{
	PlayerInfo[ENTINDEX(pEntity)].InvulnEnd = gpGlobals->time + 6;
	PlayerInfo[ENTINDEX(pEntity)].IsInvuln = 1;
	pEntity->v.takedamage = DAMAGE_NO; //Invuln
	pEntity->v.rendermode = kRenderNormal;
	pEntity->v.renderfx = kRenderFxGlowShell;
	pEntity->v.renderamt = 25;  
	pEntity->v.rendercolor.x = 255;
	pEntity->v.rendercolor.y = 100;
	pEntity->v.rendercolor.z = 0;
}

void AntiCampOver(edict_t *pEntity)
{
	PlayerInfo[ENTINDEX(pEntity)].IsInvuln = 0;
	pEntity->v.takedamage = DAMAGE_AIM; 
	pEntity->v.rendercolor.x = 0;
	pEntity->v.rendercolor.y = 0;
	pEntity->v.rendercolor.z = 0;
}

void CustomGunModelSwitch(edict_t *pEntity)
{
	if(PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun > 0)
	{
		//Energy Gun
		if(PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun == ENERGYGUN_ID)
		{
			if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_crowbar.mdl"))
			{
				pEntity->v.viewmodel = MAKE_STRING("models/autoop/v_tfc_energygun.mdl");
				pEntity->v.weaponmodel = MAKE_STRING("models/autoop/p_tfc_energygun.mdl");
				PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = 0;
				PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon = WEAPON_AUTOOP_CUSTOM;
			}
		}

		//Tazar
		if(PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun == TAZAR_ID)
		{
			if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_umbrella.mdl"))
			{
				pEntity->v.viewmodel = MAKE_STRING("models/autoop/v_tfc_tazar.mdl");
				pEntity->v.weaponmodel = MAKE_STRING("models/autoop/p_tfc_tazar.mdl");
				PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = 0;
				PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon = WEAPON_AUTOOP_CUSTOM;
			}
		}

		//Anthrax Blaster
		if(PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun == ANTHRAX_BLASTER_ID)
		{
			if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_crowbar.mdl"))
			{
				pEntity->v.viewmodel = MAKE_STRING("models/autoop/v_tfc_ablaster.mdl");
				pEntity->v.weaponmodel = MAKE_STRING("models/autoop/p_tfc_ablaster.mdl");
				PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = 0;
				PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon = WEAPON_AUTOOP_CUSTOM;
			}
		}

		//Tripmine
		if(PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun == TRIPMINE_ID)
		{
			if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_crowbar.mdl"))
			{
				pEntity->v.viewmodel = MAKE_STRING(TRIPMINE_VMODEL);
				pEntity->v.weaponmodel = MAKE_STRING(TRIPMINE_PMODEL);
				PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = 0;
				PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon = WEAPON_AUTOOP_CUSTOM;
			}
		}

		//Add gun model switching here
	}
}

