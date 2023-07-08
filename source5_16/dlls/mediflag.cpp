/*****************************************************\
*	Auto-OP Shodan
*
*	File: mediflag.cpp
*	Description: Medic's mediflag.
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
extern DLL_FUNCTIONS other_gFunctionTable;
extern int team_allies[4];

void DropFlag(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_MEDIC || pEntity->v.health <= 0)
		return;

	if(PlayerInfo[ENTINDEX(pEntity)].LastMovedFlag > gpGlobals->time)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You can't deploy your flag yet\n"));
		return;
	}

	PlayerInfo[ENTINDEX(pEntity)].LastMovedFlag = gpGlobals->time + 15.0;

	CBaseEntity *pFlag = NULL;
    while(pFlag = UTIL_FindEntityByClassname(pFlag, "ao_mediflag"))
    {
        if(pFlag->pev->euser1 == pEntity)
        {
			pFlag->pev->flags |= FL_KILLME;
		}
	}

	edict_t *pMediFlag = NULL;
    pMediFlag = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    DispatchSpawn(pMediFlag);
    pMediFlag->v.origin = pEntity->v.origin;
    pMediFlag->v.euser1 = pEntity;
    pMediFlag->v.angles = Vector(0,0,0);
    pMediFlag->v.velocity = Vector(0,0,0);
	pMediFlag->v.takedamage = DAMAGE_YES;
	pMediFlag->v.health = 100000;
	pMediFlag->v.team = pEntity->v.team;
	pMediFlag->v.movetype = MOVETYPE_TOSS;
	pMediFlag->v.nextthink = gpGlobals->time;
	pMediFlag->v.gravity = 1;
	pMediFlag->v.friction = 1;
	pMediFlag->v.sequence = 0;
	pMediFlag->v.framerate = 1.0;
	pMediFlag->v.solid = SOLID_BBOX;
    SET_MODEL(pMediFlag, "models/autoop/mediflag.mdl");
    UTIL_SetSize(VARS(pMediFlag), Vector(0, 0, 0), Vector(0, 0, 0));
    pMediFlag->v.classname = MAKE_STRING("ao_mediflag");

	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You deployed your mediflag.\n"));
	CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You deployed your mediflag.\n"));

	UTIL_LogPrintf( "[AUTOOP] %s placed a mediflag.\n", STRING(pEntity->v.netname));
}

bool MediFlagThink(edict_t *pEntity)
{
	if(pEntity->v.health <= 100000 - MEDI_FLAG_HEALTH)
	{
		DestroyFlag(pEntity, 1);
		return 1;
	}

	if(pEntity->v.euser1->v.team != pEntity->v.team)
	{
		DestroyFlag(pEntity, 0);
		return 1;
	}
	
	//First check our owner is still fit to have mines active.
	if(pEntity->v.euser1->v.playerclass != CLASS_MEDIC)
	{
		DestroyFlag(pEntity, 1);
		return 1;
	}

	if(gpGlobals->time >= pEntity->v.fuser1)
	{
		CBaseEntity *pScan = NULL;
		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, MEDI_FLAG_RADIUS)) != NULL)
		{
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{
				int Team1 = pScan->pev->team;
				int Team2 = pEntity->v.team;
				if ((Team1 == Team2) || (team_allies[(Team1-1)] & (1<<(Team2-1))))
				{
					if(pScan->pev->health < 100 && pScan->pev->health > 0 )
					{
						EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "items/medshot4.wav", 1, ATTN_NORM, 0, 100);
						pScan->pev->health += 10;
					}
				}
			}
		}
		pEntity->v.fuser1 = gpGlobals->time + 1;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void DestroyFlag(edict_t *pEntity, int iDetonate)
{
	if(iDetonate == 1)
	{
		edict_t *pBomb = NULL;
		pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
		DispatchSpawn(pBomb);
		pBomb->v.origin.x = pEntity->v.origin.x;
		pBomb->v.origin.y = pEntity->v.origin.y;
		pBomb->v.origin.z = pEntity->v.origin.z;
		pBomb->v.velocity = Vector(0,0,0);
		pBomb->v.dmg = 50;
		pBomb->v.owner = pEntity->v.euser1;
		pBomb->v.effects |= EF_NODRAW;
		pBomb->v.nextthink = gpGlobals->time;
		pBomb->v.classname = MAKE_STRING("ao_mediflag_explosion");
	}

	pEntity->v.flags |= FL_KILLME;

	UTIL_LogPrintf( "[AUTOOP] A mediflag was destroyed.\n");
}