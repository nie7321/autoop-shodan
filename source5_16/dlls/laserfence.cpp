/*****************************************************\
*	Auto-OP Shodan
*
*	File: laserfence.cpp
*	Description: Laser Fence Post Functions For AutoOP
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
extern int m_Chain;
extern int team_allies[4];

int FENCEPOST_REQUIRED = 50; //Amount of armour required for the laser fencepost

void BuildLaserFence(edict_t *pEntity)
{
	if(PlayerInfo[ENTINDEX(pEntity)].HasFencePost == 1)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cant build another sensor.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cant build another sensor.\n"));
		return;
	}

	if(GetpvPrivateInt(pEntity,208) /*pEntity->v.armorvalue*/ < FENCEPOST_REQUIRED)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough metal to build sensor.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough metal to build sensor.\n"));
		return;
	}

	//Scan to make sure we are not in a func
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot build here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot build here.\n"));
			return;
		}
	}

	if(PlayerInfo[ENTINDEX(pEntity)].LastBuilt > gpGlobals->time)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Please wait before building\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Please wait before building\n"));
		return;
	}

	PlayerInfo[ENTINDEX(pEntity)].LastBuilt = gpGlobals->time + 0.5;

    TraceResult tr;
    UTIL_MakeVectors(pEntity->v.v_angle);
    UTIL_TraceLine(pEntity->v.origin, pEntity->v.origin + Vector(gpGlobals->v_forward.x*64, gpGlobals->v_forward.y*64, 0), dont_ignore_monsters, pEntity, &tr);

    if(tr.flFraction==1.0 && UTIL_CheckBox(tr.vecEndPos, Vector(32, 32, 64)))
    {
	    //pEntity->v.armorvalue -= FENCEPOST_REQUIRED;
		PatchpvPrivateInt(pEntity,208,(GetpvPrivateInt(pEntity,208) - FENCEPOST_REQUIRED));
		PlayerInfo[ENTINDEX(pEntity)].HasFencePost = 1;
	 	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/building.wav", 1, ATTN_NORM, 0, 130);
	 	edict_t *pFencePost;
		pFencePost = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
		DispatchSpawn(pFencePost);
		pFencePost->v.origin = tr.vecEndPos;
		pFencePost->v.angles = pEntity->v.v_angle;
		pFencePost->v.angles.x = 0;
        pFencePost->v.angles.z = 0;
        pFencePost->v.angles.y += 180; // Face the player
        if(pFencePost->v.angles.y >= 360) pFencePost->v.angles.z -= 360;
		UTIL_MakeVectors(pEntity->v.v_angle);
		pFencePost->v.velocity = Vector(0, 0, 0);
		pFencePost->v.euser1 = pEntity;
		pFencePost->v.euser2 = NULL; //No Partner1 yet.
		pFencePost->v.euser3 = NULL; //No Partner2 yet.
		pFencePost->v.euser4 = NULL; //No Partner3 yet.
		pFencePost->v.iuser4 = 0; //Not looked for friends yet.
		pFencePost->v.iuser3 = 0;
		pFencePost->v.takedamage = DAMAGE_YES;
		pFencePost->v.fuser1 = gpGlobals->time + 1.5;
		pFencePost->v.health = 100000;
		pFencePost->v.max_health = FENCEPOST_HEALTH;
		pFencePost->v.movetype = MOVETYPE_TOSS;
		pFencePost->v.solid = SOLID_BBOX;
		pFencePost->v.team = pEntity->v.team;
		SET_MODEL(pFencePost, "models/autoop/sensorpost.mdl");
		UTIL_SetSize(VARS(pFencePost), Vector(-16, -16, 0), Vector(16, 16, 64));
		pFencePost->v.classname = MAKE_STRING("weapon_ao_fencepost");
		pFencePost->v.nextthink = gpGlobals->time + 0.1;

		UTIL_LogPrintf( "[AUTOOP] %s built a Sensor post\n", STRING(pEntity->v.netname));

		//Set player variables so player knows he has a fence post.
	}
}

