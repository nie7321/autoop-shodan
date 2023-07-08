/*****************************************************\
*	Auto-OP Shodan
*
*	File: messages.cpp
*	Description: The message flag.
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

void DropMailFlag(edict_t *pEntity, const char *Message)
{
	if((UTIL_SpectCheck(pEntity) == 1) || (pEntity->v.health <= 0))
		return;

	if(PlayerInfo[ENTINDEX(pEntity)].LastMovedMessage > gpGlobals->time)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You can't deploy a message at the moment.\n"));
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "You can't deploy a message at the moment.\n"));
		return;
	}

	PlayerInfo[ENTINDEX(pEntity)].LastMovedMessage = gpGlobals->time + 15.0;

	CBaseEntity *pFlag = NULL;
    while(pFlag = UTIL_FindEntityByClassname(pFlag, "ao_mailflag"))
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

	pMediFlag->v.angles.y = pEntity->v.v_angle.y;
	pMediFlag->v.angles.y += 180; // Face the player
	if(pMediFlag->v.angles.y >= 360) pMediFlag->v.angles.z -= 360;

	char Checked[41];
	strncpy(Checked,Message,41); //Max 40 characters
	Checked[41] = '\0';

	pMediFlag->v.message = ALLOC_STRING(Checked);
	pMediFlag->v.sequence = 0;
	pMediFlag->v.framerate = 1.0;
	pMediFlag->v.solid = SOLID_BBOX;
    SET_MODEL(pMediFlag, "models/autoop/mailflag.mdl");
    UTIL_SetSize(VARS(pMediFlag), Vector(0, 0, 0), Vector(0, 0, 0));
    pMediFlag->v.classname = MAKE_STRING("ao_mailflag");

	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You left a message.\n"));
	CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You left a message.\n"));

	UTIL_LogPrintf( "[AUTOOP] %s placed a mesageflag.\n", STRING(pEntity->v.netname));
}

bool MailFlagThink(edict_t *pEntity)
{
	if(gpGlobals->time >= pEntity->v.fuser1)
	{
		CBaseEntity *pScan = NULL;
		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 50)) != NULL)
		{
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{
				//Send message to client here
				CLIENT_PRINTF( pScan->edict(), print_center, UTIL_VarArgs( "%s\n- %s",STRING(pEntity->v.message),STRING(pEntity->v.euser1->v.netname)));
			}
		}
		pEntity->v.fuser1 = gpGlobals->time + 1;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void DestroyMailFlag(edict_t *pEntity)
{
	CBaseEntity *pFlag = NULL;
    while(pFlag = UTIL_FindEntityByClassname(pFlag, "ao_mailflag"))
    {
        if(pFlag->pev->euser1 == pEntity)
        {
			pFlag->pev->flags |= FL_KILLME;
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your message flag was destroyed.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your message flag was destroyed.\n"));
			return;
		}
	}
	CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You have no message flag.\n"));
	CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have no message flag.\n"));
}
