/*****************************************************\
*	Auto-OP Shodan
*
*	File: flashbang.cpp
*	Description: Flashbang grenade functions.
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

void TossFlashBang(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SCOUT || pEntity->v.health < 1)
		return;

	if(PlayerInfo[ENTINDEX(pEntity)].FBAmmo > 0)
	{
		PlayerInfo[ENTINDEX(pEntity)].FBAmmo -= 1;

		UTIL_LogPrintf( "[AUTOOP] %s threw a Flashbang\n", STRING(pEntity->v.netname));

		edict_t *pFlash;
		UTIL_MakeVectors(pEntity->v.v_angle);
		pFlash = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
		DispatchSpawn(pFlash);
		pFlash->v.origin = pEntity->v.origin;
		pFlash->v.owner = pEntity;
		pFlash->v.angles = Vector(0, pEntity->v.v_angle.y, 0);
		pFlash->v.velocity = pEntity->v.deadflag ? Vector(0, 0, 32) : pEntity->v.velocity + gpGlobals->v_forward*400 + Vector(0, 0, 32);
		pFlash->v.takedamage = DAMAGE_NO;
		pFlash->v.health = 1;
		pFlash->v.fuser1 = gpGlobals->time + 4;
		pFlash->v.movetype = MOVETYPE_TOSS;
		pFlash->v.nextthink = gpGlobals->time;
		pFlash->v.gravity = 1;
		pFlash->v.team = pEntity->v.team;
		pFlash->v.friction = 1;
		pFlash->v.solid = SOLID_BBOX;
		SET_MODEL(pFlash, "models/autoop/flashbang.mdl");
		pFlash->v.rendercolor = Vector(255, 0, 0);
		UTIL_SetSize(VARS(pFlash), Vector(0, 0, 0), Vector(0, 0, 0));
		pFlash->v.classname = MAKE_STRING("weapon_ao_flashbang");
	}
}

bool FBThink(edict_t *pEntity)
{
	if(gpGlobals->time >= pEntity->v.fuser1)
	{
		CBaseEntity *pScan = NULL;

		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 600)) != NULL)
		{
			//Is it an entity we are bothered about? (Player)
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{
				if(pScan->pev->team != pEntity->v.team)
				{
					UTIL_ScreenFade(pScan,Vector(255,255,255),5,3,100,FFADE_IN);
				} else {
					UTIL_ScreenFade(pScan,Vector(255,255,255),5,3,50,FFADE_IN);
				}
			}
		}

		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 200)) != NULL)
		{
			//Is it an entity we are bothered about? (Player)
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{
				if(pScan->pev->team != pEntity->v.team)
				{
					UTIL_ScreenFade(pScan,Vector(255,255,255),5,3,255,FFADE_IN);
				} else {
					UTIL_ScreenFade(pScan,Vector(255,255,255),5,3,100,FFADE_IN);
				}
			}
		}
		EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/concgren_blast1.wav", 1, ATTN_NORM, 0, PITCH_LOW);
		pEntity->v.flags = FL_KILLME;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

