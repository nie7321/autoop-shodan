/*****************************************************\
*	Auto-OP Shodan
*
*	File: grapple.cpp
*	Description: Grapple hook modules.
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
extern globalvars_t  *gpGlobals;
extern TFXFeatureEnabled_s Features;
extern int team_allies[4];

extern int m_Chain;			//Chain sprite
extern short g_sModelIndexBloodSpray;
extern short g_sModelIndexBloodDrop;

int GRAPPLE_DELAY = 5;		//Delay between grapple useages.
int GRAPPLE_SPEED = 400;	//Speed player climbs grapple

void MoveToDest( float OrigX, float OrigY, float OrigZ, float flSpeed, edict_t *pEntity )
{
	//Called every frame to move the player to the Origin of the grapple hook
	if(pEntity == NULL)
		return;

	if(flSpeed == 0)
		return;

	int Index = ENTINDEX(pEntity);

	//Are we supposed to redraw the line?
	if(PlayerInfo[Index].LineTime < gpGlobals->time)
	{
		BeamRedraw(pEntity);
		PlayerInfo[Index].LineTime = gpGlobals->time + 5;
	}

	PlayerInfo[Index].GrapStat = 1;

	Vector vecDest = 0;
	vecDest.x = OrigX;
	vecDest.y = OrigY;
	vecDest.z = OrigZ;

	Vector dist = (vecDest - pEntity->v.origin);

	if(dist.Length() > 50)
	{
		pEntity->v.velocity = dist.Normalize() * flSpeed;
		pEntity->v.speed = flSpeed;

	} else {

		pEntity->v.velocity = Vector ( 0, 0, 0 );
		pEntity->v.speed = 0;
		PlayerInfo[Index].GrapOrig = pEntity->v.origin;
		PlayerInfo[Index].GrapStat = 2;
	}

}

void PlayerMovedtoDest( edict_t *pEntity )
{
	if(pEntity->v.playerclass == CLASS_DEMOMAN)
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("-det5\n") );

	if(PlayerInfo[ENTINDEX(pEntity)].HangTime == 0.0)
	{
		PlayerInfo[ENTINDEX(pEntity)].HangTime = gpGlobals->time + GRAPPLE_DROPTIME;
	}

	if(PlayerInfo[ENTINDEX(pEntity)].HangTime <= gpGlobals->time && PlayerInfo[ENTINDEX(pEntity)].HangTime != 0.0)
	{
		PlayerDrops(pEntity,1);
		return;
	}

	float vdist = 0.0;
	vdist = (pEntity->v.origin - PlayerInfo[ENTINDEX(pEntity)].GrapOrig).Length();
	if(vdist > 10.0 && vdist < 200.0)
	{
		if(pEntity->v.health > 2)
		{
			pEntity->v.health -=2;
		}
	}

	if(vdist >= 200.0)
	{
		PlayerDrops(pEntity,0); //They must drop off, they've gone too far.
		return;
	}

	//Called every frame when the player is hanging on their grapple
	if (pEntity == NULL)
		return;

	int Index = ENTINDEX(pEntity);
	pEntity->v.gravity = 0;// Turn off gravity
	pEntity->v.origin = PlayerInfo[Index].GrapOrig;
	pEntity->v.movetype = MOVETYPE_NOCLIP;// Only way to make player 100% stable
	pEntity->v.velocity = Vector ( 0, 0, 0 );// Drop all velocity
	pEntity->v.avelocity = Vector ( 0, 0, 0 );// Drop all velocity
	pEntity->v.speed = 0;
	pEntity->v.flags |= FL_ONTRAIN;
}

void PlayerDrops( edict_t *pEntity, int TakeBack )
{
	//This is called when a player relases the Grapple hook
		if(pEntity == NULL)
			return;

		int Index = ENTINDEX(pEntity);

		PlayerInfo[Index].HangTime = 0.0;

		if(PlayerInfo[Index].GrapStat >= 1)
		{
			PlayerInfo[Index].GrapTime = gpGlobals->time + GRAPPLE_DELAY;
		}

		//Take grapple line off player
		MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin);
				WRITE_BYTE(TE_KILLBEAM);
				WRITE_SHORT(ENTINDEX(pEntity));
		MESSAGE_END();

		pEntity->v.gravity = 1;
		pEntity->v.movetype = MOVETYPE_WALK;
		pEntity->v.flags &= ~FL_ONTRAIN;
		if(PlayerInfo[Index].GrapStat == 2 && TakeBack == 1)
		{
			pEntity->v.origin = PlayerInfo[Index].GrapOrig;
		}
		PlayerInfo[Index].GrapStat = 0;
}

void TossGrapple( edict_t *pPlayer )
{
	if(pPlayer->v.playerclass == CLASS_DEMOMAN)
	{
		CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("-det5\n") );
		CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("-det10\n") );
		CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("-det20\n") );
		CLIENT_COMMAND ( pPlayer, UTIL_VarArgs("-det50\n") );
	}

	int Index = ENTINDEX(pPlayer);

	PlayerInfo[Index].HangTime = 0.0;

	//Shoot a traceline out of the player for 2000 units, find a grapple origin.
	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pPlayer->v.v_angle );

	v_src = pPlayer->v.origin + pPlayer->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 2000;

	UTIL_TraceLine( v_src, v_dest, ignore_monsters, pPlayer->v.pContainingEntity, &tr);

	if(tr.vecPlaneNormal.z == 1)
		return; //They are aiming at the floor

	if (tr.flFraction < 1.0)
	{
		//Ignoring anything except worldspawn, func_wall and func_door.
		if ( (strcmp("func_wall", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0))
		{
			if(Features.SkyHook == 1 || UTIL_PointContents(tr.vecEndPos) != CONTENTS_SKY)
			{
				PlayerInfo[Index].GrapOrig = tr.vecEndPos;
				PlayerInfo[Index].GrapStat = 1;
				MoveToDest( PlayerInfo[Index].GrapOrig.x,PlayerInfo[Index].GrapOrig.y,PlayerInfo[Index].GrapOrig.z,GRAPPLE_SPEED,pPlayer);
			}
		}
	}
}

void BeamRedraw( edict_t *pPlayer )
{
	//Called every 5 seconds to draw the grapple "chain" in the players team colour.
	Vector vecColor;

	if(pPlayer)
	{
		int Index = ENTINDEX(pPlayer);

		if(pPlayer->v.team == 1)
		{
			vecColor = Vector(0,0,255);
		}

		if(pPlayer->v.team == 2)
		{
			vecColor = Vector(255,0,0);
		}

		if(pPlayer->v.team == 3)
		{
			vecColor = Vector(255,255,0);
		}
		
		if(pPlayer->v.team == 4)
		{
			vecColor = Vector(0,255,0);
		}

		if(Features.TeamGrapple == 0)
			vecColor = Vector(255,255,255);

		MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pPlayer->v.origin ); //Draw 'chain'
		WRITE_BYTE( TE_BEAMENTPOINT ); 
		WRITE_SHORT( ENTINDEX(pPlayer) );
		WRITE_COORD( PlayerInfo[Index].GrapOrig.x );
		WRITE_COORD( PlayerInfo[Index].GrapOrig.y );
		WRITE_COORD( PlayerInfo[Index].GrapOrig.z );
		WRITE_SHORT( m_Chain ); 
		WRITE_BYTE( 1 ); //start frame
		WRITE_BYTE( 0 ); 
		WRITE_BYTE( 50 ); //5 Seconds
		WRITE_BYTE( 2 ); //width 
		WRITE_BYTE( 0 ); 
		WRITE_BYTE( vecColor.x ); //r
		WRITE_BYTE( vecColor.y ); //g
		WRITE_BYTE( vecColor.z ); //b
		WRITE_BYTE( 255);
		WRITE_BYTE( 10 );
		MESSAGE_END( );
	}
}

//------------------------------------ New Grapple Code
void ThrowGrapple(edict_t *pEntity)
{
	CBaseEntity *pGrappleEnt = NULL;
    while(pGrappleEnt = UTIL_FindEntityByClassname(pGrappleEnt, "weapon_grapplehook"))
        if(pGrappleEnt->pev->owner == pEntity)
			return; //Already grappling.

	if(pEntity->v.playerclass == CLASS_DEMOMAN)
	{
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("-det5\n") );
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("-det10\n") );
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("-det20\n") );
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("-det50\n") );
	}

	Vector v_src, v_dest;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs + gpGlobals->v_forward * 16;

	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/xbow_fire1.wav", 1, ATTN_NORM, 0, 100);
	edict_t *pGrapple = NULL;
    pGrapple = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    DispatchSpawn(pGrapple);
    pGrapple->v.origin = v_src;  
	pGrapple->v.angles = pEntity->v.v_angle;
	pGrapple->v.angles.z = 0;
    pGrapple->v.velocity = pEntity->v.velocity + gpGlobals->v_forward * Features.GrappleSpeed + Vector(0,0,-50);//Vector(0,0,0);
	pGrapple->v.takedamage = DAMAGE_YES;
	pGrapple->v.health = 100000;
	pGrapple->v.owner = pEntity;
	pGrapple->v.gravity = 0.1;
	pGrapple->v.fuser1 = 0.0;
	pGrapple->v.iuser1 = 0;
	pGrapple->v.iuser2 = 0;
	pGrapple->v.team = pEntity->v.team;
	pGrapple->v.movetype = MOVETYPE_FLY;
	pGrapple->v.fuser2 = pEntity->v.maxspeed;
	pGrapple->v.nextthink = gpGlobals->time;
	pGrapple->v.max_health = GRAPPLE_HEALTH;
	pGrapple->v.friction = 0.0;
	pGrapple->v.solid = SOLID_BBOX;
    SET_MODEL(pGrapple, "models/caltrop.mdl");
	UTIL_SetSize(VARS(pGrapple), Vector( -1, -1, -1), Vector(1, 1, 1));
    pGrapple->v.classname = MAKE_STRING("weapon_grapplehook");
}

void GrappleTouch(edict_t *pGrappleEnt, edict_t *pOtherEnt)
{
	if(pGrappleEnt->v.iuser2 == 1)
		return;

	if((strcmp("worldspawn", STRING(pOtherEnt->v.classname)) == 0) || (strcmp("func_wall", STRING(pOtherEnt->v.classname)) == 0))
	{
		//Stop on the wall
		pGrappleEnt->v.movetype = MOVETYPE_NONE;
		pGrappleEnt->v.velocity = Vector(0,0,0);

		//Make sparks and sound
		UTIL_Sparks(pGrappleEnt->v.origin);
		EMIT_SOUND_DYN2(pGrappleEnt, CHAN_VOICE, "weapons/xbow_hit1.wav", 1, ATTN_NORM, 0, 100);

		UTIL_SetSize(VARS(pGrappleEnt), Vector( -8, -8, -8), Vector(8, 8, 8));

		pGrappleEnt->v.iuser2 = 1; //Hooked

		//10 Seconds till we force player to drop
		pGrappleEnt->v.fuser1 = gpGlobals->time + GRAPPLE_DROPTIME;
	}
	else if(strcmp("player", STRING(pOtherEnt->v.classname)) == 0)
	{
		//Hurt player if enemy
		int Team1 = pOtherEnt->v.team;
		int Team2 = pGrappleEnt->v.team;
		if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
		{
			if(pOtherEnt->v.health > 5)
			{
				TakeDamage(pOtherEnt,pGrappleEnt,10,1);

				//Pain sound
				if (RANDOM_LONG( 0, 99 ) < 80)
					EMIT_SOUND_DYN2(pOtherEnt, CHAN_VOICE, "player/pain1.wav", 1, ATTN_NORM, 0, 100);
				else
					EMIT_SOUND_DYN2(pOtherEnt, CHAN_VOICE, "player/pain2.wav", 1, ATTN_NORM, 0, 100);

				//Blood spurt
				MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pOtherEnt->v.origin );
					WRITE_BYTE( TE_BLOODSPRITE );
					WRITE_COORD( pOtherEnt->v.origin.x);								// pos
					WRITE_COORD( pOtherEnt->v.origin.y);
					WRITE_COORD( pOtherEnt->v.origin.z);
					WRITE_SHORT( g_sModelIndexBloodSpray );				// initial sprite model
					WRITE_SHORT( g_sModelIndexBloodDrop );				// droplet sprite models
					WRITE_BYTE( 247 );								// color index into host_basepal
					WRITE_BYTE( min( max( 3, 80 / 10 ), 16 ) );		// size
				MESSAGE_END();

				//Flinch
				pOtherEnt->v.velocity.x = RANDOM_FLOAT(-25,25);
				pOtherEnt->v.velocity.y = RANDOM_FLOAT(-25,25);
			}
		}

		//Stops it sticking to people.
		KillGrapple(pGrappleEnt->v.owner);

	}
	else if((strcmp("weapon_grapplehook", STRING(pOtherEnt->v.classname)) == 0) || (strncmp("trigger_", STRING(pOtherEnt->v.classname),8) == 0) || (strcmp("func_nobuild", STRING(pOtherEnt->v.classname)) == 0) || (strcmp("func_nogrenades", STRING(pOtherEnt->v.classname)) == 0) || (strcmp("tfx_skin", STRING(pOtherEnt->v.classname)) == 0) || (strncmp("info_", STRING(pOtherEnt->v.classname),5) == 0))
	{
		//Ignore other hooks and triggers and some funcs
	}
	else
	{
		//Dont hook
		EMIT_SOUND_DYN2(pGrappleEnt, CHAN_VOICE, "weapons/xbow_hit2.wav", 1, ATTN_NORM, 0, 100);
		KillGrapple(pGrappleEnt->v.owner);
	}
}

bool GrappleHookThink(edict_t *pGrappleEnt)
{
	//We are too damaged to go on holding player
	if((pGrappleEnt->v.owner->v.health <= 0) || (pGrappleEnt->v.health <= 100000 - FENCEPOST_HEALTH))
	{
		KillGrapple(pGrappleEnt->v.owner);
		return 1;
	}

	//Player hung on too long
	if((pGrappleEnt->v.fuser1 > 0) && (pGrappleEnt->v.fuser1 < gpGlobals->time))
	{
		KillGrapple(pGrappleEnt->v.owner);
		return 1;
	}

	if(pGrappleEnt->v.velocity == Vector(0,0,0))
	{
		if(pGrappleEnt->v.iuser1 == 0)
		{
			//If the player entity is below the grapple, then pull them up
			if(pGrappleEnt->v.origin.z > pGrappleEnt->v.owner->v.origin.z)
			{
				Vector dist = (pGrappleEnt->v.origin - pGrappleEnt->v.owner->v.origin);

				if(dist.Length() > 50)
				{
					pGrappleEnt->v.owner->v.velocity = dist.Normalize() * pGrappleEnt->v.fuser2;
					pGrappleEnt->v.owner->v.speed = pGrappleEnt->v.fuser2;
				} else {
					pGrappleEnt->v.owner->v.velocity = Vector ( 0, 0, 0 );
					pGrappleEnt->v.owner->v.speed = 0;
					
					pGrappleEnt->v.iuser1 = 1; //Done pulling
				}
			}
		} else {
			//Set player up to be on the grapple
			pGrappleEnt->v.owner->v.gravity = 0;// Turn off gravity
			pGrappleEnt->v.owner->v.movetype = MOVETYPE_NOCLIP;// Only way to make player 100% stable
			pGrappleEnt->v.owner->v.velocity = Vector ( 0, 0, 0 );// Drop all velocity
			pGrappleEnt->v.owner->v.avelocity = Vector ( 0, 0, 0 );// Drop all velocity
			pGrappleEnt->v.owner->v.speed = 0;
			pGrappleEnt->v.owner->v.flags |= FL_ONTRAIN;
			pGrappleEnt->v.owner->v.flags |= FL_FROZEN;

		}
	}
	if(pGrappleEnt->v.fuser3 < gpGlobals->time)
	{
		//Redraw rope
		if(pGrappleEnt->v.owner->v.team == 1)
			pGrappleEnt->v.vuser1 = Vector(0,0,255);

		if(pGrappleEnt->v.owner->v.team == 2)
			pGrappleEnt->v.vuser1 = Vector(255,0,0);

		if(pGrappleEnt->v.owner->v.team == 3)
			pGrappleEnt->v.vuser1 = Vector(255,255,0);
		
		if(pGrappleEnt->v.owner->v.team == 4)
			pGrappleEnt->v.vuser1 = Vector(0,255,0);

		if(Features.TeamGrapple == 0)
			pGrappleEnt->v.vuser1 = Vector(150,49,0);

		MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pGrappleEnt->v.owner->v.origin ); //Draw 'chain'
		WRITE_BYTE( TE_BEAMENTS ); 
		WRITE_SHORT( ENTINDEX(pGrappleEnt->v.owner) );
		WRITE_SHORT( ENTINDEX(pGrappleEnt) );
		WRITE_SHORT( m_Chain ); 
		WRITE_BYTE( 1 ); //start frame
		WRITE_BYTE( 0 ); 
		WRITE_BYTE( 100 ); //10 Seconds
		WRITE_BYTE( 3 ); //width 
		WRITE_BYTE( 0 ); 
		WRITE_BYTE( pGrappleEnt->v.vuser1.x ); //r
		WRITE_BYTE( pGrappleEnt->v.vuser1.y ); //g
		WRITE_BYTE( pGrappleEnt->v.vuser1.z ); //b
		WRITE_BYTE( 255);
		WRITE_BYTE( 10 );
		MESSAGE_END( );

		pGrappleEnt->v.fuser3 = gpGlobals->time + 10;

	}
	pGrappleEnt->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void KillGrapple(edict_t *pPlayerEnt)
{
	//Take grapple line off player
	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pPlayerEnt->v.origin);
			WRITE_BYTE(TE_KILLBEAM);
			WRITE_SHORT(ENTINDEX(pPlayerEnt));
	MESSAGE_END();

	CBaseEntity *pGrappleEnt = NULL;
    while(pGrappleEnt = UTIL_FindEntityByClassname(pGrappleEnt, "weapon_grapplehook"))
    {
        if(pGrappleEnt->pev->owner == pPlayerEnt)
		{
			UTIL_Sparks(pGrappleEnt->pev->origin);
			pGrappleEnt->pev->flags = FL_KILLME;

			if(pGrappleEnt->pev->iuser1 == 1)
			{
				pPlayerEnt->v.gravity = 1;
				pPlayerEnt->v.movetype = MOVETYPE_WALK;
				pPlayerEnt->v.flags &= ~FL_ONTRAIN;
				pPlayerEnt->v.flags &= ~FL_FROZEN;
			}
		}
	}
}

