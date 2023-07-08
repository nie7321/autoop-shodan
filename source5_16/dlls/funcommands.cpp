/*****************************************************\
*	Auto-OP Shodan
*
*	File: funcommands.cpp
*	Description: Code for gimmiks.
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
#include "aohgrunt.h"
#include "aogarg.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;
extern DLL_FUNCTIONS other_gFunctionTable;

void AdmCreateLight(edict_t *pAdmin, const char *Player, const char *Red, const char *Green, const char *Blue)
{
	//Give a player god mode
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	int iRed = atoi(Red);
	int iGreen = atoi(Green);
	int iBlue = atoi(Blue);


	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
		WRITE_BYTE(TE_DLIGHT);
		WRITE_COORD(pEntity->v.origin.x);
		WRITE_COORD(pEntity->v.origin.y);
		WRITE_COORD(pEntity->v.origin.z);
		WRITE_BYTE(25);
		WRITE_BYTE(iRed);
		WRITE_BYTE(iGreen);
		WRITE_BYTE(iBlue);
		WRITE_BYTE(255);
		WRITE_BYTE(0);
	MESSAGE_END();
}

void AdmRing(edict_t *pAdmin, const char *Player1, const char *Player2)
{
	char *PlayerInf = (char *)Player1;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "First player specified cannot be found.\n"));
		return;
	}

	PlayerInf = (char *)Player2;
	int Index2 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index2 == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Second player specified cannot be found.\n"));
		return;
	}

	if(Index1 == Index2)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "You must specify two unique players for the ring.\n"));
		return;
	}

	CBaseEntity *pPlayer1 = UTIL_PlayerByIndex(Index1);
	CBaseEntity *pPlayer2 = UTIL_PlayerByIndex(Index2);

	short sprite1;
	sprite1 = PRECACHE_MODEL("sprites/laserbeam.spr");

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pPlayer1->pev->origin);
		WRITE_BYTE(TE_BEAMRING);
		WRITE_SHORT(pPlayer1->entindex());
		WRITE_SHORT(pPlayer2->entindex());
		WRITE_SHORT(sprite1);
		WRITE_BYTE(1);
		WRITE_BYTE(10);
		WRITE_BYTE(100);
		WRITE_BYTE(10);
		WRITE_BYTE(255);
		WRITE_BYTE(200);
		WRITE_BYTE(200);
		WRITE_BYTE(0);
		WRITE_BYTE(255);
		WRITE_BYTE(10);
	MESSAGE_END();
}

void AdmScreenShake(edict_t *pAdmin)
{
	UTIL_ScreenShakeAll( Vector(0,0,0), 5.0, 3.0, 5.0);
}

void AdmScreenFade(edict_t *pAdmin, const char *Red, const char *Green, const char *Blue)
{
	int iRed = atoi(Red);
	int iGreen = atoi(Green);
	int iBlue = atoi(Blue);

	UTIL_ScreenFadeAll( Vector(iRed,iGreen,iBlue), 8, 2, 128, FFADE_IN );
}

void AdmSpawnGrunt(edict_t *pAdmin, const char *CanWalk)
{
	int iCanWalk = atoi(CanWalk);

	if(iCanWalk == 1)
		CreateGrunt(pAdmin,true);
	else
		CreateGrunt(pAdmin,false);
}

void AdmSpawnGarg(edict_t *pAdmin, const char *CanWalk)
{
	int iCanWalk = atoi(CanWalk);

	if(iCanWalk == 1)
		CreateGarg(pAdmin,true);
	else
		CreateGarg(pAdmin,false);
}

void AdmTrail(edict_t *pAdmin, const char *Player, const char *Red, const char *Green, const char *Blue)
{
	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index1);

	int iRed = atoi(Red);
	int iGreen = atoi(Green);
	int iBlue = atoi(Blue);

	short sprite;
	sprite = PRECACHE_MODEL("sprites/laserbeam.spr");

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pPlayer->pev->origin);
				WRITE_BYTE(TE_BEAMFOLLOW);
				WRITE_SHORT(pPlayer->entindex());
				WRITE_SHORT(sprite);
				WRITE_BYTE(100);
				WRITE_BYTE(25);
				WRITE_BYTE(iRed);
				WRITE_BYTE(iGreen);
				WRITE_BYTE(iBlue);
				WRITE_BYTE(150);
	MESSAGE_END();
}

void AdmDizzyness(edict_t *pAdmin, const char *Player, const char *Value)
{
	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index1);

	int iVal = atoi(Value);

	if(iVal!=0) {
		pPlayer->edict()->v.punchangle.z = iVal;
	}
}

void AdmDropModel(edict_t *pAdmin,const char *Player, const char *Model)
{
	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index1);

	char *ModelName = (char *)Model;
	short model;
	model = PRECACHE_MODEL(ModelName);

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pPlayer->pev->origin);
		WRITE_BYTE(TE_MODEL);
		WRITE_COORD(pPlayer->pev->origin.x);
		WRITE_COORD(pPlayer->pev->origin.y);
		WRITE_COORD(pPlayer->pev->origin.z);
		WRITE_COORD(RANDOM_LONG(-150, 150));
		WRITE_COORD(RANDOM_LONG(-150, 150));
		WRITE_COORD(RANDOM_LONG(-150, 150));
		WRITE_ANGLE(25);
		WRITE_SHORT(model);
		WRITE_BYTE(TE_BOUNCE_SHELL);
		WRITE_BYTE(100);
	MESSAGE_END();
}

void AdmAdminStrike(edict_t *pAdmin, const char *Player)
{
	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	edict_t *pVictim = INDEXENT(Index1);

	StrikeLocation(pVictim->v.origin,1);
}

void AdmEntCreate(edict_t *pAdmin, const char *Value)
{
	int iVal = atoi(Value);
	int istr = 0;

	switch (iVal) {
	case 1:
		istr = MAKE_STRING("xen_spore_large");
		break;
	case 2:
		istr = MAKE_STRING("xen_spore_medium");
		break;
	case 3:
		istr = MAKE_STRING("xen_spore_small");
		break;
	case 4:
		istr = MAKE_STRING("xen_tree");
		break;
	case 5:
		istr = MAKE_STRING("xen_hair");
		break;
	case 6:
		istr = MAKE_STRING("xen_plantlight");
		break;
	case 7:
		istr = MAKE_STRING("item_artifact_invisibility");
		break;
	case 8:
		istr = MAKE_STRING("item_artifact_invulnerability");
		break;
	case 9:
		istr = MAKE_STRING("item_artifact_super_damage");
		break;
	case 10:
		istr = MAKE_STRING("monster_turret");
		break;
	case 11:
		istr = MAKE_STRING("monster_miniturret");
		break;
	case 12:
		istr = MAKE_STRING("item_cells");
		break;
	case 13:
		istr = MAKE_STRING("ao_tfc_gun");
		break;
	default:
		istr = MAKE_STRING("xen_tree");
	}
	
	//Now spawn the thing
	edict_t	*pent;

	pent = CREATE_NAMED_ENTITY(istr);
	if ( FNullEnt( pent ) )
	{
		ALERT ( at_console, "NULL Ent in UTIL_PrecacheOther\n" );
		return;
	}

	VARS( pent )->origin = pAdmin->v.origin;
	VARS( pent )->angles = pAdmin->v.angles;

	if(iVal == 13)
	{
		CBaseEntity *pTest = (CBaseEntity *)GET_PRIVATE(pent);
		if (pTest) { // run the ptimer spawn
			pTest->Spawn();   
		}
	} else {
		DispatchSpawn( pent );
	}
}

void AdmToss(edict_t *pAdmin, const char *Player)
{
	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	edict_t *pVictim = INDEXENT(Index1);

	pVictim->v.velocity.x += floor(RANDOM_FLOAT(-1500,1500));
	pVictim->v.velocity.y += floor(RANDOM_FLOAT(-1500,1500));
	if (pVictim->v.velocity.z >= 200) {
		pVictim->v.velocity.z -= floor(RANDOM_FLOAT(900,1900));
	} else {
		pVictim->v.velocity.z += floor(RANDOM_FLOAT(900,1900));
	}

	short sprite1;
	sprite1 = PRECACHE_MODEL("sprites/steam1.spr");

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pVictim->v.origin);
		WRITE_BYTE(TE_LARGEFUNNEL);
		WRITE_COORD(pVictim->v.origin.x);
		WRITE_COORD(pVictim->v.origin.y);
		WRITE_COORD(pVictim->v.origin.z);
		WRITE_SHORT(sprite1);
		WRITE_SHORT(0);
	MESSAGE_END();
}

void AdmKillTrail(edict_t *pAdmin, const char *Player)
{
	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	edict_t *pVictim = INDEXENT(Index1);

	MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pVictim->v.origin);
			WRITE_BYTE(TE_KILLBEAM);
			WRITE_SHORT(ENTINDEX(pVictim));
	MESSAGE_END();
}

void AdmPlaySound(edict_t *pAdmin, const char *FileName)
{
	CBaseEntity *pSoundEnt = UTIL_FindEntityByTargetname( NULL, "ao_dllsound");

	if(pSoundEnt != NULL)
	{
		char* szSoundFile = (char*) STRING(pSoundEnt->pev->message);
		EMIT_AMBIENT_SOUND(pSoundEnt->edict(), Vector(0,0,0), szSoundFile, 0, 0, SND_STOP, 0);
		pSoundEnt->pev->flags = FL_KILLME;
	}

	edict_t *pEntity = NULL;
	pEntity = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	pEntity->v.targetname = MAKE_STRING("ao_dllsound");
	pEntity->v.message = ALLOC_STRING(FileName);

	EMIT_AMBIENT_SOUND(pEntity, Vector(0,0,0), FileName, 1, ATTN_NONE, 0, 100);
}

void AdmPermLight(edict_t *pAdmin, const char *Red, const char *Green, const char *Blue)
{
	int iRed = atoi(Red);
	int iGreen = atoi(Green);
	int iBlue = atoi(Blue);

	edict_t *pLight = NULL;
	pLight = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
	DispatchSpawn(pLight);
	pLight->v.origin = pAdmin->v.origin;
	pLight->v.angles = Vector(0,0,0);
	pLight->v.velocity = Vector(0,0,0);
	pLight->v.takedamage = DAMAGE_NO;
	pLight->v.health = 1;
	pLight->v.movetype = MOVETYPE_NONE;
	pLight->v.nextthink = gpGlobals->time;
	pLight->v.effects = EF_NODRAW;
	pLight->v.solid = SOLID_BBOX;

	pLight->v.vuser1.x = iRed;
	pLight->v.vuser1.y = iGreen;
	pLight->v.vuser1.z = iBlue;

	SET_MODEL(pLight, "models/nail.mdl");
	UTIL_SetSize(VARS(pLight), Vector(0, 0, 0), Vector(0, 0, 0));
	pLight->v.classname = MAKE_STRING("ao_light");

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Permlight(): Admin created an ao_light entity\n");
		fclose(fp);
	}
}

void DropWeapon(edict_t *pAdmin,const char *WeaponName)
{
	int Weapon = 0;
	if ( FStrEq(WeaponName, "rpg" ))
		Weapon = WEAPON_ROCKET_LAUNCHER;

	if ( FStrEq(WeaponName, "sniperrifle" ))
		Weapon = WEAPON_SNIPER_RIFLE;

	if ( FStrEq(WeaponName, "autorifle" ))
		Weapon = WEAPON_AUTO_RIFLE;

	if ( FStrEq(WeaponName, "shotgun" ))
		Weapon = WEAPON_TF_SHOTGUN;

	if ( FStrEq(WeaponName, "supershotgun" ))
		Weapon = WEAPON_SUPER_SHOTGUN;

	if ( FStrEq(WeaponName, "ng" ))
		Weapon = WEAPON_NAILGUN;

	if ( FStrEq(WeaponName, "superng" ))
		Weapon = WEAPON_SUPER_NAILGUN;

	if ( FStrEq(WeaponName, "gl" ))
		Weapon = WEAPON_GRENADE_LAUNCHER;

	if ( FStrEq(WeaponName, "flamethrower" ))
		Weapon = WEAPON_FLAMETHROWER;

	if ( FStrEq(WeaponName, "ic" ))
		Weapon = WEAPON_INCENDIARY;

	if ( FStrEq(WeaponName, "ac" ))
		Weapon = WEAPON_ASSAULT_CANNON;

	if ( FStrEq(WeaponName, "tranq" ))
		Weapon = WEAPON_TRANQ;

	if ( FStrEq(WeaponName, "railgun" ))
		Weapon = WEAPON_LASER;

	if ( FStrEq(WeaponName, "pl" ))
		Weapon = WEAPON_PIPEBOMB_LAUNCHER;


	UTIL_MakeVectors( pAdmin->v.v_angle );
	TraceResult tr;
	Vector trace_origin;

	trace_origin = pAdmin->v.origin;
	if ( pAdmin->v.flags & FL_DUCKING )
	{
		trace_origin = trace_origin - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	}

	UTIL_TraceLine( trace_origin + gpGlobals->v_forward * 20, trace_origin + gpGlobals->v_forward * 64, dont_ignore_monsters, NULL, &tr );

	if (tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction > 0.25)
	{
		if(Weapon > 0)
		{
			//Player drops the gun they are carrying to the floor
			EMIT_SOUND_DYN2(pAdmin, CHAN_VOICE, "items/weapondrop1.wav", 1, ATTN_NORM, 0, PITCH_NORM);
			edict_t *pDropped = NULL;
			pDropped = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
			pDropped->v.origin = tr.vecEndPos;
			pDropped->v.angles = pAdmin->v.v_angle;
			pDropped->v.takedamage = DAMAGE_NO;
			pDropped->v.velocity = pAdmin->v.velocity + gpGlobals->v_forward*400;
			pDropped->v.gravity = 0.5;
			pDropped->v.friction = 0.5;
			pDropped->v.health = 100000;
			pDropped->v.solid = SOLID_SLIDEBOX;
			pDropped->v.movetype = MOVETYPE_TOSS;
			pDropped->v.iuser1 = Weapon; //Weapon index
			pDropped->v.iuser2 = 0;
			pDropped->v.iuser2 |= (1<<Weapon); //True index
			pDropped->v.fuser1 = gpGlobals->time + 2; //2 sec pickup delay
			pDropped->v.fuser2 = gpGlobals->time + 120; //Life of gun
			pDropped->v.team = pAdmin->v.team;
			pDropped->v.iuser4 = 1; //This means we respawn!
			pDropped->v.owner = pAdmin;
			DispatchSpawn(pDropped);
			SET_MODEL(pDropped, "models/autoop/w_guns.mdl");
			pDropped->v.classname = MAKE_STRING("autoop_dropped_weapon");
			pDropped->v.nextthink = gpGlobals->time + 0.1;
			UTIL_SetSize(VARS(pDropped), Vector( -4, -4, 0), Vector(4, 4, 8));
			UTIL_SetOrigin( VARS(pDropped), pDropped->v.origin );

			if(Features.Debug >= 1)
			{
				fp=fopen("sdebug.log","a");
				fprintf(fp, "DropWeapon(): Admin dropped a weapon of ID: %d\n",Weapon);
				fclose(fp);
			}
		}
	}
}

void AdmVEffect(edict_t *pAdmin, const char *Player, const char *EffectCode)
{
	int iEffectCode = atoi(EffectCode);

	char *PlayerInf = (char *)Player;
	int Index1 = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index1 == 0)
		return;

	PlayerInfo[Index1].EffectNextThink = 0.0;
	PlayerInfo[Index1].VisualCounter = 0;
	PlayerInfo[Index1].VisualEffect = iEffectCode;
}

void AdmResetBuild(edict_t *pAdmin)
{
	PlayerInfo[ENTINDEX(pAdmin)].HasFencePost = 0;
	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Sensor build flag reset.\n"));
}
