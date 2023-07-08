/*****************************************************\
*	Auto-OP Shodan
*
*	File: leveleditor.cpp
*	Description: Code for entity manipulation
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
#include "leveleditor.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern DLL_FUNCTIONS other_gFunctionTable;

extern TFXFeatureEnabled_s Features;
extern FILE *fp;

void SelectEntity(edict_t *pEntity)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "SelectEntity(): Player selected an entity.\n");
		fclose(fp);
	}

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 4000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if(IsValidEnt(tr.pHit) == 1)
		{
			if(FindFreeSlot(pEntity) > 0)
			{
				//Store its original render settings
				tr.pHit->v.iuser1 = tr.pHit->v.rendermode;
				tr.pHit->v.fuser1 = tr.pHit->v.renderamt;
				tr.pHit->v.iuser3 = tr.pHit->v.renderfx;
				tr.pHit->v.vuser1 = tr.pHit->v.rendercolor;

				//Colour it here
				char entity_model[64];
				strcpy(entity_model, STRING(tr.pHit->v.model));
				if (entity_model[0] == '*')
				{
					//colour brush


					
					tr.pHit->v.rendermode = 1;
					tr.pHit->v.renderamt = 255;
					tr.pHit->v.rendercolor.x = 213;
					tr.pHit->v.rendercolor.y = 0;
					tr.pHit->v.rendercolor.z = 0;
				} else {
					//colour point
					tr.pHit->v.rendermode = kRenderNormal;
					tr.pHit->v.renderfx = kRenderFxGlowShell;
					tr.pHit->v.renderamt = 25;  
					tr.pHit->v.rendercolor.x = 213;
					tr.pHit->v.rendercolor.y = 0;
					tr.pHit->v.rendercolor.z = 0;
				}

				PlayerInfo[ENTINDEX(pEntity)].Selected.SelectedEnts[FindFreeSlot(pEntity)] = ENTINDEX(tr.pHit);
	
			} else {
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "All of your workslots are in use.\nDeselect something and try again.\n"));
			}
		}
	}

	return;
}

int IsValidEnt(edict_t *pEntity)
{
	if(Features.EditorProtection == 0)
		return 1; //Bypass protection if the CVAR is off.
	
	//Returns 1 is its selectable, 0 if it isn't..

	if(pEntity->v.rendercolor.x == 213)
		return 0; //Already selected.

	if (strcmp(STRING(pEntity->v.classname), "player") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "func_nobuild") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "building_sentrygun") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "building_sentrygun_base") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "building_dispenser") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "trigger_hurt") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "trigger_multiple") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "func_nogrenades") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "info_tfgoal") == 0)
	{
		if(pEntity->v.origin == Vector(0,0,0))
		{
			return 0;
		}
	}

	if (strcmp(STRING(pEntity->v.classname), "multi_manager") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "worldspawn") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_proximity") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_fencepost") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "ao_airstrike_start") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "ao_airstrike_counter") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "ao_superflame_start") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_superflame") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_tripmine") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "c4_bombcounter") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "ao_mediflag") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_snark") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_guncontrol") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "weapon_ao_flashbang") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "tfx_gib") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "ao_stealthent") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "tfx_skin") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "ao_holosnipe") == 0)
		return 0;

	if (strcmp(STRING(pEntity->v.classname), "autoop_dropped_weapon") == 0)
		return 0;

	return 1;
}

int FindFreeSlot(edict_t *pEntity)
{
	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[ENTINDEX(pEntity)].Selected.SelectedEnts[i] == 0)
			return i;
	}

	return 0;
}

void DeselectEntity(edict_t *pEntity)
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "DeselectEntity(): Entity deselected.\n");
		fclose(fp);
	}

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 4000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		int SelIndex = ENTINDEX(tr.pHit);
		int Match = 0;
		
		for (int i = 1; i <= MAX_ENTS; i++)
		{
			if(PlayerInfo[ENTINDEX(pEntity)].Selected.SelectedEnts[i] == SelIndex)
			{
				PlayerInfo[ENTINDEX(pEntity)].Selected.SelectedEnts[i] = 0;
				Match = 1;
			}
		}

		if(Match == 1)
		{
			//Colour it here
			char entity_model[64];
			strcpy(entity_model, STRING(tr.pHit->v.model));
			if (entity_model[0] == '*')
			{
				//colour brush
				//tr.pHit->v.rendermode = 0;
				//tr.pHit->v.renderamt = 213;
				//tr.pHit->v.rendercolor.x = 0;
				//tr.pHit->v.rendercolor.y = 0;
				//tr.pHit->v.rendercolor.z = 0;
				tr.pHit->v.renderamt = tr.pHit->v.fuser1;
				tr.pHit->v.rendermode = tr.pHit->v.iuser1;
				tr.pHit->v.renderfx = tr.pHit->v.iuser3;
				tr.pHit->v.rendercolor = tr.pHit->v.vuser1;
			} else {
				//colour point
				//tr.pHit->v.rendermode = 0;
				//tr.pHit->v.renderfx = 0;
				//tr.pHit->v.renderamt = 213;  
				//tr.pHit->v.rendercolor.x = 0;
				//tr.pHit->v.rendercolor.y = 0;
				//tr.pHit->v.rendercolor.z = 0;
				tr.pHit->v.renderamt = tr.pHit->v.fuser1;
				tr.pHit->v.rendermode = tr.pHit->v.iuser1;
				tr.pHit->v.renderfx = tr.pHit->v.iuser3;
				tr.pHit->v.rendercolor = tr.pHit->v.vuser1;
			}
		}
	}

	return;
}

void PointTransEnt(edict_t *pEntity, int iTransVal)
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "PointTransent(): Transent used with value %d.\n",iTransVal);
		fclose(fp);
	}

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 2000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if(IsValidEnt(tr.pHit) == 1)
		{
			if(iTransVal <= 255)
			{
				tr.pHit->v.rendermode = 1;
				tr.pHit->v.renderamt = iTransVal;
			}

			if(iTransVal == 256)
			{
				tr.pHit->v.rendermode = 0;
				tr.pHit->v.renderamt = 255;
			}

			if(iTransVal == 257)
			{
				tr.pHit->v.rendermode = 4;
				tr.pHit->v.renderamt = 255;
			}

			if(iTransVal == 258)
			{
				tr.pHit->v.rendermode = 2;
				tr.pHit->v.renderamt = 80;
			}

			if(iTransVal == 259)
			{
				tr.pHit->v.rendermode = 3;
				tr.pHit->v.renderamt = 150;
				tr.pHit->v.renderfx = 14;
			}

			if(iTransVal == 260)
			{
				tr.pHit->v.rendermode = kRenderNormal;
				tr.pHit->v.renderamt = 100;
				tr.pHit->v.renderfx = kRenderFxGlowShell;
			}

			if(iTransVal == 997)
			{
				tr.pHit->v.movetype = MOVETYPE_NONE;
				tr.pHit->v.gravity = 0;
			}

			if(iTransVal == 998)
			{
				tr.pHit->v.takedamage = DAMAGE_AIM;
			}

			if(iTransVal == 999)
			{
				tr.pHit->v.takedamage = DAMAGE_NO;
			}
		}
	}
}

void PointColorEnt(edict_t *pEntity, Vector vColors)
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "PointColorEnt(): Used with %d,%d,%d.\n",vColors.x,vColors.y,vColors.z);
		fclose(fp);
	}

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 2000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if(IsValidEnt(tr.pHit) == 1)
		{	
			tr.pHit->v.rendercolor.x = vColors.x;
			tr.pHit->v.rendercolor.y = vColors.y;
			tr.pHit->v.rendercolor.z = vColors.z;
		}
	}
}

void MoveEnt(edict_t *pEntity, int Direction)
{
	if(Direction < 1 || Direction > 6)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Invalid Direction.\n"));
		return;
	}

	int Index = ENTINDEX(pEntity);

	edict_t *pEnt = NULL;

	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);

			if(Direction == 1)
			{
				if(pEnt != NULL)
				{
					pEnt->v.origin.x = pEnt->v.origin.x + 20;
					UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
				} else {
					PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				}

			}

			if(Direction == 2)
			{
				if(pEnt != NULL)
				{
					pEnt->v.origin.x = pEnt->v.origin.x - 20;
					UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
				} else {
					PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				}

			}

			if(Direction == 3)
			{
				if(pEnt != NULL)
				{
					pEnt->v.origin.y = pEnt->v.origin.y + 20;
					UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
				} else {
					PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				}

			}

			if(Direction == 4)
			{
				if(pEnt != NULL)
				{
					pEnt->v.origin.y = pEnt->v.origin.y - 20;
					UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
				} else {
					PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				}

			}

			if(Direction == 5)
			{
				if(pEnt != NULL)
				{
					pEnt->v.origin.z = pEnt->v.origin.z + 20;
					UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
				} else {
					PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				}

			}

			if(Direction == 6)
			{
				if(pEnt != NULL)
				{
					pEnt->v.origin.z = pEnt->v.origin.z - 20;
					UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
				} else {
					PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				}

			}
		}
	}
}

void CloneEnt(edict_t *pEntity)
{
	//Make sure its safe to clone
	if(GetEntAmountState() == 1)
		return;

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "CloneEnt(): Entity was cloned.\n");
		fclose(fp);
	}

	int Index = ENTINDEX(pEntity);
	int iClass = 0;

	edict_t *pEnt = NULL;
	edict_t *pClone = NULL;
		
	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);			
			if(pEnt)
			{
				iClass = pEnt->v.classname;
				pClone = CREATE_NAMED_ENTITY(iClass);
				memcpy(pClone->pvPrivateData,pEnt->pvPrivateData,sizeof(pClone->pvPrivateData));
				pClone->v.origin = pEnt->v.origin;
				pClone->v.angles = pEnt->v.angles;
				pClone->v.velocity = Vector(0,0,0);
				pClone->v.speed = 0;
				pClone->v.targetname = pEnt->v.targetname;
				pClone->v.movetype = MOVETYPE_NONE;
				pClone->v.target = pEnt->v.target;

				pClone->v.rendermode = pEnt->v.iuser1;
				pClone->v.renderfx = pEnt->v.iuser3;
				pClone->v.renderamt = pEnt->v.fuser1;
				pClone->v.rendercolor = pEnt->v.vuser1;
				//pClone->v.angles = pEnt->v.angles;
				SET_MODEL( ENT(VARS(pClone)), STRING(pEnt->v.model) );
				DispatchSpawn(pClone);
				pClone->v.angles = pEnt->v.angles;
			} else {
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
			}
		}
	}
}

void DeselectAll(edict_t *pEntity)
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "DeselectAll(): Deselected All.\n");
		fclose(fp);
	}

	int Index = ENTINDEX(pEntity);

	edict_t *pEnt = NULL;

	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);

			if(pEnt != NULL)
			{
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				//Colour it here
				char entity_model[64];
				strcpy(entity_model, STRING(pEnt->v.model));
				if (entity_model[0] == '*')
				{
					//colour brush
					//pEnt->v.rendermode = 0;
					//pEnt->v.renderamt = 213;
					//pEnt->v.rendercolor.x = 0;
					//pEnt->v.rendercolor.y = 0;
					//pEnt->v.rendercolor.z = 0;
					pEnt->v.renderamt = pEnt->v.fuser1;
					pEnt->v.rendermode = pEnt->v.iuser1;
					pEnt->v.renderfx = pEnt->v.iuser3;
					pEnt->v.rendercolor = pEnt->v.vuser1;
				} else {
					//colour point
					//pEnt->v.rendermode = 0;
					//pEnt->v.renderfx = 0;
					//pEnt->v.renderamt = 213;  
					//pEnt->v.rendercolor.x = 0;
					//pEnt->v.rendercolor.y = 0;
					//pEnt->v.rendercolor.z = 0;
					pEnt->v.renderamt = pEnt->v.fuser1;
					pEnt->v.rendermode = pEnt->v.iuser1;
					pEnt->v.renderfx = pEnt->v.iuser3;
					pEnt->v.rendercolor = pEnt->v.vuser1;
				}
			} else {
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
			}
		}
	}
}

void DeleteEntity(edict_t *pEntity)
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "DeleteEntity(): Entity deleted.\n");
		fclose(fp);
	}

	int Index = ENTINDEX(pEntity);

	edict_t *pEnt = NULL;

	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);

			if(pEnt != NULL)
			{
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
				pEnt->v.flags |= FL_KILLME;
			} else {
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
			}
		}
	}
}

void QuickMove(edict_t *pEntity)
{
	int Index = ENTINDEX(pEntity);

	edict_t *pEnt1 = NULL;

	if(PlayerInfo[Index].Selected.SelectedEnts[1] > 0)
		pEnt1 = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[1]);

	if(pEnt1 != NULL)
	{
		Vector v_src, v_dest, v_temp;
		TraceResult tr;

		UTIL_MakeVectors( pEntity->v.v_angle );

		v_src = pEntity->v.origin + pEntity->v.view_ofs;
		v_dest = v_src + gpGlobals->v_forward * 8000;

		UTIL_TraceLine(v_src,v_dest,ignore_monsters,pEnt1,&tr);

		//UTIL_TraceHull(v_src,v_dest,ignore_monsters,point_hull,pEnt1,&tr);
		//UTIL_TraceMonsterHull(pEnt1,pEnt1->v.mins,pEnt1->v.maxs,ignore_monsters,pEntity,&tr);
		
		if (tr.flFraction < 1.0)
		{
			v_temp = tr.vecEndPos;
			pEnt1->v.origin = pEnt1->v.origin + (v_temp - VecBModelOrigin(VARS(pEnt1)));
			UTIL_SetOrigin( VARS(pEnt1), pEnt1->v.origin );

		}
	} else {
		PlayerInfo[Index].Selected.SelectedEnts[1] = 0;
	}
}

void QuickMoveX(edict_t *pEntity)
{
	int Index = ENTINDEX(pEntity);

	edict_t *pEnt1 = NULL;

	if(PlayerInfo[Index].Selected.SelectedEnts[1] > 0)
		pEnt1 = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[1]);

	if(pEnt1 != NULL)
	{
		Vector v_src, v_dest, v_temp;
		TraceResult tr;

		UTIL_MakeVectors( pEntity->v.v_angle );

		v_src = pEntity->v.origin + pEntity->v.view_ofs;
		v_dest = v_src + gpGlobals->v_forward * 8000;

		UTIL_TraceLine(v_src,v_dest,ignore_monsters,pEnt1,&tr);

		//UTIL_TraceHull(v_src,v_dest,ignore_monsters,point_hull,pEnt1,&tr);
		//UTIL_TraceMonsterHull(pEnt1,pEnt1->v.mins,pEnt1->v.maxs,ignore_monsters,pEntity,&tr);
		
		if (tr.flFraction < 1.0)
		{
			v_temp = tr.vecEndPos;
			pEnt1->v.origin.x = pEnt1->v.origin.x + (v_temp.x - VecBModelOrigin(VARS(pEnt1)).x);
			UTIL_SetOrigin( VARS(pEnt1), pEnt1->v.origin );

		}
	} else {
		PlayerInfo[Index].Selected.SelectedEnts[1] = 0;
	}
}

void QuickMoveY(edict_t *pEntity)
{
	int Index = ENTINDEX(pEntity);

	edict_t *pEnt1 = NULL;

	if(PlayerInfo[Index].Selected.SelectedEnts[1] > 0)
		pEnt1 = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[1]);

	if(pEnt1 != NULL)
	{
		Vector v_src, v_dest, v_temp;
		TraceResult tr;

		UTIL_MakeVectors( pEntity->v.v_angle );

		v_src = pEntity->v.origin + pEntity->v.view_ofs;
		v_dest = v_src + gpGlobals->v_forward * 8000;

		UTIL_TraceLine(v_src,v_dest,ignore_monsters,pEnt1,&tr);

		//UTIL_TraceHull(v_src,v_dest,ignore_monsters,point_hull,pEnt1,&tr);
		//UTIL_TraceMonsterHull(pEnt1,pEnt1->v.mins,pEnt1->v.maxs,ignore_monsters,pEntity,&tr);
		
		if (tr.flFraction < 1.0)
		{
			v_temp = tr.vecEndPos;
			pEnt1->v.origin.y = pEnt1->v.origin.y + (v_temp.y - VecBModelOrigin(VARS(pEnt1)).y);
			UTIL_SetOrigin( VARS(pEnt1), pEnt1->v.origin );

		}
	} else {
		PlayerInfo[Index].Selected.SelectedEnts[1] = 0;
	}
}

void QuickMoveZ(edict_t *pEntity)
{
	int Index = ENTINDEX(pEntity);

	edict_t *pEnt1 = NULL;

	if(PlayerInfo[Index].Selected.SelectedEnts[1] > 0)
		pEnt1 = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[1]);

	if(pEnt1 != NULL)
	{
		Vector v_src, v_dest, v_temp;
		TraceResult tr;

		UTIL_MakeVectors( pEntity->v.v_angle );

		v_src = pEntity->v.origin + pEntity->v.view_ofs;
		v_dest = v_src + gpGlobals->v_forward * 8000;

		UTIL_TraceLine(v_src,v_dest,ignore_monsters,pEnt1,&tr);

		//UTIL_TraceHull(v_src,v_dest,ignore_monsters,point_hull,pEnt1,&tr);
		//UTIL_TraceMonsterHull(pEnt1,pEnt1->v.mins,pEnt1->v.maxs,ignore_monsters,pEntity,&tr);
		
		if (tr.flFraction < 1.0)
		{
			v_temp = tr.vecEndPos;
			pEnt1->v.origin.z = pEnt1->v.origin.z + (v_temp.z - VecBModelOrigin(VARS(pEnt1)).z);
			UTIL_SetOrigin( VARS(pEnt1), pEnt1->v.origin );

		}
	} else {
		PlayerInfo[Index].Selected.SelectedEnts[1] = 0;
	}
}

void RotatePoint(edict_t *pEntity)
{
	int Index = ENTINDEX(pEntity);

	edict_t *pEnt = NULL;

	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);

			if(pEnt != NULL)
			{
				char entity_model[64];
				strcpy(entity_model, STRING(pEnt->v.model));
				if (entity_model[0] != '*')
				{
					pEnt->v.angles.y += 90;
				}
			} else {
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
			}
		}
	}
}

void RotateBrush(edict_t *pEntity)
{
	//When i know how to do this i will.

	//UPDATE: Use chkv angles "x y z" - will only work with
	//brushes with an ORIGIN brush in their centre (see roblego maps)
}

void ChangeKeyValue(edict_t *pEntity, const char *Key, const char *KValue)
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "ChangeKeyValue(): Keyvalue changed.\n");
		fclose(fp);
	}

	//Changes a key value on a Turret
	KeyValueData pkvd;
	char classname[32];
	char keyname[32];
	char value[32];
	pkvd.szClassName = classname;
	pkvd.szKeyName = keyname;
	pkvd.szValue = value;
	strcpy(keyname, Key);
	strcpy(value, KValue);

	int Index = ENTINDEX(pEntity);

	edict_t *pEnt = NULL;

	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);

			if(pEnt != NULL)
			{
				strcpy(classname, STRING(pEnt->v.classname));
				(*other_gFunctionTable.pfnKeyValue)(pEnt, &pkvd);
			} else {
				PlayerInfo[Index].Selected.SelectedEnts[i] = 0;
			}

		}
	}
}

void TriggerEntity(edict_t *pEntity)
{
	//Triggers entity player is pointing at
	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 2000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if(IsValidEnt(tr.pHit) == 1)
		{
			edict_t	*pTrigger = NULL;
			pTrigger = CREATE_NAMED_ENTITY(MAKE_STRING("trigger_auto"));
			pTrigger->v.origin = Vector(0,0,-4000);
			
			if(tr.pHit->v.targetname != 0)
			{
				pTrigger->v.target = tr.pHit->v.targetname;
			} else {
				pTrigger->v.target = tr.pHit->v.target;
			}

			DispatchSpawn(pTrigger);
		}
	}
}

void SelectClass(edict_t *pEntity, const char *ClassName, const char *Range)
{
	int iRange = atoi(Range);
	int iCount = 0;

	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, iRange)) != NULL)
	{
		if(IsValidEnt(pScan->edict()) == 1 && FStrEq(STRING(pScan->pev->classname),ClassName))
		{
			if(FindFreeSlot(pEntity) > 0)
			{

				//Copy render properties
				pScan->pev->iuser1 = pScan->pev->rendermode;
				pScan->pev->fuser1 = pScan->pev->renderamt;
				pScan->pev->iuser3 = pScan->pev->renderfx;
				pScan->pev->vuser1 = pScan->pev->rendercolor;

				//Colour it here
				char entity_model[64];
				strcpy(entity_model, STRING(pScan->pev->model));
				if (entity_model[0] == '*')
				{
					//colour brush
					pScan->pev->rendermode = 1;
					pScan->pev->renderamt = 255;
					pScan->pev->rendercolor.x = 213;
					pScan->pev->rendercolor.y = 0;
					pScan->pev->rendercolor.z = 0;
				} else {
					//colour point
					pScan->pev->rendermode = kRenderNormal;
					pScan->pev->renderfx = kRenderFxGlowShell;
					pScan->pev->renderamt = 25;  
					pScan->pev->rendercolor.x = 213;
					pScan->pev->rendercolor.y = 0;
					pScan->pev->rendercolor.z = 0;
				}

				PlayerInfo[ENTINDEX(pEntity)].Selected.SelectedEnts[FindFreeSlot(pEntity)] = ENTINDEX(pScan->edict());
				iCount += 1;
	
			} else {
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "All of your workslots are in use.\nDeselect something and try again.\n"));
				return;
			}
		}
	}
	CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "%d entities selected.\n",iCount));

	return;
}

void ScanRadius(edict_t *pAdmin, int Info)
{
	CBaseEntity *pMapEnt = NULL;
	char str[BUF_SIZE];

	while ((pMapEnt = UTIL_FindEntityInSphere(pMapEnt, pAdmin->v.origin, 40)) != NULL)
	{
		if (pMapEnt->pev->owner == NULL)  // make sure item has no owner..
		{
			if (pMapEnt->pev->classname != pAdmin->v.classname && (IsValidEnt(pMapEnt->edict()) == 1))
			{	
				//List of what its found
				if(Info == 0)
					sprintf(str, "CLASS: %s | TARG: %s | TARGN: %s | MODEL %s |\n",STRING(pMapEnt->pev->classname),STRING(pMapEnt->pev->target), STRING(pMapEnt->pev->targetname), STRING(pMapEnt->pev->model));

				if(Info == 1)
					sprintf(str, "CLASS: %s |R-amt: %f|R-fx: %d|R-mode: %d|R-color: %d,%d,%d|MODEL %s \n",STRING(pMapEnt->pev->classname),pMapEnt->pev->renderamt,pMapEnt->pev->renderfx,pMapEnt->pev->rendermode,pMapEnt->pev->rendercolor.x,pMapEnt->pev->rendercolor.y,pMapEnt->pev->rendercolor.z,STRING(pMapEnt->pev->model));

				CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s\n",str));
			}
		}
	}
}

void StartPull(edict_t *pAdmin)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "StartPull(): Player used pull.\n");
		fclose(fp);
	}

	PlayerInfo[ENTINDEX(pAdmin)].Selected.PullOrig = pAdmin->v.origin;
	PlayerInfo[ENTINDEX(pAdmin)].Selected.IsPulling = 1;
}

void PullThink(edict_t *pAdmin)
{
	int Index = ENTINDEX(pAdmin);

	edict_t *pEnt = NULL;

	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[Index].Selected.SelectedEnts[i] > 0)
		{
			pEnt = INDEXENT(PlayerInfo[Index].Selected.SelectedEnts[i]);

			if(pEnt != NULL)
			{
				pEnt->v.origin = pEnt->v.origin + (pAdmin->v.origin - PlayerInfo[Index].Selected.PullOrig);
				UTIL_SetOrigin( VARS(pEnt), pEnt->v.origin );
			}
		}
	}
}

void EndPull(edict_t *pAdmin)
{
	PlayerInfo[ENTINDEX(pAdmin)].Selected.PullOrig = Vector(0,0,0);
	PlayerInfo[ENTINDEX(pAdmin)].Selected.IsPulling = 0;
}

bool SaveEnts(edict_t *pAdmin, const char *FileName)
{
	if(FStrEq(FileName,""))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a filename.\n"));
		return 0;
	}
	
	//Create Header
	TFXAEFHeader_s Header;
	memset(Header.MapName, 0, sizeof(Header.MapName));
	strncpy(Header.MapName, STRING(gpGlobals->mapname), 31);
	Header.MapName[31] = 0;

	memset(Header.AuthorName, 0, sizeof(Header.AuthorName));
	strncpy(Header.AuthorName, STRING(pAdmin->v.netname), MAX_NAME_LENGTH);
	Header.AuthorName[MAX_NAME_LENGTH] = 0;

	Header.Ver = AEF_VERSION;

	//Do the entcount
	int EntCount = 0;
	edict_t *CurrentEnt = NULL;
	for (int i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[i] > 0)
		{
			CurrentEnt = INDEXENT(PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[i]);
			if(CurrentEnt)
			{
				EntCount += 1;
			}
		}
	}

	Header.EntNumber = EntCount;

	//Open the file
	_mkdir( "tfc\\autoop" );
	_mkdir( "tfc\\autoop\\entfiles" );

	static char filenameread[256];
	strcpy(filenameread, "tfc\\autoop\\entfiles\\");
	strcat(filenameread,FileName);
	strcat(filenameread, ".aef");

	FILE *cfile = fopen(filenameread, "wb");

	//Write header
	fwrite(&Header, sizeof(Header), 1, cfile);
	
	//Loop through all entities and save
	TFXAEFEntity_s EntData;
	//edict_t *CurrentEnt = NULL;
	for (i = 1; i <= MAX_ENTS; i++)
	{
		if(PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[i] > 0)
		{
			CurrentEnt = INDEXENT(PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[i]);
			if(CurrentEnt)
			{
				EntData.angles = CurrentEnt->v.angles;

				memset(EntData.classname, 0, sizeof(EntData.classname));
				strncpy(EntData.classname, STRING(CurrentEnt->v.classname), 63);
				EntData.classname[63] = 0;


				memset(EntData.model, 0, sizeof(EntData.model));
				strncpy(EntData.model, STRING(CurrentEnt->v.model), 63);
				EntData.model[63] = 0;

				memset(EntData.target, 0, sizeof(EntData.target));
				strncpy(EntData.target, STRING(CurrentEnt->v.target), 63);
				EntData.target[63] = 0;

				memset(EntData.targetname, 0, sizeof(EntData.targetname));
				strncpy(EntData.targetname, STRING(CurrentEnt->v.targetname), 63);
				EntData.targetname[63] = 0;

				EntData.origin = CurrentEnt->v.origin;

				EntData.dmg = CurrentEnt->v.dmg;
				EntData.movetype = CurrentEnt->v.movetype;
				EntData.speed = CurrentEnt->v.speed;
				EntData.effects = CurrentEnt->v.effects;
				
				//Use backup stored render properties
				EntData.renderamt = CurrentEnt->v.fuser1;
				EntData.rendercolor = CurrentEnt->v.vuser1;
				EntData.renderfx = CurrentEnt->v.iuser3;
				EntData.rendermode = CurrentEnt->v.iuser1;				
				
				CurrentEnt = NULL;
				fwrite(&EntData, sizeof(EntData), 1, cfile);
			}
		}
	}

	//Close file
	fclose(cfile);

	return 1; //Succeed;
}

bool LoadEnts(edict_t *pAdmin, const char *FileName)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "LoadEnts: User loading entities.\n");
		fclose(fp);
	}

	if(pAdmin != NULL)
	{
		if(FStrEq(FileName,""))
		{
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a filename.\n"));
			return 0;
		}
	}

	//Open the file
	_mkdir( "tfc\\autoop" );
	_mkdir( "tfc\\autoop\\entfiles" );

	static char filenameread[256];
	strcpy(filenameread, "tfc\\autoop\\entfiles\\");
	strcat(filenameread,FileName);
	strcat(filenameread, ".aef");

	FILE *cfile = fopen(filenameread, "rb");

	if(pAdmin != NULL)
	{
		if(cfile == NULL)
		{
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Entfile not found!\n"));
			return 0;
		}
	}

	TFXAEFHeader_s Header;
	
	fread(&Header, sizeof(Header), 1, cfile);

	Header.MapName[31] = 0;

	if(pAdmin != NULL)
	{
		if (stricmp(Header.MapName, STRING(gpGlobals->mapname)) != 0)
		{
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Entfile not made for this map, is made for %s!\n",Header.MapName));
			fclose(cfile);
			return 0;
		}
	}

	//Version error
	if(Header.Ver > AEF_VERSION)
	{
		if(pAdmin != NULL)
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Entfile made with a newer version of AutoOP!\n"));

		fclose(cfile);
		return 0;
	}

	Header.AuthorName[MAX_NAME_LENGTH-1] = 0; //Set the NULL here

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "LoadEnts: %d Entities found in file.\n",(Header.EntNumber - 1));
		fclose(fp);
	}

	//Make sure the file contains entities.
	if(Header.EntNumber == 0)
	{
		if(pAdmin != NULL)
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Entfile has no entities in it!\n"));

		fclose(cfile);

		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "LoadEnts: ERROR - Loaded file contained no entities.\n");
			fclose(fp);
		}

		return 0;
	}

	//Make sure the file contains entities.
	if(Header.EntNumber > MAX_ENTS)
	{
		if(pAdmin != NULL)
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Entfile has greater numberof ents than MAX_ENTS!\n"));

		fclose(cfile);

		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "LoadEnts: ERROR - Loaded file contained too many entities!\n");
			fclose(fp);
		}

		return 0;
	}

	//Load in the entities
	TFXAEFEntity_s EntData;
	edict_t *pCreate = NULL;
	for (int i = 0; i <= (Header.EntNumber-1); i++)
	{
		//Make sure its safe to clone
		if(GetEntAmountState() == 1)
		{
			if(pAdmin != NULL)
				CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Cant complete load, not enough free edicts in HLDS.\n"));
			return 1;
		}
		
		fread(&EntData, sizeof(EntData), 1, cfile);

		EntData.classname[63] = 0;
		EntData.model[63] = 0;
		EntData.target[63]= 0;
		EntData.targetname[63] = 0;

		pCreate = CREATE_NAMED_ENTITY(ALLOC_STRING(EntData.classname));
				
		pCreate->v.origin = EntData.origin;
		pCreate->v.angles = EntData.angles;

		UTIL_SetOrigin(VARS(pCreate), pCreate->v.origin);
		SET_MODEL(pCreate, EntData.model);
		pCreate->v.model = ALLOC_STRING(EntData.model);

		DispatchSpawn(pCreate);
		pCreate->v.angles = EntData.angles;
		pCreate->v.netname = ALLOC_STRING(FileName); //Identifier

		pCreate->v.renderamt = EntData.renderamt;
		pCreate->v.rendercolor = EntData.rendercolor;
		pCreate->v.renderfx = EntData.renderfx;
		pCreate->v.rendermode = EntData.rendermode;

		pCreate->v.dmg = EntData.dmg;
		pCreate->v.speed = EntData.speed;
		pCreate->v.movetype = EntData.movetype;

		pCreate->v.targetname = ALLOC_STRING(EntData.targetname);
		pCreate->v.target = ALLOC_STRING(EntData.target);

		pCreate->v.effects = EntData.effects;

		if(_stricmp(STRING(pCreate->v.classname),"func_rotating") == 0)
		{
			edict_t	*pTrigger = NULL;
			pTrigger = CREATE_NAMED_ENTITY(MAKE_STRING("trigger_auto"));
			pTrigger->v.origin = Vector(0,0,-4000);
			pTrigger->v.netname = ALLOC_STRING(FileName); //Identifier
			
			if(pCreate->v.targetname != 0)
			{
				pTrigger->v.target = pCreate->v.targetname;
			} else {
				pTrigger->v.target = pCreate->v.target;
			}
			DispatchSpawn(pTrigger);
		}

		if(_stricmp(STRING(pCreate->v.classname),"func_train") == 0)
		{
			edict_t	*pTrigger = NULL;
			pTrigger = CREATE_NAMED_ENTITY(MAKE_STRING("trigger_auto"));
			pTrigger->v.origin = Vector(0,0,-4000);
			pTrigger->v.netname = ALLOC_STRING(FileName); //Identifier
			
			if(pCreate->v.targetname != 0)
			{
				pTrigger->v.target = pCreate->v.targetname;
			} else {
				pTrigger->v.target = pCreate->v.target;
			}
			DispatchSpawn(pTrigger);

			pTrigger = NULL;
			pTrigger = CREATE_NAMED_ENTITY(MAKE_STRING("trigger_auto"));
			pTrigger->v.origin = Vector(0,0,-4000);
			pTrigger->v.netname = ALLOC_STRING(FileName); //Identifier
			
			if(pCreate->v.targetname != 0)
			{
				pTrigger->v.target = pCreate->v.targetname;
			} else {
				pTrigger->v.target = pCreate->v.target;
			}
			DispatchSpawn(pTrigger);
		}
	}

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "LoadEnts: Entities loaded and created.\n");
		fclose(fp);
	}
	
	fclose(cfile);

	return 1;

}

void KillAEFEnts(edict_t *pAdmin, const char *FileName)
{
	//Kills all entities loaded from <FileName>

	if(FStrEq(FileName,""))
	{
		if(pAdmin != NULL)
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a filename.\n"));
		return;
	}

	//Open the file
	_mkdir( "tfc\\autoop" );
	_mkdir( "tfc\\autoop\\entfiles" );

	static char filenameread[256];
	strcpy(filenameread, "tfc\\autoop\\entfiles\\");
	strcat(filenameread,FileName);
	strcat(filenameread, ".aef");

	FILE *cfile = fopen(filenameread, "rb");

	if(cfile == NULL)
	{
		if(pAdmin != NULL)
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "The Entfile these entities were loaded from does not exist!\n"));
		return;
	}
	fclose(cfile);

	CBaseEntity *pEntity = NULL;

	while ((pEntity = UTIL_FindEntityByString( pEntity, "netname", FileName)) != NULL)
	{
		pEntity->pev->flags = FL_KILLME;
	}

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "KillAEFEnts: Entities loaded from %s.aef deleted.\n",FileName);
		fclose(fp);
	}
	
	if(pAdmin != NULL)
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Entities loaded from %s.aef have been deleted.\n",FileName));
}
	
void SelectLoaded(edict_t *pAdmin, const char *FileName)
{
	//Kills all entities loaded from <FileName>

	if(FStrEq(FileName,""))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must enter a filename.\n"));
		return;
	}

	//Open the file
	_mkdir( "tfc\\autoop" );
	_mkdir( "tfc\\autoop\\entfiles" );

	static char filenameread[256];
	strcpy(filenameread, "tfc\\autoop\\entfiles\\");
	strcat(filenameread,FileName);
	strcat(filenameread, ".aef");

	FILE *cfile = fopen(filenameread, "rb");

	if(cfile == NULL)
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "The Entfile these entities were loaded from does not exist!\n"));
		return;
	}
	fclose(cfile);

	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityByString( pScan, "netname", FileName)) != NULL)
	{
		if(IsValidEnt(pScan->edict()) == 1)
		{
			if(FindFreeSlot(pAdmin) > 0)
			{
				pScan->pev->iuser1 = pScan->pev->rendermode;
				pScan->pev->fuser1 = pScan->pev->renderamt;
				pScan->pev->iuser3 = pScan->pev->renderfx;
				pScan->pev->vuser1 = pScan->pev->rendercolor;
				//Colour it here
				char entity_model[64];
				strcpy(entity_model, STRING(pScan->pev->model));
				if (entity_model[0] == '*')
				{
					//colour brush
					pScan->pev->rendermode = 1;
					pScan->pev->renderamt = 255;
					pScan->pev->rendercolor.x = 213;
					pScan->pev->rendercolor.y = 0;
					pScan->pev->rendercolor.z = 0;
				} else {
					//colour point
					pScan->pev->rendermode = kRenderNormal;
					pScan->pev->renderfx = kRenderFxGlowShell;
					pScan->pev->renderamt = 25;  
					pScan->pev->rendercolor.x = 213;
					pScan->pev->rendercolor.y = 0;
					pScan->pev->rendercolor.z = 0;
				}

				PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[FindFreeSlot(pAdmin)] = ENTINDEX(pScan->edict());
	
			} else {
				CLIENT_PRINTF( pAdmin, print_center, UTIL_VarArgs( "All of your workslots are in use.\nDeselect something and try again.\n"));
				return;
			}
		}
	}
}





