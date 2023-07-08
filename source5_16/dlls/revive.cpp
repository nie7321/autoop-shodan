/*****************************************************\
*	Auto-OP Shodan
*
*	File: revive.cpp
*	Description: Medics Revival Thing.
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

void RevivePlayer(edict_t *pEntity)
{
	//Revives a dead player and gives them guns.

	if(pEntity->v.deadflag == DEAD_NO)
		return;

	pEntity->v.deadflag = DEAD_NO;
	pEntity->v.health = 30;
	pEntity->v.solid = SOLID_SLIDEBOX;
	pEntity->v.takedamage = DAMAGE_AIM;
	pEntity->v.view_ofs = Vector(0,0,28);
	ReSupplyPlayer(pEntity); //Give them guns back
}

void MediRevive(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_MEDIC || pEntity->v.health <= 0)
		return;

	if(PlayerInfo[ENTINDEX(pEntity)].AlreadyHealed == 1)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot revive more\nthan once per life\n"));
		return;
	}

	//Search for dead players...
	//Scan to make sure we are not in a func
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		if (strcmp(STRING(pScan->pev->classname), "player") == 0)
		{
			if(pScan->pev->deadflag != DEAD_NO && pScan->pev->team == pEntity->v.team && pScan->pev->playerclass != CLASS_CIVILIAN)
			{
				PlayerInfo[ENTINDEX(pEntity)].AlreadyHealed = 1;
				RevivePlayer(pScan->edict());
				return;
			}

		}
	}
	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Found noone to revive.\n"));
}

void ReSupplyPlayer(edict_t *pEntity)
{
	if(pEntity->v.playerclass == CLASS_SCOUT)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(4,pEntity);
		TFXGiveWeapon(8,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_SNIPER)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(12,pEntity);
		TFXGiveWeapon(13,pEntity);
		TFXGiveWeapon(8,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_SOLDIER)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(1,pEntity);
		TFXGiveWeapon(4,pEntity);
		TFXGiveWeapon(6,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_DEMOMAN)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(4,pEntity);
		TFXGiveWeapon(16,pEntity);
		TFXGiveWeapon(17,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_MEDIC)
	{
		TFXGiveWeapon(3,pEntity);
		TFXGiveWeapon(4,pEntity);
		TFXGiveWeapon(9,pEntity);
		TFXGiveWeapon(6,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_HWGUY)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(2,pEntity);
		TFXGiveWeapon(4,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_PYRO)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(4,pEntity);
		TFXGiveWeapon(14,pEntity);
		TFXGiveWeapon(15,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_SPY)
	{
		TFXGiveWeapon(5,pEntity);
		TFXGiveWeapon(8,pEntity);
		TFXGiveWeapon(10,pEntity);
		TFXGiveWeapon(6,pEntity);
		return;
	}

	if(pEntity->v.playerclass == CLASS_ENGINEER)
	{
		TFXGiveWeapon(18,pEntity);
		TFXGiveWeapon(11,pEntity);
		TFXGiveWeapon(6,pEntity);
		return;
	}
}

