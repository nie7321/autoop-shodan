/*****************************************************\
*	Auto-OP Shodan
*
*	File: servercommands.cpp
*	Description: HLDS Console commands
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
#include "skins.h"
#include "credits.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;

extern int ConnectCount;

void HandleServerCommands()
{
	const char *pcmd = CMD_ARGV(1);
	const char *arg1 = CMD_ARGV(2);
	const char *arg2 = CMD_ARGV(3);
	const char *arg3 = CMD_ARGV(4);
	const char *arg4 = CMD_ARGV(5);

	//Timed slot reservation system
	if ( FStrEq(pcmd, "reserveslot" ))
	{
		if(Features.ReservedSlotTime > gpGlobals->time)
		{
			ALERT ( at_console, "There is already a reserved slot in operation for %d more seconds.\n", ((int)Features.ReservedSlotTime - (int)gpGlobals->time));
			return;
		}

		if ( FStrEq(arg1, "" ))
		{
			ALERT ( at_console, "useage: admc reserveslot <seconds>\nReserves a slot for specified number of seconds.\n");
			return;
		}

		int Time = atoi(arg1);

		if(Time > 30 || Time < 5)
			Time = 10;

		Features.ReservedSlotTime = gpGlobals->time + Time;

		ALERT ( at_console, "A Reserved slot set for %d seconds.\n", Time);
		void KickHighPing();
		return;
	}

	if ( FStrEq(pcmd, "unreserveslot" ))
	{
		if(Features.ReservedSlotTime > gpGlobals->time)
		{
			Features.ReservedSlotTime = 0;
			ALERT ( at_console, "Reserved slot cancelled.\n");
		} else {
			ALERT ( at_console, "There is no reserved slot currently active.\n");
		}
		return;
	}

	if ( FStrEq(pcmd, "version" ))
	{
		ALERT ( at_console, "AutoOP Shodan version %s, rev %s\n",MOD_VERSION,MOD_REVISION);
		return;
	}

	if ( FStrEq(pcmd, "sysinfo" ))
	{
		int AdminCount = 0;

		for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
			if(PlayerInfo[i].IsAdmin > LEVEL_NONE)
				AdminCount += 1;
		}

		ALERT ( at_console, "AutoOP System info\n------------------\nVersion: %s\nRevision: %s\nSkins: %d\nPlayer connections: %d\nOnline Admins: %d\n",MOD_VERSION,MOD_REVISION,TOTALSKINS,ConnectCount,AdminCount);
		return;
	}

	if ( FStrEq(pcmd, "addadmin" ))
	{
		if ( FStrEq(arg1, "" ) || FStrEq(arg2, "" ))
		{
			ALERT ( at_console, "useage: admc addadmin <wonid> <level>\n");
			ALERT ( at_console, "addadmin: creates an admin with specified WONID and level.\n");
		} else {
			AddAdmin(NULL,arg1,arg2);
		}
		return;
	}

	if ( FStrEq(pcmd, "deladmin" ))
	{
		if ( FStrEq(arg1, "" ))
		{
			ALERT ( at_console, "useage: admc deladmin <wonid>\n");
			ALERT ( at_console, "addadmin: deletes an admin with specified WONID.\n");
		} else {
			DelAdmin(NULL,arg1);
		}
		return;
	}			
		
	//Default if no proper command is specified
	if ( FStrEq(pcmd, "" ))
	{
		ALERT ( at_console, "useage: admc <command> <parameters>\nAutoOP HLDS console command.\n");
	} else {
		ALERT ( at_console, "\"%s\" is an unrecognised AutoOP HLDS Console command.\n",pcmd);
	}
}

void KickHighPing()
{
	//Kicks player with the highest ping and tells them why.
	int max_ping = 0,player_num = 0;
	int ping = 0,loss = 0;
	char szCommand[BUF_SIZE];
	edict_t *pPlayer = NULL;

    for (int i = 1; i <= gpGlobals->maxClients; i++) {
		pPlayer= INDEXENT(i);

		if (pPlayer == NULL)
			continue;
		if (pPlayer->free)
			continue;
		if (FStrEq(STRING(pPlayer->v.netname), ""))
			continue;

	PLAYER_CNX_STATS(pPlayer,&ping,&loss);
	if ( ping > max_ping) {
		if ( PlayerInfo[i].IsAdmin <= LEVEL_NONE)
			player_num=i;
			max_ping=ping;
		}

	}

	if(player_num != 0)
	{
		_snprintf(szCommand,BUF_SIZE,"kick # %i\n", GETPLAYERUSERID( pPlayer));
		CLIENT_PRINTF(pPlayer, print_console, "You were kicked automatically to make space for an administrator.\n");
		SERVER_COMMAND(szCommand);
	
		UTIL_LogPrintf( "[AUTOOP] %s was kicked due to the slot reservation system\n",STRING(pPlayer->v.netname) );
	}
}
