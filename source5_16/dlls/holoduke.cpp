/*****************************************************\
*	Auto-OP Shodan
*
*	File: holoduke.cpp
*	Description: Snipers hologram clone
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

void DeployHoloDuke(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SNIPER)
		return;

	if( pEntity->v.armorvalue < 5 )
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough armor to activate holosnipe.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough armor to activate holosnipe.\n"));
		return;
	}

	CBaseEntity *pScan = NULL;
    while(pScan = UTIL_FindEntityByClassname(pScan, "ao_holosnipe"))
    {
        if(pScan->pev->euser1 == pEntity)
        {
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You must deactivate your holosnipe\nbefore you can activate him!\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You must deactivate your holosnipe before you can activate him!\n"));
			return;
		}
	}
	
	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "buttons/button9.wav", 1, ATTN_NORM, 0, 100);
	
	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Holosnipe activated.\n"));
	CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Holosnipe activated.\n"));

	edict_t *pHoloSnipe;
	pHoloSnipe = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	DispatchSpawn(pHoloSnipe);
	pHoloSnipe->v.origin = pEntity->v.origin;
	pHoloSnipe->v.angles = pEntity->v.v_angle;
	pHoloSnipe->v.velocity = Vector(0, 0, 0);
	pHoloSnipe->v.euser1 = pEntity;
	pHoloSnipe->v.takedamage = DAMAGE_NO;
	pHoloSnipe->v.health = 100000;
	pHoloSnipe->v.movetype = MOVETYPE_NONE;
	pHoloSnipe->v.fuser1 = gpGlobals->time;
	pHoloSnipe->v.solid = SOLID_BBOX;
	pHoloSnipe->v.team = pEntity->v.team;
	SET_MODEL(pHoloSnipe, "models/player/sniper/sniper2.mdl");
	pHoloSnipe->v.colormap = pEntity->v.colormap;
	UTIL_SetSize(VARS(pHoloSnipe), Vector(0,0,0), Vector(0,0,0));
	pHoloSnipe->v.classname = MAKE_STRING("ao_holosnipe");
	pHoloSnipe->v.nextthink = gpGlobals->time + 0.1;

	UTIL_LogPrintf( "[AUTOOP] %s deployed a holosnipe.\n", STRING(pEntity->v.netname));
}

bool HoloDukeThink(edict_t *pEntity)
{
	if(pEntity->v.euser1->v.team != pEntity->v.team)
	{
		KillHoloDuke(pEntity);
		return 1;
	}
	
	//First check our owner is still fit to have mines active.
	if(pEntity->v.euser1->v.playerclass != CLASS_SNIPER)
	{
		KillHoloDuke(pEntity);
		return 1;
	}

	//Should we deduct armor?
	if(pEntity->v.fuser1 < gpGlobals->time)
	{
		pEntity->v.euser1->v.armorvalue -= 1;
		pEntity->v.fuser1 = gpGlobals->time + 1;
	}

	//Look the same direction as our owner
	pEntity->v.angles.y = pEntity->v.euser1->v.v_angle.y;

	pEntity->v.nextthink = gpGlobals->time + 0.1;

	return 1;
}

void KillHoloDuke(edict_t *pEntity)
{
	if(pEntity->v.euser1)
	{
		CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "Holosnipe deactivated.\n"));
		EMIT_SOUND_DYN2(pEntity->v.euser1, CHAN_VOICE, "buttons/button9.wav", 1, ATTN_NORM, 0, 100);
	}

	pEntity->v.flags = FL_KILLME;
}

void DeactivateHoloDuke(edict_t *pEntity)
{
	CBaseEntity *pScan = NULL;
    while(pScan = UTIL_FindEntityByClassname(pScan, "ao_holosnipe"))
    {
        if(pScan->pev->euser1 == pEntity)
        {
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Holosnipe deactivated.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Holosnipe deactivated.\n"));
			pScan->pev->flags = FL_KILLME;
			return;
		}
	}
}


