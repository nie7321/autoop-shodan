/*****************************************************\
*	Auto-OP Shodan
*
*	File: stealth.cpp
*	Description: Code for the spies stealth camo.
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

void ActivateStealth(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SPY)
		return;

	if(pEntity->v.health <= 5)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Health too low to use stealth.\n"));
		return;
	}

	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Stealth camoflauge activated...\n"));

	PlayerInfo[ENTINDEX(pEntity)].StealthOn = 1;

	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "player/geiger6.wav", 1, ATTN_NORM, 0, 130);
	pEntity->v.health -= 1;

	edict_t *pStealthEnt = NULL;
	pStealthEnt = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));

	pStealthEnt->v.origin = Vector(0,0,-4000);
	pStealthEnt->v.velocity = Vector(0,0,0);
	pStealthEnt->v.takedamage = DAMAGE_NO;
	pStealthEnt->v.health = 100;
	pStealthEnt->v.effects = EF_NODRAW;
	pStealthEnt->v.euser1 = pEntity;
	pStealthEnt->v.team = pEntity->v.team;

	DispatchSpawn(pStealthEnt);

	pStealthEnt->v.classname = MAKE_STRING("ao_stealthent");
	pStealthEnt->v.nextthink = gpGlobals->time + 1;

	CBaseEntity *pFadePlayer = UTIL_PlayerByIndex(ENTINDEX(pEntity));
	UTIL_ScreenFade(pFadePlayer,Vector(20,141,7),10,10,200,FFADE_STAYOUT);

	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("r_drawviewmodel 0\n") );

	UTIL_LogPrintf( "[AUTOOP] %s activated stealth camouflage.\n", STRING(pEntity->v.netname));

}

bool StealthEntThink(edict_t *pEntity)
{
	if(PlayerInfo[ENTINDEX(pEntity->v.euser1)].StealthOn == 0)
	{
		pEntity->v.flags = FL_KILLME;
		return 1;
	}

	//Sucks owners health and makes them partially transparent.
	if(pEntity->v.euser1->v.health > STEALTH_DRAIN && pEntity->v.euser1->v.team == pEntity->v.team)
	{
		EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "player/geiger6.wav", 1, ATTN_NORM, 0, 130);
		pEntity->v.euser1->v.health -= STEALTH_DRAIN;
		if(pEntity->v.euser1->v.velocity == Vector(0,0,0))
		{
			pEntity->v.euser1->v.rendermode = 1;
			pEntity->v.euser1->v.renderamt = 0;
		} else {
			pEntity->v.euser1->v.rendermode = 1;
			pEntity->v.euser1->v.renderamt = 20;
		}

		//Make sure their screen fades and viewmodel is hidden
		CBaseEntity *pFadePlayer = UTIL_PlayerByIndex(ENTINDEX(pEntity->v.euser1));
		UTIL_ScreenFade(pFadePlayer,Vector(20,141,7),10,10,200,FFADE_STAYOUT);
		CLIENT_COMMAND ( pEntity->v.euser1, UTIL_VarArgs("r_drawviewmodel 0\n") );

		//every 2 seconds update targeters
		if(pEntity->v.iuser4 >= 2)
		{
			pEntity->v.iuser4 = 0; //reset counter

			//Spin through all players and draw targeters on the stealth ones
			for(int i = 1;i <= gpGlobals->maxClients; i++)
			{
				if((PlayerInfo[i].StealthOn == 1) && (i != ENTINDEX(pEntity->v.euser1)))
				{
					//draw targeter on this player
					short sprPredAimRed = PRECACHE_MODEL( "sprites/autoop/predaim_red.spr");
					MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, pEntity->v.euser1);
						WRITE_BYTE(TE_PLAYERATTACHMENT);
						WRITE_BYTE(i);
						WRITE_COORD(5);
						WRITE_SHORT(sprPredAimRed);
						WRITE_SHORT(20);
					MESSAGE_END();
				}
			}
		}

		CBaseEntity *pScan = NULL;
		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.euser1->v.origin, 1000)) != NULL)
		{
			//Is it an entity we are bothered about? (Players and grens)
			if (strcmp(STRING(pScan->pev->classname), "building_sentrygun") == 0)
			{
				//Put a sprite on the sentryguns
				short sprPredAimYel = PRECACHE_MODEL( "sprites/autoop/predaim_yel.spr");
				MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, pEntity->v.euser1);
					WRITE_BYTE(TE_SPRITE);
					WRITE_COORD(pScan->pev->origin.x);
					WRITE_COORD(pScan->pev->origin.y);
					WRITE_COORD(pScan->pev->origin.z);
					WRITE_SHORT(sprPredAimYel);
					WRITE_BYTE(10);
					WRITE_BYTE(200);
				MESSAGE_END();
			}
		}

	} else {

		DeactivateStealth(pEntity->v.euser1);
		pEntity->v.flags = FL_KILLME;
		return 1;
	}
	// Add to timer
	pEntity->v.iuser4 += 1;

	pEntity->v.nextthink = gpGlobals->time + 1;
	return 1;
}

void DeactivateStealth(edict_t *pEntity)
{
	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "player/geiger6.wav", 1, ATTN_NORM, 0, 130);
	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Stealth camoflauge deactivated...\n"));

	PlayerInfo[ENTINDEX(pEntity)].StealthOn = 0;

	pEntity->v.rendermode = 1;
	pEntity->v.renderamt = 255;

	CBaseEntity *pFadePlayer = UTIL_PlayerByIndex(ENTINDEX(pEntity));
	UTIL_ScreenFade(pFadePlayer,Vector(20,141,7),1,0.5,200,0);

	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("r_drawviewmodel 1\n") );

	UTIL_LogPrintf( "[AUTOOP] %s deactivated stealth camouflage.\n", STRING(pEntity->v.netname));
}