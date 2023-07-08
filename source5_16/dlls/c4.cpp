/*****************************************************\
*	Auto-OP Shodan
*
*	File: c4.cpp
*	Description: Code for the medic's C4.
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
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern int team_allies[4];
extern DLL_FUNCTIONS other_gFunctionTable;

void PlaceC4(edict_t *pEntity, int iTimer)
{
	if(iTimer > 0)
	{
		if((iTimer < 30) || (iTimer > 300))
		{
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Valid C4 timer values are between 30 and 300 seconds.\n"));
			return;
		}
	}

	if( PlayerInfo[ENTINDEX(pEntity)].C4Ammo <= 0)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You have no C4 left.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have no C4 left.\n"));
		return;
	}

	if(HasAlreadyPlaced(pEntity) == 1)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You already have a C4 active.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You already have a C4 active\n"));
		return;
	}

	//Scan to make sure we are not in a func or too near another mine
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 50)) != NULL)
	{
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0 || strcmp(STRING(pScan->pev->classname), "weapon_ao_c4") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place C4 here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place C4 here.\n"));
			return;
		}
	}

	if(pEntity->v.playerclass != CLASS_MEDIC || pEntity->v.health <= 0)
		return;

	//Draw little traceline from player to about 128 cells in front of him
	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 128;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			//Flat walls will have a vecPlanenormal.z of 0, and an x any of either 1 or -1.
			if(tr.vecPlaneNormal.z != 0)
			{
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place C4 here.\n"));
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place C4 here.\n"));
				return;
			}

			if(tr.vecPlaneNormal.x != 0 && tr.vecPlaneNormal.x!= -1 && tr.vecPlaneNormal.x != 1)
			{
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place C4 here.\n"));
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place C4 here.\n"));
				return;
			}

			if(tr.vecPlaneNormal.y != 0 && tr.vecPlaneNormal.y != -1 && tr.vecPlaneNormal.y != 1)
			{
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place C4 here.\n"));
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place C4 here.\n"));
				return;
			}

			PlayerInfo[ENTINDEX(pEntity)].C4Ammo -= 1;
			//Create the C4
			edict_t *pC4;
			pC4 = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
			DispatchSpawn(pC4);
			pC4->v.origin = tr.vecEndPos;
			pC4->v.angles = pEntity->v.v_angle;
			pC4->v.angles.x = 0;
			pC4->v.angles.z = 0;
			pC4->v.angles.y += 180; // Face the player
			if(pC4->v.angles.y >= 360) pC4->v.angles.z -= 360;
			UTIL_MakeVectors(pEntity->v.v_angle);
			pC4->v.velocity = Vector(0, 0, 0);
			pC4->v.euser1 = pEntity;
			pC4->v.takedamage = DAMAGE_YES;
			
			if(Features.C4AutoDet > 0)
				pC4->v.fuser2 = gpGlobals->time + Features.C4AutoDet;
			else
				pC4->v.fuser2 = -1; //Never autodet

			if(iTimer > 0)
			{
				pC4->v.fuser2 = gpGlobals->time + iTimer;
				pC4->v.iuser3 = 1;
			}

			pC4->v.health = 100000;
			pC4->v.movetype = MOVETYPE_NONE;
			pC4->v.solid = SOLID_BBOX;
			pC4->v.team = pEntity->v.team;
			SET_MODEL(pC4, "models/autoop/aoc4.mdl");
			UTIL_SetSize(VARS(pC4), Vector(0,0,0), Vector(0,0,0));
			pC4->v.classname = MAKE_STRING("weapon_ao_c4");
			pC4->v.nextthink = gpGlobals->time + 0.1;
			EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "player/pl_fallpain1.wav", 1, ATTN_NORM, 0, 100);

			UTIL_LogPrintf( "[AUTOOP] %s planted a C4\n", STRING(pEntity->v.netname));
		}

		if( (strcmp("player", STRING(tr.pHit->v.classname)) == 0) && (Features.C4OnPlayers == 1))
		{
			int Team1 = tr.pHit->v.team;
			int Team2 = pEntity->v.team;
			if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
			{
				PlayerInfo[ENTINDEX(pEntity)].C4Ammo -= 1;
				//Create the Tripmine
				edict_t *pC4;
				pC4 = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
				DispatchSpawn(pC4);
				pC4->v.velocity = Vector(0, 0, 0);
				pC4->v.euser1 = pEntity;
				pC4->v.takedamage = DAMAGE_YES;
				//pC4->v.fuser1 = gpGlobals->time + 4.0; // 4 sec delay
				pC4->v.health = 100000;
				pC4->v.movetype = MOVETYPE_FOLLOW;
				pC4->v.aiment = tr.pHit;
				pC4->v.solid = SOLID_BBOX;
				pC4->v.team = pEntity->v.team;
				pC4->v.iuser4 = 1; //this means we are on a player
				SET_MODEL(pC4, "models/autoop/aoc4.mdl");
				UTIL_SetSize(VARS(pC4), Vector(0,0,0), Vector(0,0,0));
				pC4->v.classname = MAKE_STRING("weapon_ao_c4");
				pC4->v.nextthink = gpGlobals->time + 0.1;
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "player/pl_fallpain1.wav", 1, ATTN_NORM, 0, 100);

				UTIL_LogPrintf( "[AUTOOP] %s planted a C4\n", STRING(pEntity->v.netname));
			}
		}
	}
}

int HasAlreadyPlaced(edict_t *pEntity)
{
	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_c4"))
	{
        if(pMine->pev->euser1 == pEntity)
			return 1;
	}
	return 0;
}

bool C4Think(edict_t *pEntity)
{
	if(pEntity->v.euser1->v.team != pEntity->v.team)
	{
		pEntity->v.flags = FL_KILLME;
		return 1;
	}

	if(pEntity->v.fuser2 > -1)
	{
		if(pEntity->v.fuser2 < gpGlobals->time)
		{
			CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "Your C4 Detonated.\n"));
			CLIENT_PRINTF( pEntity->v.euser1, print_console,UTIL_VarArgs( "Your C4 Detonated.\n"));
			DetonateC4(pEntity->v.euser1,1);
			return 1;
		}
	}
	
	//First check our owner is still fit to have mines active.
	if(pEntity->v.euser1->v.health <= 0 || pEntity->v.euser1->v.playerclass != CLASS_MEDIC)
	{
		pEntity->v.flags = FL_KILLME;
		return 1;
	}

	//If we are on a player, make sure we can still live
	if(pEntity->v.aiment != NULL)
		if(pEntity->v.aiment->v.health <= 0)
			DetonateC4(pEntity->v.euser1,0);

	if(pEntity->v.health <= 100000 - C4_HEALTH)
	{
		DetonateC4(pEntity->v.euser1,1);
		//pEntity->v.fuser2 = gpGlobals->time + C4_DETDELAY;
		return 1;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void DetonateC4(edict_t *pEntity, int iDetonate)
{
	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_c4"))
    {
        if(pMine->pev->euser1 == pEntity)
        {
			if(iDetonate == 1)
			{
				int m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
				MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
					WRITE_BYTE( TE_BEAMTORUS);
					WRITE_COORD(pMine->pev->origin.x);
					WRITE_COORD(pMine->pev->origin.y);
					WRITE_COORD(pMine->pev->origin.z + 32);
					WRITE_COORD(pMine->pev->origin.x);
					WRITE_COORD(pMine->pev->origin.y);
					WRITE_COORD(pMine->pev->origin.z + 32 + 300 * 2 / .2); // reach damage radius over .3 seconds
					WRITE_SHORT(m_spriteTexture );
					WRITE_BYTE( 0 ); // startframe
					WRITE_BYTE( 0 ); // framerate
					WRITE_BYTE( 2 ); // life
					WRITE_BYTE( 12 );  // width
					WRITE_BYTE( 0 );   // noise
					WRITE_BYTE( 255 );   // r, g, b
					WRITE_BYTE( 160 );   // r, g, b
					WRITE_BYTE( 100 );   // r, g, b
					WRITE_BYTE( 255 );	// brightness
					WRITE_BYTE( 0 );		// speed
				MESSAGE_END();

				edict_t *pBomb = NULL;
				pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
				DispatchSpawn(pBomb);
				pBomb->v.origin = pMine->pev->origin;
				pBomb->v.velocity = Vector(0,0,0);
				pBomb->v.dmg = 500;
				pBomb->v.fuser1 = gpGlobals->time;
				pBomb->v.iuser1 = 0;
				pBomb->v.team = pEntity->v.team;
				pBomb->v.euser1 = pEntity;
				pBomb->v.owner = pEntity;
				pBomb->v.effects |= EF_NODRAW;
				pBomb->v.nextthink = gpGlobals->time + 0.1;
				pBomb->v.classname = MAKE_STRING("c4_bombcounter");
				UTIL_LogPrintf( "[AUTOOP] A C4 bomb detonated.\n");

				pMine->pev->flags = FL_KILLME;
			} else {
				pMine->pev->flags = FL_KILLME;
			}
		}
	}
}

bool C4BombCounterThink(edict_t *pEntity)
{
	if(pEntity->v.euser1->v.team != pEntity->v.team)
	{
		pEntity->v.flags |= FL_KILLME;
		return 1;
	}

	if(pEntity->v.iuser1 > 4)
	{
		pEntity->v.flags |= FL_KILLME;
		return 1;
	}

	if(gpGlobals->time >= pEntity->v.fuser1)
	{
		if(pEntity->v.iuser1 = 0)
		{
			edict_t *pBomb = NULL;
			pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
			DispatchSpawn(pBomb);
			pBomb->v.origin.x = pEntity->v.origin.x;
			pBomb->v.origin.y = pEntity->v.origin.y;
			pBomb->v.origin.z = pEntity->v.origin.z;
			pBomb->v.velocity = Vector(0,0,0);
			pBomb->v.dmg = 250;
			pBomb->v.owner = pEntity->v.owner;
			pBomb->v.effects |= EF_NODRAW;
			pBomb->v.nextthink = gpGlobals->time;
			pBomb->v.classname = MAKE_STRING("c4_blast");
		}

		if(pEntity->v.iuser1 = 1)
		{
			edict_t *pBomb = NULL;
			pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
			DispatchSpawn(pBomb);
			pBomb->v.origin.x = pEntity->v.origin.x + 100;
			pBomb->v.origin.y = pEntity->v.origin.y;
			pBomb->v.origin.z = pEntity->v.origin.z;
			pBomb->v.velocity = Vector(0,0,0);
			pBomb->v.dmg = 250;
			pBomb->v.owner = pEntity->v.owner;
			pBomb->v.effects |= EF_NODRAW;
			pBomb->v.nextthink = gpGlobals->time;
			pBomb->v.classname = MAKE_STRING("c4_blast");
		}

		if(pEntity->v.iuser1 = 2)
		{
			edict_t *pBomb = NULL;
			pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
			DispatchSpawn(pBomb);
			pBomb->v.origin.x = pEntity->v.origin.x - 100;
			pBomb->v.origin.y = pEntity->v.origin.y;
			pBomb->v.origin.z = pEntity->v.origin.z;
			pBomb->v.velocity = Vector(0,0,0);
			pBomb->v.dmg = 250;
			pBomb->v.owner = pEntity->v.owner;
			pBomb->v.effects |= EF_NODRAW;
			pBomb->v.nextthink = gpGlobals->time;
			pBomb->v.classname = MAKE_STRING("c4_blast");
		}

		if(pEntity->v.iuser1 = 3)
		{
			edict_t *pBomb = NULL;
			pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
			DispatchSpawn(pBomb);
			pBomb->v.origin.x = pEntity->v.origin.x;
			pBomb->v.origin.y = pEntity->v.origin.y + 100;
			pBomb->v.origin.z = pEntity->v.origin.z;
			pBomb->v.velocity = Vector(0,0,0);
			pBomb->v.dmg = 250;
			pBomb->v.owner = pEntity->v.owner;
			pBomb->v.effects |= EF_NODRAW;
			pBomb->v.nextthink = gpGlobals->time;
			pBomb->v.classname = MAKE_STRING("c4_blast");
		}

		if(pEntity->v.iuser1 = 4)
		{
			edict_t *pBomb = NULL;
			pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
			DispatchSpawn(pBomb);
			pBomb->v.origin.x = pEntity->v.origin.x;
			pBomb->v.origin.y = pEntity->v.origin.y - 100;
			pBomb->v.origin.z = pEntity->v.origin.z;
			pBomb->v.velocity = Vector(0,0,0);
			pBomb->v.dmg = 250;
			pBomb->v.owner = pEntity->v.owner;
			pBomb->v.effects |= EF_NODRAW;
			pBomb->v.nextthink = gpGlobals->time;
			pBomb->v.classname = MAKE_STRING("c4_blast");
		}
		pEntity->v.fuser1 = gpGlobals->time + 0.5;
		pEntity->v.iuser1++;
	}

	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

