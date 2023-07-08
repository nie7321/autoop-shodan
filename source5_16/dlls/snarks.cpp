/*****************************************************\
*	Auto-OP Shodan
*
*	File: snarks.cpp
*	Description: Snipers Snark functions.
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

extern short g_sModelIndexBloodSpray;
extern short g_sModelIndexBloodDrop;

void ThrowSnark(edict_t *pEntity)
{
	if(gpGlobals->time < PlayerInfo[ENTINDEX(pEntity)].SnarkLastThrow)
		return; //Snark throw delay
	
	if(pEntity->v.playerclass != CLASS_SNIPER || pEntity->v.health <= 0)
		return;

	if(PlayerInfo[ENTINDEX(pEntity)].SnarkAmmo < 1)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You have no snarks left.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have no snarks left.\n"));
		return;
	}

	PlayerInfo[ENTINDEX(pEntity)].SnarkLastThrow = gpGlobals->time + 0.5;

	UTIL_MakeVectors( pEntity->v.v_angle );
	TraceResult tr;
	Vector trace_origin;

	trace_origin = pEntity->v.origin;
	if ( pEntity->v.flags & FL_DUCKING )
	{
		trace_origin = trace_origin - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	}

	UTIL_TraceLine( trace_origin + gpGlobals->v_forward * 20, trace_origin + gpGlobals->v_forward * 64, dont_ignore_monsters, NULL, &tr );

	if (tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > 0.25)
	{
		PlayerInfo[ENTINDEX(pEntity)].SnarkAmmo -= 1;

		int gmsgSecAmmoVal = REG_USER_MSG("SecAmmoVal", 3);

		if(gmsgSecAmmoVal < MAX_USERMSG)
		{
			MESSAGE_BEGIN( MSG_ONE, gmsgSecAmmoVal, NULL, pEntity );
				WRITE_BYTE(1);
				WRITE_BYTE((int)PlayerInfo[ENTINDEX(pEntity)].SnarkAmmo);
			MESSAGE_END();
		}

		//Create Snark Entity
		edict_t *pSnark = NULL;
		pSnark = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
		pSnark->v.origin = tr.vecEndPos;
		pSnark->v.owner = pEntity;
		pSnark->v.euser1 = NULL; //Current victim
		pSnark->v.angles = pEntity->v.v_angle;
		pSnark->v.takedamage = DAMAGE_AIM;
		pSnark->v.gravity = 0.5;
		pSnark->v.fuser1 = gpGlobals->time + 15.0; //Snark Detonate delay
		pSnark->v.fuser2 = 0.0; //Hunt Delay.
		pSnark->v.fuser3 = gpGlobals->time; //Sound Delay
		pSnark->v.friction = 0.5;
		pSnark->v.vuser1 = Vector(0,0,0); //vTarget
		pSnark->v.health = 100000;
		pSnark->v.solid = SOLID_SLIDEBOX;
		pSnark->v.movetype = MOVETYPE_TOSS;
		pSnark->v.dmg = 20;
		pSnark->v.team = pEntity->v.team;
		pSnark->v.sequence = 3; //Run
		pSnark->v.framerate = 1.0;
		DispatchSpawn(pSnark);
		SET_MODEL(pSnark, "models/w_squeak.mdl");
		pSnark->v.classname = MAKE_STRING("weapon_ao_snark");
		pSnark->v.nextthink = gpGlobals->time + 0.1;
		UTIL_SetSize(VARS(pSnark), Vector( -4, -4, 0), Vector(4, 4, 8));
		UTIL_SetOrigin( VARS(pSnark), pSnark->v.origin );

		pSnark->v.velocity = gpGlobals->v_forward * 200 + pEntity->v.velocity;

		float flRndSound = RANDOM_FLOAT ( 0 , 1 );

		if ( flRndSound <= 0.5 )
			EMIT_SOUND_DYN2(pSnark, CHAN_VOICE, "squeek/sqk_hunt2.wav", 1, ATTN_NORM, 0, 105);
		else 
			EMIT_SOUND_DYN2(pSnark, CHAN_VOICE, "squeek/sqk_hunt3.wav", 1, ATTN_NORM, 0, 105);

		UTIL_LogPrintf( "[AUTOOP] %s dropped a snark.\n", STRING(pEntity->v.netname));

	}
}

void SnarkTouch(edict_t *pEntity, edict_t *pVictim) //Now obselete.
{
	//Do stuff here
	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "squeek/sqk_deploy1.wav", 1, ATTN_NORM, 0, 120);
	return;
}

bool SnarkThink(edict_t *pEntity)
{
	if(pEntity->v.owner->v.team != pEntity->v.team)
	{
		KillSnark(pEntity);
		return 1;
	}
	
	if(pEntity->v.euser1 != NULL)
	{
		float vdist;
		vdist = (pEntity->v.origin - pEntity->v.euser1->v.origin).Length();
		//CLIENT_PRINTF( pEntity->v.owner, print_center, UTIL_VarArgs( "SNARK TO VICTIM DISTANCE - %f0.0\n",vdist));

		if ((pEntity->v.origin - pEntity->v.euser1->v.origin).Length() < 50.0)
		{
			if(pEntity->v.fuser3 < gpGlobals->time) //Delay the sound to stop overflows.
			{ 
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "squeek/sqk_deploy1.wav", 1, ATTN_NORM, 0, 120);
				if(pEntity->v.takedamage != DAMAGE_NO)
				{
					if(pEntity->v.euser1->v.health <= 8)
					{
						edict_t *pBomb;
						pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
						DispatchSpawn(pBomb);
						pBomb->v.origin = pEntity->v.origin;
						pBomb->v.dmg = 100;
						pBomb->v.owner = pEntity->v.owner;
						pBomb->v.classname = MAKE_STRING("ao_snark_kill");
						pBomb->v.effects |= EF_NODRAW;
						pBomb->v.nextthink = gpGlobals->time;
					} else {
						TakeDamage(pEntity->v.euser1,NULL,8,1);
						//pEntity->v.euser1->v.health -= 8;
					}
				}
				pEntity->v.fuser3 = gpGlobals->time + 1;
			}
		}
	}

	if(pEntity->v.health <= 100000 - SNARK_HEALTH)
	{
		KillSnark(pEntity);
		return 1;
	}

	pEntity->v.nextthink = gpGlobals->time + 0.1;
	
	if(gpGlobals->time >= pEntity->v.fuser1)
	{
		KillSnark(pEntity);
		return 1;
	}

	// float
	if (pEntity->v.waterlevel != 0)
	{
		if (pEntity->v.movetype == MOVETYPE_BOUNCE)
		{
			pEntity->v.movetype = MOVETYPE_FLY;
		}
		pEntity->v.velocity = pEntity->v.velocity * 0.9;
		pEntity->v.velocity.z += 8.0;
	}
	else if (pEntity->v.movetype = MOVETYPE_FLY)
	{
		pEntity->v.movetype = MOVETYPE_BOUNCE;
	}

	// return if not time to hunt
	if (pEntity->v.fuser2 > gpGlobals->time)
		return 1;

	pEntity->v.fuser2 = gpGlobals->time + 1.0;

	CBaseEntity *pOther = NULL;
	Vector vecDir;
	TraceResult tr;

	Vector vecFlat = pEntity->v.velocity;
	vecFlat.z = 0;
	vecFlat = vecFlat.Normalize( );

	UTIL_MakeVectors( pEntity->v.angles );
	
	if (pEntity->v.euser1 == NULL || pEntity->v.euser1->v.health <= 0 || pEntity->v.euser1->v.deadflag != DEAD_NO)
	{
		// find target, bounce a bit towards it.
		pEntity->v.euser1 = FindVictim(pEntity,512);
	}

	// squeek if it's about time blow up
	if ((pEntity->v.fuser1 - gpGlobals->time <= 0.5) && (pEntity->v.fuser1 - gpGlobals->time >= 0.3))
	{
		EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "squeek/sqk_die1.wav", 1, ATTN_NORM, 0, 100 + RANDOM_LONG(0,0x3F));
	}

	if (pEntity->v.euser1 != NULL)
	{
		if (EnemyIsVisible(pEntity,pEntity->v.euser1))
		{
			vecDir = (pEntity->v.euser1->v.origin + pEntity->v.euser1->v.view_ofs) - pEntity->v.origin;
			pEntity->v.vuser1 = vecDir.Normalize( );
		}

		float flVel = pEntity->v.velocity.Length();
		float flAdj = 50.0 / (flVel + 10.0);

		if (flAdj > 1.2)
			flAdj = 1.2;

		pEntity->v.velocity = pEntity->v.velocity * flAdj + pEntity->v.vuser1 * 300;
	}

	if (pEntity->v.flags & FL_ONGROUND)
	{
		pEntity->v.avelocity = Vector( 0, 0, 0 );
	}
	else
	{
		if (pEntity->v.avelocity == Vector( 0, 0, 0))
		{
			pEntity->v.avelocity.x = RANDOM_FLOAT( -100, 100 );
			pEntity->v.avelocity.z = RANDOM_FLOAT( -100, 100 );
		}
	}

	if ((pEntity->v.origin - pEntity->v.vuser2).Length() < 1.0)
	{
		pEntity->v.velocity.x = RANDOM_FLOAT( -100, 100 );
		pEntity->v.velocity.y = RANDOM_FLOAT( -100, 100 );
	}
	pEntity->v.vuser2 = pEntity->v.origin;

	pEntity->v.angles = UTIL_VecToAngles( pEntity->v.velocity );
	pEntity->v.angles.z = 0;
	pEntity->v.angles.x = 0;
	return 1;
}

edict_t *FindVictim(edict_t *pEntity, int iDistance)
{
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, iDistance)) != NULL)
	{
		//Is it an entity we are bothered about? (Players)
		if (strcmp(STRING(pScan->pev->classname), "player") == 0)
		{
			//Can we see this potential enemy?
			//If I fire a traceline to the entity, if it gets blocked
			//By something I can ignore it else i must be able to see it.
			TraceResult tr;
				
			UTIL_TraceLine( pEntity->v.origin + pEntity->v.view_ofs, pScan->pev->origin, ignore_monsters, pEntity->v.pContainingEntity, &tr);

			if (tr.flFraction == 1.0)
			{
				if(UTIL_SpectCheck(pScan->edict()) == 0 && pScan->pev->health > 0 && pScan->pev->deadflag == DEAD_NO)
				{
					int Team1 = pScan->pev->team;
					int Team2 = pEntity->v.team;
					if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
					{
						//Found enemy!
						return pScan->edict();
					}
				}
			}
		}
	}
	return NULL;
}

int EnemyIsVisible(edict_t *pSnark, edict_t *pTarget)
{
	TraceResult tr;
		
	UTIL_TraceLine( pSnark->v.origin + pSnark->v.view_ofs, pTarget->v.origin, ignore_monsters, pSnark->v.pContainingEntity, &tr);

	if (tr.flFraction == 1.0)
	{
		return 1;
	} else {
		return 0;
	}
}

void KillSnark(edict_t *pEntity)
{
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	pEntity->v.model = 0;
	pEntity->v.flags |= FL_KILLME;
	pEntity->v.takedamage = DAMAGE_NO;
	pEntity->v.effects = EF_NODRAW;
	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "squeek/sqk_blast1.wav", 0.75, ATTN_NORM, 0, 105);

	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin );
		WRITE_BYTE( TE_BLOODSPRITE );
		WRITE_COORD( pEntity->v.origin.x);								// pos
		WRITE_COORD( pEntity->v.origin.y);
		WRITE_COORD( pEntity->v.origin.z);
		WRITE_SHORT( g_sModelIndexBloodSpray );				// initial sprite model
		WRITE_SHORT( g_sModelIndexBloodDrop );				// droplet sprite models
		WRITE_BYTE( 195 );								// color index into host_basepal
		WRITE_BYTE( min( max( 3, 80 / 10 ), 16 ) );		// size
	MESSAGE_END();
}



