/*****************************************************\
*	Auto-OP Shodan
*
*	File: 3rdperson.cpp
*	Description: Thirdperson functions.
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
#include "3rdperson.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];

extern TFXFeatureEnabled_s Features;
extern FILE *fp;

void Start3rdPerson(edict_t *pEntity)
{
	if((pEntity->v.team > 4) || (pEntity->v.team < 1))
		return;

	CBaseEntity *psCamera = NULL;
    while(psCamera = UTIL_FindEntityByClassname(psCamera, "ao_3rdperson_camera"))
    {
        if(psCamera->pev->iuser3 == ENTINDEX(pEntity))
			return;
	}

	edict_t *pCamera = NULL;
	pCamera = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	DispatchSpawn(pCamera);
	pCamera->v.origin = pEntity->v.origin;
	pCamera->v.angles = pEntity->v.v_angle;
	pCamera->v.takedamage = DAMAGE_NO;
	pCamera->v.solid = SOLID_NOT;
	pCamera->v.iuser3 = ENTINDEX(pEntity);
	pCamera->v.movetype = MOVETYPE_NOCLIP;
	pCamera->v.classname = MAKE_STRING("ao_3rdperson_camera");
	pCamera->v.nextthink = gpGlobals->time + 0.1;
    pCamera->v.renderamt = 0;
    pCamera->v.rendermode = kRenderTransColor;
    pCamera->v.renderfx = kRenderFxNone;
	SET_MODEL(pCamera, "models/nail.mdl");

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Start3rdPerson(): Setting users view.\n");
		fclose(fp);
	}
	SET_VIEW(pEntity, pCamera);
}

bool CameraThink(edict_t *pEntity)
{
	edict_t *pPlayer = INDEXENT(pEntity->v.iuser3);

	if (!pPlayer || (FStrEq(STRING(pPlayer->v.netname),"")))
	{
		pEntity->v.flags |= FL_KILLME;
		return 1;
	}

	//Draw little traceline from player to about 150 cells behind him
	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pPlayer->v.v_angle );

	v_src = pPlayer->v.origin + pPlayer->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * -150;

	UTIL_TraceLine( v_src, v_dest, ignore_monsters, pPlayer->v.pContainingEntity, &tr);

	pEntity->v.origin = tr.vecEndPos;
	pEntity->v.angles = pPlayer->v.v_angle;

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Camerathink(): Setting users view.\n");
		fclose(fp);
	}

	SET_VIEW(pPlayer, pEntity);
	pEntity->v.nextthink = gpGlobals->time;// + 0.1;
	return 1;
}

void End3rdPerson(edict_t *pPlayer)
{
	CBaseEntity *pCamera = NULL;
    while(pCamera = UTIL_FindEntityByClassname(pCamera, "ao_3rdperson_camera"))
    {
        if(pCamera->pev->iuser3 == ENTINDEX(pPlayer))
        {
			if(pPlayer)
			{
				if(Features.Debug >= 1)
				{
					fp=fopen("sdebug.log","a");
					fprintf(fp, "End3rdPerson(): Resetting users view.\n");
					fclose(fp);
				}
				SET_VIEW(pPlayer, pPlayer);
			}
			pCamera->pev->flags |= FL_KILLME;
		}
	}
}