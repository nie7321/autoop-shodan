/*****************************************************\
*	Auto-OP Shodan
*
*	File: buying.cpp
*	Description: Functions for user purchasing system.
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
#include "stripper.h"
#include "buying.h"
#include "skins.h"
#include "voting.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern DLL_FUNCTIONS other_gFunctionTable;

extern char *g_SkinNames[];

//Powerup buying
int QUAD_CREDS = 0;
int GOD_CREDS = 0;
int INVIS_CREDS = 0;
int GRAV_CREDS = 0;
int SPEED_CREDS = 0;
int CIV_CREDS = 0;
int HEAL_CREDS = 0;
int MAPVOTE_CREDS = 0;
int TRAIL_CREDS = 0;
int ADMINSKIN_CREDS = 0;

//Ammo Buying
int AMMO_NAILS_CREDS = 0;
int AMMO_SHELLS_CREDS = 0;
int AMMO_ROCKETS_CREDS = 0;
int AMMO_CELLS_CREDS = 0;
int AMMO_GREN1_CREDS = 0;
int AMMO_GREN2_CREDS = 0;

//Weapon Buying
int GUN_CREDS = 0;

//Speech buying
int SPEECH_CREDS = 0;

//Building buying
int BUILD_CREDS = 0;

/*

Buying Menu Map

(1)Main
   |
   +----(2)Powerups
   |
   |
   +----(3)Ammo
   |
   |
   +----(4)Weapons
   |
   |
   +----(5)Speech

*/

void ShowSGOptions(edict_t *pEntity)
{
	PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_SGOPTIONS;

	int bitsel = 0;

	bitsel|=1<<0;
	bitsel|=1<<1;
	bitsel|=1<<9;

	UTIL_ShowMenu(pEntity,bitsel,-1,0,"Select an option:\n\n1. Rotate Sentry 180 Degrees\n2. Rotate Sentry 45 Degrees\n\n0. Exit");
}

void ShowBuyMenu(edict_t *pEntity)
{
	if(pEntity->v.health <= 0)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Cant buy while dead!\n"));
		CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Cant buy while dead!\n"));
		return;
	}

	if(PlayerInfo[ENTINDEX(pEntity)].IsInvuln == 1)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Please wait a few seconds before buying anything.\n"));
		CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Please wait a few seconds before buying anything.\n"));
		return;
	}

	PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_MAIN;

	int bitsel = 0;

	bitsel|=1<<0;
	bitsel|=1<<1;
	bitsel|=1<<2;
	bitsel|=1<<3;
	bitsel|=1<<4;
	bitsel|=1<<9;

	UTIL_ShowMenu(pEntity,bitsel,-1,0,"Select an item type to buy:\n\n1. Powerups\n2. Ammo\n3. Weapons\n4. Speech\n5. Building\n\n0. Exit");

}

