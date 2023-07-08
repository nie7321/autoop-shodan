/*****************************************************\
*	Auto-OP Shodan
*
*	File: energyblast.cpp
*	Description: Soldiers Energy Blast Gun.
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
#include "customweapons.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>
#include "player.h"

extern AOPlayerInfo_s PlayerInfo[];
extern int m_Chain;
extern int team_allies[4];

extern TFXFeatureEnabled_s Features;
extern FILE *fp;

void SwitchToEnergyGun(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SOLDIER || pEntity->v.health <= 0 || pEntity->v.weaponmodel == MAKE_STRING(ENERGY_PMODEL))
		return;

	CLIENT_COMMAND(pEntity, "tf_weapon_axe\n");
	PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = ENERGYGUN_ID;

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

void FireEnergy(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_SOLDIER || pEntity->v.health <= 0)
		return;

	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Fireenergy(): Player fired energy cannon.\n");
		fclose(fp);
	}

	if(pEntity->v.armorvalue < ENERGY_AMMO)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough armor to fire gun.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough armor to fire gun.\n"));
		PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[ENERGYGUN_ID] = gpGlobals->time + 2; //So the server doesnt spam the message too much.
		return;
	}

	pEntity->v.armorvalue -= ENERGY_AMMO;

	PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[ENERGYGUN_ID] = gpGlobals->time + ENERGY_DELAY;

	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "buttons/spark6.wav", 1, ATTN_NORM, 0, 100);
	//Shoot the blast
	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 9000;

	UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		//Create Energy Ball Entity
		edict_t *pEnergy = NULL;
    	pEnergy = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
    	DispatchSpawn(pEnergy);
    	pEnergy->v.origin = pEntity->v.origin + pEntity->v.view_ofs;
    	pEnergy->v.euser1 = pEntity;
    	pEnergy->v.angles = Vector(0,0,0);
    	pEnergy->v.velocity = Vector(0,0,0);
		pEnergy->v.takedamage = DAMAGE_NO;
		pEnergy->v.health = 1;
		pEnergy->v.team = pEntity->v.team;
		pEnergy->v.movetype = MOVETYPE_FLY;
		pEnergy->v.nextthink = gpGlobals->time;
		pEnergy->v.gravity = 0.0;
		pEnergy->v.fuser1 = gpGlobals->time + 6;
		pEnergy->v.vuser1 = tr.vecEndPos;
		pEnergy->v.friction = 0.0;
		pEnergy->v.solid = SOLID_NOT;
		pEnergy->v.rendermode = 3;
		pEnergy->v.renderfx = 14;
		pEnergy->v.renderamt = 150;
		pEnergy->v.rendercolor = Vector(0,200,0);

    	SET_MODEL(pEnergy, "sprites/flare1.spr");
    	UTIL_SetSize(VARS(pEnergy), Vector(0, 0, 0), Vector(0, 0, 0));
    	pEnergy->v.classname = MAKE_STRING("weapon_ao_energyblast");

		Vector vecDest = 0;
		vecDest.x = tr.vecEndPos.x;
		vecDest.y = tr.vecEndPos.y;
		vecDest.z = tr.vecEndPos.z;

		Vector dist = (vecDest - pEntity->v.origin);

		pEnergy->v.velocity = dist.Normalize() * ENERGY_BALL_SPEED;
		pEnergy->v.speed = ENERGY_BALL_SPEED;
	}
}

bool EnergyBlastThink(edict_t *pEntity)
{
	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "EnergyBlastThink(): Think function called.\n");
		fclose(fp);
	}

	//Check Owner is still able to own us
	if(pEntity->v.euser1->v.team != pEntity->v.team || pEntity->v.euser1->v.playerclass != CLASS_SOLDIER || pEntity->v.euser1->v.health <= 0 || pEntity->v.fuser1 < gpGlobals->time)
	{
		KillEnergyBlast(pEntity,0);
	}

	if ((pEntity->v.origin - pEntity->v.vuser1).Length() < 50.0)
	{
		KillEnergyBlast(pEntity,1);
	}

	if(gpGlobals->time >= pEntity->v.fuser2)
	{
		CBaseEntity *pScan = NULL;

		while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 600)) != NULL)
		{
			//Is it an entity we are bothered about? (Player)
			if (strcmp(STRING(pScan->pev->classname), "player") == 0 && UTIL_SpectCheck(pScan->edict()) == 0)
			{
				int Team1 = pScan->pev->team;
				int Team2 = pEntity->v.team;
				if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
				{
					if(EnemyIsVisible(pEntity,pScan->edict()))
					{
						EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/electro5.wav", 1, ATTN_NORM, 0, 100);
						MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pEntity->v.origin);
							WRITE_BYTE(TE_BEAMENTS);
							WRITE_SHORT(ENTINDEX(pEntity));
							WRITE_SHORT(ENTINDEX(pScan->edict()));
							WRITE_SHORT(m_Chain);
							WRITE_BYTE(1);// byte (starting frame) 
							WRITE_BYTE(0);// byte (frame rate in 0.1's) 
							WRITE_BYTE(10);// byte (life in 0.1's) 
							WRITE_BYTE(7);// byte (line width in 0.1's) 
							WRITE_BYTE(25);// byte (noise amplitude in 0.01's) 
							WRITE_BYTE(0);// byte,byte,byte (color)
							WRITE_BYTE(255);
							WRITE_BYTE(0);
							WRITE_BYTE(200);// byte (brightness)
							WRITE_BYTE(10);// byte (scroll speed in 0.1's)
						MESSAGE_END();
						if(pScan->pev->takedamage != DAMAGE_NO)
						{
							if(pScan->pev->health > 30)
							{
								TakeDamage(pScan->edict(),NULL,30,256);
								//pScan->pev->health -= 25;
							} else {
								edict_t *pBomb;
								pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
								DispatchSpawn(pBomb);
								pBomb->v.origin = pScan->pev->origin;
								pBomb->v.dmg = 150;
								pBomb->v.team = pEntity->v.team;
								pBomb->v.owner = pEntity->v.euser1;
								pBomb->v.classname = MAKE_STRING("ao_energyblastkill");
								pBomb->v.effects |= EF_NODRAW;
								pBomb->v.nextthink = gpGlobals->time;
							}
						}
					}
				}
			}
		}
		pEntity->v.fuser2 = gpGlobals->time + 1;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void KillEnergyBlast(edict_t *pEntity, int Explode)
{
	if(Features.Debug >= 3)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "KillEnergyBlast(): Energyball was killed.\n");
		fclose(fp);
	}

	pEntity->v.flags = FL_KILLME;

	int m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_BEAMTORUS);
		WRITE_COORD(pEntity->v.origin.x);
		WRITE_COORD(pEntity->v.origin.y);
		WRITE_COORD(pEntity->v.origin.z + 32);
		WRITE_COORD(pEntity->v.origin.x);
		WRITE_COORD(pEntity->v.origin.y);
		WRITE_COORD(pEntity->v.origin.z + 32 + 300 * 2 / .2); // reach damage radius over .3 seconds
		WRITE_SHORT(m_spriteTexture );
		WRITE_BYTE( 0 ); // startframe
		WRITE_BYTE( 0 ); // framerate
		WRITE_BYTE( 2 ); // life
		WRITE_BYTE( 12 );  // width
		WRITE_BYTE( 0 );   // noise
		WRITE_BYTE( 0 );   // r, g, b
		WRITE_BYTE( 200 );   // r, g, b
		WRITE_BYTE( 0 );   // r, g, b
		WRITE_BYTE( 200 );	// brightness
		WRITE_BYTE( 0 );		// speed
	MESSAGE_END();

	if(Explode == 1)
	{
		edict_t *pBomb;
		pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
		DispatchSpawn(pBomb);
		pBomb->v.origin = pEntity->v.origin;
		pBomb->v.dmg = 300;
		pBomb->v.team = pEntity->v.team;
		pBomb->v.owner = pEntity->v.euser1;
		pBomb->v.classname = MAKE_STRING("ao_energyblastkill");
		pBomb->v.effects |= EF_NODRAW;
		pBomb->v.nextthink = gpGlobals->time;
	}
}