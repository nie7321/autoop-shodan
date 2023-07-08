/*****************************************************\
*	Auto-OP Shodan
*
*	File: superflame.cpp
*	Description: Pyros superflame functions.
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
extern int team_allies[4];

extern short m_Fire;

void ChargeFlame(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_PYRO)
		return;

	//Make sure we arent starting a superflame already
	CBaseEntity *pScan = NULL;
	while(pScan = UTIL_FindEntityByClassname(pScan, "ao_superflame_start"))
	{
		if(pScan->pev->euser1 == pEntity)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your last superflame is still active.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your last superflame is still active.\n"));
			return;
		}
	}
	
	//Then check there isnt one active thats ours
	while(pScan = UTIL_FindEntityByClassname(pScan, "weapon_ao_superflame"))
	{
		if(pScan->pev->euser1 == pEntity)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your last superflame is still active.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your last superflame is still active.\n"));
			return;
		}
	}

	//Make sure he isnt near a func_nobuild
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot flame this area.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot flame this area.\n"));
			return;
		}
	}

	//Begin launch sequence.
	edict_t *pStrikeStart = NULL;
    pStrikeStart = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    DispatchSpawn(pStrikeStart);
    pStrikeStart->v.origin = pEntity->v.origin;
    pStrikeStart->v.euser1 = pEntity;
    pStrikeStart->v.angles = Vector(0,0,0);
    pStrikeStart->v.velocity = Vector(0,0,0);
	pStrikeStart->v.takedamage = DAMAGE_NO;
	pStrikeStart->v.iuser4 = 20; //No of 1/2 seconds pyro cannot move.
	pStrikeStart->v.health = 1;
	pStrikeStart->v.team = pEntity->v.team;
	pStrikeStart->v.movetype = MOVETYPE_NONE;
	pStrikeStart->v.nextthink = gpGlobals->time;
	pStrikeStart->v.framerate = 1.0;
	pStrikeStart->v.sequence = 0;
	pStrikeStart->v.gravity = 0.5;
	pStrikeStart->v.friction = 0.8;
	pStrikeStart->v.effects = EF_NODRAW;
	pStrikeStart->v.solid = SOLID_BBOX;
    SET_MODEL(pStrikeStart, ("models/autoop/airstrike.mdl"));
    UTIL_SetSize(VARS(pStrikeStart), Vector(0, 0, 0), Vector(0, 0, 0));
    pStrikeStart->v.classname = MAKE_STRING("ao_superflame_start");
	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You started to charge a superflame.\n"));
}

bool StartFlameThink(edict_t *pEntity)
{
	//The Counter
	if(pEntity->v.iuser4 > 0)
	{
		if(pEntity->v.euser1->v.health < 10)
		{
			CancelFlame(pEntity->v.euser1);
		} else {
			pEntity->v.iuser4 -= 1;
			pEntity->v.euser1->v.flags |= FL_FROZEN; //No movement whilst doing thing

			//Inform Demoman at certian points
			if(pEntity->v.iuser4 == 60)
			{
				CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "30 Seconds till superflame...\n"));
				pEntity->v.euser1->v.rendermode = kRenderNormal;
				pEntity->v.euser1->v.renderfx = kRenderFxGlowShell;
				pEntity->v.euser1->v.renderamt = 25;  
				pEntity->v.euser1->v.rendercolor.x = 255;
				pEntity->v.euser1->v.rendercolor.y = 0;
				pEntity->v.euser1->v.rendercolor.z = 0;
			}

			if(pEntity->v.iuser4 == 40)
			{
				CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "20 Seconds till superflame...\n"));
				pEntity->v.euser1->v.rendermode = kRenderNormal;
				pEntity->v.euser1->v.renderfx = kRenderFxGlowShell;
				pEntity->v.euser1->v.renderamt = 50;  
				pEntity->v.euser1->v.rendercolor.x = 255;
				pEntity->v.euser1->v.rendercolor.y = 100;
				pEntity->v.euser1->v.rendercolor.z = 0;
			}

			if(pEntity->v.iuser4 == 20)
			{
				CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "10 Seconds till superflame...\n"));
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "ambience/particle_suck1.wav", 1, ATTN_NORM, 0, 90);
				pEntity->v.euser1->v.rendermode = kRenderNormal;
				pEntity->v.euser1->v.renderfx = kRenderFxGlowShell;
				pEntity->v.euser1->v.renderamt = 100;  
				pEntity->v.euser1->v.rendercolor.x = 255;
				pEntity->v.euser1->v.rendercolor.y = 255;
				pEntity->v.euser1->v.rendercolor.z = 0;
			}

			pEntity->v.nextthink = gpGlobals->time + 0.5;
			return 1;
		}
		return 1;
	}

	if(pEntity->v.iuser4 == 0)
	{
		pEntity->v.euser1->v.flags &= ~FL_FROZEN;
		pEntity->v.euser1->v.rendermode = 0;
		pEntity->v.euser1->v.renderfx = 0;
		pEntity->v.euser1->v.renderamt = 255;  
		CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "SuperFlame Engaged...\n"));
		DoTheFlame(pEntity->v.euser1->v.origin,pEntity->v.euser1);
		pEntity->v.flags |= FL_KILLME;
	}
	return 1;
}

void CancelFlame(edict_t *pEntity)
{
	//Search for the flame starter
	CBaseEntity *pStarter = NULL;
	while(pStarter = UTIL_FindEntityByClassname(pStarter, "ao_superflame_start"))
	{
		if(pStarter->pev->euser1 == pEntity)
		{
			pEntity->v.flags &= ~FL_FROZEN; //Unfreeze
			pEntity->v.rendermode = 0;
			pEntity->v.renderfx = 0;
			pEntity->v.renderamt = 255;  
			pStarter->pev->flags |= FL_KILLME; //Kill Starter
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You stopped charging your Superflame.\n"));
			pEntity->v.flags &= ~FL_FROZEN; //Unfreeze to be sure
		}
	}
}

void DoTheFlame(Vector vArea, edict_t *pOwner)
{
	edict_t *pSuperFlame;
    pSuperFlame = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    DispatchSpawn(pSuperFlame);
    pSuperFlame->v.origin = vArea;
    pSuperFlame->v.euser1 = pOwner;
    pSuperFlame->v.angles = Vector(0,0,0);
    pSuperFlame->v.velocity = Vector(0,0,0);
	pSuperFlame->v.takedamage = DAMAGE_NO;
	pSuperFlame->v.iuser4 = 100; //No of 1/2 seconds this thing lives.
	pSuperFlame->v.fuser4 = 0;
	pSuperFlame->v.health = 1;
	pSuperFlame->v.team = pOwner->v.team;
	pSuperFlame->v.movetype = MOVETYPE_NONE;
	pSuperFlame->v.nextthink = gpGlobals->time;
	pSuperFlame->v.framerate = 1.0;
	pSuperFlame->v.sequence = 0;
	pSuperFlame->v.gravity = 0.5;
	pSuperFlame->v.friction = 0.8;
	pSuperFlame->v.effects = EF_NODRAW;
	pSuperFlame->v.solid = SOLID_BBOX;
    SET_MODEL(pSuperFlame, ("models/autoop/airstrike.mdl"));
    UTIL_SetSize(VARS(pSuperFlame), Vector(0, 0, 0), Vector(0, 0, 0));
    pSuperFlame->v.classname = MAKE_STRING("weapon_ao_superflame");
}

bool SuperFlameThink(edict_t *pEntity)
{
	pEntity->v.iuser4 -= 1;

	if(pEntity->v.iuser4 <= 0)
	{
		pEntity->v.flags |= FL_KILLME;
		return 1;
	}

	//Check our owner is still a pyro etc.
	if((pEntity->v.euser1->v.playerclass != CLASS_PYRO) || (pEntity->v.euser1->v.team != pEntity->v.team))
	{
		pEntity->v.flags |= FL_KILLME;
		return 1;
	}

	//Check if we have to do the flame effect.
	if(pEntity->v.fuser4 <= gpGlobals->time)
	{
		EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "ambience/flameburst1.wav", 1, ATTN_NORM, 0, 100);

		pEntity->v.fuser4 = gpGlobals->time + 2; //When to do next flame effect
		
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_FIREFIELD);
			WRITE_COORD(pEntity->v.origin.x);
			WRITE_COORD(pEntity->v.origin.y);
			WRITE_COORD(pEntity->v.origin.z);
			WRITE_SHORT(150);
			WRITE_SHORT(m_Fire);
			WRITE_BYTE(60);
			WRITE_BYTE(TEFIRE_FLAG_LOOP|TEFIRE_FLAG_SOMEFLOAT); //|TEFIRE_FLAG_PLANAR
			WRITE_BYTE(100);
		MESSAGE_END();

		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_STREAK_SPLASH);
			WRITE_COORD(pEntity->v.origin.x);
			WRITE_COORD(pEntity->v.origin.y);
			WRITE_COORD(pEntity->v.origin.z - 40);
			WRITE_COORD(0);
			WRITE_COORD(0);
			WRITE_COORD(10);
			WRITE_BYTE(10);
			WRITE_SHORT(100);
			WRITE_SHORT(100);
			WRITE_SHORT(100);
		MESSAGE_END();

	}

	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 150)) != NULL)
	{
		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "player") == 0)
		{
			int Team1 = pScan->pev->team;
			int Team2 = pEntity->v.team;
			if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
			{
				if(pScan->pev->health > 10 && pScan->pev->takedamage != DAMAGE_NO)
				{
					TakeDamage(pScan->edict(),NULL,10,8);
					//pScan->pev->health -= 10;
				}
			}
		}
	}
	pEntity->v.nextthink = gpGlobals->time + 0.5;
	return 1;
}
