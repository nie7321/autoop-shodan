/*****************************************************\
*	AutoOP Shodan
*
*	File: biggun.cpp
*	Description: Manual sentry gun Engineeer builds
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

void BuildGun(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_ENGINEER || pEntity->v.health <= 0)
		return;

	if(PlayerInfo[ENTINDEX(pEntity)].HasBigGun == 1)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cant build another gun.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cant build another gun.\n"));
		return;
	}

	if(GetpvPrivateInt(pEntity,208) /*pEntity->v.armorvalue*/ < BIGGUN_REQUIRED)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough metal to build gun.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough metal to build gun.\n"));
		return;
	}

	if(PlayerInfo[ENTINDEX(pEntity)].LastBuilt > gpGlobals->time)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Please wait before building\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Please wait before building\n"));
		return;
	}

	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Is it an entity we are bothered about? (nobuild)
		if (strcmp(STRING(pScan->pev->classname), "func_nobuild") == 0)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot build here.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot build here.\n"));
			return;
		}
	}

    TraceResult tr;
    UTIL_MakeVectors(pEntity->v.v_angle);
    UTIL_TraceLine(pEntity->v.origin, pEntity->v.origin + Vector(gpGlobals->v_forward.x*64, gpGlobals->v_forward.y*64, 0), dont_ignore_monsters, pEntity, &tr);

    if(tr.flFraction==1.0 && UTIL_CheckBox(tr.vecEndPos, Vector(32, 32, 64)))
    {
		PatchpvPrivateInt(pEntity,208,(GetpvPrivateInt(pEntity,208) - BIGGUN_REQUIRED));
	    //pEntity->v.armorvalue -= BIGGUN_REQUIRED;
		PlayerInfo[ENTINDEX(pEntity)].HasBigGun = 1;
	 	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/building.wav", 1, ATTN_NORM, 0, 130);
	 	edict_t *pBigGun;
		pBigGun = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
		DispatchSpawn(pBigGun);
		pBigGun->v.origin = tr.vecEndPos;
		pBigGun->v.angles = pEntity->v.v_angle;
		pBigGun->v.angles.x = 0;
        pBigGun->v.angles.z = 0;
        pBigGun->v.angles.y = 0;
        if(pBigGun->v.angles.y >= 360) pBigGun->v.angles.z -= 360;
		UTIL_MakeVectors(pEntity->v.v_angle);
		pBigGun->v.velocity = Vector(0, 0, 0);
		pBigGun->v.euser1 = pEntity; //Owner
		pBigGun->v.euser2 = NULL; //No user yet
		pBigGun->v.fuser1 = gpGlobals->time; //Shooting delay
		pBigGun->v.vuser1 = pBigGun->v.angles; //Original angles
		pBigGun->v.vuser2 = Vector(0,0,0);
		pBigGun->v.takedamage = DAMAGE_YES;
		pBigGun->v.health = 100000;
		pBigGun->v.max_health = BIGGUN_HEALTH;
		pBigGun->v.movetype = MOVETYPE_TOSS;
		pBigGun->v.solid = SOLID_BBOX;
		pBigGun->v.team = pEntity->v.team;
		SET_MODEL(pBigGun, "models/sentry.mdl");
		UTIL_SetSize(VARS(pBigGun), Vector(-16, -16, 0), Vector(16, 16, 64));
		UTIL_SetOrigin(VARS(pBigGun), pBigGun->v.origin);
		pBigGun->v.classname = MAKE_STRING("weapon_ao_guncontrol");
		pBigGun->v.nextthink = gpGlobals->time + 0.1;
		UTIL_LogPrintf( "[AUTOOP] %s built %s\n", STRING(pEntity->v.netname), STRING(pBigGun->v.classname));
		PlayerInfo[ENTINDEX(pEntity)].LastBuilt = gpGlobals->time + 0.5;
	}
}

