/*****************************************************\
*	Auto-OP Shodan
*
*	File: gundrop.cpp
*	Description: Gun drop entity
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
extern FILE *fp;

void DropGun(edict_t *pEntity)
{
	int Weapon = PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon;

	if(Weapon <= 0)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot drop this weapon.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot drop this weapon.\n"));
		return;
	}

	if((Weapon == WEAPON_AXE) || (Weapon == WEAPON_SPANNER) || (Weapon == WEAPON_KNIFE) || (Weapon == WEAPON_MEDIKIT) || (Weapon == WEAPON_ASSAULT_CANNON))
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot drop this weapon.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot drop this weapon.\n"));
		return;
	}
	
	if((Weapon == WEAPON_AUTOOP_CUSTOM) || !(pEntity->v.weapons & (1<<Weapon)))
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot drop this weapon.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot drop this weapon.\n"));
		return;
	}

	UTIL_MakeVectors( pEntity->v.v_angle );
	TraceResult tr;
	Vector trace_origin;

	trace_origin = pEntity->v.origin;
	if ( pEntity->v.flags & FL_DUCKING )
	{
		trace_origin = trace_origin - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	}

	UTIL_TraceLine( trace_origin + gpGlobals->v_forward * 20, trace_origin + gpGlobals->v_forward * 64, dont_ignore_monsters, NULL, &tr );

	if (tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > 0.25)
	{
		//Player loses weapon
		pEntity->v.viewmodel = 0;
		pEntity->v.weaponmodel = 0;

		int gmsgCurWeapon = 0;
		gmsgCurWeapon = REG_USER_MSG("CurWeapon", 3);

		if(gmsgCurWeapon > MAX_USERMSG)
			return;

		MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pEntity );
			WRITE_BYTE(0);
			WRITE_BYTE(0);
			WRITE_BYTE(0);
		MESSAGE_END();

		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("tf_weapon_axe;tf_weapon_spanner;tf_weapon_knife;tf_weapon_medikit\n") );

		pEntity->v.weapons &= ~(1<<Weapon);

		//Player drops the gun they are carrying to the floor
		EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "items/weapondrop1.wav", 1, ATTN_NORM, 0, PITCH_NORM);
		edict_t *pDropped = NULL;
		pDropped = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
		pDropped->v.origin = tr.vecEndPos;
		pDropped->v.angles = pEntity->v.v_angle;
		pDropped->v.takedamage = DAMAGE_NO;
		pDropped->v.velocity = pEntity->v.velocity + gpGlobals->v_forward*400;
		pDropped->v.gravity = 0.5;
		pDropped->v.friction = 0.5;
		pDropped->v.health = 100000;
		pDropped->v.solid = SOLID_SLIDEBOX;
		pDropped->v.movetype = MOVETYPE_TOSS;
		pDropped->v.iuser1 = Weapon; //Weapon index
		pDropped->v.iuser2 = 0;
		pDropped->v.iuser2 |= (1<<Weapon); //True index
		pDropped->v.fuser1 = gpGlobals->time + 2; //2 sec pickup delay
		pDropped->v.fuser2 = gpGlobals->time + 30; //Life of gun
		pDropped->v.team = pEntity->v.team;
		pDropped->v.owner = pEntity;
		DispatchSpawn(pDropped);
		SET_MODEL(pDropped, "models/autoop/w_guns.mdl");
		pDropped->v.classname = MAKE_STRING("autoop_dropped_weapon");
		pDropped->v.nextthink = gpGlobals->time + 0.1;
		UTIL_SetSize(VARS(pDropped), Vector( -4, -4, 0), Vector(4, 4, 8));
		UTIL_SetOrigin( VARS(pDropped), pDropped->v.origin );

		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "DropGun(): Client dropped a weapon of ID: %d\n",Weapon);
			fclose(fp);
		}
	}
}

bool DroppedGunThink(edict_t *pEntity)
{
	if((pEntity->v.iuser4 == 0) && (pEntity->v.fuser2 < gpGlobals->time))
	{
		pEntity->v.flags = FL_KILLME;
		return 1;
	}

	if((pEntity->v.iuser4 == 0) && (pEntity->v.owner->v.team != pEntity->v.team))
	{
		pEntity->v.flags = FL_KILLME;
		return 1;
	}

	//Make sure we look like the weapon that was dropped.
	if(pEntity->v.iuser1 == WEAPON_LASER)
		pEntity->v.body = 0;

	if((pEntity->v.iuser1 == WEAPON_TF_SHOTGUN) || (pEntity->v.iuser1 == WEAPON_SUPER_SHOTGUN))
		pEntity->v.body = 1;

	if(pEntity->v.iuser1 == WEAPON_ROCKET_LAUNCHER)
		pEntity->v.body = 2;

	if((pEntity->v.iuser1 == WEAPON_GRENADE_LAUNCHER) || (pEntity->v.iuser1 == WEAPON_PIPEBOMB_LAUNCHER))
		pEntity->v.body = 3;

	if(pEntity->v.iuser1 == WEAPON_SPANNER)
		pEntity->v.body = 4;

	if((pEntity->v.iuser1 == WEAPON_SNIPER_RIFLE) || (pEntity->v.iuser1 == WEAPON_AUTO_RIFLE))
		pEntity->v.body = 5;

	if(pEntity->v.iuser1 == WEAPON_SUPER_NAILGUN)
		pEntity->v.body = 6;

	if(pEntity->v.iuser1 == WEAPON_NAILGUN)
		pEntity->v.body = 7;

	if(pEntity->v.iuser1 == WEAPON_MEDIKIT)
		pEntity->v.body = 8;

	if(pEntity->v.iuser1 == WEAPON_KNIFE)
		pEntity->v.body = 9;

	if(pEntity->v.iuser1 == WEAPON_FLAMETHROWER)
		pEntity->v.body = 10;

	if(pEntity->v.iuser1 == WEAPON_ASSAULT_CANNON)
		pEntity->v.body = 11;

	if(pEntity->v.iuser1 == WEAPON_TRANQ)
		pEntity->v.body = 12;

	if(pEntity->v.iuser1 == WEAPON_INCENDIARY)
		pEntity->v.body = 13;

	if(pEntity->v.fuser1 < gpGlobals->time)
	{
		//Now look for nearby players, and see if we are picked up
		CBaseEntity *pScan = NULL;
		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
		{
			//Is it an entity we are bothered about? (Players and grens)
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{	
				if ((pEntity->v.origin - pScan->pev->origin).Length() < 50.0)
				{
					if(!(pScan->pev->weapons & (1<<pEntity->v.iuser1)))
					{
						//User picks up gun
						UserPickedMeUp(pEntity,pScan);
						break;
					}
				}
			}
		}
	}

	pEntity->v.nextthink = gpGlobals->time + 0.1;

	return 1;
}

void UserPickedMeUp(edict_t *pEntity, CBaseEntity *pPlayer)
{
	int GiveWeaponID = 0;
	int Weapon = pEntity->v.iuser2;

	if(Weapon & (1<<WEAPON_MEDIKIT))
	{
		GiveWeaponID = 3;
		pPlayer->pev->weapons |= (1<<WEAPON_MEDIKIT);
	}

	if(Weapon & (1<<WEAPON_SPANNER))
	{
		GiveWeaponID = 7;
		pPlayer->pev->weapons |= (1<<WEAPON_SPANNER);
		if(pPlayer->pev->playerclass == CLASS_ENGINEER)
		{
			if(pEntity->v.iuser4 == 0)
			{
				pEntity->v.flags = FL_KILLME;
			} else {
				pEntity->v.effects = EF_NODRAW;
				pEntity->v.fuser4 = gpGlobals->time + GUN_RESPAWN_DELAY;
			}
			return;
		}
	}

	if(Weapon & (1<<WEAPON_SNIPER_RIFLE))
	{
		GiveWeaponID = 12;
		pPlayer->pev->weapons |= (1<<WEAPON_SNIPER_RIFLE);
	}

	if(Weapon & (1<<WEAPON_AUTO_RIFLE))
	{
		GiveWeaponID = 13;
		pPlayer->pev->weapons |= (1<<WEAPON_AUTO_RIFLE);
	}

	if(Weapon & (1<<WEAPON_TF_SHOTGUN))
	{
		GiveWeaponID = 4;
		pPlayer->pev->weapons |= (1<<WEAPON_TF_SHOTGUN);
	}

	if(Weapon & (1<<WEAPON_SUPER_SHOTGUN))
	{
		GiveWeaponID = 6;
		pPlayer->pev->weapons |= (1<<WEAPON_SUPER_SHOTGUN);
	}

	if(Weapon & (1<<WEAPON_NAILGUN))
	{
		GiveWeaponID = 8;
		pPlayer->pev->weapons |= (1<<WEAPON_NAILGUN);
	}

	if(Weapon & (1<<WEAPON_SUPER_NAILGUN))
	{
		GiveWeaponID = 9;
		pPlayer->pev->weapons |= (1<<WEAPON_SUPER_NAILGUN);
	}

	if(Weapon & (1<<WEAPON_GRENADE_LAUNCHER))
	{
		GiveWeaponID = 16;
		pPlayer->pev->weapons |= (1<<WEAPON_GRENADE_LAUNCHER);
	}

	if(Weapon & (1<<WEAPON_FLAMETHROWER))
	{
		GiveWeaponID = 14;
		pPlayer->pev->weapons |= (1<<WEAPON_FLAMETHROWER);
		
		if(pPlayer->pev->playerclass == CLASS_PYRO)
		{
			if(pEntity->v.iuser4 == 0)
			{
				pEntity->v.flags = FL_KILLME;
			} else {
				pEntity->v.effects = EF_NODRAW;
				pEntity->v.fuser4 = gpGlobals->time + GUN_RESPAWN_DELAY;
			}
			return;
		}
	}

	if(Weapon & (1<<WEAPON_ROCKET_LAUNCHER))
	{
		GiveWeaponID = 1;
		pPlayer->pev->weapons |= (1<<WEAPON_ROCKET_LAUNCHER);
		
		if(pPlayer->pev->playerclass == CLASS_SOLDIER)
		{
			if(pEntity->v.iuser4 == 0)
			{
				pEntity->v.flags = FL_KILLME;
			} else {
				pEntity->v.effects = EF_NODRAW;
				pEntity->v.fuser4 = gpGlobals->time + GUN_RESPAWN_DELAY;
			}
			return;
		}
	}

	if(Weapon & (1<<WEAPON_INCENDIARY))
	{
		GiveWeaponID = 15;
		pPlayer->pev->weapons |= (1<<WEAPON_INCENDIARY);
	}

	if(Weapon & (1<<WEAPON_ASSAULT_CANNON))
	{
		GiveWeaponID = 2;
		pPlayer->pev->weapons |= (1<<WEAPON_ASSAULT_CANNON);
	}

	if(Weapon & (1<<WEAPON_TRANQ))
	{
		GiveWeaponID = 10;
		pPlayer->pev->weapons |= (1<<WEAPON_TRANQ);
	}

	if(Weapon & (1<<WEAPON_LASER))
	{
		GiveWeaponID = 11;
		pPlayer->pev->weapons |= (1<<WEAPON_LASER);

		if(pPlayer->pev->playerclass == CLASS_ENGINEER)
		{
			if(pEntity->v.iuser4 == 0)
			{
				pEntity->v.flags = FL_KILLME;
			} else {
				pEntity->v.effects = EF_NODRAW;
				pEntity->v.fuser4 = gpGlobals->time + GUN_RESPAWN_DELAY;
			}
			return;
		}
	}

	if(Weapon & (1<<WEAPON_PIPEBOMB_LAUNCHER))
	{
		GiveWeaponID = 17;
		pPlayer->pev->weapons |= (1<<WEAPON_PIPEBOMB_LAUNCHER);
	}

	if(Weapon & (1<<WEAPON_KNIFE))
	{
		GiveWeaponID = 5;
		pPlayer->pev->weapons |= (1<<WEAPON_KNIFE);
	}

	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "items/gunpickup2.wav", 1, ATTN_NORM, 0, PITCH_NORM);
	
	TFXGiveWeapon(GiveWeaponID,pPlayer->edict());

	if(pEntity->v.iuser4 == 0)
	{
		pEntity->v.flags = FL_KILLME;
	} else {
		pEntity->v.effects = EF_NODRAW;
		pEntity->v.fuser4 = gpGlobals->time + GUN_RESPAWN_DELAY;
	}
}

int StartingWeapon(edict_t *pEntity)
{
	if(pEntity->v.playerclass == CLASS_SCOUT)
		return WEAPON_NAILGUN;

	if(pEntity->v.playerclass == CLASS_SNIPER)
		return WEAPON_SNIPER_RIFLE;

	if(pEntity->v.playerclass == CLASS_SOLDIER)
		return WEAPON_ROCKET_LAUNCHER;

	if(pEntity->v.playerclass == CLASS_DEMOMAN)
		return WEAPON_GRENADE_LAUNCHER;

	if(pEntity->v.playerclass == CLASS_MEDIC)
		return WEAPON_SUPER_NAILGUN;

	if(pEntity->v.playerclass == CLASS_HWGUY)
		return WEAPON_ASSAULT_CANNON;

	if(pEntity->v.playerclass == CLASS_PYRO)
		return WEAPON_FLAMETHROWER;

	if(pEntity->v.playerclass == CLASS_SPY)
		return WEAPON_TRANQ;

	if(pEntity->v.playerclass == CLASS_ENGINEER)
		return WEAPON_LASER;

	return WEAPON_AXE; //Safe as cannot be dropped
}