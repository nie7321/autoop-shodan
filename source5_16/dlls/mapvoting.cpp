/*****************************************************\
*	Auto-OP Shodan
*
*	File: mapvoting.cpp
*	Description: Modules for map voting system.
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
#include "voting.h"
#include "stripper.h"
#include "userstrings.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;
extern DLL_FUNCTIONS other_gFunctionTable;

extern  DLL_GLOBAL mapcycle_t mapcycle;

MapVoteItem_s VotedMaps[MAX_MAPS];
extern int UsedSlots;
extern float MapLastCalled;
extern int MapVoteStat;
extern float MapVoteLast;

extern float MapChange;
extern char *NextMapChange;

int CheckMap(const char *MapName2)
{
	//Cycle through looking for a match
	for( int i = 0; i <= MAX_MAPS; i++)
	{
		if(VotedMaps[i].MapName != NULL)
			if(_stricmp(MapName2,VotedMaps[i].MapName) == 0)
				return i;
	}

	//No match so create a slot for this new map if we can
	if(UsedSlots < MAX_MAPS)
	{
		UsedSlots += 1;

		strcpy(VotedMaps[UsedSlots].MapName, MapName2);

		return UsedSlots;
	} else {
		return -1; //-1 means no more maps accepted
	}

}

void VoteCast(edict_t *pEntity, const char *Speech)
{
	if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanVote == 1)
		return; //This user cannot vote

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Mapvote - VoteCast() : Mapvote was cast by player %d.\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	char MapName[BUF_SIZE];
	int Length = strlen(Speech);

	//Remove "vote " from the beginning of the string.
	for( int i=5; i<Length+1; i++)
		MapName[i-5] = Speech[i];
	MapName[i-5] = NULL;

	_strupr(MapName);

	if ( !IS_MAP_VALID(MapName) && _stricmp( MapName, "extend" ) != 0 ) {
		BotTalkAllArgs(MV_NOTFOUND,MapName);
		return;
	}

	int Result = CheckMap(MapName);

	if(Result == -1)
	{
		BotTalkAllArgs(MV_MAXED);
	} else {

		if(PlayerInfo[ENTINDEX(pEntity)].VotedMapIndex != 0)
		{
			VotedMaps[PlayerInfo[ENTINDEX(pEntity)].VotedMapIndex].Votes -= 1;
			UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs(MV_REMOVEDVOTE,VotedMaps[PlayerInfo[ENTINDEX(pEntity)].VotedMapIndex].MapName));
		}

		VotedMaps[Result].Votes += 1;

		PlayerInfo[ENTINDEX(pEntity)].VotedMapIndex = Result;

		BotTalkAllArgs(MV_CASTVOTE,STRING(pEntity->v.netname),VotedMaps[Result].MapName,VotedMaps[Result].Votes);
		UTIL_LogPrintf("[AUTOOP] Vote cast by %s, %s now has %d vote(s).\n",STRING(pEntity->v.netname),VotedMaps[Result].MapName,VotedMaps[Result].Votes);

	}
}

void VoteStart(edict_t *pCaller)
{
	if(pCaller != NULL)
	{
		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "Mapvote - VoteStart() : Mapvote was started by player %d.\n",ENTINDEX(pCaller));
			fclose(fp);
		}

		int i;

		if(((gpGlobals->time >= MapVoteLast) || PlayerInfo[ENTINDEX(pCaller)].IsAdmin > 1) && MapVoteStat == 0) 
		{
			//Reset all the votes and names
			for( i = 0; i <= MAX_MAPS; i++)
			{
				VotedMaps[i].MapName[0] = NULL;
				VotedMaps[i].Votes = 0;
			}

			//Reset everyones voted map variable.
			for( i = 0; i <= gpGlobals->maxClients; i++)
			{
				PlayerInfo[i].VotedMapIndex = 0;
			}

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
			m_textParms.y = 0.3;
			m_textParms.effect= 2;
			m_textParms.channel=2;
			char Text[256];
			_snprintf(Text,256,"Shodan Map Voting system enabled for 3 minutes\nSay \"vote (mapname)\" to vote i.e \"vote 2fort\"");	
			UTIL_HudMessageAll( m_textParms,Text );

			UTIL_LogPrintf( "[AUTOOP] %s started a map vote\n", STRING(pCaller->v.netname));

			MapVoteLast = gpGlobals->time + 600;
			MapLastCalled = gpGlobals->time;// + 180;
			UsedSlots = 0;
			MapVoteStat = 1;

		} else {
			
			BotTalkAllArgs(MV_CANTSTART);
		}
	} else {
		int i;

		if((gpGlobals->time >= MapVoteLast) && (MapVoteStat == 0)) 
		{
			//Reset all the votes and names
			for( i = 0; i <= MAX_MAPS; i++)
			{
				VotedMaps[i].MapName[0] = NULL;
				VotedMaps[i].Votes = 0;
			}

			//Reset everyones voted map variable.
			for( i = 0; i <= gpGlobals->maxClients; i++)
			{
				PlayerInfo[i].VotedMapIndex = 0;
			}

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
			m_textParms.y = 0.3;
			m_textParms.effect= 2;
			m_textParms.channel=2;
			char Text[256];
			_snprintf(Text,256,"Shodan Map Voting system enabled for 3 minutes\nSay \"vote (mapname)\" to vote i.e \"vote 2fort\"");	
			UTIL_HudMessageAll( m_textParms,Text );

			MapVoteLast = gpGlobals->time + 600;
			MapLastCalled = gpGlobals->time;// + 180;
			UsedSlots = 0;
			MapVoteStat = 1;
		}
	}
}

void VoteEnd( )
{

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Mapvote - VoteEnd() : Mapvote ended.\n");
		fclose(fp);
	}

	UTIL_LogPrintf( "[AUTOOP] Map vote ended.\n");

	MapVoteStat = 0;

	if(UsedSlots == 0)
	{
		BotTalkAllArgs(MV_NOVOTES);
		UTIL_LogPrintf("[AUTOOP] No map got enough votes to win.\n");
		return;
	}

	int WinnerIndex = 0;
	int WinningVotes = 0;

	//Find winning map
	for( int i = 0; i <= UsedSlots + 1; i++)
	{
		if(VotedMaps[i].Votes > WinningVotes)
		{
			WinningVotes = VotedMaps[i].Votes;
			WinnerIndex = i;
		}
	}

	//Get 50 percent of total people on server
	int PlayerCount;
	int VotesNeeded;

	PlayerCount = 0;
	for ( i = 1; i <= gpGlobals->maxClients; i++ ) {
		CBaseEntity *pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer) 
			if(!FStrEq(STRING(pPlayer->pev->netname),""))
				PlayerCount++;
	}
	
	VotesNeeded = PlayerCount * Features.VoteRatio / 100;
	if(WinningVotes < VotesNeeded)
		WinningVotes = 0; //Vote failed ratio test.

	if(WinningVotes > 0 && WinnerIndex > 0)
	{
		if(_stricmp( VotedMaps[WinnerIndex].MapName, "extend" ) == 0)
		{
			BotTalkAllArgs(MV_EXTENDWIN);
			
			int iTime = (int)CVAR_GET_FLOAT("mp_timelimit");
			iTime = iTime + 30;

			char TimeCommand[BUF_SIZE];
			_snprintf(TimeCommand,99,"mp_timelimit %d\n", iTime);

			SERVER_COMMAND(TimeCommand);

			MapVoteStat = 0;
			return;
		}
		
		BotTalkAllArgs(MV_WINNER,VotedMaps[WinnerIndex].MapName,VotedMaps[WinnerIndex].Votes);

		SERVER_COMMAND("mp_timelimit 0.1\n");
		
		NextMapChange = VotedMaps[WinnerIndex].MapName;
		MapChange = gpGlobals->time + 2;

		MapVoteStat = 0; //We dont want any more map votes starting.

		//CHANGE_LEVEL( VotedMaps[WinnerIndex].MapName, NULL );
	} else {
		UTIL_LogPrintf("[AUTOOP] No map got the %d votes needed to win.\n",VotesNeeded);
		BotTalkAllArgs(MV_NOWINNER,VotesNeeded);
		return;
	}
}

void CancelMapVote( )
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Mapvote - CancelMapVote() : Mapvote was cancelled.\n");
		fclose(fp);
	}

	MapVoteStat = 0;

	BotTalkAllArgs(MV_CANCELLED);
	UTIL_LogPrintf( "[AUTOOP] Mapvote cancelled\n");
}

void SayWinningMap( )
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Mapvote - SayWinningMap() : Called.\n");
		fclose(fp);
	}

	int WinnerIndex = 0;
	int WinningVotes = 0;
	int i;

	if(UsedSlots == 0)
	{
		BotTalkAllArgs("No map has enough votes to win yet.\n");
		return;
	}

	//Find winning map
	for( i = 0; i <= UsedSlots + 1; i++)
	{
		if(VotedMaps[i].Votes > WinningVotes)
		{
			WinningVotes = VotedMaps[i].Votes;
			WinnerIndex = i;
		}
	}

	//Get 50 percent of total people on server
	int PlayerCount;
	int VotesNeeded;

	PlayerCount = 0;
	for ( i = 1; i <= gpGlobals->maxClients; i++ ) {
		CBaseEntity *pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer) 
			if(!FStrEq(STRING(pPlayer->pev->netname),""))
				PlayerCount++;
	}
	
	VotesNeeded = PlayerCount * Features.VoteRatio / 100;
	if(WinningVotes < VotesNeeded)
		WinningVotes = 0; //Vote failed ratio test.

	if(WinningVotes > 0 && WinnerIndex > 0)
	{
		if(_stricmp( VotedMaps[WinnerIndex].MapName, "extend" ) == 0)
		{
			BotTalkAllArgs(MV_EXTENDLEAD);
			return;
		}

		BotTalkAllArgs(MV_WINNER,VotedMaps[WinnerIndex].MapName,VotedMaps[WinnerIndex].Votes);

		UTIL_LogPrintf( "[AUTOOP] Winning map is %s with %d vote(s).\n",VotedMaps[WinnerIndex].MapName,VotedMaps[WinnerIndex].Votes);


	} else {

		BotTalkAllArgs(MV_NOTENOUGHVOTES,VotesNeeded);

		UTIL_LogPrintf( "[AUTOOP] No map has the %d votes needed to win yet\n",VotesNeeded);
		return;
	}
}

void SayNextMap( )
{
	mapcycle_item_s *item;
	char *mapcfile = (char*)CVAR_GET_STRING( "mapcyclefile" );
	DestroyMapCycle( &mapcycle );
	ReloadMapCycleFile( mapcfile, &mapcycle );
	item=CurrentMap(&mapcycle);

	BotTalkAllArgs(NEXTMAP_STRING,item->mapname);
}