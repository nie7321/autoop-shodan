/*****************************************************\
*	Auto-OP Shodan
*
*	File:playercommands.cpp
*	Description: Commands related to player manipulation.
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
extern TFXFeatureEnabled_s Features;

extern DLL_FUNCTIONS other_gFunctionTable;

void AdmForceBlue(edict_t *pAdmin, const char *Player)
{
	//Force a player to blue team
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 1\n") );
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s moved to blue team.\n",STRING(pEntity->v.netname)));

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s moved %s to blue team.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
}

void AdmForceRed(edict_t *pAdmin, const char *Player)
{
	//Force a player to red team
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 2\n") );
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s moved to red team.\n",STRING(pEntity->v.netname)));

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s moved %s to red team.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
}

void AdmForceYellow(edict_t *pAdmin, const char *Player)
{
	//Force a player to yellow team
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 3\n") );
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s moved to yellow team.\n",STRING(pEntity->v.netname)));

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s moved %s to yellow team.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
}

void AdmForceGreen(edict_t *pAdmin, const char *Player)
{
	//Force a player to blue team
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("jointeam 4\n") );
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s moved to green team.\n",STRING(pEntity->v.netname)));

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s moved %s to green team.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
}

void AdmClientExecute(edict_t *pAdmin, const char *Player, const char *Command)
{
	//Force a player to run a command
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,-1);

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		CLIENT_COMMAND ( pEntity, UTIL_VarArgs("%s\n",Command) );
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Console command run on users console.\n"));
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s ran command on %s: %s\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname),Command);

}

void AdmVoxToEveryone(edict_t *pAdmin, const char *Text)
{
	edict_t *pEntity = NULL;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pEntity = INDEXENT(i);

		if(pEntity)
		{
			if(!FStrEq(STRING(pEntity->v.netname),""))
			{
				CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"%s\"\n",Text) );
			}
		}
	}
	
	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s used VOX with:%s.\n", STRING(pAdmin->v.netname),Text);
}

void AdmExecuteToAll(edict_t *pAdmin, const char *Command)
{
	edict_t *pEntity = NULL;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pEntity = INDEXENT(i);

		if(pEntity)
		{
			if(!FStrEq(STRING(pEntity->v.netname),""))
			{
				CLIENT_COMMAND ( pEntity, UTIL_VarArgs("%s\n",Command) );
			}
		}
	}
}

void AdmMoveHere(edict_t *pAdmin, const char *Player)
{
	//Move a player to the caller
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,-1);

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		pEntity->v.origin = pAdmin->v.origin;
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s moved %s to himself.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
}

void AdmMoveThere(edict_t *pAdmin, const char *Player)
{
	//Move a player to the caller
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,-1);

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		pAdmin->v.origin = pEntity->v.origin;
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s moved to %s.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));
}

void AdmChangeClass(edict_t *pAdmin, const char *Player, const char *Class)
{
	int iClass = atoi(Class);

	//Move a player to the caller
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		pEntity->v.playerclass = iClass;
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s changed %s to class number %d.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname),iClass);
}

void AdmTakeGuns(edict_t *pAdmin, const char *Player)
{
	//Move a player to the caller
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pEntity = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		pEntity->v.weaponmodel = 0;
		pEntity->v.viewmodel = 0;
		pEntity->v.weapons = 0;

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
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}

	if(Features.LogStuff == 1)
		UTIL_LogPrintf( "[AUTOOP] %s took all guns from %s.\n", STRING(pAdmin->v.netname),STRING(pEntity->v.netname));

}

void AdmSpray(edict_t *pAdmin, const char *Player)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{
		if(PlayerInfo[Index].LoadedInfo.CanSpray == 0)
		{
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player cannot use spraypaint.\n"));
			PlayerInfo[Index].LoadedInfo.CanSpray = 1;

		} else {

			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player may use spraypaint.\n"));
			PlayerInfo[Index].LoadedInfo.CanSpray = 0;

		}
	}
}

void AdmAllowVote(edict_t *pAdmin, const char *Player)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{
		if(PlayerInfo[Index].LoadedInfo.CanVote == 0)
		{
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player cannot vote in a map or kick vote.\n"));
			PlayerInfo[Index].LoadedInfo.CanVote = 1;

		} else {

			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player may vote in a map or kick vote.\n"));
			PlayerInfo[Index].LoadedInfo.CanVote = 0;

		}
	}
}

void AdmWarn(edict_t *pAdmin, const char *Player)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	edict_t *pPlayer = INDEXENT(Index);

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{
		PlayerInfo[Index].LoadedInfo.Warnings += 1;
		
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player warned - player now has %d warnings.\n",PlayerInfo[Index].LoadedInfo.Warnings));
		CLIENT_PRINTF( pPlayer, print_center, UTIL_VarArgs( "You have been warned by %s\nYou now have %d warning(s).",STRING(pAdmin->v.netname),PlayerInfo[Index].LoadedInfo.Warnings));
	}
}

void AdmSetCustomSpawn(edict_t *pAdmin, const char *Player)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	edict_t *pPlayer = INDEXENT(Index);

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		PlayerInfo[Index].CustomSpawnPoint = pAdmin->v.origin;
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}
	
	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Custom spawn point set for %s\n",STRING(pPlayer->v.netname)));
}

void AdmKillCustomSpawn(edict_t *pAdmin, const char *Player)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	edict_t *pPlayer = INDEXENT(Index);

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin)) == 0)
	{
		PlayerInfo[Index].CustomSpawnPoint = Vector(0,0,0);
	} else {
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "The player you are trying to mess about with has more or equal admin power than you.\n"));
	}
	
	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Killed custom spawn point linked to %s\n",STRING(pPlayer->v.netname)));
}

void AdmSetTime(edict_t *pAdmin, const char *Player, int Hours)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	edict_t *pPlayer = INDEXENT(Index);

	if(Index == 0)
		return;

	PlayerInfo[Index].LoadedInfo.TimeLen = (Hours * 3600);
	
	CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Changed time of %s to %d hours.\n",STRING(pPlayer->v.netname),Hours));
}

void PrivateMessage(edict_t *pSender, const char *Player, const char *Message)
{
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,-1);

	if(Index < 1)
		return;

	edict_t *pPlayer = INDEXENT(Index);

	if(PlayerInfo[Index].PMBlock == 1)
	{
		CLIENT_PRINTF( pSender, print_console, UTIL_VarArgs("Player is not accepting private messages.\n"));
		ClientPrint( &pSender->v, HUD_PRINTTALK, UTIL_VarArgs("Player is not accepting private messages.\n"));
		return;
	}

	//Send to reciever
	ClientPrint( &pPlayer->v, HUD_PRINTTALK, UTIL_VarArgs( "(PRIV) %s: %s\n",STRING(pSender->v.netname),Message ) );

	//Send a copy to sender
	ClientPrint( &pSender->v, HUD_PRINTTALK, UTIL_VarArgs( "Sent to %s: %s\n",STRING(pPlayer->v.netname),Message ) );

	if(Features.AdminSeePriv == 1)
	{
		//Send a copy to admins...
		edict_t *pAdmin = NULL;
		for (int i = 1; i <= gpGlobals->maxClients; i++) {
			if(PlayerInfo[i].IsAdmin >= LEVEL_SUPEROP)
			{
				pAdmin = INDEXENT(i);
				if(pAdmin)
				{
					if((pAdmin != pPlayer) && (pAdmin != pSender))
					{
						if(!FStrEq(STRING(pAdmin->v.netname),"") && (FStrEq(STRING(pAdmin->v.classname),"player")))
							ClientPrint( &pAdmin->v, HUD_PRINTTALK, UTIL_VarArgs( "From %s to %s: %s\n",STRING(pSender->v.netname),STRING(pPlayer->v.netname),Message ) );
					}
				}
			}
		}
	}
}

void AdmAllowSpec(edict_t *pAdmin, const char *Player)
{
	//Makes a player a spectator, bypassing tfx_adminspect
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	edict_t *pPlayer = INDEXENT(Index);

	if(Index == 0)
		return;

	PlayerInfo[Index].SpecOverride = 1;

	CLIENT_COMMAND(pPlayer, "spectate\n");
}

void AdmMsgBox(edict_t *pAdmin, const char *Text)
{
	edict_t *pEntity = NULL;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pEntity = INDEXENT(i);

		if(pEntity)
		{
			if(!FStrEq(STRING(pEntity->v.netname),""))
			{
				CLIENT_COMMAND ( pEntity, UTIL_VarArgs("scr_connectmsg \"%s\"\n",Text) );
			}
		}
	}
}



