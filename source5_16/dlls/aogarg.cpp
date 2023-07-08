/*****************************************************\
*	Auto-OP Shodan
*
*	File: aogarg.cpp
*	Description: Gargantua Code.
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
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
#include "aogarg.h"
#include "decals.h"
#include "aomonsters.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern DLL_FUNCTIONS other_gFunctionTable;
extern TFXFeatureEnabled_s Features;

extern short g_sModelIndexBloodSpray;
extern short g_sModelIndexBloodDrop;

void CreateGargCoords(bool CanWalk, int x, int y, int z)
{
	edict_t *pGarg = NULL;
	pGarg = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	SET_MODEL(pGarg, "models/garg.mdl");
	pGarg->v.effects	= 0;
	pGarg->v.euser1		= NULL; //Store current enemy here
	pGarg->v.fuser2		= gpGlobals->time; //Talk wait time
	pGarg->v.fuser3		= gpGlobals->time; //Attack wait time
	pGarg->v.fuser4		= gpGlobals->time; //Sequence reset time
	pGarg->v.iuser4		= 0; //1 if dead.
	pGarg->v.iuser3		= 14; //Set death seq
	
	if(CanWalk)
		pGarg->v.iuser2		= 0; //1 - Walk, 2 - Run
	else
		pGarg->v.iuser2		= -1;//-1 - Never move

	pGarg->v.origin.x = x;
	pGarg->v.origin.y = y;
	pGarg->v.origin.z = z;

	//Set animation
	pGarg->v.framerate = 1;
	pGarg->v.sequence = 0;

	DispatchSpawn(pGarg);
	
	pGarg->v.health		= 100000;
	pGarg->v.fuser1		= pGarg->v.health; //For health loss comparison

	UTIL_SetOrigin(VARS(pGarg),pGarg->v.origin);
	pGarg->v.solid			= SOLID_SLIDEBOX;
	pGarg->v.movetype		= MOVETYPE_TOSS;
	UTIL_SetSize(VARS(pGarg), Vector( -32, -32, 0 ), Vector( 32, 32, 64 ));
	pGarg->v.dmg = 80; //Amount of damage to do every attack
	pGarg->v.classname = MAKE_STRING("ao_gargantua");
	pGarg->v.takedamage = DAMAGE_YES;
	pGarg->v.maxspeed = 300;

	pGarg->v.nextthink = gpGlobals->time + 0.1;
}

void CreateGarg(edict_t *pPlayer, bool CanWalk)
{
	edict_t *pGarg = NULL;
	pGarg = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	SET_MODEL(pGarg, "models/garg.mdl");
	pGarg->v.effects	= 0;
	pGarg->v.euser1		= NULL; //Store current enemy here
	pGarg->v.fuser2		= gpGlobals->time; //Talk wait time
	pGarg->v.fuser3		= gpGlobals->time; //Attack wait time
	pGarg->v.fuser4		= gpGlobals->time; //Sequence reset time
	pGarg->v.iuser4		= 0; //1 if dead.
	pGarg->v.iuser3		= 14; //Set death seq
	
	if(CanWalk)
		pGarg->v.iuser2		= 0; //1 - Walk, 2 - Run
	else
		pGarg->v.iuser2		= -1;//-1 - Never move

	pGarg->v.origin		= pPlayer->v.origin;

	//Set animation
	pGarg->v.framerate = 1;
	pGarg->v.sequence = 0;

	DispatchSpawn(pGarg);
	
	pGarg->v.health		= 100000;
	pGarg->v.fuser1		= pGarg->v.health; //For health loss comparison

	UTIL_SetOrigin(VARS(pGarg),pGarg->v.origin);
	pGarg->v.solid			= SOLID_SLIDEBOX;
	pGarg->v.movetype		= MOVETYPE_TOSS;
	UTIL_SetSize(VARS(pGarg), Vector( -32, -32, 0 ), Vector( 32, 32, 64 ));
	pGarg->v.dmg = 80; //Amount of damage to do every attack
	pGarg->v.classname = MAKE_STRING("ao_gargantua");
	pGarg->v.takedamage = DAMAGE_YES;
	pGarg->v.maxspeed = 300;

	//Created garg, now create his eyepiece.
	/*edict_t *pEyeGlow = NULL;
	pEyeGlow = CREATE_NAMED_ENTITY(MAKE_STRING("env_sprite"));
	SET_MODEL(pEyeGlow, "sprites/gargeye1.spr");
	pEyeGlow->v.origin = pGarg->v.origin;
	pEyeGlow->v.solid = SOLID_NOT;

	DispatchSpawn(pEyeGlow);
	
	pEyeGlow->v.rendermode = kRenderGlow;
	pEyeGlow->v.rendercolor.x = 255;
	pEyeGlow->v.rendercolor.y = 255;
	pEyeGlow->v.rendercolor.z = 255;
	pEyeGlow->v.renderamt = 0;
	pEyeGlow->v.renderfx = kRenderFxNoDissipation;

	pEyeGlow->v.skin = ENTINDEX(pGarg);
	pEyeGlow->v.body = 1;
	pEyeGlow->v.aiment = pGarg;
	pEyeGlow->v.movetype = MOVETYPE_FOLLOW;*/

	//pGarg->v.euser2 = pEyeGlow;

	pGarg->v.nextthink = gpGlobals->time + 0.1;
}

