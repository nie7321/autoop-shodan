/*****************************************************\
*	Auto-OP Shodan
*
*	File: basiccommands.cpp
*	Description: Basic administrative functions.
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
#include "credits.h"
#include "userstrings.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;

extern DLL_FUNCTIONS other_gFunctionTable;

extern float MapChange;
extern char *NextMapChange;

FILE *logfile; //For debug

void AdmBan(edict_t *pAdmin, const char *Player, const char *Time, const char *Reason)
{
	//AdmBan() - Bans Player specified in Param1 for <Param2> minutes.

	int iTime = atoi(Time);

	if(iTime < 0) { iTime = 0; }

	int IsWONID = 0;
	char *PlayerInfo = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInfo,ENTINDEX(pAdmin));
	char BanCommand[256];

	if(strlen(Reason) == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Please specify a reason.\n"));
		return;
	}

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{

		edict_t *pBan = INDEXENT(Index);

		CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "\n\n\n******************************\n"));
		
		if(iTime > 0)
			CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "You have been banned for %d minutes.\n",iTime));
		else
			CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "You have been banned indefinately.\n"));

		CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "By Admin %s, Reason: %s\n",STRING(pAdmin->v.netname),Reason));
		CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "\nContact rob@planetfortress.com If you believe this to be in error.\n"));
		CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "Make sure you quote the admin and reason if you expect help.\n"));
		CLIENT_PRINTF( pBan, print_console, UTIL_VarArgs( "******************************\n"));

		if(atoi(Player) != 0)
		{
			_snprintf(BanCommand,256,"banid %i %u\n",iTime,atoi(Player));
		} else {
			_snprintf(BanCommand,256,"banid %i %s\n",iTime,GETPLAYERAUTHID( pBan ));
		}

		UTIL_LogPrintf( "[AUTOOP] Ban: %s banned %s\n", STRING(pAdmin->v.netname),STRING(pBan->v.netname));

		SERVER_COMMAND(BanCommand);
		SERVER_COMMAND("writeid\n");
		CLIENT_COMMAND ( pBan, UTIL_VarArgs("disconnect\n") );

		static SYSTEMTIME timeinfo;
		GetLocalTime(&timeinfo);

		const char *TempID = NULL;
		char AdminID[256];
		char PlayerID[256];

		TempID = GETPLAYERAUTHID(pAdmin);
		strcpy(AdminID,TempID);

		TempID = GETPLAYERAUTHID(pBan);
		strcpy(PlayerID,TempID);

		if(Features.ICQNotify == 1)
		{
			char ICQString[512];
			_snprintf( ICQString,511,"(%d-%d-%d %02d:%02d:%02d) %s(%s) banned %s(%s) for %i minutes. Reason: %s.",timeinfo.wDay,timeinfo.wMonth,timeinfo.wYear,timeinfo.wHour,timeinfo.wMinute,timeinfo.wSecond,STRING(pAdmin->v.netname),AdminID,STRING(pBan->v.netname),PlayerID,iTime,Reason);
			ICQString[512] = '\0';
			SendStandardICQ("ALL","Ban",ICQString);
		}

		logfile=fopen("shodan_ban.log","a");
		fprintf(logfile, "\n");
		fprintf( logfile,"(%d-%d-%d %02d:%02d:%02d) %s(%s) banned %s(%s) for %i minutes. Reason: %s.",timeinfo.wDay,timeinfo.wMonth,timeinfo.wYear,timeinfo.wHour,timeinfo.wMinute,timeinfo.wSecond,STRING(pAdmin->v.netname),AdminID,STRING(pBan->v.netname),PlayerID,iTime,Reason);
		fclose(logfile);
	}
}

void AdmKick(edict_t *pAdmin, const char *Player, const char *Reason)
{
	//Admkick() - Kicks a player from the server

	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(strlen(Reason) == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Please specify a reason.\n"));
		return;
	}

	if(Index == 0)
		return;

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{
		edict_t *pKick = INDEXENT(Index);

		//char KickCommand[256];
		//_snprintf(KickCommand,256,"kick # %i\n", GETPLAYERUSERID( pKick ));

		CLIENT_PRINTF( pKick, print_console, UTIL_VarArgs( "\n\n\n*************************\n"));
		CLIENT_PRINTF( pKick, print_console, UTIL_VarArgs( "You were kicked by %s\n",STRING(pAdmin->v.netname)));
		CLIENT_PRINTF( pKick, print_console, UTIL_VarArgs( "Reason: %s\n", Reason));
		CLIENT_PRINTF( pKick, print_console, UTIL_VarArgs( "*************************\n"));

		UTIL_LogPrintf( "[AUTOOP] Kick: %s kicked %s (%s)\n", STRING(pAdmin->v.netname),STRING(pKick->v.netname),Reason);

		static SYSTEMTIME timeinfo;
		GetLocalTime(&timeinfo);

		const char *TempID = NULL;
		char AdminID[256];
		char PlayerID[256];

		TempID = GETPLAYERAUTHID(pAdmin);
		strcpy(AdminID,TempID);

		TempID = GETPLAYERAUTHID(pKick);
		strcpy(PlayerID,TempID);

		if(Features.ICQNotify == 1)
		{
			char ICQString[512];
			_snprintf( ICQString,511,"(%d-%d-%d %02d:%02d:%02d) %s(%s) kicked %s(%s) Reason: %s.",timeinfo.wDay,timeinfo.wMonth,timeinfo.wYear,timeinfo.wHour,timeinfo.wMinute,timeinfo.wSecond,STRING(pAdmin->v.netname),AdminID,STRING(pKick->v.netname),PlayerID,Reason);
			ICQString[512] = '\0';
			SendStandardICQ("ALL","Kick",ICQString);
		}

		logfile=fopen("shodan_kick.log","a");
		fprintf(logfile, "\n");
		fprintf( logfile,"(%d-%d-%d %02d:%02d:%02d) %s(%s) kicked %s(%s) Reason: %s.",timeinfo.wDay,timeinfo.wMonth,timeinfo.wYear,timeinfo.wHour,timeinfo.wMinute,timeinfo.wSecond,STRING(pAdmin->v.netname),AdminID,STRING(pKick->v.netname),PlayerID,Reason);
		fclose(logfile);

		//SERVER_COMMAND(KickCommand);
		CLIENT_COMMAND ( pKick, UTIL_VarArgs("disconnect\n") );
	}
}

void AdmSay(edict_t *pAdmin, const char *Text)
{
	//Admsay() - Send text to clients
	UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("%s\n",Text));
}

void AdmTeamplay(edict_t *pAdmin, const char *Value)
{
	//AdmTeamplay() - Change teamplay value.
	char Command[256];

	_snprintf(Command,256,"mp_teamplay %s\n", Value);

	SERVER_COMMAND(Command);
}

void AdmSpecttalk(edict_t *pAdmin, const char *Value)
{
	//Admspectalk() - Change spectator talk mode
	char Command[256];
	int SpecVal = atoi(Value);

	if(SpecVal == 1)
	{
		_snprintf(Command,256,"tfc_spectchat 1\n");
	} else {
		_snprintf(Command,256,"tfc_spectchat 0\n");
	}

	SERVER_COMMAND(Command);
}

void AdmGravity(edict_t *pAdmin, const char *Value)
{
	//AdmGravity() - Change gravity value.
	char Command[256];

	_snprintf(Command,256,"sv_gravity %s\n", Value);

	SERVER_COMMAND(Command);
}

void AdmGodMessage(edict_t *pAdmin, const char *Text)
{
	//AdmGodMessage1() - Send HUD message to clients.
	
	hudtextparms_t m_textParms;

	m_textParms.r1 = 200;
	m_textParms.g1 = 150;
	m_textParms.b1 = 10;
	m_textParms.a1 = 0;
	m_textParms.r2 = 0;
	m_textParms.g2 = 0;
	m_textParms.b2 = 200;
	m_textParms.a2 = 0;
	m_textParms.fadeinTime = 0.02;
	m_textParms.fadeoutTime = 1.0;
	m_textParms.holdTime = 10;   
	m_textParms.fxTime = (10 / 4);
	m_textParms.x = -1;
	m_textParms.y = 0.4;
	m_textParms.effect= 2;
	m_textParms.channel=2;
 
	UTIL_HudMessageAll( m_textParms,Text );

	UTIL_LogPrintf( "[AUTOOP] god_msg \"%s\"\n", Text);
}

void AdmGag(edict_t *pAdmin, const char *Player)
{
	//Toggle a players gag variable.
	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pGagPlr = INDEXENT(Index);

	if(IsMorePowerful(Index,ENTINDEX(pAdmin) == 0))
	{
		if(PlayerInfo[Index].LoadedInfo.CanTalk == 0)
		{
			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player gagged.\n"));
			PlayerInfo[Index].LoadedInfo.CanTalk = 1;
			strcpy(PlayerInfo[Index].GaggedName,STRING(pGagPlr->v.netname));

		} else {

			CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player ungagged.\n"));
			PlayerInfo[Index].LoadedInfo.CanTalk = 0;

		}
	}
}

void AdmMap(edict_t *pAdmin, const char *MapName)
{
	//Change the map
	char NextMap[32];

	if(MapName == NULL)
		return;

	strncpy( NextMap, MapName,32);
	fix_string(NextMap,strlen(NextMap));

	if ( !IS_MAP_VALID(NextMap) ) {
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "This map does not exist\n"));
		return;
	}

	UTIL_LogPrintf( "[AUTOOP] %s changed map to %s\n", STRING(pAdmin->v.netname),NextMap);

	CHANGE_LEVEL( NextMap, NULL );

	/*SERVER_COMMAND("mp_timelimit 0.1\n");
		
	NextMapChange = NextMap;
	MapChange = gpGlobals->time + 1;*/
}

