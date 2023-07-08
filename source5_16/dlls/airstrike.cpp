/*****************************************************\
*	Auto-OP Shodan
*
*	File: airstrike.cpp
*	Description: Code for the Airstrike marker and laucher.
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

void PlantMarker(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SCOUT)
		return;

	//First make sure some demoman isnt doing a launch
	CBaseEntity *pScan = NULL;
	while(pScan = UTIL_FindEntityByClassname(pScan, "ao_airstrike_start"))
	{
		if(pScan->pev->team == pEntity->v.team)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Someone is already calling the airstrike.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Someone is already calling the airstrike.\n"));
			return;
		}
	}

	if( PlayerInfo[ENTINDEX(pEntity)].PlacedPoint > gpGlobals->time)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot place a marker at the moment.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot place a marker at the moment.\n"));
		return;
	}

	TraceResult tr;
	UTIL_TraceLine( pEntity->v.origin, Vector(pEntity->v.origin.x,pEntity->v.origin.y, pEntity->v.origin.z + 4000), ignore_monsters, pEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			if(UTIL_PointContents(tr.vecEndPos) != CONTENTS_SKY)
			{
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You must plant a marker under the sky.\n"));
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You must plant a marker under the sky.\n"));
				return;
			}
		}
	}

	UTIL_TraceLine( pEntity->v.origin, Vector(pEntity->v.origin.x,pEntity->v.origin.y, pEntity->v.origin.z - 200), ignore_monsters, pEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			if(UTIL_PointContents(tr.vecEndPos) != CONTENTS_SKY)
			{
				//Search for an existing team marker and if found, destroy it
				CBaseEntity *pMarkScan = NULL;
				while(pMarkScan = UTIL_FindEntityByClassname(pMarkScan, "ao_airstrike_marker"))
				{
					if(pMarkScan->pev->team == pEntity->v.team)
					{
						pMarkScan->pev->flags |= FL_KILLME;
					}
				}				

				PlayerInfo[ENTINDEX(pEntity)].PlacedPoint = gpGlobals->time + 60;
				//Create a mine
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "buttons/blip2.wav", 1, ATTN_NORM, 0, 140);
				edict_t *pAirPoint = NULL;
    			pAirPoint = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    			DispatchSpawn(pAirPoint);
    			pAirPoint->v.origin = Vector(tr.vecEndPos.x,tr.vecEndPos.y,tr.vecEndPos.z + 20);
    			pAirPoint->v.euser1 = pEntity;
    			pAirPoint->v.angles = Vector(0,0,0);
    			pAirPoint->v.velocity = Vector(0,0,0);
				pAirPoint->v.takedamage = DAMAGE_NO;
				pAirPoint->v.health = 1;
				pAirPoint->v.team = pEntity->v.team;
				pAirPoint->v.movetype = MOVETYPE_NOCLIP;
				pAirPoint->v.nextthink = gpGlobals->time;
				pAirPoint->v.gravity = 0.5;
				pAirPoint->v.friction = 1.0;
				pAirPoint->v.framerate = 1.0;
				pAirPoint->v.sequence = 0;
				pAirPoint->v.solid = SOLID_BBOX;
    			SET_MODEL(pAirPoint, "models/autoop/marker.mdl");
    			UTIL_SetSize(VARS(pAirPoint), Vector(0, 0, 0), Vector(0, 0, 0));
    			pAirPoint->v.classname = MAKE_STRING("ao_airstrike_marker");
				BotTalkAllArgs("%s positioned his teams airstrike marker.\n",STRING(pEntity->v.netname));
			}
		}
	}
}

void SendStrike(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_DEMOMAN)
		return;

	//First make sure some other demoman isnt doing a launch first!
	CBaseEntity *pScan = NULL;
	while(pScan = UTIL_FindEntityByClassname(pScan, "ao_airstrike_start"))
	{
		if(pScan->pev->team == pEntity->v.team)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Someone is already calling the airstrike.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Someone is already calling the airstrike.\n"));
			return;
		}
	}

	//Make sure he isnt near a func_nobuild
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot launch from here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot launch from here.\n"));
			return;
		}
	}

	//Ok, now search for his teams marker
	CBaseEntity *pMarker = NULL;
	while(pMarker = UTIL_FindEntityByClassname(pMarker, "ao_airstrike_marker"))
	{
		if(pMarker->pev->team == pEntity->v.team)
		{
			//Begin launch sequence.
			edict_t *pStrikeStart = NULL;
    		pStrikeStart = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    		DispatchSpawn(pStrikeStart);
    		pStrikeStart->v.origin = pEntity->v.origin;
    		pStrikeStart->v.euser1 = pEntity;
			pStrikeStart->v.euser2 = pMarker->edict(); //Pass marker with this ent
    		pStrikeStart->v.angles = Vector(0,0,0);
    		pStrikeStart->v.velocity = Vector(0,0,0);
			pStrikeStart->v.takedamage = DAMAGE_NO;
			pStrikeStart->v.iuser4 = (AIRSTRIKE_CHARGE_TIME * 2); //No of 1/2 seconds demoman cannot move.
			pStrikeStart->v.vuser4 = pMarker->pev->origin; //Where to strike.
			pStrikeStart->v.health = 1;
			pStrikeStart->v.team = pEntity->v.team;
			pStrikeStart->v.movetype = MOVETYPE_NONE;
			pStrikeStart->v.nextthink = gpGlobals->time;
			pStrikeStart->v.framerate = 1.0;
			pStrikeStart->v.sequence = 0;
			pStrikeStart->v.gravity = 0.5;
			pStrikeStart->v.friction = 0.8;
			//pStrikeStart->v.effects = EF_NODRAW;
			pStrikeStart->v.solid = SOLID_BBOX;
    		SET_MODEL(pStrikeStart, ("models/autoop/airstrike.mdl"));
    		UTIL_SetSize(VARS(pStrikeStart), Vector(0, 0, 0), Vector(0, 0, 0));
    		pStrikeStart->v.classname = MAKE_STRING("ao_airstrike_start");
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You started to launch the airstrike.\n"));
		}	
	}
}

bool StartStrikeThink(edict_t *pEntity)
{
	//The Counter
	if(pEntity->v.iuser4 > 0)
	{
		if(pEntity->v.euser1->v.health < 10)
		{
			CancelLaunch(pEntity->v.euser1);
		} else {
			pEntity->v.iuser4 -= 1;
			pEntity->v.euser1->v.flags |= FL_FROZEN; //No movement whilst doing thing

			//Inform Demoman at certian points
			if(pEntity->v.iuser4 == 60)
				CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "30 Seconds till strike...\n"));

			if(pEntity->v.iuser4 == 40)
				CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "20 Seconds till strike...\n"));

			if(pEntity->v.iuser4 == 20)
				CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "10 Seconds till strike...\n"));

			pEntity->v.nextthink = gpGlobals->time + 0.5;
			return 1;
		}
		return 1;
	}

	if(pEntity->v.iuser4 == 0)
	{
		pEntity->v.euser1->v.flags &= ~FL_FROZEN;
		pEntity->v.euser2->v.flags |= FL_KILLME;
		pEntity->v.flags |= FL_KILLME;
		DoTheStrike(pEntity->v.vuser4);
		CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "AirStrike launched...\n"));
	}
	return 1;
}

void CancelLaunch(edict_t *pEntity)
{
	//Search for the airstrike starter
	CBaseEntity *pStarter = NULL;
	while(pStarter = UTIL_FindEntityByClassname(pStarter, "ao_airstrike_start"))
	{
		if(pStarter->pev->euser1 == pEntity)
		{
			pEntity->v.flags &= ~FL_FROZEN; //Unfreeze
			pStarter->pev->flags |= FL_KILLME; //Kill Starter
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cancelled the Air-Strike launch.\n"));
			pEntity->v.flags &= ~FL_FROZEN; //Unfreeze to be sure
		}
	}
}

void DoTheStrike(Vector vArea)
{
	//Create an entity which will think for us
	edict_t *pStrikeStart = NULL;
    pStrikeStart = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    DispatchSpawn(pStrikeStart);
    pStrikeStart->v.origin = vArea;
    pStrikeStart->v.angles = Vector(0,0,0);
    pStrikeStart->v.velocity = Vector(0,0,0);
	pStrikeStart->v.takedamage = DAMAGE_NO;
	pStrikeStart->v.iuser4 = AIRSTRIKE_AMOUNT; //No of bombs
	pStrikeStart->v.vuser4 = vArea; //pass on the vArea
	pStrikeStart->v.health = 1;
	pStrikeStart->v.movetype = MOVETYPE_NONE;
	pStrikeStart->v.nextthink = gpGlobals->time;
	pStrikeStart->v.gravity = 0.5;
	pStrikeStart->v.friction = 0.8;
	pStrikeStart->v.effects = EF_NODRAW;
	pStrikeStart->v.solid = SOLID_BBOX;
    SET_MODEL(pStrikeStart, "models/pent.mdl");
    UTIL_SetSize(VARS(pStrikeStart), Vector(0, 0, 0), Vector(0, 0, 0));
    pStrikeStart->v.classname = MAKE_STRING("ao_airstrike_counter");
	EMIT_SOUND_DYN2(pStrikeStart, CHAN_VOICE, "weapons/mortar.wav", 1, ATTN_NORM, 0, 100);

}

bool AirStrikeCounterThink(edict_t *pEntity)
{
	//Do a bomb
	StrikeLocation(pEntity->v.vuser4,pEntity->v.iuser4);

	//Minus one from our internal counter
	pEntity->v.iuser4 -= 1;

	pEntity->v.nextthink = gpGlobals->time + 1; //Next bomb in 1 second

	if(pEntity->v.iuser4 <= 0)
	{
		pEntity->v.flags |= FL_KILLME;
	}
	return 1;
}



void StrikeLocation(Vector vArea, int iBomb)
{
	Vector vLocation;
	int iDoSound = 0;

	//If its bomb 1, go right on target and do landing sound
	if(iBomb==1)
	{
		vLocation = vArea;
		iDoSound = 1;
	} else {
		//Randomly set an X and Y position (Z is done via tracelines)
		vLocation.x = 0;
		vLocation.y = 0;
		vLocation.z = vArea.z;

		vLocation.x = vArea.x + RANDOM_LONG(-AIRSTRIKE_RANDOMNESS,AIRSTRIKE_RANDOMNESS);
		vLocation.y = vArea.y + RANDOM_LONG(-AIRSTRIKE_RANDOMNESS,AIRSTRIKE_RANDOMNESS);
	}

	//Now hurt any poor bastards nearby.
	TraceResult tr;
	UTIL_TraceLine( vLocation, Vector(vLocation.x, vLocation.y, vLocation.z - 4000), ignore_monsters, NULL, &tr);

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			if(UTIL_PointContents(tr.vecEndPos) != CONTENTS_SKY)
			{
				/*if(iDoSound == 1)
				{
					EMIT_SOUND_DYN2(tr.pHit, CHAN_VOICE, "weapons/mortar.wav", 1, ATTN_NORM, 0, 100);
				}*/
				//Do a beamy thing here that will make it look real
				int m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
				MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
				WRITE_BYTE( TE_BEAMPOINTS );
				WRITE_COORD(vLocation.x);
				WRITE_COORD(vLocation.y);
				WRITE_COORD(vLocation.z);
				WRITE_COORD(tr.vecEndPos.x);
				WRITE_COORD(tr.vecEndPos.y);
				WRITE_COORD(tr.vecEndPos.z + 1024);
				WRITE_SHORT(m_spriteTexture );
				WRITE_BYTE( 0 ); // framerate
				WRITE_BYTE( 0 ); // framerate
				WRITE_BYTE( 1 ); // life
				WRITE_BYTE( 40 );  // width
				WRITE_BYTE( 0 );   // noise
				WRITE_BYTE( 255 );   // r, g, b
				WRITE_BYTE( 160 );   // r, g, b
				WRITE_BYTE( 100 );   // r, g, b
				WRITE_BYTE( 128 );	// brightness
				WRITE_BYTE( 0 );		// speed
				MESSAGE_END();

				//Hurt some stuff
				edict_t *pBomb;
				pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
				DispatchSpawn(pBomb);
				pBomb->v.origin = tr.vecEndPos;
				pBomb->v.velocity = Vector(0,0,0);
				pBomb->v.dmg = 300;
				//pBomb->v.owner = ???;
				pBomb->v.effects |= EF_NODRAW;
				pBomb->v.nextthink = gpGlobals->time;
				pBomb->v.classname = MAKE_STRING("weapon_ao_airstrike");

				MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
					WRITE_BYTE( TE_BEAMTORUS);
					WRITE_COORD(tr.vecEndPos.x);
					WRITE_COORD(tr.vecEndPos.y);
					WRITE_COORD(tr.vecEndPos.z + 32);
					WRITE_COORD(tr.vecEndPos.x);
					WRITE_COORD(tr.vecEndPos.y);
					WRITE_COORD(tr.vecEndPos.z + 32 + 300 * 2 / .2); // reach damage radius over .3 seconds
					WRITE_SHORT(m_spriteTexture );
					WRITE_BYTE( 0 ); // startframe
					WRITE_BYTE( 0 ); // framerate
					WRITE_BYTE( 2 ); // life
					WRITE_BYTE( 12 );  // width
					WRITE_BYTE( 0 );   // noise
					WRITE_BYTE( 255 );   // r, g, b
					WRITE_BYTE( 160 );   // r, g, b
					WRITE_BYTE( 100 );   // r, g, b
					WRITE_BYTE( 255 );	// brightness
					WRITE_BYTE( 0 );		// speed
				MESSAGE_END();

			}
		}
	}
}
					















