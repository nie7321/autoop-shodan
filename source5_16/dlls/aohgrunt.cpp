/*****************************************************\
*	Auto-OP Shodan
*
*	File: aohgrunt.cpp
*	Description: Human Grunt Code.
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
#include "aohgrunt.h"
#include "aomonsters.h"
#include "aohgrunt.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern DLL_FUNCTIONS other_gFunctionTable;
extern TFXFeatureEnabled_s Features;

extern short g_sModelIndexBloodSpray;
extern short g_sModelIndexBloodDrop;
extern int m_Chain;			//Chain sprite

void CreateGruntCoords(bool CanWalk, int x, int y, int z)
{
	edict_t *pGrunt = NULL;
	pGrunt = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	SET_MODEL(pGrunt, "models/hgrunt.mdl");
	pGrunt->v.effects		= 0;
	pGrunt->v.euser1		= NULL; //Store current enemy here
	pGrunt->v.fuser2		= gpGlobals->time; //Talk wait time
	pGrunt->v.fuser3		= gpGlobals->time; //Attack wait time
	pGrunt->v.fuser4		= gpGlobals->time; //Sequence reset time
	pGrunt->v.iuser4		= 0; //1 if dead.
	pGrunt->v.iuser3		= (int)RANDOM_LONG(33,38); //Set death seq
	
	if(CanWalk)
		pGrunt->v.iuser2		= 0; //1 - Walk, 2 - Run
	else
		pGrunt->v.iuser2		= -1;//-1 - Never move

	pGrunt->v.origin.x = x;
	pGrunt->v.origin.y = y;
	pGrunt->v.origin.z = z;
	//pGrunt->v.gravity		= 1;
	//pGrunt->v.friction		= 1;

	//Set animation
	pGrunt->v.framerate = 1;
	pGrunt->v.sequence = 13;

	if (RANDOM_LONG( 0, 99 ) < 80)
		pGrunt->v.skin = 0;	// light skin
	else
		pGrunt->v.skin = 1;	// dark skin

	DispatchSpawn(pGrunt);
	
	pGrunt->v.health		= 100000;
	pGrunt->v.fuser1		= pGrunt->v.health; //For health loss comparison

	UTIL_SetOrigin(VARS(pGrunt),pGrunt->v.origin);
	pGrunt->v.solid			= SOLID_SLIDEBOX;
	pGrunt->v.movetype		= MOVETYPE_TOSS;
	UTIL_SetSize(VARS(pGrunt), VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);
	pGrunt->v.dmg = 15; //Amount of damage to do every attack
	pGrunt->v.classname = MAKE_STRING("ao_hgrunt");
	pGrunt->v.takedamage = DAMAGE_YES;
	pGrunt->v.maxspeed = 2000;

	pGrunt->v.nextthink = gpGlobals->time + 0.1;
}

void CreateGrunt(edict_t *pPlayer, bool CanWalk)
{
	edict_t *pGrunt = NULL;
	pGrunt = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	SET_MODEL(pGrunt, "models/hgrunt.mdl");
	pGrunt->v.effects		= 0;
	pGrunt->v.euser1		= NULL; //Store current enemy here
	pGrunt->v.fuser2		= gpGlobals->time; //Talk wait time
	pGrunt->v.fuser3		= gpGlobals->time; //Attack wait time
	pGrunt->v.fuser4		= gpGlobals->time; //Sequence reset time
	pGrunt->v.iuser4		= 0; //1 if dead.
	pGrunt->v.iuser3		= (int)RANDOM_LONG(33,38); //Set death seq
	
	if(CanWalk)
		pGrunt->v.iuser2		= 0; //1 - Walk, 2 - Run
	else
		pGrunt->v.iuser2		= -1;//-1 - Never move

	pGrunt->v.origin		= pPlayer->v.origin;
	//pGrunt->v.gravity		= 1;
	//pGrunt->v.friction		= 1;

	//Set animation
	pGrunt->v.framerate = 1;
	pGrunt->v.sequence = 13;

	if (RANDOM_LONG( 0, 99 ) < 80)
		pGrunt->v.skin = 0;	// light skin
	else
		pGrunt->v.skin = 1;	// dark skin

	DispatchSpawn(pGrunt);
	
	pGrunt->v.health		= 100000;
	pGrunt->v.fuser1		= pGrunt->v.health; //For health loss comparison

	UTIL_SetOrigin(VARS(pGrunt),pGrunt->v.origin);
	pGrunt->v.solid			= SOLID_SLIDEBOX;
	pGrunt->v.movetype		= MOVETYPE_TOSS;
	UTIL_SetSize(VARS(pGrunt), VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);
	pGrunt->v.dmg = 15; //Amount of damage to do every attack
	pGrunt->v.classname = MAKE_STRING("ao_hgrunt");
	pGrunt->v.takedamage = DAMAGE_YES;
	pGrunt->v.maxspeed = 2000;

	pGrunt->v.nextthink = gpGlobals->time + 0.1;
}

bool GruntThink(edict_t *pGrunt)
{
	pGrunt->v.controller[0] = 90;// Make head look straight ahead

	if(pGrunt->v.waterlevel != 0)
		pGrunt->v.health = (100000 - (HGRUNT_HEALTH + 1)); //Kill grunts that fall in the water

	//If Grunt idealyaw and angle.y dont match, gradually
	//change the angle.y to match (help stop superhuman turning)
	if(pGrunt->v.iuser4 == 0) //If not dead...
		MonsterChangeYaw(pGrunt,250);

	//If grunt is dead, is it time to erase his entity?
	if(pGrunt->v.iuser4 == 1 && pGrunt->v.fuser4 < gpGlobals->time)
	{
		pGrunt->v.flags = FL_KILLME;
		return 1;
	}

	//Took damage?
	if(pGrunt->v.health < pGrunt->v.fuser1)
	{
		if(pGrunt->v.health <= (100000 - HGRUNT_HEALTH))
		{
			//If health is -50
			if(pGrunt->v.health < (100000 - (HGRUNT_HEALTH + 50)))
			{
				//Gib
				FakeGib(pGrunt);

				//Set health to 0
				pGrunt->v.health = (100000 - (HGRUNT_HEALTH + 1));
			}

			if(pGrunt->v.iuser4 == 0)
			{
				pGrunt->v.fuser4 = gpGlobals->time + 8;//Time to destroy entity
				EMIT_SOUND( pGrunt, CHAN_VOICE, "hgrunt/gr_die1.wav", 1, ATTN_NORM );
			}

			pGrunt->v.iuser4 = 1; //Mark as dead

			pGrunt->v.takedamage = DAMAGE_NO; //No damage whilst dead
			pGrunt->v.solid = SOLID_NOT; //Corpse not solid

			MonsterMove(pGrunt,0,12,2); //Stop moving if we are
						
			pGrunt->v.sequence = pGrunt->v.iuser3; //dead sequence
			pGrunt->v.framerate = 0.5;
			if(pGrunt->v.frame >= 1 && pGrunt->v.sequence == pGrunt->v.iuser3) //On final frame
				pGrunt->v.framerate = 0;

			pGrunt->v.nextthink = gpGlobals->time + 0.1;

			return 1;
		}

		MonsterBleed(pGrunt,BLOOD_COLOR_RED);

		//Maybe scream if health > 0
		if(pGrunt->v.health > (100000 - HGRUNT_HEALTH))
		{
			switch ( RANDOM_LONG(0,8) )
			{
			case 0:	
				EMIT_SOUND( pGrunt, CHAN_VOICE, "hgrunt/gr_pain3.wav", 1, ATTN_NORM );	
				break;
			case 1:
				EMIT_SOUND( pGrunt, CHAN_VOICE, "hgrunt/gr_pain4.wav", 1, ATTN_NORM );	
				break;
			case 2:
				EMIT_SOUND( pGrunt, CHAN_VOICE, "hgrunt/gr_pain5.wav", 1, ATTN_NORM );	
				break;
			case 3:
				EMIT_SOUND( pGrunt, CHAN_VOICE, "hgrunt/gr_pain1.wav", 1, ATTN_NORM );	
				break;
			case 4:
				EMIT_SOUND( pGrunt, CHAN_VOICE, "hgrunt/gr_pain2.wav", 1, ATTN_NORM );	
				break;
			}

			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pGrunt->v.origin );
				WRITE_BYTE( TE_BLOODSPRITE );
				WRITE_COORD( pGrunt->v.origin.x);								// pos
				WRITE_COORD( pGrunt->v.origin.y);
				WRITE_COORD( pGrunt->v.origin.z);
				WRITE_SHORT( g_sModelIndexBloodSpray );				// initial sprite model
				WRITE_SHORT( g_sModelIndexBloodDrop );				// droplet sprite models
				WRITE_BYTE( 247 );								// color index into host_basepal
				WRITE_BYTE( min( max( 3, 80 / 10 ), 16 ) );		// size
			MESSAGE_END();

			pGrunt->v.fuser1 = pGrunt->v.health;
		}
	}

	//If movement value is set then walk\run accordingly.
	if(pGrunt->v.iuser2 > 0)
	{
		//When moving we cant get enemies
		pGrunt->v.euser1 = NULL;

		Vector dist = (pGrunt->v.vuser2 - pGrunt->v.origin);

		if(dist.Length() > 40)
		{
			float flSpeed = 80.0;

			if(pGrunt->v.iuser2 == 1)
			{
				pGrunt->v.sequence = 0;
				flSpeed = 50.0;
			}

			if(pGrunt->v.iuser2 == 2)
			{
				flSpeed = 225.0;
				pGrunt->v.sequence = 1;
			}
			
			pGrunt->v.velocity = dist.Normalize() * flSpeed;
			pGrunt->v.speed = flSpeed;
			
			pGrunt->v.vuser3 = pGrunt->v.origin;

			if(pGrunt->v.vuser4.x <= gpGlobals->time)
			{
				//Assume we are stuck
				if(pGrunt->v.vuser3 == pGrunt->v.origin)
					MonsterMove(pGrunt,0,12,2);

				pGrunt->v.vuser4.x = gpGlobals->time + 4;
			}				

		} else {
			//Stop when reached destination
			MonsterMove(pGrunt,0,12,2);
		}
	}

	//Do we have an enemy? If not find one
	if((pGrunt->v.euser1 == NULL) && (pGrunt->v.iuser2 <= 0))
	{
		//No enemy so just look straight ahead
		pGrunt->v.blending[0] = 90;

		pGrunt->v.euser1 = MonsterFindEnemy(pGrunt);
		
		//Do we still not have an enemy?
		if(pGrunt->v.euser1 == NULL)
		{
			//Can we say something?
			if(pGrunt->v.fuser2 < gpGlobals->time)
			{
				//Talk here
				MonsterMove(pGrunt,2,12,2);
				//EMIT_SOUND_DYN2( pGrunt, CHAN_VOICE, "hgrunt/clik (t25) stay(e80) alert people clik(p120)", 1, ATTN_NORM, 0, PITCH_NORM );
				pGrunt->v.fuser2 = gpGlobals->time + RANDOM_LONG(2,10); //10;
			}
		}
	}

	//If we have an enemy...
	if(pGrunt->v.euser1 != NULL)
	{	
		//Are they visible?
		if(MonsterCanSeeEnemy(pGrunt) == false)
		{
			//Enemy no longer visible, forget them.
			pGrunt->v.euser1 = NULL;
			pGrunt->v.nextthink = gpGlobals->time + 0.1;
			return 1;
		}

		//Are they alive?
		if(pGrunt->v.euser1->v.health <= 0)
		{
			//Enemy no longer alive, forget them.
			pGrunt->v.euser1 = NULL;
			pGrunt->v.nextthink = gpGlobals->time + 0.1;
			return 1;
		}

		//Enemy is alive and in view so...
		//Look at them
		Vector v_enemy = pGrunt->v.euser1->v.origin - pGrunt->v.origin;
		Vector grunt_angles = UTIL_VecToAngles( v_enemy );
		//pGrunt->v.angles.y = grunt_angles.y;
		pGrunt->v.vuser1 = grunt_angles; //Store angle of last location of enemy
		
		pGrunt->v.ideal_yaw = grunt_angles.y;
		MonsterFixIdealYaw(pGrunt);
		
		//Make grunt look at the enemy
		int BlendVal = ((v_enemy.z * 255) / 360) + 70;
		if(BlendVal > 255) { BlendVal = 255; }
		if(BlendVal < 0) { BlendVal = 0; }
		pGrunt->v.blending[0] = BlendVal;//(v_enemy.z * 255) / 360;

		//Shoot if we can
		//if(pGrunt->v.angles.y == pGrunt->v.ideal_yaw)
		GruntAttackEnemy(pGrunt);
	}

	//Reset sequence if we should
	if((pGrunt->v.fuser4 < gpGlobals->time) && (pGrunt->v.iuser2 <= 0))
	{
		//Change sequence back
		pGrunt->v.sequence = 13;

		pGrunt->v.fuser4 = gpGlobals->time + 2;
	}

	pGrunt->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void GruntAttackEnemy(edict_t *pGrunt)
{
	if(pGrunt->v.fuser3 < gpGlobals->time)
	{
		if(MonsterCanSeeEnemy(pGrunt) == true)
		{
			pGrunt->v.sequence = 18; //Play shooting anim

			//Play shooting sound
			if ( RANDOM_LONG(0,1) )
			{
				EMIT_SOUND_DYN2(pGrunt, CHAN_WEAPON, "hgrunt/gr_mgun1.wav", 1, ATTN_NORM, 0, PITCH_NORM);
			}
			else
			{
				EMIT_SOUND_DYN2(pGrunt, CHAN_WEAPON, "hgrunt/gr_mgun2.wav", 1, ATTN_NORM, 0, PITCH_NORM);
			}

			if(pGrunt->v.euser1->v.takedamage != DAMAGE_NO)
			{
				//Do damage to player (randomise damage a bit)
				int iDmgToTake = pGrunt->v.dmg + (int)RANDOM_LONG(-2,2);

				if(pGrunt->v.euser1->v.health <= iDmgToTake)
				{
					pGrunt->v.euser1->v.health = 1;

					//If player health < pgrunt->dmg then kill them
					edict_t *pBomb = NULL;
					pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
					DispatchSpawn(pBomb);
					pBomb->v.origin.x = pGrunt->v.euser1->v.origin.x;
					pBomb->v.origin.y = pGrunt->v.euser1->v.origin.y;
					pBomb->v.origin.z = pGrunt->v.euser1->v.origin.z;
					pBomb->v.velocity = Vector(0,0,0);
					pBomb->v.dmg = 15;
					pBomb->v.owner = pGrunt;
					pBomb->v.effects |= EF_NODRAW;
					pBomb->v.nextthink = gpGlobals->time;
					pBomb->v.classname = MAKE_STRING("monster_hgrunt");

					//CLIENT_COMMAND(pGrunt->v.euser1, UTIL_VarArgs("kill\n"))
						;
				} else {
					TakeDamage(pGrunt->v.euser1,pGrunt,iDmgToTake,0);
				}
			}

			pGrunt->v.fuser3 = gpGlobals->time + 0.5;
		}
	}
}