bool GargThink(edict_t *pGarg)
{
	pGarg->v.controller[0] = 90;// Make head look straight ahead

	if(pGarg->v.iuser4 == 0) //If not dead...
		MonsterChangeYaw(pGarg,250);

	//If garg is dead, is it time to erase his entity?
	if(pGarg->v.iuser4 == 1 && pGarg->v.fuser4 < gpGlobals->time)
	{
		pGarg->v.flags = FL_KILLME;
		return 1;
	}

	//Took damage?
	if(pGarg->v.health < pGarg->v.fuser1)
	{
		if(pGarg->v.health <= (100000 - GARG_HEALTH))
		{
			if(pGarg->v.iuser4 == 0)
			{
				pGarg->v.fuser4 = gpGlobals->time + 8;//Time to destroy entity
				EMIT_SOUND( pGarg, CHAN_VOICE, "garg/gar_pain3.wav", 1, ATTN_NORM );
			}

			pGarg->v.iuser4 = 1; //Mark as dead

			//Remove eye glow
			/*if(pGarg->v.euser2 != NULL)
				pGarg->v.euser2->v.flags = FL_KILLME;*/

			pGarg->v.takedamage = DAMAGE_NO; //No damage whilst dead
			pGarg->v.solid = SOLID_NOT; //Corpse not solid

			MonsterMove(pGarg,0,12,2); //Stop moving if we are
						
			pGarg->v.sequence = pGarg->v.iuser3; //dead sequence
			pGarg->v.framerate = 0.5;
			if(pGarg->v.frame >= 1 && pGarg->v.sequence == pGarg->v.iuser3) //On final frame
				pGarg->v.framerate = 0;

			pGarg->v.nextthink = gpGlobals->time + 0.1;

			return 1;
		}

		MonsterBleed(pGarg,BLOOD_COLOR_YELLOW);

		if(pGarg->v.health > (100000 - GARG_HEALTH))
		{
			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pGarg->v.origin );
				WRITE_BYTE( TE_BLOODSPRITE );
				WRITE_COORD( pGarg->v.origin.x);								// pos
				WRITE_COORD( pGarg->v.origin.y);
				WRITE_COORD( pGarg->v.origin.z);
				WRITE_SHORT( g_sModelIndexBloodSpray );				// initial sprite model
				WRITE_SHORT( g_sModelIndexBloodDrop );				// droplet sprite models
				WRITE_BYTE( BLOOD_COLOR_YELLOW );								// color index into host_basepal
				WRITE_BYTE( min( max( 3, 80 / 2 ), 16 ) );		// size
			MESSAGE_END();

			pGarg->v.fuser1 = pGarg->v.health;
		}
	}

	//If movement value is set then walk\run accordingly.
	if(pGarg->v.iuser2 > 0)
	{
		//When moving we cant get enemies
		pGarg->v.euser1 = NULL;

		Vector dist = (pGarg->v.vuser2 - pGarg->v.origin);

		if(dist.Length() > 64)
		{
			float flSpeed = 80.0;

			if(pGarg->v.iuser2 == 1)
			{
				pGarg->v.sequence = 4;
				flSpeed = 50.0;
			}

			if(pGarg->v.iuser2 == 2)
			{
				flSpeed = 150.0;
				pGarg->v.sequence = 4;
			}
			
			pGarg->v.velocity = dist.Normalize() * flSpeed;
			pGarg->v.speed = flSpeed;
			
			pGarg->v.vuser3 = pGarg->v.origin;

			if(pGarg->v.vuser4.x <= gpGlobals->time)
			{
				//Assume we are stuck
				if(pGarg->v.vuser3 == pGarg->v.origin)
					MonsterMove(pGarg,0,0,4);

				pGarg->v.vuser4.x = gpGlobals->time + 4;
			}				

		} else {
			//Stop when reached destination
			MonsterMove(pGarg,0,0,4);
		}
	}

	//Do we have an enemy? If not find one
	if((pGarg->v.euser1 == NULL) && (pGarg->v.iuser2 <= 0))
	{
		//No enemy so just look straight ahead
		pGarg->v.blending[0] = 90;

		pGarg->v.euser1 = MonsterFindEnemy(pGarg);
		
		//Do we still not have an enemy?
		if(pGarg->v.euser1 == NULL)
		{
			/*//Eye is dull
			if(pGarg->v.euser2 != NULL)
			{
				pGarg->v.euser2->v.effects = EF_NODRAW;
				UTIL_SetOrigin( VARS(pGarg->v.euser2), pGarg->v.euser2->v.origin );
			}*/

			//Can we say something?
			if(pGarg->v.fuser2 < gpGlobals->time)
			{
				//Talk here
				MonsterMove(pGarg,2,0,4);
				//EMIT_SOUND_DYN2( pGrunt, CHAN_VOICE, "hgrunt/clik (t25) stay(e80) alert people clik(p120)", 1, ATTN_NORM, 0, PITCH_NORM );
				pGarg->v.fuser2 = gpGlobals->time + RANDOM_LONG(2,10); //10;
			}
		}
	}

	//If we have an enemy...
	if(pGarg->v.euser1 != NULL)
	{
		/*//Eye glows
		if(pGarg->v.euser2 != NULL)
		{
			pGarg->v.euser2->v.effects = 0;
			pGarg->v.euser2->v.renderamt = UTIL_Approach(200,pGarg->v.euser2->v.renderamt,26);
			UTIL_SetOrigin( VARS(pGarg->v.euser2), pGarg->v.euser2->v.origin );
		}*/

		//Are they visible?
		if(MonsterCanSeeEnemy(pGarg) == false)
		{
			//Enemy no longer visible, forget them.
			pGarg->v.euser1 = NULL;
			pGarg->v.nextthink = gpGlobals->time + 0.1;
			return 1;
		}

		//Are they alive?
		if(pGarg->v.euser1->v.health <= 0)
		{
			//Enemy no longer alive, forget them.
			pGarg->v.euser1 = NULL;
			pGarg->v.nextthink = gpGlobals->time + 0.1;
			return 1;
		}

		//Enemy is alive and in view so...
		//Look at them
		Vector v_enemy = pGarg->v.euser1->v.origin - pGarg->v.origin;
		Vector grunt_angles = UTIL_VecToAngles( v_enemy );
		//pGrunt->v.angles.y = grunt_angles.y;
		pGarg->v.vuser1 = grunt_angles; //Store angle of last location of enemy
		
		pGarg->v.ideal_yaw = grunt_angles.y;
		MonsterFixIdealYaw(pGarg);
		
		//Make grunt look at the enemy
		int BlendVal = ((v_enemy.z * 255) / 360) + 70;
		if(BlendVal > 255) { BlendVal = 255; }
		if(BlendVal < 0) { BlendVal = 0; }
		pGarg->v.controller[1] = -BlendVal;//(v_enemy.z * 255) / 360;

		//Shoot if we can
		GargAttackEnemy(pGarg);
	}

	//Reset sequence if we should
	if((pGarg->v.fuser4 < gpGlobals->time) && (pGarg->v.iuser2 <= 0) && (pGarg->v.vuser4.y == 0.0))
	{
		//Change sequence back
		pGarg->v.sequence = 0;

		pGarg->v.fuser4 = gpGlobals->time + 2;
	}

	pGarg->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

