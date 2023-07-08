/*****************************************************\
*	Auto-OP Shodan
*
*	File: tripmine.cpp
*	Description: Soldiers tripmine functions.
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
extern int team_allies[4];

void SwitchToTripMine(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SOLDIER || pEntity->v.health <= 0 || pEntity->v.weaponmodel == MAKE_STRING(TRIPMINE_PMODEL))
		return;

	CLIENT_COMMAND(pEntity, "tf_weapon_axe\n");
	PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = TRIPMINE_ID;
	CBasePlayer *pPlayer = (CBasePlayer *)pEntity;

	pPlayer->m_pActiveItem = NULL;

	int gmsgCurWeapon = REG_USER_MSG("CurWeapon", 3);

	if(gmsgCurWeapon > MAX_USERMSG)
		return;

	MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pEntity );
		WRITE_BYTE(0);
		WRITE_BYTE(0);
		WRITE_BYTE(0);
	MESSAGE_END();
}

void PlaceTripMine(edict_t *pEntity)
{
	//PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[TRIPMINE_ID] = gpGlobals->time + TRIPMINE_DELAY; //So the server doesnt spam the message too much.

	if( PlayerInfo[ENTINDEX(pEntity)].TripMineAmmo <= 0)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You have no tripmines left.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have no tripmines left.\n"));
		return;
	}

	//Scan to make sure we are not in a func or too near another mine
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 50)) != NULL)
	{
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0 || strcmp(STRING(pScan->pev->classname), "weapon_ao_tripmine") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place a tripmine here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place a tripmine here.\n"));
			return;
		}
	}

	if(pEntity->v.playerclass != CLASS_SOLDIER || pEntity->v.health <= 0)
		return;

	//Make sure soldier does not already have 3 tripmines placed
	pScan = NULL;
	int TripCount = 0;
    while(pScan = UTIL_FindEntityByClassname(pScan, "weapon_ao_tripmine"))
    {
		if (pScan->pev->euser1 == pEntity)
		{
			TripCount += 1;
		}
	}

	if(TripCount >= 3)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot have more than 3\ntripmines active at once.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot have more than 3\ntripmines active at once.\n"));
		return;
	}

	//Draw little traceline from player to about 128 cells in front of him
	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 128;

	UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if(tr.vecPlaneNormal.z != 0)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place a tripmine here.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place a tripmine here.\n"));
		return;
	}

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			PlayerInfo[ENTINDEX(pEntity)].TripMineAmmo -= 1;
			//Create the Tripmine
			edict_t *pTripMine;
			pTripMine = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
			DispatchSpawn(pTripMine);
			pTripMine->v.origin = tr.vecEndPos;
			pTripMine->v.angles = UTIL_VecToAngles( tr.vecPlaneNormal ); //pEntity->v.v_angle;
			UTIL_MakeVectors(pEntity->v.v_angle);
			pTripMine->v.velocity = Vector(0, 0, 0);
			pTripMine->v.euser1 = pEntity;
			pTripMine->v.euser2 = NULL; //Will store the beams entity here.
			pTripMine->v.fuser1 = gpGlobals->time + 4; //Priming time
			pTripMine->v.iuser1 = 0; //1 = Is charged
			pTripMine->v.takedamage = DAMAGE_YES;
			pTripMine->v.health = 100000;
			pTripMine->v.movetype = MOVETYPE_NONE;
			pTripMine->v.solid = SOLID_BBOX;
			pTripMine->v.team = pEntity->v.team;
			SET_MODEL(pTripMine, "models/autoop/aotripmine.mdl");
			UTIL_SetSize(VARS(pTripMine), Vector(0,0,0), Vector(0,0,0));
			pTripMine->v.classname = MAKE_STRING("weapon_ao_tripmine");
			pTripMine->v.nextthink = gpGlobals->time + 0.1;
			EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/mine_deploy.wav", 1, ATTN_NORM, 0, 100);
			EMIT_SOUND_DYN2(pEntity, CHAN_BODY, "weapons/mine_charge.wav", 1, ATTN_NORM, 0, 100);

			UTIL_LogPrintf( "[AUTOOP] %s placed a tripmine\n", STRING(pEntity->v.netname));
		}

		//If his tripmine ammo goes to 0 take off his model
		/*if(PlayerInfo[ENTINDEX(pEntity)].TripMineAmmo == 0)
		{
			pEntity->v.viewmodel = MAKE_STRING("models/v_tfc_crowbar.mdl");
			pEntity->v.weaponmodel = MAKE_STRING("models/p_crowbar.mdl");
		}*/
	}
}

