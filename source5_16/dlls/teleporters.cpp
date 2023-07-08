/*****************************************************\
*	AutoOP Shodan
*
*	File: teleporters.cpp
*	Description: Functions for creating teleporters.
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
#include "player.h"

extern AOPlayerInfo_s PlayerInfo[];
extern DLL_FUNCTIONS other_gFunctionTable;

void AdmCreateTeleStart(edict_t *pAdmin, const char *szName)
{
	if(FStrEq(szName,""))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a name for the teleporter.\n"));
		return;
	}
	
	edict_t *pTeleporter = NULL;

	pTeleporter = CREATE_NAMED_ENTITY(MAKE_STRING("trigger_teleport"));

	pTeleporter->v.origin = pAdmin->v.origin;
	pTeleporter->v.angles = pAdmin->v.angles;
	pTeleporter->v.target = ALLOC_STRING(szName);

	SET_MODEL(pTeleporter,"sprites/enter1.spr");

	DispatchSpawn(pTeleporter);

	pTeleporter->v.effects = 0;
	pTeleporter->v.renderamt = 100;
	pTeleporter->v.rendercolor = Vector(0,0,0);
	pTeleporter->v.framerate = 1.0;
	pTeleporter->v.scale = 1;
	pTeleporter->v.rendermode = 5;

	CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Teleporter created.\n"));
}

void AdmCreateTeleEnd(edict_t *pAdmin, const char *szName)
{
	if(FStrEq(szName,""))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a name for the teleporter.\n"));
		return;
	}
	
	//Make sure this destination doesnt already exist.
	CBaseEntity *pScan = NULL;
    while(pScan = UTIL_FindEntityByClassname(pScan, "info_teleport_destination"))
    {
		if(FStrEq(STRING(pScan->pev->targetname),szName))
		{
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Destination already exists.\n"));
			return;
		}
	}

	edict_t *pTeleEnd = NULL;

	pTeleEnd = CREATE_NAMED_ENTITY(MAKE_STRING("info_teleport_destination"));

	pTeleEnd->v.origin = pAdmin->v.origin;
	pTeleEnd->v.angles = pAdmin->v.angles;
	pTeleEnd->v.targetname = ALLOC_STRING(szName);

	SET_MODEL(pTeleEnd,"sprites/exit1.spr");

	DispatchSpawn(pTeleEnd);
	pTeleEnd->v.renderamt = 100;
	pTeleEnd->v.rendercolor = Vector(0,0,0);
	pTeleEnd->v.framerate = 1.0;
	pTeleEnd->v.scale = 1;
	pTeleEnd->v.rendermode = 5;
	pTeleEnd->v.effects = 0;

	CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Teleporter destination created.\n"));
}

void AdmKillTeleStructure(edict_t *pAdmin, const char *szName)
{
	if(FStrEq(szName,""))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a name for the teleporter.\n"));
		return;
	}
	
	CBaseEntity *pScan = NULL;
    while(pScan = UTIL_FindEntityByClassname(pScan, "info_teleport_destination"))
    {
		if(FStrEq(STRING(pScan->pev->targetname),szName))
		{
			pScan->pev->flags = FL_KILLME;
		}
	}

    while(pScan = UTIL_FindEntityByClassname(pScan, "trigger_teleport"))
    {
		if(FStrEq(STRING(pScan->pev->target),szName))
		{
			pScan->pev->flags = FL_KILLME;
		}
	}

	CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Teleporters killed.\n"));
}