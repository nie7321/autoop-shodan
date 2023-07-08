/*****************************************************\
*	Auto-OP Shodan
*
*	File: cheatcommands.cpp
*	Description: Cheat commands code.
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
extern TFXFeatureEnabled_s Features;

void AdmGod(edict_t *pAdmin, const char *Player, const char *Value)
{
	//Give a player god mode
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));
	int iVal = atoi(Value);

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{

		edict_t *pEntity = INDEXENT(Index);

		if(iVal == 1)
		{
			pEntity->v.takedamage = DAMAGE_NO;
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "God Mode applied to %s\n",STRING(pEntity->v.netname)));
			
			if(Features.LogStuff == 1)
				UTIL_LogPrintf( "[AUTOOP] %s applied godmode to %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
		} else {
			pEntity->v.takedamage = DAMAGE_AIM;
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "God Mode taken off %s\n",STRING(pEntity->v.netname)));
			
			if(Features.LogStuff == 1)
				UTIL_LogPrintf( "[AUTOOP] %s took godmode off %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
		}
	}
}

void AdmNoclip(edict_t *pAdmin, const char *Player, const char *Value)
{
	//Make a player noclip
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));
	int iVal = atoi(Value);

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{

		edict_t *pEntity = INDEXENT(Index);

		if(iVal == 1)
		{
			pEntity->v.movetype = MOVETYPE_NOCLIP;
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Noclip applied to %s\n",STRING(pEntity->v.netname)));
			
			if(Features.LogStuff == 1)
				UTIL_LogPrintf( "[AUTOOP] %s applied noclip to %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
		} else {
			pEntity->v.movetype = MOVETYPE_WALK;
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Noclip taken off %s\n",STRING(pEntity->v.netname)));
			
			if(Features.LogStuff == 1)
				UTIL_LogPrintf( "[AUTOOP] %s took noclip off %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
		}
	}
}

void AdmHologram(edict_t *pAdmin, const char *Player, const char *Value)
{
	//Turn a player into a hologram
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));
	int iVal = atoi(Value);

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{
	
		edict_t *pEntity = INDEXENT(Index);

		if(iVal == 1)
		{
			pEntity->v.renderfx = 16;
			pEntity->v.rendermode = 1;
			pEntity->v.renderamt = 100;
			pEntity->v.solid = SOLID_NOT;
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s has been made into a hologram.\n",STRING(pEntity->v.netname)));
			
			if(Features.LogStuff == 1)
				UTIL_LogPrintf( "[AUTOOP] %s applied hologram mode to %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
		} else {
			pEntity->v.renderfx = 0;
			pEntity->v.rendermode = 0;
			pEntity->v.renderamt = 255;
			pEntity->v.solid = SOLID_SLIDEBOX;
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s has been made solid.\n",STRING(pEntity->v.netname)));
			
			if(Features.LogStuff == 1)
				UTIL_LogPrintf( "[AUTOOP] %s took hologram mode off %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
		}
	}
}

void AdmInvis(edict_t *pAdmin, const char *Player, const char *Value)
{
	//Make a player invisible
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));
	int iVal = atoi(Value);

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(iVal == 1)
	{
		pEntity->v.rendermode = 1;
		pEntity->v.renderamt = 0;
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s has been made invisible.\n",STRING(pEntity->v.netname)));
		
		if(Features.LogStuff == 1)
			UTIL_LogPrintf( "[AUTOOP] %s applied invisibility to %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
    } else {
		pEntity->v.rendermode = 0;
		pEntity->v.renderamt = 255;
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s has been made visible.\n",STRING(pEntity->v.netname)));
		
		if(Features.LogStuff == 1)
			UTIL_LogPrintf( "[AUTOOP] %s took invisibility off %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
	}
}

void AdmHealth(edict_t *pAdmin, const char *Player, const char *Value)
{
	//Change a players health
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));
	int iVal = atoi(Value);

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(iVal != 0)
	{
		pEntity->v.health = iVal;
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s has now got %d health.\n",STRING(pEntity->v.netname),iVal));
		
		if(Features.LogStuff == 1)
			UTIL_LogPrintf( "[AUTOOP] %s gave %s %d health.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname),iVal);
	}
}

void AdmChangeSpeed(edict_t *pAdmin, const char *Player, const char *Speed)
{
	int iVal = atoi(Speed);

	//Change a players speed
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(iVal != 0)
	{
		pEntity->v.maxspeed = iVal;
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s had their speed changed to %d.\n",STRING(pEntity->v.netname),iVal));
		
		if(Features.LogStuff == 1)
			UTIL_LogPrintf( "[AUTOOP] %s changed speed of %s to %d.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname),iVal);
	}
}

void AdmGiveAllGuns(edict_t *pAdmin, const char *Player)
{

	//Give a player all the guns of the game
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	UTIL_GiveAllWeapons(pPlayer);
	
	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s gave %s all the guns.\n", STRING(pAdmin->v.netname),STRING(pPlayer->pev->netname));
}

void AdmArmor(edict_t *pAdmin, const char *Player, const char *Value)
{
	//Change a players health
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));
	int iVal = atoi(Value);

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(iVal != 0)
	{
		pEntity->v.armorvalue = iVal;
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s has now got %d armor.\n",STRING(pEntity->v.netname),iVal));
		
		if(Features.LogStuff == 1)
			UTIL_LogPrintf( "[AUTOOP] %s gave %s %d armor.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname),iVal);
	}
}

void AdmChangeAmmo(edict_t *pAdmin, const char *AmmoType, const char *Amount, const char *Player)
{
	//Change a players ammo
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	int offset = 0;

	if(_stricmp(AmmoType, "nails" ) == 0)
		offset = 200;

	if(_stricmp(AmmoType, "rockets" ) == 0)
		offset = 216;

	if(_stricmp(AmmoType, "shells" ) == 0)
		offset = 192;

	if(_stricmp(AmmoType, "cells" ) == 0)
		offset = 208;

	if(_stricmp(AmmoType, "gren1" ) == 0)
		offset = 56;

	if(_stricmp(AmmoType, "gren2" ) == 0)
		offset = 60;

	if(offset == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Please enter valid ammo type.\n"));
		return;
	}

	int value = atoi(Amount);//90;

	//*(int *)((char *)pEntity->pvPrivateData + offset) = value;

	PatchpvPrivateInt(pEntity,offset,value);
	
	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Ammo changed.\n"));
}

void AdmMultiBuild(edict_t *pAdmin, const char *Type, const char *Player)
{
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	int offset = 0;

	if(_stricmp(Type, "sentrygun" ) == 0)
		offset = 326;

	if(_stricmp(Type, "dispenser" ) == 0)
		offset = 322;

	if(offset == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Please enter valid build type.\n"));
		return;
	}

	*(word *)((char *)pEntity->pvPrivateData + offset) = 0x0000;

	//PatchpvPrivateInt(pEntity,offset,value);

	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Successful.\n"));
}






	