bool TripMineThink(edict_t *pEntity)
{
	if(pEntity->v.health <= 100000 - TRIPMINE_HEALTH)
	{
		DestroyTripMine(pEntity);
		return 1;
	}

	if(pEntity->v.euser1->v.team != pEntity->v.team)
	{
		DestroyPlayerTripMines(pEntity->v.euser1,0);
		return 1;
	}
	
	if(pEntity->v.euser1->v.playerclass != CLASS_SOLDIER)
	{
		DestroyPlayerTripMines(pEntity->v.euser1,1);
		return 1;
	}
	
	if(gpGlobals->time >= pEntity->v.fuser1 && pEntity->v.iuser1 == 0) //Charged
	{

		//Draw little traceline from tripmine to 9000 cells in front, this thing has pretty much an infinite range
		Vector v_src, v_dest;
		TraceResult tr;

		UTIL_MakeVectors( pEntity->v.angles );

		v_src = pEntity->v.origin + pEntity->v.view_ofs;
		v_dest = v_src + gpGlobals->v_forward * 9000;

		UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEntity->v.pContainingEntity, &tr);

		pEntity->v.iuser1 = 1; //Tell mr.trippy that he has charged.
		EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/mine_activate.wav", 1, ATTN_NORM, 0, 100);
		edict_t *pBeam = NULL;
		pBeam = CREATE_NAMED_ENTITY(MAKE_STRING("beam"));
		DispatchSpawn(pBeam);
		
		pBeam->v.origin = pEntity->v.origin;
		
		pBeam->v.angles = tr.vecEndPos;

		/*pBeam->v.angles.x = pEntity->v.origin.x + 200;
		pBeam->v.angles.y = pEntity->v.origin.y;
		pBeam->v.angles.z = pEntity->v.origin.z;*/

		//Beaminit
		pBeam->v.flags |= FL_CUSTOMENTITY;
		pBeam->v.rendercolor = Vector(0,214,98);
		pBeam->v.renderamt = 64;
		pBeam->v.body = 0;
		pBeam->v.frame = 0;
		pBeam->v.animtime = 255;
		pBeam->v.model = MAKE_STRING("sprites/laserbeam.spr");
		pBeam->v.modelindex = PRECACHE_MODEL("sprites/laserbeam.spr");
		pBeam->v.scale = 10;
		pBeam->v.skin = 0;
		pBeam->v.sequence = 0;
		pBeam->v.rendermode = 0;
		
		//Points init
		pBeam->v.rendermode = (pBeam->v.rendermode & 0xF0) | (0&0x0F); //BEAM_POINTS = 0
		pBeam->v.skin = 0;
		pBeam->v.sequence = 0;

		//Relink Beam		
		const Vector &startPos = pBeam->v.origin, &endPos = pBeam->v.angles;

		pBeam->v.mins.x = min( startPos.x, endPos.x );
		pBeam->v.mins.y = min( startPos.y, endPos.y );
		pBeam->v.mins.z = min( startPos.z, endPos.z );
		pBeam->v.maxs.x = max( startPos.x, endPos.x );
		pBeam->v.maxs.y = max( startPos.y, endPos.y );
		pBeam->v.maxs.z = max( startPos.z, endPos.z );
		pBeam->v.mins = pBeam->v.mins - pBeam->v.origin;
		pBeam->v.maxs = pBeam->v.maxs - pBeam->v.origin;

		UTIL_SetSize(VARS(pBeam), pBeam->v.mins, pBeam->v.maxs );
		UTIL_SetOrigin( VARS(pBeam), pBeam->v.origin );
		pEntity->v.euser2 = pBeam;
	}

	if(pEntity->v.iuser1 == 1) //If we are charged...
	{
		//Do the traceline thing.
		TraceResult tr2;
		UTIL_TraceLine( pEntity->v.origin, pEntity->v.euser2->v.angles, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr2);

		if(tr2.flFraction < 1.0)
		{
			if ( (strcmp("player", STRING(tr2.pHit->v.classname)) == 0))
			{
				int Team1 = tr2.pHit->v.team;
				int Team2 = pEntity->v.team;
				if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
				{
					DestroyTripMine(pEntity);
				}
			} else {

				DestroyTripMine(pEntity);
			}
		}
	}

	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void DestroyTripMine(edict_t *pEntity)
{
	pEntity->v.flags |= FL_KILLME; //Kill the tripmine

	if(pEntity->v.iuser1 == 1)
		pEntity->v.euser2->v.flags |= FL_KILLME; //Kill the laser

	edict_t *pBomb;
	pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
	DispatchSpawn(pBomb);
	pBomb->v.origin = pEntity->v.origin;
	pBomb->v.velocity = Vector(0,0,0);
	pBomb->v.dmg = TRIPMINE_DAMAGE;
	pBomb->v.owner = pEntity->v.euser1;
	pBomb->v.team = pEntity->v.team;
	pBomb->v.effects |= EF_NODRAW;
	pBomb->v.nextthink = gpGlobals->time;
	pBomb->v.classname = MAKE_STRING("weapon_ao_tripminedetonate");
	return;
}

void DestroyPlayerTripMines(edict_t *pEntity, int iDetonate)
{
	UTIL_LogPrintf( "[AUTOOP] %s's tripmines were destroyed.\n", STRING(pEntity->v.netname));

	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_tripmine"))
    {
        if(pMine->pev->euser1 == pEntity)
        {
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your tripmines were destroyed.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your tripmines were destroyed.\n"));
            pMine->pev->flags |= FL_KILLME;
			if(pMine->pev->iuser1 == 1)
				pMine->pev->euser2->v.flags |= FL_KILLME;

			if (iDetonate == 1)
			{
				edict_t *pBomb;
				pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
				DispatchSpawn(pBomb);
				pBomb->v.origin = pMine->pev->origin;
				pBomb->v.velocity = Vector(0,0,0);
				pBomb->v.dmg = TRIPMINE_DAMAGE;
				pBomb->v.owner = pEntity;
				pBomb->v.effects |= EF_NODRAW;
				pBomb->v.nextthink = gpGlobals->time;
				pBomb->v.classname = MAKE_STRING("weapon_ao_tripminedetonate");
			}
		}
	}
	return;
}