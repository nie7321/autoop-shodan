/*****************************************************\
*	Auto-OP Shodan
*
*	File: kickvote.cpp
*	Description: Modules for kickvote.
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
extern int VoteStatus;
extern float LastCalled;
extern int iKickUser;

void CastKickVote(edict_t *pVoter, const char *VoteSpeech)
{
	if(FStrEq(VoteSpeech, "yes"))
	{
		if(PlayerInfo[ENTINDEX(pVoter)].LoadedInfo.CanVote == 0)
		{
			PlayerInfo[ENTINDEX(pVoter)].KickVote = 1;
			edict_t *pKickUser = INDEXENT(iKickUser);
			BotTalkAllArgs(KV_VOTEYES,STRING(pVoter->v.netname),STRING(pKickUser->v.netname));
		}
	}

	//NO
	if(FStrEq(VoteSpeech, "no"))
	{
		if(PlayerInfo[ENTINDEX(pVoter)].LoadedInfo.CanVote == 0)
		{
			edict_t *pKickUser = INDEXENT(iKickUser);
			PlayerInfo[ENTINDEX(pVoter)].KickVote = 2;
			BotTalkAllArgs(KV_VOTENO,STRING(pVoter->v.netname),STRING(pKickUser->v.netname));
		}
	}
}

void EndTheVote()
{
	BotTalkAllArgs(KV_VOTEEND);

	VoteStatus = 0; //Stop taking more votes in

	edict_t *pKickUser = INDEXENT(iKickUser);
	
	int Yes = 0;
	int No = 0;
	
	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) 
	{
		if(PlayerInfo[i].KickVote == 1)
			Yes += 1;

		if(PlayerInfo[i].KickVote == 2)
			No += 1;

		PlayerInfo[i].KickVote = 0; //Clear the persons vote for next time.
	}

	if(Yes == 1 && No != 1)
		BotTalkAllArgs("%d person voted yes, %d people voted no.\n",Yes,No);

	if(Yes != 1 && No == 1)
		BotTalkAllArgs("%d people voted yes, %d person voted no.\n",Yes,No);

	if(Yes == 1 && No == 1)
		BotTalkAllArgs("%d person voted yes, %d person voted no.\n",Yes,No);

	if(Yes != 1 && No != 1)
		BotTalkAllArgs("%d people voted yes, %d people voted no.\n",Yes,No);

	if(Yes > No)
	{
		CLIENT_PRINTF( pKickUser, print_console, UTIL_VarArgs( "\n\n\n*************************\n"));
		CLIENT_PRINTF( pKickUser, print_console, UTIL_VarArgs( "You were kicked for 5 minutes due to a kickvote\n"));
		CLIENT_PRINTF( pKickUser, print_console, UTIL_VarArgs( "Contact the server owner for details.\n"));
		CLIENT_PRINTF( pKickUser, print_console, UTIL_VarArgs( "*************************\n"));

		char szCommand[256];
		_snprintf(szCommand,256,"banid 5 %s\n",GETPLAYERAUTHID(pKickUser));
		SERVER_COMMAND(szCommand);
		SERVER_COMMAND("writeid\n");

		CLIENT_COMMAND ( pKickUser, UTIL_VarArgs("disconnect\n") );
	}
	iKickUser = 0;
}