bool BigGunThink(edict_t *pEntity)
{
	//First check we can still live
	if(pEntity->v.euser1->v.playerclass != CLASS_ENGINEER || (pEntity->v.euser1->v.team != pEntity->v.team) || (pEntity->v.health <= 100000 - BIGGUN_HEALTH))
	{
		DestroyGun(pEntity);
		return 1;
	}

	//are we stood on a moving platform? of so destroy ourselves.
	TraceResult tr;
	UTIL_TraceLine(pEntity->v.origin, pEntity->v.origin + Vector(0, 0, -200), ignore_monsters, pEntity, &tr);
	if ((strcmp("func_door", STRING(tr.pHit->v.classname)) == 0) || (strcmp("func_platform", STRING(tr.pHit->v.classname)) == 0))
	{
		DestroyGun(pEntity);
		return 1;
	}
	
	if(pEntity->v.euser2 != NULL) //We have a controller?
	{
		pEntity->v.owner = pEntity->v.euser2; //so he cant hurt us.
		pEntity->v.vuser2.y = pEntity->v.euser2->v.v_angle.y;
		pEntity->v.vuser2.x = pEntity->v.euser2->v.v_angle.x;
		pEntity->v.controller[0] = (pEntity->v.vuser2.y * 256) / 360;
		pEntity->v.controller[1] = 100; //-pEntity->v.vuser2.x;
		pEntity->v.sequence = 2;
		pEntity->v.framerate = 1.0;
		//CLIENT_PRINTF( pEntity->v.euser2, print_center, UTIL_VarArgs( "X:%f Y:%f Z:%f\n",pEntity->v.euser2->v.v_angle.x,pEntity->v.euser2->v.v_angle.y,pEntity->v.euser2->v.v_angle.z));
		//pEntity->v.controller[1] = pEntity->v.angles.x;

		//Rotate the gun here somehow

		if(pEntity->v.euser2->v.button & IN_ATTACK)
		{
			if(pEntity->v.fuser1 <= gpGlobals->time)
			{
				pEntity->v.fuser1 = gpGlobals->time + 2.5; // 1.5 sec Firing delay

				//Shoot here
				SentryShoot(pEntity);
			}
		}

		//Check if owner has wandered off or become unusable in any other way.
		float vdist;
		vdist = (pEntity->v.origin - pEntity->v.euser2->v.origin).Length();
		
		if ((pEntity->v.origin - pEntity->v.euser2->v.origin).Length() > 100.0 || pEntity->v.euser2->v.health <= 0 || pEntity->v.euser2->v.viewmodel != 0)
		{
			CBasePlayer *pPlayer = (CBasePlayer *)pEntity->v.euser2;
			pPlayer->m_iHideHUD &= ~HIDEHUD_WEAPONS;
			CLIENT_PRINTF( pEntity->v.euser2, print_center, UTIL_VarArgs( "You lost control of the gun.\n"));
			pEntity->v.owner = NULL;
			PlayerInfo[ENTINDEX(pEntity->v.euser2)].IsUsingTurret = 0;
			pEntity->v.euser2 = NULL; //He is not our owner anymore!
		}
	} else {
		pEntity->v.sequence = 0;
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 1;
}

void UseGun(edict_t *pEntity)
{
	if(UTIL_SpectCheck(pEntity) == 1)
		return;

	int ValidModel = 0;

	if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_crowbar.mdl"))
		ValidModel = 1;

	if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_knife.mdl"))
		ValidModel = 1;

	if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_medkit.mdl"))
		ValidModel = 1;

	if(FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_spanner.mdl"))
		ValidModel = 1;

	if(FStrEq(STRING(pEntity->v.viewmodel),"models/autoop/v_tfc_dualbar.mdl"))
		ValidModel = 1;

	if(pEntity->v.viewmodel == 0)
		ValidModel = 1;

	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		if(PlayerInfo[ENTINDEX(pEntity)].IsUsingTurret == 1)
		{
			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot use more than one gun at a time.\n"));
			CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "You cannot use more than one gun at a time.\n"));
			return;
		}

		if (strcmp(STRING(pScan->pev->classname), "weapon_ao_guncontrol") == 0)
		{
			if(pScan->pev->euser2 == NULL && pScan->pev->team == pEntity->v.team)
			{
				if(ValidModel == 0)
				{
					CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You must select your melee weapon\nbefore using the manual sentry.\n"));
					CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "You must select your melee weapon before using the manual sentry.\n"));
					return;
				}

				pScan->pev->euser2 = pEntity;
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You took control of a manual sentry.\n"));
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You took control of a manual sentry.\n"));

				UTIL_LogPrintf( "[AUTOOP] %s took control of a manual sentry\n", STRING(pEntity->v.netname));

				//Hide the players weapon
				CBasePlayer *pPlayer = (CBasePlayer *)pEntity;
				PlayerInfo[ENTINDEX(pEntity)].IsUsingTurret = 1;
				pEntity->v.viewmodel = 0;
				pEntity->v.weaponmodel = 0;
				pPlayer->m_iHideHUD |= HIDEHUD_WEAPONS;
				return;

			} else {
				CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "You cannot use this gun.\n"));
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot use this gun.\n"));
			}
		}
	}
}