bool LaserFenceThink(edict_t *pEntity)
{
	//First check we can still live
	if(pEntity->v.euser1->v.playerclass != CLASS_ENGINEER || (pEntity->v.euser1->v.team != pEntity->v.team) || (pEntity->v.health <= 100000 - FENCEPOST_HEALTH))
	{
		DetonateFencePost(pEntity);

		//kill lasers
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin);
				WRITE_BYTE(TE_KILLBEAM);
				WRITE_SHORT(ENTINDEX(pEntity));
		MESSAGE_END();

		return 1;
	}

	//are we stood ono a moving platform? of so destroy ourselves.
	TraceResult tr;
	UTIL_TraceLine(pEntity->v.origin, pEntity->v.origin + Vector(0, 0, -200), ignore_monsters, pEntity, &tr);
	if ((strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_platform", STRING(tr.pHit->v.classname)) == 0))
	{
		DetonateFencePost(pEntity);
		return 1;
	}

	//----FINDING A PARTNER POST----

	if((pEntity->v.iuser4 == 0) && (pEntity->v.fuser1 < gpGlobals->time))
	{
		pEntity->v.iuser4 = 1;

		int iScanned = 1;
		//Next check for potential partners if we don't have one.
		CBaseEntity *pPartner = NULL;
		while ((pPartner = UTIL_FindEntityInSphere(pPartner, pEntity->v.origin, 500)) != NULL)
		{
			//Is it a fence post?
			if((pPartner->edict() != pEntity) && pPartner->pev->classname == MAKE_STRING("weapon_ao_fencepost"))
			{
				//Is this fencepost on the same ground level as us?
				if((pPartner->pev->origin.z <= pEntity->v.origin.z + 20) && (pPartner->pev->origin.z >= pEntity->v.origin.z - 20))
				{
					if(pPartner->pev->team == pEntity->v.team)
					{
						//Looks good, we got ourselves a friend :)
						if(iScanned == 1)
							pEntity->v.euser2 = pPartner->edict();

						if(iScanned == 2)
							pEntity->v.euser3 = pPartner->edict();

						if(iScanned == 3)
							pEntity->v.euser4 = pPartner->edict();

						if(iScanned < 3)
						{
							iScanned++;
							CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "Your sensor joined with another.\n"));
							CLIENT_PRINTF( pPartner->pev->euser1, print_center, UTIL_VarArgs( "Your sensor joined with another.\n"));
						} else {
							pEntity->v.nextthink = gpGlobals->time;
							return 1;
						}
					}
				}
			}
		}
		pEntity->v.fuser3 = gpGlobals->time + 1;
		pEntity->v.nextthink = gpGlobals->time + 0.1;
		return 1; //Failed to find partner
	}

	if(pEntity->v.fuser3 <= gpGlobals->time)
	{
		pEntity->v.iuser3 = 1;
		pEntity->v.fuser3 = gpGlobals->time + 10;
	}
	
	//----WHAT TO DO IF WE HAVE A PARTNER----
	
	edict_t *pTemp = NULL;

	int iLoop = 0;

	for(iLoop = 1;iLoop <= 3;iLoop++)
	{
		if(iLoop == 1)
			pTemp = pEntity->v.euser2;

		if(iLoop == 2)
			pTemp = pEntity->v.euser3;

		if(iLoop == 3)
			pTemp = pEntity->v.euser4;
	
		//Check partner is still here
		if(FNullEnt(pTemp) || pTemp->free)
		{
			if(iLoop == 1)
				pEntity->v.euser2 = NULL; //No longer have partner

			if(iLoop == 2)
				pEntity->v.euser3 = NULL; //No longer have partner

			if(iLoop == 3)
				pEntity->v.euser4 = NULL; //No longer have partner
			
			//kill lasers
			MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin);
					WRITE_BYTE(TE_KILLBEAM);
					WRITE_SHORT(ENTINDEX(pEntity));
			MESSAGE_END();

		}

		if((pTemp != NULL) && (pTemp->v.health > 100000 - FENCEPOST_HEALTH))
		{
			if(pEntity->v.iuser3 == 1)
			{
				//Draw Beam Here
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/electro6.wav", 1, ATTN_NORM, 0, 140);

				Vector vColor = Vector(255,0,0); //Default laser color

				if(pEntity->v.team == 1)
				{
					vColor.x = 0;
					vColor.y = 0;
					vColor.z = 200;
				}

				if(pEntity->v.team == 2)
				{
					vColor.x = 200;
					vColor.y = 0;
					vColor.z = 0;
				}

				if(pEntity->v.team == 3)
				{
					vColor.x = 200;
					vColor.y = 200;
					vColor.z = 0;
				}

				if(pEntity->v.team == 4)
				{
					vColor.x = 0;
					vColor.y = 200;
					vColor.z = 0;
				}
				
				//BOTTOM LASER
				MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin ); //Draw sensor energy.
				WRITE_BYTE( TE_BEAMPOINTS ); 
				WRITE_COORD( pEntity->v.origin.x );
				WRITE_COORD( pEntity->v.origin.y );
				WRITE_COORD( pEntity->v.origin.z + 38 );
				WRITE_COORD( pTemp->v.origin.x );
				WRITE_COORD( pTemp->v.origin.y );
				WRITE_COORD( pTemp->v.origin.z + 38 );
				WRITE_SHORT( m_Chain ); 
				WRITE_BYTE( 1 ); //start frame
				WRITE_BYTE( 10 );
				WRITE_BYTE( 95 ); //9.5 Seconds
				WRITE_BYTE( 2 ); //width 
				WRITE_BYTE( 2 ); 
				WRITE_BYTE( vColor.x ); //r
				WRITE_BYTE( vColor.y ); //g
				WRITE_BYTE( vColor.z ); //b
				WRITE_BYTE( 75 );
				WRITE_BYTE( 10 );
				MESSAGE_END( );
				
				//MIDDLE LASER
				MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin ); //Draw sensor energy.
				WRITE_BYTE( TE_BEAMPOINTS ); 
				WRITE_COORD( pEntity->v.origin.x );
				WRITE_COORD( pEntity->v.origin.y );
				WRITE_COORD( pEntity->v.origin.z + 47 );
				WRITE_COORD( pTemp->v.origin.x );
				WRITE_COORD( pTemp->v.origin.y );
				WRITE_COORD( pTemp->v.origin.z + 47 );
				WRITE_SHORT( m_Chain ); 
				WRITE_BYTE( 1 ); //start frame
				WRITE_BYTE( 10 );
				WRITE_BYTE( 95 ); //9.5 Seconds
				WRITE_BYTE( 2 ); //width 
				WRITE_BYTE( 2 ); 
				WRITE_BYTE( vColor.x ); //r
				WRITE_BYTE( vColor.y ); //g
				WRITE_BYTE( vColor.z ); //b
				WRITE_BYTE( 75 );
				WRITE_BYTE( 10 );
				MESSAGE_END( );

				//TOP LASER
				MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin ); //Draw sensor energy.
				WRITE_BYTE( TE_BEAMPOINTS ); 
				WRITE_COORD( pEntity->v.origin.x );
				WRITE_COORD( pEntity->v.origin.y );
				WRITE_COORD( pEntity->v.origin.z + 56 );
				WRITE_COORD( pTemp->v.origin.x );
				WRITE_COORD( pTemp->v.origin.y );
				WRITE_COORD( pTemp->v.origin.z + 56 );
				WRITE_SHORT( m_Chain ); 
				WRITE_BYTE( 1 ); //start frame
				WRITE_BYTE( 10 );
				WRITE_BYTE( 95 ); //9.5 Seconds
				WRITE_BYTE( 2 ); //width 
				WRITE_BYTE( 2 ); 
				WRITE_BYTE( vColor.x ); //r
				WRITE_BYTE( vColor.y ); //g
				WRITE_BYTE( vColor.z ); //b
				WRITE_BYTE( 75 );
				WRITE_BYTE( 10 );
				MESSAGE_END( );
			}

			//Draw traceline to partner to check for enemies.
			TraceResult tr;
			Vector vOne = Vector(pEntity->v.origin.x,pEntity->v.origin.y,pEntity->v.origin.z + 38);
			Vector vTwo = Vector(pTemp->v.origin.x,pTemp->v.origin.y,pTemp->v.origin.z + 38);
			UTIL_TraceLine(vOne,vTwo, dont_ignore_monsters, pEntity, &tr);	
			if (tr.flFraction < 1.0)
			{
				//Ignoring anything except players.
				if ( (strcmp("player", STRING(tr.pHit->v.classname)) == 0))
				{
					int Team1 = tr.pHit->v.team;
					int Team2 = pEntity->v.team;
					if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
					{
						edict_t *pBomb;
						pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
						DispatchSpawn(pBomb);
						pBomb->v.origin = tr.pHit->v.origin + Vector(0, 0, 32);
						pBomb->v.dmg = 200;
						pBomb->v.team = pEntity->v.team;
						pBomb->v.owner = pEntity->v.euser1;
						pBomb->v.classname = MAKE_STRING("ao_sensorkill");
						pBomb->v.effects |= EF_NODRAW;
						pBomb->v.nextthink = gpGlobals->time;
					}
				}			
			}
		} //for
	}
	pEntity->v.iuser3 = 0;
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void DetonateFencePost(edict_t *pEntity)
{
	CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "Your sensor was destroyed.\n"));
	CLIENT_PRINTF( pEntity->v.euser1, print_console,UTIL_VarArgs( "Your sensor was destroyed.\n"));
	PlayerInfo[ENTINDEX(pEntity->v.euser1)].HasFencePost = 0;

	//Disconnect other sensors from me.
	SensorDisconnect(pEntity);

   	edict_t *pBomb;
	pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
	DispatchSpawn(pBomb);
	pBomb->v.origin = pEntity->v.origin;
	pBomb->v.velocity = Vector(0,0,0);
	pBomb->v.dmg = 5;
	pBomb->v.team = pEntity->v.team;
	pBomb->v.owner = pEntity->v.euser1;
	pBomb->v.effects |= EF_NODRAW;
	pBomb->v.nextthink = gpGlobals->time;
	pBomb->v.classname = MAKE_STRING("ao_sensordetonate");

	pEntity->v.flags |= FL_KILLME;

	UTIL_LogPrintf( "[AUTOOP] A fence post detonated\n");
	return;
}

