/*****************************************************\
*	Auto-OP Shodan
*
*	File: aomonsters.cpp
*	Description: Generic Monster Code.
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
#include "aomonsters.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern DLL_FUNCTIONS other_gFunctionTable;
extern TFXFeatureEnabled_s Features;

// From the SDK
//=========================================================
// Changeyaw - turns a monster towards its ideal_yaw
//=========================================================
float MonsterChangeYaw( edict_t *pEdict, float flspeed )
{
	float		ideal, current, move, speed;

	current = UTIL_AngleMod( pEdict->v.angles.y );
	ideal = pEdict->v.ideal_yaw;
	if (current != ideal)
	{
		speed = flspeed * gpGlobals->frametime * 10;
		move = ideal - current;

		if (ideal > current)
		{
			if (move >= 180)
				move = move - 360;
		}
		else
		{
			if (move <= -180)
				move = move + 360;
		}

		if (move > 0)
		{// turning to the monster's left
			if (move > speed)
				move = speed;
		}
		else
		{// turning to the monster's right
			if (move < -speed)
				move = -speed;
		}
		
		pEdict->v.angles.y = UTIL_AngleMod (current + move);
	}
	else
		move = 0;

	return move;
}

edict_t *MonsterFindEnemy(edict_t *pGrunt)
{
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pGrunt->v.origin, 512)) != NULL)
	{
		//Is it an entity we are bothered about? (Players)
		if (strcmp(STRING(pScan->pev->classname), "player") == 0)
		{
			//Can we see this potential enemy?
			//If I fire a traceline to the entity, if it gets blocked
			//By something I can ignore it else i must be able to see it.
			TraceResult tr;
				
			UTIL_TraceLine( pGrunt->v.origin /*+ pGrunt->v.view_ofs*/, pScan->pev->origin, ignore_monsters, pGrunt->v.pContainingEntity, &tr);

			if (tr.flFraction == 1.0)
			{
				if(UTIL_SpectCheck(pScan->edict()) == 0 && pScan->pev->health > 0 && pScan->pev->deadflag == DEAD_NO)
				{
					return pScan->edict();
				}
			}
		}
	}
	return NULL;
}

bool MonsterCanSeeEnemy(edict_t *pGrunt)
{
	TraceResult tr;
		
	UTIL_TraceLine( pGrunt->v.origin/* + pGrunt->v.view_ofs*/, pGrunt->v.euser1->v.origin, ignore_monsters, pGrunt->v.pContainingEntity, &tr);

	if(UTIL_SpectCheck(pGrunt->v.euser1) == 1)
		return false;

	if (tr.flFraction == 1.0)
	{
		return true;
	} else {
		return false;
	}
}

void MonsterMove(edict_t *pGrunt, int iType, int StatSeq, int Multiplier)
{
	//If this is true, grunt is a non moveable grunt
	if(pGrunt->v.iuser2 == -1)
	{
		//Look in random direction
		int iDirection = (int)RANDOM_LONG(0,359);

		//Look that way
		pGrunt->v.ideal_yaw = iDirection;
		return;
	}

	if(iType == 0)
	{
		pGrunt->v.velocity = Vector ( 0, 0, 0 );
		pGrunt->v.speed = 0;
		pGrunt->v.iuser2 = 0; //Stop moving.
		pGrunt->v.sequence = StatSeq; //Play standing sequence.
		pGrunt->v.vuser2 = Vector(0,0,0); //Set destination to 0

		//If alive, turn 180 if we are facing a wall.
		if(pGrunt->v.iuser4 != 1)
		{
			Vector v_src, v_dest;
			TraceResult tr;

			UTIL_MakeVectors( Vector(pGrunt->v.angles.x,pGrunt->v.ideal_yaw,pGrunt->v.angles.z) );

			v_src = pGrunt->v.origin;
			v_src.z += 20;
			v_dest = v_src + gpGlobals->v_forward * 45;

			UTIL_TraceLine( v_src, v_dest, ignore_monsters, pGrunt->v.pContainingEntity, &tr);

			if(tr.flFraction < 1.0)
			{
				pGrunt->v.ideal_yaw += 180;
				MonsterFixIdealYaw(pGrunt);
			}
		}
		return;
	}

	if (pGrunt->v.flags & FL_ONGROUND)
	{
		if((iType > 0) && (pGrunt->v.iuser2 == 0))
		{
			if(pGrunt->v.vuser2 == Vector(0,0,0))
			{
				pGrunt->v.friction = 0.3;
				//Decide on where to walk to
				int iDirection = (int)RANDOM_LONG(0,359);

				//Look that way
				pGrunt->v.ideal_yaw = iDirection;

				//Do the traceline
				Vector v_src, v_dest;
				TraceResult tr;

				UTIL_MakeVectors( Vector(pGrunt->v.angles.x,pGrunt->v.ideal_yaw,(pGrunt->v.angles.z + 30)) );

				v_src = pGrunt->v.origin + Vector(0,0,(10 * Multiplier));

				v_dest = v_src + gpGlobals->v_forward * RANDOM_LONG(100,600);

				UTIL_TraceLine( v_src, v_dest, ignore_monsters, pGrunt->v.pContainingEntity, &tr);

				//This is our target destination.
				pGrunt->v.vuser2 = tr.vecEndPos;

				//pGrunt->v.vuser2.z = pGrunt->v.vuser2.z + 20; //Why the hell is this needed???

				//Set what we are doing
				pGrunt->v.iuser2 = iType;

				pGrunt->v.vuser4.x = gpGlobals->time + 4;
			}
		}
	}
}

void MonsterFixIdealYaw(edict_t *pGrunt)
{
   // check for wrap around of angle...
   if (pGrunt->v.ideal_yaw > 180)
      pGrunt->v.ideal_yaw -= 360;

   if (pGrunt->v.ideal_yaw < -180)
      pGrunt->v.ideal_yaw += 360;
}

void MonsterBleed(edict_t *pGrunt, int BloodColor)
{
	TraceResult	tr;
	Vector		vecSpot;

	if(RANDOM_LONG(0,2) > 0)
	{
		vecSpot = pGrunt->v.origin + Vector ( RANDOM_LONG(0,8) , RANDOM_LONG(0,8) , 8 );//move up a bit, and trace down.
		UTIL_TraceLine ( vecSpot, vecSpot + Vector ( 0, 0, -24 ),  ignore_monsters, pGrunt, &tr);

		UTIL_BloodDecalTrace( &tr, BloodColor );
	} else {
		UTIL_MakeVectors( Vector(pGrunt->v.angles.x,pGrunt->v.ideal_yaw,(pGrunt->v.angles.z + 20)) );

		Vector v_src = pGrunt->v.origin + Vector(0,0,32);
		Vector v_dest = v_src + gpGlobals->v_forward * -70;

		UTIL_TraceLine( v_src, v_dest, ignore_monsters, pGrunt->v.pContainingEntity, &tr);

		/*MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_SHOWLINE);
			WRITE_COORD( v_src.x );
			WRITE_COORD( v_src.y );
			WRITE_COORD( v_src.z );
			
			WRITE_COORD( tr.vecEndPos.x );
			WRITE_COORD( tr.vecEndPos.y );
			WRITE_COORD( tr.vecEndPos.z );
		MESSAGE_END();*/

		UTIL_BloodDecalTrace( &tr, BloodColor );
	}
}