void BuyMenuSelect(edict_t *pEntity, int iOptionSelected, int iMenuNumber)
{
	int bitsel = 0;
	int PlayerIndex = ENTINDEX(pEntity);

	if(iOptionSelected == 10)
	{
		PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_NONE; //not in a menu now
		return;
	}

	if(iMenuNumber == MENU_MAIN)
	{
		if(iOptionSelected == 1)
		{
			PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_POWERUPS;
			bitsel|=1<<0;
			bitsel|=1<<1;
			bitsel|=1<<2;
			bitsel|=1<<3;
			bitsel|=1<<4;
			bitsel|=1<<5;
			bitsel|=1<<6;
			bitsel|=1<<7;
			bitsel|=1<<8;
			bitsel|=1<<9;

			char MenuString[512];

			sprintf(MenuString, "Select a powerup to buy:\n\n1. 30 Sec Quad Damage ($%d)\n2. 30 Sec God Mode ($%d)\n3. Invisibility until dead ($%d)\n",QUAD_CREDS,GOD_CREDS,INVIS_CREDS);
			
			UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

			sprintf(MenuString, "4. Scout Speed until dead ($%d)\n5. Low grav for 2 lives ($%d)\n6. Change to civilian ($%d)\n7. Heal Infection ($%d)\n8. Start a mapvote ($%d)\n9. Next page\n\n0. Exit",SPEED_CREDS,GRAV_CREDS,CIV_CREDS,HEAL_CREDS,MAPVOTE_CREDS);

			UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);
			return;
		}

		if(iOptionSelected == 2)
		{
			PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_AMMO;
			bitsel|=1<<0;
			bitsel|=1<<1;
			bitsel|=1<<2;
			bitsel|=1<<3;
			bitsel|=1<<4;
			bitsel|=1<<5;
			bitsel|=1<<9;

			char MenuString[512];

			sprintf(MenuString, "Select ammunition to buy:\n\n1. 1000 Nails ($%d)\n2. 1000 Shells ($%d)\n3. 1000 Rockets ($%d)\n4. 1000 Cells ($%d)\n",AMMO_NAILS_CREDS,AMMO_SHELLS_CREDS,AMMO_ROCKETS_CREDS,AMMO_CELLS_CREDS);
			
			UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

			sprintf(MenuString, "5. 5 Type 1 Grenades ($%d)\n6. 5 Type 2 Grenades ($%d)\n\n0. Exit",AMMO_GREN1_CREDS,AMMO_GREN2_CREDS);

			UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);
			return;
		}

		if(iOptionSelected == 3)
		{
			PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_WEAPONS;
			bitsel|=1<<0;
			bitsel|=1<<1;
			bitsel|=1<<2;
			bitsel|=1<<3;
			bitsel|=1<<4;
			bitsel|=1<<9;

			char MenuString[512];

			sprintf(MenuString, "Select weapon to buy:\n\n1. RPG ($%d)\n2. Chaingun ($%d)\n3. Sniper Rifle ($%d)\n4. Shotgun ($%d)\n",GUN_CREDS,GUN_CREDS,GUN_CREDS,GUN_CREDS);
			
			UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

			sprintf(MenuString, "5. Nailgun ($%d)\n\n0. Exit",GUN_CREDS);

			UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);
			return;
		}

		if(iOptionSelected == 4)
		{
			if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 1)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Cant buy speech as you are gagged.\n"));
				PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_NONE; //not in a menu now
				return;
			} else {
				PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_SPEECH;
				bitsel|=1<<0;
				bitsel|=1<<1;
				bitsel|=1<<2;
				bitsel|=1<<3;
				bitsel|=1<<4;
				bitsel|=1<<5;
				bitsel|=1<<6;
				bitsel|=1<<7;
				bitsel|=1<<8;
				bitsel|=1<<9;

				char MenuString[512];

				sprintf(MenuString, "Select speech to say:\n\n1. 'The enemy are idiots' ($%d)\n2. 'w00t!' ($%d)\n3. 'Cover my ass, team!' ($%d)\n",SPEECH_CREDS,SPEECH_CREDS,SPEECH_CREDS);
				
				UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

				sprintf(MenuString, "4. 'Affirmative' ($%d)\n5. 'I have the flag' ($%d)\n6. 'Defense required' ($%d)\n7. 'Move people!' ($%d)\n",SPEECH_CREDS,SPEECH_CREDS,SPEECH_CREDS,SPEECH_CREDS);

				UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

				sprintf(MenuString, "8. 'Negative' ($%d)\n\n9. Next Page...\n0. Exit",SPEECH_CREDS);

				UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);

				return;
			}
		}

		if(iOptionSelected == 5)
		{
			if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 1)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Cant buy speech as you are gagged.\n"));
				PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_NONE; //not in a menu now
				return;
			} else {
				PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_BUILDING;
				bitsel|=1<<0;
				bitsel|=1<<1;
				bitsel|=1<<2;
				bitsel|=1<<3;
				bitsel|=1<<9;

				char MenuString[512];

				sprintf(MenuString, "Select building to buy:\n\n1. Another Sentry Gun ($%d)\n2. Another Dispenser ($%d)\n3. Another Sensor ($%d)\n",BUILD_CREDS,BUILD_CREDS,BUILD_CREDS);
				
				UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

				sprintf(MenuString, "4. Another Manual Turret ($%d)\n\n0. Exit",BUILD_CREDS);

				UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);
				return;
			}
		}
	}

	//--- Powerups and abilities menu
	if(iMenuNumber == MENU_POWERUPS)
	{
		//Quad Damage
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < QUAD_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= QUAD_CREDS;

				edict_t	*pQuad = NULL;
				pQuad = CREATE_NAMED_ENTITY(MAKE_STRING("item_artifact_super_damage"));

				pQuad->v.origin = pEntity->v.origin;
				pQuad->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pQuad );
				DispatchTouch( pQuad, ENT( pEntity ) );

				pQuad->v.flags = FL_KILLME;

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//God Mode
		if(iOptionSelected == 2)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GOD_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GOD_CREDS;

				edict_t	*pPentagram = NULL;
				pPentagram = CREATE_NAMED_ENTITY(MAKE_STRING("item_artifact_invulnerability"));

				pPentagram->v.origin = pEntity->v.origin;
				pPentagram->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pPentagram );
				DispatchTouch( pPentagram, ENT( pEntity ) );

				pPentagram->v.flags = FL_KILLME;

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Invisibility
		if(iOptionSelected == 3)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < INVIS_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= INVIS_CREDS;

				KillSkin(pEntity); //Dont want skins with invis

				//Make invisible.
				pEntity->v.rendermode = 1;
				pEntity->v.renderamt = 0;

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Scout speed
		if(iOptionSelected == 4)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEED_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEED_CREDS;

				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_SCOUTSPEED;

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Low Grav
		if(iOptionSelected == 5)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GRAV_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GRAV_CREDS;

				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_LOWGRAV;
				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_LOWGRAV2;

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Civilian
		if(iOptionSelected == 6)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < CIV_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= CIV_CREDS;

				pEntity->v.playerclass = CLASS_CIVILIAN;

				CLIENT_COMMAND(pEntity,"kill\n");

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Heal
		if(iOptionSelected == 7)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < HEAL_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {

				if(pEntity->v.playerclass == CLASS_MEDIC)
				{
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Medics cannot buy this item.\n"));
					return; //Medics cannot be infected.
				}

				CBaseEntity *pSentryEnt = NULL;
				
				while ((pSentryEnt = UTIL_FindEntityByClassname(pSentryEnt, "timer")) != NULL)
				{
					if(pSentryEnt->pev->owner == pEntity)
					{
						PlayerInfo[PlayerIndex].LoadedInfo.Credits -= HEAL_CREDS;
						pSentryEnt->pev->flags = FL_KILLME;
						CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"fvox/antidote_shot\"\n") );
						CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
						return;
					}
				}

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase cancelled - not infected.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Mapvote
		if(iOptionSelected == 8)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < MAPVOTE_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				
				VoteStart(pEntity);
				
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= MAPVOTE_CREDS;
				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
				return;
			}
		}

		//Next page
		if(iOptionSelected == 9)
		{
			int bitsel = 0;

			PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_POWERUPS2;
			bitsel|=1<<0;
			bitsel|=1<<1;
			bitsel|=1<<9;

			char MenuString[512];

			sprintf(MenuString, "Select a powerup to buy:\n\n1. Colored trail ($%d)\n2. Use an admin skin ($%d)\n\n0. Exit",SPEECH_CREDS,ADMINSKIN_CREDS);
			
			UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);

			return;
		}
	}

	//--- Ammunition Buying menu
	if(iMenuNumber == MENU_AMMO)
	{
		//1000 Nails
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < AMMO_NAILS_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= AMMO_NAILS_CREDS;

				PatchpvPrivateInt(pEntity,200,1000);

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//1000 Shells
		if(iOptionSelected == 2)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < AMMO_SHELLS_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= AMMO_SHELLS_CREDS;

				PatchpvPrivateInt(pEntity,192,1000);

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//1000 Rockets
		if(iOptionSelected == 3)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < AMMO_ROCKETS_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= AMMO_ROCKETS_CREDS;

				PatchpvPrivateInt(pEntity,216,1000);

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//1000 Rockets
		if(iOptionSelected == 4)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < AMMO_CELLS_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= AMMO_CELLS_CREDS;

				PatchpvPrivateInt(pEntity,208,1000);

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//5 Gren1
		if(iOptionSelected == 5)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < AMMO_GREN1_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= AMMO_GREN1_CREDS;
				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_5GREN1;

				PatchpvPrivateInt(pEntity,56,5);

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//5 Gren2
		if(iOptionSelected == 6)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < AMMO_GREN2_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= AMMO_GREN2_CREDS;
				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_5GREN2;

				PatchpvPrivateInt(pEntity,60,5);

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}
	}

	//---Weapon Buying Menu
	if(iMenuNumber == MENU_WEAPONS)
	{
		//RPG
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GUN_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GUN_CREDS;

				edict_t	*pWeapon = NULL;
				pWeapon = CREATE_NAMED_ENTITY(MAKE_STRING("tf_weapon_rpg"));

				pWeapon->v.origin = pEntity->v.origin;
				pWeapon->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pWeapon );
				DispatchTouch( pWeapon, ENT( pEntity ) );				

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_SPAWNRPG;

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Chaingun
		if(iOptionSelected == 2)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GUN_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GUN_CREDS;

				edict_t	*pWeapon = NULL;
				pWeapon = CREATE_NAMED_ENTITY(MAKE_STRING("tf_weapon_ac"));

				pWeapon->v.origin = pEntity->v.origin;
				pWeapon->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pWeapon );
				DispatchTouch( pWeapon, ENT( pEntity ) );				

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_SPAWNCHAINGUN;

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Sniperrifle
		if(iOptionSelected == 3)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GUN_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GUN_CREDS;

				edict_t	*pWeapon = NULL;
				pWeapon = CREATE_NAMED_ENTITY(MAKE_STRING("tf_weapon_sniperrifle"));

				pWeapon->v.origin = pEntity->v.origin;
				pWeapon->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pWeapon );
				DispatchTouch( pWeapon, ENT( pEntity ) );				

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_SPAWNSNIPERRIFLE;

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Shotgun
		if(iOptionSelected == 4)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GUN_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GUN_CREDS;

				edict_t	*pWeapon = NULL;
				pWeapon = CREATE_NAMED_ENTITY(MAKE_STRING("tf_weapon_supershotgun"));

				pWeapon->v.origin = pEntity->v.origin;
				pWeapon->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pWeapon );
				DispatchTouch( pWeapon, ENT( pEntity ) );				

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_SPAWNSHOTGUN;

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Nailgun
		if(iOptionSelected == 5)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < GUN_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= GUN_CREDS;

				edict_t	*pWeapon = NULL;
				pWeapon = CREATE_NAMED_ENTITY(MAKE_STRING("tf_weapon_superng"));

				pWeapon->v.origin = pEntity->v.origin;
				pWeapon->v.spawnflags |= SF_NORESPAWN;
			
				DispatchSpawn( pWeapon );
				DispatchTouch( pWeapon, ENT( pEntity ) );				

				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}
	}

	//---Speech
	if(iMenuNumber == MENU_SPEECH)
	{
		//The enemy are idiot
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: The enemy are idiots.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"the enemy are idiot\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//w00t
		if(iOptionSelected == 2)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: w00t.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"woop\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Cover my ass, team!
		if(iOptionSelected == 3)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Cover my ass, team!\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/cover! my! ass! team!\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Affirmative
		if(iOptionSelected == 4)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Affirmative.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/affirmative\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//I have the flag
		if(iOptionSelected == 5)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: I have the flag.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"i have the flag\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Defense required
		if(iOptionSelected == 6)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Defense required.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"defense required\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Move people!
		if(iOptionSelected == 7)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Move, people!.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/move! people!\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Negative
		if(iOptionSelected == 8)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Negative.\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/negative\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}
		if(iOptionSelected == 9)
		{
			int bitsel = 0;

			PlayerInfo[ENTINDEX(pEntity)].MenuStatus = MENU_SPEECH2;
			bitsel|=1<<0;
			bitsel|=1<<1;
			bitsel|=1<<2;
			bitsel|=1<<9;

			char MenuString[512];

			sprintf(MenuString, "Select speech to say:\n\n1. 'Hostiles neutralized!' ($%d)\n2. 'Guard the command post!' ($%d)\n3. 'Clear!' ($%d)\n",SPEECH_CREDS,SPEECH_CREDS,SPEECH_CREDS);
			
			UTIL_ShowMenu(pEntity,bitsel,-1,1,MenuString);

			sprintf(MenuString, "\n0. Exit");

			UTIL_ShowMenu(pEntity,bitsel,-1,0,MenuString);

			return;
		}
	}

	//---Speech page 2
	if(iMenuNumber == MENU_SPEECH2)
	{
		//hostiles neutralized!
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Hostiles neutralized!\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/hostiles! neutralized!\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Guard the command post!
		if(iOptionSelected == 2)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Guard the command post!\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/guard! the! command! post!\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}
		//Clear!
		if(iOptionSelected == 3)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < SPEECH_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {
				PlayerInfo[PlayerIndex].LoadedInfo.Credits -= SPEECH_CREDS;

				UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("(RADIO) %s: Clear!\n",STRING(pEntity->v.netname)));

				edict_t *pPlayer = NULL;

				for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
					pPlayer = INDEXENT(i);
					if(pPlayer)
						if(!FStrEq(STRING(pPlayer->v.netname),""))
							CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("speak \"hgrunt/clear!\"\n") );
				}

				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}
	}

	//---Building Menu
	if(iMenuNumber == MENU_BUILDING)
	{
		//Another SG
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < BUILD_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {

				if(*(byte *)((char *)pEntity->pvPrivateData + 326) == 0x00)
				{
					//Not already got an SG
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You need to build an SG before you\ncan buy another one.\n"));
				} else {
					//Engy has an SG

					PlayerInfo[PlayerIndex].LoadedInfo.Credits -= BUILD_CREDS;

					*(word *)((char *)pEntity->pvPrivateData + 326) = 0x0000; //Reset build flag
					
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
				}
				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Another Dispenser
		if(iOptionSelected == 2)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < BUILD_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {

				if(*(byte *)((char *)pEntity->pvPrivateData + 322) == 0x00)
				{
					//Not already got a disp
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You need to build a dispenser before you\ncan buy another one.\n"));
				} else {
					//Engy has a disp

					PlayerInfo[PlayerIndex].LoadedInfo.Credits -= BUILD_CREDS;

					*(word *)((char *)pEntity->pvPrivateData + 322) = 0x0000; //Reset build flag
					
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
				}
				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Another Sensor
		if(iOptionSelected == 3)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < BUILD_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {

				if(PlayerInfo[ENTINDEX(pEntity)].HasFencePost == 0)
				{
					//Not already got a sensor
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You need to build a sensor before you\ncan buy another one.\n"));
				} else {
					//Engy has a sensor

					PlayerInfo[PlayerIndex].LoadedInfo.Credits -= BUILD_CREDS;

					PlayerInfo[PlayerIndex].HasFencePost = 0;
					
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
				}
				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		//Another Manual Turret
		if(iOptionSelected == 4)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < BUILD_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {

				if(PlayerInfo[ENTINDEX(pEntity)].HasBigGun == 0)
				{
					//Not already got a sensor
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You need to build a manual turret before\nyou can buy another one.\n"));
				} else {
					//Engy has a sensor

					PlayerInfo[PlayerIndex].LoadedInfo.Credits -= BUILD_CREDS;

					PlayerInfo[PlayerIndex].HasBigGun = 0;
					
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
				}
				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}
	}

	//---SG Options
	if(iMenuNumber == MENU_SGOPTIONS)
	{
		//Rotate 180
		if(iOptionSelected == 1)
		{
			UTIL_RotateSG(pEntity,TRUE);
			PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
			return;
		}

		//Rotate 45
		if(iOptionSelected == 2)
		{
			UTIL_RotateSG(pEntity,FALSE);
			PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
			return;
		}
	}

	//--- Powerups 2
	if(iMenuNumber == MENU_POWERUPS2)
	{
		if(iOptionSelected == 1)
		{
			if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < TRAIL_CREDS)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
			} else {

				int iRed = (int)RANDOM_LONG(50,255);
				int iGreen = (int)RANDOM_LONG(50,255);
				int iBlue = (int)RANDOM_LONG(50,255);

				short sprite;
				sprite = PRECACHE_MODEL("sprites/laserbeam.spr");

				//Find index to players skin entity
				CBaseEntity *pSkinEnt = NULL;
				pSkinEnt = UTIL_FindEntityByTargetname( NULL, g_SkinNames[ENTINDEX(pEntity)]);

				if(!pSkinEnt)
				{
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase not successful.\n"));
				} else {
					MESSAGE_BEGIN(MSG_ALL, SVC_TEMPENTITY);
								WRITE_BYTE(TE_BEAMFOLLOW);
								WRITE_SHORT(pSkinEnt->entindex());
								WRITE_SHORT(sprite);
								WRITE_BYTE(100);
								WRITE_BYTE(25);
								WRITE_BYTE(iRed);
								WRITE_BYTE(iGreen);
								WRITE_BYTE(iBlue);
								WRITE_BYTE(150);
					MESSAGE_END();	

					PlayerInfo[PlayerIndex].LoadedInfo.Credits -= TRAIL_CREDS;
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));
				}
				PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
				return;
			}
		}

		if(iOptionSelected == 2)
		{
			if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_ADMINSKIN)
			{
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You have already got this item!\n"));
			} else {

				if(PlayerInfo[PlayerIndex].LoadedInfo.Credits < ADMINSKIN_CREDS)
				{
					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "You do not have enough credits to\npurchase this item.\n"));
				} else {
					PlayerInfo[PlayerIndex].LoadedInfo.Credits -= ADMINSKIN_CREDS;

					PlayerInfo[PlayerIndex].AbilityOverride |= ABILITY_ADMINSKIN;

					CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Item purchase successful.\n"));

					PlayerInfo[PlayerIndex].MenuStatus = MENU_NONE;
					return;
				}
			}
		}
	}
}