edict_t *GetEyeGlow(edict_t *pGarg)
{
	CBaseEntity *pEyeGlow = NULL;
    while(pEyeGlow = UTIL_FindEntityByClassname(pEyeGlow, "env_sprite"))
    {
		if(pEyeGlow->pev->aiment == pGarg)
			return pEyeGlow->edict();
	}
	return NULL;
}

void GargAttackEnemy(edict_t *pGarg)
{
	if((pGarg->v.fuser3 < gpGlobals->time) && (pGarg->v.vuser4.y == 0.0))
	{
		//Choose an attack
		pGarg->v.vuser4.y = 1; //RANDOM_LONG(1,2);

		//Start sequence
		
		//Stomp
		if(pGarg->v.vuser4.y == 1)
			pGarg->v.sequence = 9;

		//Flame
		if(pGarg->v.vuser4.y > 1)
			pGarg->v.sequence = 7;

		//How long to stay in attack
		pGarg->v.fuser3 = gpGlobals->time + 3;
	}

	//Stomping
	if(pGarg->v.vuser4.y == 1)
	{
		//pGarg->v.framerate = 0;
		//Do the stomp
		TraceResult trace;

		UTIL_MakeVectors( pGarg->v.angles );
		Vector vecStart = pGarg->v.origin + Vector(0,0,60) + 35 * gpGlobals->v_forward;
		Vector vecAim = gpGlobals->v_forward;
		Vector vecEnd = (vecAim * 1024) + vecStart;

		UTIL_TraceLine( vecStart, vecEnd, ignore_monsters, pGarg, &trace );
		UTIL_ScreenShake( pGarg->v.origin, 12.0, 100.0, 2.0, 1000 );
		EMIT_SOUND_DYN2 ( pGarg, CHAN_WEAPON, "garg/gar_stomp1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM + RANDOM_LONG(-10,10) );

		UTIL_TraceLine( pGarg->v.origin, pGarg->v.origin - Vector(0,0,20), ignore_monsters, pGarg, &trace );
		if ( trace.flFraction < 1.0 )
			UTIL_DecalTrace( &trace, DECAL_GARGSTOMP1 );

		int m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_BEAMTORUS);
			WRITE_COORD(pGarg->v.origin.x);
			WRITE_COORD(pGarg->v.origin.y);
			WRITE_COORD(pGarg->v.origin.z + 32);
			WRITE_COORD(pGarg->v.origin.x);
			WRITE_COORD(pGarg->v.origin.y);
			WRITE_COORD(pGarg->v.origin.z + 32 + 200 * 2 / .2); // reach damage radius over .3 seconds
			WRITE_SHORT(m_spriteTexture );
			WRITE_BYTE( 0 ); // startframe
			WRITE_BYTE( 0 ); // framerate
			WRITE_BYTE( 2 ); // life
			WRITE_BYTE( 30 );  // width
			WRITE_BYTE( 0 );   // noise
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 210 );   // r, g, b
			WRITE_BYTE( 200 );   // r, g, b
			WRITE_BYTE( 210 );	// brightness
			WRITE_BYTE( 0 );		// speed
		MESSAGE_END();

		CBaseEntity *pScan = NULL;
		while ((pScan = UTIL_FindEntityInSphere(pScan, pGarg->v.origin, 512)) != NULL)
		{
			//Is it an entity we are bothered about? (Players)
			if (strcmp(STRING(pScan->pev->classname), "player") == 0)
			{
				if(pScan->pev->takedamage != DAMAGE_NO)
				{
					//Do damage to player (randomise damage a bit)
					int iDmgToTake = pGarg->v.dmg + (int)RANDOM_LONG(-4,4);

					if(pScan->pev->health <= iDmgToTake)
					{
						pScan->pev->health = 1;

						//If player health < pgrunt->dmg then kill them
						edict_t *pBomb = NULL;
						pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
						DispatchSpawn(pBomb);
						pBomb->v.origin.x = pScan->pev->origin.x;
						pBomb->v.origin.y = pScan->pev->origin.y;
						pBomb->v.origin.z = pScan->pev->origin.z;
						pBomb->v.velocity = Vector(0,0,0);
						pBomb->v.dmg = 15;
						pBomb->v.owner = pGarg;
						pBomb->v.effects |= EF_NODRAW;
						pBomb->v.nextthink = gpGlobals->time;
						pBomb->v.classname = MAKE_STRING("monster_gargantua");

						//CLIENT_COMMAND(pGrunt->v.euser1, UTIL_VarArgs("kill\n"))
					} else {
						TakeDamage(pScan->edict(),pGarg,iDmgToTake,0);
					}
				}
			}

			pGarg->v.vuser4.y = 4; //Set it to a null weapon so he doesnt loop this
		}
	}

	//Flaming
	if(pGarg->v.vuser4.y == 2)
	{
		if(pGarg->v.vuser4.z == 0)
		{
			//Create Flame
			pGarg->v.vuser4.z = 1;
		}

		if(pGarg->v.vuser4.z == 1)
		{
			//Hold Flame

		}
	}

	//Attack reset
	if(pGarg->v.vuser4.y > 0)
	{
		if(pGarg->v.fuser3 < gpGlobals->time)
		{
			pGarg->v.sequence = 0;
			pGarg->v.vuser4.y = 0.0;
			pGarg->v.vuser4.z = 0.0;
			pGarg->v.fuser3 = gpGlobals->time + 2;
		}
	}
}