void RepairSensor(edict_t *pEntity)
{
	if(pEntity->v.armorvalue < FENCEPOST_REQUIRED)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough armor to repair sensor.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough armor to repair sensor.\n"));
		return;
	}

	CBaseEntity *pFencePost = NULL;
    while(pFencePost = UTIL_FindEntityByClassname(pFencePost, "weapon_ao_fencepost"))
    {
        if((pFencePost->pev->euser1 == pEntity) && (pFencePost->pev->health < 100000))
        {
			CBaseEntity *pScan = NULL;
			while ((pScan = UTIL_FindEntityInSphere(pScan, pFencePost->pev->origin, 100)) != NULL)
			{
				//Is it an entity we are bothered about? (Players)
				if (strcmp(STRING(pScan->pev->classname), "player") == 0)
				{
					if(pScan->edict() == pEntity) //My owner?
					{
						CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your sensor was recharged.\n"));
						CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your sensor was recharged.\n"));
						pFencePost->pev->health = 100000;
					}
				}
			}
		}
	}

	UTIL_LogPrintf( "[AUTOOP] A fencepost was repaired\n");
	return;
}

void SensorDisconnect(edict_t *pEntity)
{
	CBaseEntity *pFencePost = NULL;
    while(pFencePost = UTIL_FindEntityByClassname(pFencePost, "weapon_ao_fencepost"))
    {
		if(pFencePost->pev->euser2 == pEntity) { pFencePost->pev->euser2 = NULL; }
		if(pFencePost->pev->euser3 == pEntity) { pFencePost->pev->euser3 = NULL; }
		if(pFencePost->pev->euser4 == pEntity) { pFencePost->pev->euser4 = NULL; }
	}
}