void TimeLeft( )
{
	long int Seconds = (int)CVAR_GET_FLOAT("mp_timeleft");
	long int Minutes = (int)CVAR_GET_FLOAT("mp_timeleft");
	long int Hours = (int)CVAR_GET_FLOAT("mp_timeleft");

	Hours = Hours / 3600;
	Minutes = Seconds / 60 - (Hours * 60);
	Seconds = Seconds - (Minutes * 60) - (Hours * 3600);

	hudtextparms_t m_textParms;

	m_textParms.r1 = RANDOM_LONG(50,255);
	m_textParms.g1 = RANDOM_LONG(50,255);
	m_textParms.b1 = RANDOM_LONG(50,255);
	m_textParms.a1 = 0;
	m_textParms.r2 = RANDOM_LONG(50,255);
	m_textParms.g2 = RANDOM_LONG(50,255);
	m_textParms.b2 = RANDOM_LONG(50,255);
	m_textParms.a2 = 0;
	m_textParms.fadeinTime = 0.02;
	m_textParms.fadeoutTime = 1.0;
	m_textParms.holdTime = 7;   
	m_textParms.fxTime = (7 / 2);
	m_textParms.x = -1;
	m_textParms.y = 0.2;
	m_textParms.effect= 2;
	m_textParms.channel=1;

	char Text[256];

	_snprintf(Text,256,TIMELEFT_STRING,Hours,Minutes,Seconds);

	UTIL_HudMessageAll( m_textParms,Text );
}

