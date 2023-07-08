/*****************************************************\
*	Auto-OP Shodan
*
*	File: lights.cpp
*	Description: AutoOP Light entity
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
#include "commands.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;

bool LightThink(edict_t *pEntity)
{
	if(gpGlobals->time > pEntity->v.fuser1)
	{
		MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_DLIGHT);
			WRITE_COORD(pEntity->v.origin.x);
			WRITE_COORD(pEntity->v.origin.y);
			WRITE_COORD(pEntity->v.origin.z);
			WRITE_BYTE(25);
			WRITE_BYTE(pEntity->v.vuser1.x);
			WRITE_BYTE(pEntity->v.vuser1.y);
			WRITE_BYTE(pEntity->v.vuser1.z);
			WRITE_BYTE(250);
			WRITE_BYTE(0);
		MESSAGE_END();

		pEntity->v.fuser1 = gpGlobals->time + 24.9;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;

	return 1;
}