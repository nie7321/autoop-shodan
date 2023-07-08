/*****************************************************\
*	Auto-OP Shodan
*
*	File: nightgame.cpp
*	Description: Functions for Night Time (NAO) Maps
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

extern int NightTime;

void MakePlayersShine( )
{
	CBaseEntity *pPlayer = NULL;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) 
	{
		pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer)
		{
			if(pPlayer->pev->team > 0 && pPlayer->pev->team < 5 && pPlayer->pev->movetype != MOVETYPE_NOCLIP)
			{
				if(PlayerInfo[i].FlashLightOff == 0)
				{
					pPlayer->pev->effects = EF_DIMLIGHT;
				} else {
					pPlayer->pev->effects = 0;
				}
			}
		}
	}
}

void MakePlayersDull( )
{
	CBaseEntity *pPlayer = NULL;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) 
	{
		pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer)
		{
			if(pPlayer->pev->health > 0 && pPlayer->pev->team > 0 && pPlayer->pev->team < 5 && pPlayer->pev->movetype != MOVETYPE_NOCLIP)
			{
				pPlayer->pev->effects = 0;
			}
		}
	}
}

void NAOChecks( )
{
	//---------Night Time Map Check
	int IsNight = strnicmp( STRING(gpGlobals->mapname), "nao" , 3 );//look for nao on the map name
	if(IsNight == 0) //Is it a night map?
	{
		NightTime = 1;
		CVAR_SET_FLOAT ("sv_allowdownload",0);
	} else {
		NightTime = 0;
	}

	//---------sv_zmax check
	int zmax = (int)CVAR_GET_FLOAT("sv_zmax");
	if (zmax == 1){
		CVAR_SET_FLOAT ("sv_zmax",4096);
	}
	if (zmax == 2){
		CVAR_SET_FLOAT ("sv_zmax",5800);
	}
	if (zmax == 3){
		CVAR_SET_FLOAT ("sv_zmax",8192);
	}
}

void BLDChecks( )
{
	int IsNight = strnicmp( STRING(gpGlobals->mapname), "bld" , 3 );//look for nao on the map name
	if(IsNight == 0) //Is it a night map?
		CVAR_SET_FLOAT ("sv_allowdownload",0);
}