void AdmSetAdminTalk(edict_t *pAdmin)
{
	//Setup a player to do a secure message
	PlayerInfo[ENTINDEX(pAdmin)].AdminChat = 1;

	CLIENT_COMMAND ( pAdmin, UTIL_VarArgs("messagemode\n") );
}

void AdmAdminChat(edict_t *pAdmin, const char *Speech)
{
	PlayerInfo[ENTINDEX(pAdmin)].AdminChat = 0; //Reset Adminchat flag.

	if(strlen(Speech) == 0)
		return;

	UTIL_LogPrintf( "[AUTOOP] secure_chat %s: %s \n", STRING(pAdmin->v.netname), Speech);

	char Text[300];

	_snprintf(Text,299,"(SECURE) %s: %s\n",STRING(pAdmin->v.netname), Speech);	


	CBaseEntity *pEntity = NULL;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		if(PlayerInfo[i].IsAdmin >= LEVEL_OP)
		{
			pEntity = UTIL_PlayerByIndex(i);

			if(pEntity)
			{
				ClientPrint( pEntity->pev, HUD_PRINTTALK, Text );
			}
		}
	}
}

void AdmForceWriteBack(edict_t *pAdmin)
{
	// --- AdmFirceWriteBack() - Forces AutoOP to commit all users data to disk.
	WriteBackFiles();
	UTIL_LogPrintf( "[AUTOOP] %s forced the server to save everyones credits.\n", STRING(pAdmin->v.netname));
}