void DestroyPlayersGun(edict_t *pEntity)
{
	CBaseEntity *pFencePost = NULL;
    while(pFencePost = UTIL_FindEntityByClassname(pFencePost, "weapon_ao_guncontrol"))
    {
        if(pFencePost->pev->euser1 == pEntity)
        {
			if(pFencePost->pev->euser2 != NULL)
			{
				CBasePlayer *pPlayer = (CBasePlayer *)pFencePost->pev->euser2;
				pPlayer->m_iHideHUD &= ~HIDEHUD_WEAPONS;
				CLIENT_PRINTF( pFencePost->pev->euser2, print_center, UTIL_VarArgs( "You lost control of the gun.\n"));
				PlayerInfo[ENTINDEX(pFencePost->pev->euser2)].IsUsingTurret = 0;
				pFencePost->pev->euser2 = NULL; //He is not our owner anymore!
			}

			CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your manual sentry was destroyed.\n"));
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your manual sentry was destroyed.\n"));
			PlayerInfo[ENTINDEX(pEntity)].HasBigGun = 0;
            pFencePost->pev->flags |= FL_KILLME;

			edict_t *pBomb;
			pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
			DispatchSpawn(pBomb);
			pBomb->v.origin = pFencePost->pev->origin;
			pBomb->v.velocity = Vector(0,0,0);
			pBomb->v.dmg = 5;
			pBomb->v.owner = pEntity;
			pBomb->v.effects |= EF_NODRAW;
			pBomb->v.nextthink = gpGlobals->time;
			pBomb->v.classname = MAKE_STRING("weapon_ao_gundetonate");
		}
	}
	return;
}

void DestroyGun(edict_t *pEntity)
{
	if(pEntity->v.euser2 != NULL)
	{
		CBasePlayer *pPlayer = (CBasePlayer *)pEntity->v.euser2;
		pPlayer->m_iHideHUD &= ~HIDEHUD_WEAPONS;
		CLIENT_PRINTF( pEntity->v.euser2, print_center, UTIL_VarArgs( "You lost control of the gun.\n"));
		PlayerInfo[ENTINDEX(pEntity->v.euser2)].IsUsingTurret = 0;
		pEntity->v.euser2 = NULL; //He is not our owner anymore!
	}

	CLIENT_PRINTF( pEntity->v.euser1, print_center, UTIL_VarArgs( "Your manual sentry was destroyed.\n"));
	CLIENT_PRINTF( pEntity->v.euser1, print_console,UTIL_VarArgs( "Your manual sentry was destroyed.\n"));
	PlayerInfo[ENTINDEX(pEntity->v.euser1)].HasBigGun = 0;
    pEntity->v.flags |= FL_KILLME;

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
	pBomb->v.classname = MAKE_STRING("weapon_ao_gundetonate");
}

void SentryShoot(edict_t *pEntity)
{
	Vector v_src, v_dest;
	TraceResult tr;

	//Stop it shooting down
	if(pEntity->v.vuser2.x > -1)
		pEntity->v.vuser2.x = -1;
	
	UTIL_MakeVectors( pEntity->v.vuser2 );

	v_src = pEntity->v.origin;
	v_dest = v_src + gpGlobals->v_forward * 9000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		int m_spriteTexture = PRECACHE_MODEL( "sprites/lgtning.spr" );
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_BEAMPOINTS );
		WRITE_COORD(pEntity->v.origin.x);
		WRITE_COORD(pEntity->v.origin.y);
		WRITE_COORD(pEntity->v.origin.z + 50);
		WRITE_COORD(tr.vecEndPos.x);
		WRITE_COORD(tr.vecEndPos.y);
		WRITE_COORD(tr.vecEndPos.z);
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

		edict_t *pBomb;
		pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
		DispatchSpawn(pBomb);
		pBomb->v.origin = tr.vecEndPos;
		pBomb->v.velocity = Vector(0,0,0);
		pBomb->v.dmg = 200;
		pBomb->v.owner = pEntity->v.euser2;
		pBomb->v.effects |= EF_NODRAW;
		pBomb->v.nextthink = gpGlobals->time;
		pBomb->v.classname = MAKE_STRING("ao_gunshoot");
	}
}

void FixSentry(edict_t *pEntity)
{
	if(pEntity->v.armorvalue < BIGGUN_REQUIRED)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough armor to repair gun.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough armor to repair gun.\n"));
		return;
	}

	CBaseEntity *pFencePost = NULL;
    while(pFencePost = UTIL_FindEntityByClassname(pFencePost, "weapon_ao_guncontrol"))
    {
        if(pFencePost->pev->euser1 == pEntity)
        {
			CBaseEntity *pScan = NULL;
			while ((pScan = UTIL_FindEntityInSphere(pScan, pFencePost->pev->origin, 100)) != NULL)
			{
				//Is it an entity we are bothered about? (Players)
				if (strcmp(STRING(pScan->pev->classname), "player") == 0)
				{
					if(pScan->edict() == pEntity) //My owner?
					{
						CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Your gun was repaired.\n"));
						CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Your gun was repaired.\n"));
						pFencePost->pev->health = 100000;
						pEntity->v.armorvalue -= BIGGUN_REQUIRED;
					}
				}
			}
		}
	}
	return;
}

