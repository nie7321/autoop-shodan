/*****************************************************\
*	Auto-OP Shodan
*
*	File: proxmine.cpp
*	Description: Proximity mine functions for spy.
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

int PROXIMITY_RADIUS = 100;

void DropMine(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SPY)
		return;

	//Scan to make sure we are not in a func
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place a mine here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place a mine here.\n"));
			return;
		}

		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "weapon_ao_proximity") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place a mine here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place a mine here.\n"));
			return;
		}

		if (strcmp(STRING(pScan->pev->classname), "item_tfgoal") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place a mine here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place a mine here.\n"));
			return;
		}
	}

	if( PlayerInfo[ENTINDEX(pEntity)].ProxMineAmmo <= 0)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You have no proximity mines left.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have no proximity mines left.\n"));
		return;
	}
	PlayerInfo[ENTINDEX(pEntity)].ProxMineAmmo -= 1;

	TraceResult tr;
	UTIL_TraceLine( pEntity->v.origin, Vector(pEntity->v.origin.x,pEntity->v.origin.y, pEntity->v.origin.z - 200), ignore_monsters, pEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			if(UTIL_PointContents(tr.vecEndPos) != CONTENTS_SKY)
			{
				//Create a mine
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/mine_deploy.wav", 1, ATTN_NORM, 0, 140);
				edict_t *pProx = NULL;
    			pProx = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    			DispatchSpawn(pProx);
    			pProx->v.origin = tr.vecEndPos;
    			pProx->v.euser1 = pEntity;
    			pProx->v.angles = Vector(0,0,0);
    			pProx->v.velocity = Vector(0,0,0);
				pProx->v.takedamage = DAMAGE_NO;
				pProx->v.health = 1;
				pProx->v.fuser1 = gpGlobals->time + 3;
				pProx->v.iuser1 = 0; //1 when charged.
				pProx->v.team = pEntity->v.team;
				pProx->v.movetype = MOVETYPE_BOUNCE;
				pProx->v.nextthink = gpGlobals->time;
				pProx->v.gravity = 0.5;
				pProx->v.friction = 0.8;
				pProx->v.solid = SOLID_BBOX;
    			SET_MODEL(pProx, "models/autoop/proxmine.mdl");
    			UTIL_SetSize(VARS(pProx), Vector(0, 0, 0), Vector(0, 0, 0));
    			pProx->v.classname = MAKE_STRING("weapon_ao_proximity");

				UTIL_LogPrintf( "[AUTOOP] %s dropped a proximity mine.\n", STRING(pEntity->v.netname));
			}
		}
	}
}

void DetonateMines(edict_t *pEntity, int iDetonate)
{
	UTIL_LogPrintf( "[AUTOOP] %s's mines blew up.\n", STRING(pEntity->v.netname));

	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_proximity"))
    {
        if(pMine->pev->euser1 == pEntity)
        {
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your mines were destroyed.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your mines were destroyed.\n"));
            pMine->pev->flags |= FL_KILLME;

			if (iDetonate == 1)
			{
				edict_t *pBomb;
				pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
				DispatchSpawn(pBomb);
				pBomb->v.origin = pMine->pev->origin;
				pBomb->v.velocity = Vector(0,0,0);
				pBomb->v.dmg = 200;
				pBomb->v.owner = pEntity;
				pBomb->v.effects |= EF_NODRAW;
				pBomb->v.nextthink = gpGlobals->time;
				pBomb->v.classname = MAKE_STRING("weapon_ao_minedetonate");
			}
		}
	}
	return;
}

bool ProxMineThink(edict_t *pEntity)
{
	if(pEntity->v.euser1->v.team != pEntity->v.team)
	{
		DetonateMines(pEntity->v.euser1, 0);
		return 1;
	}
	
	//First check our owner is still fit to have mines active.
	if(pEntity->v.euser1->v.health <= 0 || pEntity->v.euser1->v.playerclass != CLASS_SPY)
	{
		//ADDED: If not charged dont blow up
		if(pEntity->v.iuser1 == 1)
		{
			DetonateMines(pEntity->v.euser1, 0); //Change: dont let mines detonate when you die.
		} else {
			DetonateMines(pEntity->v.euser1, 0);
		}
		return 1;
	}

	if(gpGlobals->time >= pEntity->v.fuser1 && pEntity->v.iuser1 == 0)
	{
		pEntity->v.iuser1 = 1; //Charged
	}

	if(pEntity->v.iuser1 == 1)
	{
		//Scan the area for enemy players and enemy grenades.
		CBaseEntity *pScan = NULL;
		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, PROXIMITY_RADIUS)) != NULL)
		{
			//Is it an entity we are bothered about? (Players and grens)
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{
				if ((pScan->pev->movetype != MOVETYPE_NOCLIP) && (UTIL_SpectCheck(pScan->edict()) == 0))
				{
					int Team1 = pScan->pev->team;
					int Team2 = pEntity->v.team;
					if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
					{
						//If a scout is crawling and goes near the mine
						//he disarms it.
						if((pScan->pev->playerclass == CLASS_SCOUT) || (pScan->pev->playerclass == CLASS_ENGINEER) || (pScan->pev->playerclass == CLASS_SPY))
						{
							if(pScan->pev->button & IN_DUCK)
							{
								pEntity->v.flags |= FL_KILLME;
								CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "A mine got disarmed.\n"));
								CLIENT_PRINTF( pEntity->v.euser1, print_console,UTIL_VarArgs( "A mine got disarmed.\n"));
								CLIENT_PRINTF( pScan->edict(), print_center, UTIL_VarArgs( "You disarmed a proximity mine.\n"));
								pEntity->v.nextthink = gpGlobals->time + 0.1;
								return 1;
							}
						}

						//its not... DETONATE!
						CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "A mine got triggered.\n"));
						CLIENT_PRINTF( pEntity->v.euser1, print_console,UTIL_VarArgs( "A mine got triggered.\n"));
						pEntity->v.flags |= FL_KILLME;

						edict_t *pBomb;
						pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
						DispatchSpawn(pBomb);
						pBomb->v.origin = pEntity->v.origin;
						pBomb->v.velocity = Vector(0,0,0);
						pBomb->v.dmg = 200;
						pBomb->v.owner = pEntity->v.euser1;
						pBomb->v.effects |= EF_NODRAW;
						pBomb->v.nextthink = gpGlobals->time;
						pBomb->v.classname = MAKE_STRING("weapon_ao_minedetonate");
					}
				}
			}
		}
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