void AdmClientPrint(edict_t *pAdmin, const char *User, const char *Message)
{
	// --- AdmClientPrint() - prints text to users com line

	char *PlayerInf = (char *)User;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pSend = INDEXENT(Index);

	ClientPrint( &pSend->v, HUD_PRINTTALK, UTIL_VarArgs( "%s\n",Message) );

	UTIL_LogPrintf( "[AUTOOP] %s printed to %s: %s\n", STRING(pAdmin->v.netname),STRING(pSend->v.netname),Message);
}

void AdmDisinfectPlayer(edict_t *pAdmin, const char *User)
{
	char *PlayerInf = (char *)User;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
		return;

	edict_t *pUser = INDEXENT(Index);

	if(pUser->v.playerclass == CLASS_MEDIC)
		return; //Medics cannot be infected.

	CBaseEntity *pSentryEnt = NULL;
	
	while ((pSentryEnt = UTIL_FindEntityByClassname(pSentryEnt, "timer")) != NULL)
	{
		if(pSentryEnt->pev->owner == pUser)
			pSentryEnt->pev->flags = FL_KILLME;
	}
}

void AdmChangeCreds(edict_t *pAdmin, const char *User, const char *Credits)
{
	// --- AdmChangeCreds() - Adds specified amount to players credit bank.

	int iCreds = atoi(Credits);

	if(iCreds == 0)
		return;

	char *PlayerInf = (char *)User;
	int Index = GetPartialPlayerIndex(PlayerInf,ENTINDEX(pAdmin));

	if(Index == 0)
	{
		CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "Player specified cannot be found.\n"));
		return;
	}

	edict_t *pCred = INDEXENT(Index);
	
	PlayerInfo[Index].LoadedInfo.Credits += iCreds;

	CLIENT_PRINTF( pCred, print_center, "An admin changed your credits\n");

	UTIL_LogPrintf( "[AUTOOP] %s added %d to %s's credits.\n", STRING(pAdmin->v.netname),iCreds,STRING(pCred->v.netname));
}

void AdmAdminList(edict_t *pAdmin)
{
	// --- AdmAdminList() - Creates an admin list including stealth players.
	edict_t *pAdmin2 = NULL;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
		if(PlayerInfo[i].LoadedInfo.flag2 > 0)
		{
			pAdmin2 = INDEXENT(i);
			if(PlayerInfo[i].NoGreen == 1)
			{
				CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s as %s (Level %d) (Stealth)\n",PlayerInfo[i].OrigName,STRING(pAdmin2->v.netname),PlayerInfo[i].LoadedInfo.flag2));
			} else {
				CLIENT_PRINTF( pAdmin, print_console, UTIL_VarArgs( "%s as %s (Level %d)\n",PlayerInfo[i].OrigName,STRING(pAdmin2->v.netname),PlayerInfo[i].LoadedInfo.flag2));
			}
		}
	}
}

void AdmExtendMap(edict_t *pAdmin)
{
	// --- AdmExtendMap() - Extends map for 30 minutes and tells everyone who did it.

	BotTalkAllArgs("Admin %s extended the time.\n",STRING(pAdmin->v.netname));
	
	int iTime = (int)CVAR_GET_FLOAT("mp_timelimit");
	iTime = iTime + 30;

	char TimeCommand[BUF_SIZE];
	_snprintf(TimeCommand,99,"mp_timelimit %d\n", iTime);

	SERVER_COMMAND(TimeCommand);

	UTIL_LogPrintf("[AUTOOP] Admin %s extended the map.\n",STRING(pAdmin->v.netname));
}

void AdmICQ(edict_t *pAdmin, const char *Message)
{
	char From[128];
	strcpy(From,"Message from ");
	strcat(From,STRING(pAdmin->v.netname));

	SendStandardICQ("ALL",From,(char *)Message);
}















	




	



