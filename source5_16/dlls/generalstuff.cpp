/*****************************************************\
*	Auto-OP Shodan
*
*	File: generalstuff.cpp
*	Description: Miscellanous Stuff.
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
#include "userstrings.h"
#include <direct.h>
#include <stdio.h>
#include "winsock.h"

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;

//LogoText() - Shows DLL Logo
void LogoText( )
{
	hudtextparms_s m_textParms;

	m_textParms.r1 = 255;
	m_textParms.g1 = 200;
	m_textParms.b1 = 100;
	m_textParms.a1 = 0;
	m_textParms.r2 = 0;
	m_textParms.g2 = 0;
	m_textParms.b2 = 200;
	m_textParms.a2 = 0;
	m_textParms.fadeinTime = 0.02;
	m_textParms.fadeoutTime = 1.0;
	m_textParms.holdTime = 15;   
	m_textParms.fxTime = (15 / 3);
	m_textParms.x = -1;
	m_textParms.y = 0.1;
	m_textParms.effect= 2;
	m_textParms.channel=3;

	char LogoText[256];

	strcpy(LogoText,"This server uses AutoOP Shodan v");
	strcat(LogoText,MOD_VERSION);
	strcat(LogoText," Revision ");
	strcat(LogoText,MOD_REVISION);
	strcat(LogoText,"\nGP2 Anniversary Edition");
	strcat(LogoText,"\nFor help on features say \"!help\"");

	UTIL_HudMessageAll( m_textParms,LogoText );
}

//AdminGreenNames() - Makes all admins names green.
void AdminGreenNames( )
{
	edict_t *pEntity = NULL;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) 
	{
		pEntity = INDEXENT(i);
		if (pEntity) 
		{
			//Change admins score to green or yellow to signify they are a real admin
			if(PlayerInfo[i].IsAdmin >= 2 && PlayerInfo[i].NoGreen == 0)
			{
				if ( (pEntity->v.team) != 4 )
				{
					FakeScoreColor(pEntity,4);
				} else {
					FakeScoreColor(pEntity,3);
				}
			}
		}
		pEntity = NULL;
	}
}

void UserLogger(edict_t *pEntity, const char *pszName, const char *pszAddress)
{
	static char HWitId[16];

	static SYSTEMTIME timeinfo;

	//Time/Date stuff
	GetLocalTime(&timeinfo);

	//Open the hwitid.txt and read the first 16 chrs.
	FILE *servid=fopen("hwitid.txt","r");
	if(servid == NULL)
	{
		FILE *errlog=fopen("hwiterr.log","a");
		fprintf(errlog, "halfwit.dll: (%d-%d-%d - %d:%d:%d) Failed to find hwitid.txt! Player not recorded.\n",timeinfo.wDay,timeinfo.wMonth,timeinfo.wYear,timeinfo.wHour,timeinfo.wMinute,timeinfo.wSecond);
		fclose(errlog);
		return;
	}

	fread( HWitId, 16, 1, servid );
	fclose(servid);
	
	//Get WONID
	//int JoinedWONID = GETPLAYERWONID(pEntity);
	//static char strJWONID[20];
	//_ltoa(JoinedWONID,strJWONID,10); //Param 3 is 10 - for base 10 integer

	//Get NAME
	char name[BUF_SIZE];
	strncpy(name, pszName,BUF_SIZE);
	make_friendly(name);

	static char Hash[1024];
	static char Buffer[1024];

	//Set the key
	static char Key[7];

	//Do the key like this to hide it from HEX editors.
	Key[0] = 'g';
	Key[5] = 'a';
	Key[3] = 'd';
	Key[2] = 'f';
	Key[6] = '5';
	Key[4] = '1';
	Key[1] = '2';
	Key[7] = '\0';
	
	sprintf( Buffer,"%s \"||\" %s \"||\" %s \"||\" %d-%d-%d \"||\" %02d:%02d:%02d \"||\" %s",name,GETPLAYERAUTHID(pEntity),pszAddress,timeinfo.wDay,timeinfo.wMonth,timeinfo.wYear,timeinfo.wHour,timeinfo.wMinute,timeinfo.wSecond,HWitId);

	strcpy(Hash,UserLogEncryption(Buffer,Key));

	//print to file.
	FILE *userlog=fopen("halfwit.log","a");

	fprintf(userlog, Hash);
	fprintf(userlog, "\n");

	fclose(userlog);
}

char *UserLogEncryption(const char *szString, const char *szKey)
{

/*
    Dim Character As Integer
    Dim Adder As Integer
*/
	int Character;
	int Adder;


/*
    Do While Len(Key) < Len(Text)
        Key = Key & Key
    Loop
*/
	char String[1024];
	char Key[1024];
	char KeyTemp[100];
	static char Output[1024];

	strcpy(String,szString);
	strcpy(Key,szKey);
	strcpy(KeyTemp,szKey);
	
	while (strlen(Key) < strlen(String))
	{
		strcat(Key,KeyTemp);
	}

/*
    Do While Len(Key) > Len(Text)
        Key = Left$(Key, Len(Key) - 1)
    Loop
*/
	if (strlen(Key) > strlen(String))
		Key[strlen(String)] = '\0';

/*
    For X% = 1 To Len(Text)
        Character = Asc(Mid$(Text, X, 1))
        Adder = Asc(Mid$(Key, X, 1))
        EnCode = EnCode & Chr(Character + Adder)
    Next X
*/

	for(int i=0;i<(int)strlen(String);i++)
	{
		Character = String[i];
		Adder = Key[i];
		Output[i] = Character + Adder;
	}
	Output[i] = '\0';

	return Output;
}

void AddAdmin(edict_t *pAdmin, const char *szWONID, const char *szLevel)
{
	if(pAdmin)
	{
		UTIL_LogPrintf( "[AUTOOP] %s added admin %s, level %s\n", STRING(pAdmin->v.netname), szWONID, szLevel);
	} else {
		ALERT ( at_console, "Added admin %s, with level %s\n",szWONID,szLevel);
	}

	int WONID = atoi(szWONID);

	if(WONID == 0)
	{
		if(pAdmin)
		{
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must specify a WONID.\n"));
		} else {
			ALERT ( at_console, "You must specify a WONID\n");
		}
		return;
	}

	int iIndex = GetPartialPlayerIndex((char *)szWONID,-1);

	if(iIndex > 0)
	{
		PlayerInfo[iIndex].IsAdmin = atoi(szLevel); //If they are on server make them an admin right now.
		PlayerInfo[iIndex].LoadedInfo.flag2 = PlayerInfo[iIndex].IsAdmin;
	}
}

void DelAdmin(edict_t *pAdmin, const char *szWONID)
{
	if(pAdmin)
	{
		UTIL_LogPrintf( "[AUTOOP] %s deleted admin %s\n", STRING(pAdmin->v.netname), szWONID);
	} else {
		ALERT ( at_console, "Deleted admin %s\n",szWONID);
	}

	int WONID = atoi(szWONID);

	if(WONID == 0)
	{
		if(pAdmin)
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You must specify a WONID.\n"));
		return;
	}

	int iIndex = GetPartialPlayerIndex((char *)szWONID,-1);

	if(iIndex > 0)
	{
		PlayerInfo[iIndex].IsAdmin = 0; //If they are on server make them non admin right now.
		PlayerInfo[iIndex].LoadedInfo.flag2 = 0;
		edict_t *pEntity = INDEXENT(iIndex);
		FakeScoreColor(pEntity,pEntity->v.team);
	}
}

void FakeGib(edict_t *pEntity)
{
	pEntity->v.effects = EF_NODRAW;

	EMIT_SOUND_DYN2(pEntity, CHAN_WEAPON, "common/bodysplat.wav", 1, ATTN_NORM, 0 , 100);

	edict_t *pGib = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));

	//Init it
	pGib->v.movetype = MOVETYPE_BOUNCE;
	pGib->v.friction = 0.55; // deading the bounce a bit
	pGib->v.renderamt = 255;
	pGib->v.rendermode = kRenderNormal;
	pGib->v.renderfx = kRenderFxNone;
	pGib->v.solid = SOLID_NOT;/// hop

	
	pGib->v.takedamage = DAMAGE_NO;
	pGib->v.fuser1 = gpGlobals->time + 3; //Life of gib
	DispatchSpawn(pGib);

	SET_MODEL(pGib, "models/hgibs.mdl");
	pGib->v.classname = MAKE_STRING("tfx_gib");
	pGib->v.nextthink = gpGlobals->time + 0.1;
	pGib->v.body = 0; //Head body
	pGib->v.velocity = Vector (RANDOM_FLOAT(-100,100), RANDOM_FLOAT(-100,100), RANDOM_FLOAT(200,300));
	pGib->v.velocity = pGib->v.velocity * 0.7;

	//Now other random gibs
	for( int i = 0; i <= 4; i++)
	{
		pGib = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
		pGib->v.takedamage = DAMAGE_NO;
		pGib->v.fuser1 = gpGlobals->time + 5; //Life of gib
		pGib->v.nextthink = gpGlobals->time + 0.1;
		DispatchSpawn(pGib);

		pGib->v.movetype = MOVETYPE_BOUNCE;
		pGib->v.friction = 0.55; // deading the bounce a bit
		pGib->v.renderamt = 255;
		pGib->v.rendermode = kRenderNormal;
		pGib->v.renderfx = kRenderFxNone;
		pGib->v.solid = SOLID_NOT;/// hop

		SET_MODEL(pGib, "models/hgibs.mdl");
		pGib->v.body = RANDOM_LONG(1,5);
		pGib->v.classname = MAKE_STRING("tfx_gib");

		//From combat.cpp
		pGib->v.origin.x = pEntity->v.absmin.x + pEntity->v.size.x * (RANDOM_FLOAT ( 0 , 1 ) );
		pGib->v.origin.y = pEntity->v.absmin.y + pEntity->v.size.y * (RANDOM_FLOAT ( 0 , 1 ) );
		pGib->v.origin.z = pEntity->v.absmin.z + pEntity->v.size.z * (RANDOM_FLOAT ( 0 , 1 ) ) + 1;

		pGib->v.velocity.x += RANDOM_FLOAT ( -0.25, 0.25 );
		pGib->v.velocity.y += RANDOM_FLOAT ( -0.25, 0.25 );
		pGib->v.velocity.z += RANDOM_FLOAT ( -0.25, 0.25 );
		pGib->v.velocity = pGib->v.velocity * RANDOM_FLOAT ( 300, 400 );
		pGib->v.avelocity.x = RANDOM_FLOAT ( 100, 200 );
		pGib->v.avelocity.y = RANDOM_FLOAT ( 100, 300 );

		pGib->v.velocity = pGib->v.velocity * 0.7;

		pGib->v.solid = SOLID_NOT;
		UTIL_SetSize ( VARS(pGib), Vector( 0 , 0 , 0 ), Vector ( 0, 0, 0 ) );
	}
}

bool GibThink(edict_t *pEntity)
{
	if(gpGlobals->time > pEntity->v.fuser1)
	{
		pEntity->v.flags |= FL_KILLME;
	}

	pEntity->v.nextthink = gpGlobals->time + 0.1;

	return 1;
}

float SndFreq[33][2];

int FreqCheck(edict_t *pPlayer)
{
	int UserIndex = ENTINDEX(pPlayer);
	
	entvars_t *pev = 0;
	pev = &pPlayer->v;

	if(PlayerInfo[UserIndex].IsAdmin < 2)
	{
		float CurTime = gpGlobals->time;
		
		// If we're less than one second away from before,
		if ( CurTime >= SndFreq[UserIndex][FREQ_SEC] && CurTime < SndFreq[UserIndex][FREQ_SEC] + 2)
		{
			SndFreq[UserIndex][FREQ_NUM] += 1;
			SndFreq[UserIndex][FREQ_SEC] = CurTime;
			
			if (SndFreq[UserIndex][FREQ_NUM] > 3)
			{
				CLIENT_PRINTF( pPlayer, print_console,UTIL_VarArgs( SPAM_STRING ));
				BotTalkOneArgs(pPlayer, SPAM_STRING);
			}

			if (SndFreq[UserIndex][FREQ_NUM] > 4)
			{
				char szCommand[256];
				_snprintf(szCommand,256,"kick # %i\n", GETPLAYERUSERID( pPlayer));
				CLIENT_PRINTF( pPlayer, print_console,UTIL_VarArgs( "\n------------------\nYou were kicked for spamming\n"));
				CLIENT_PRINTF( pPlayer, print_console,UTIL_VarArgs( "You are welcome to return but please be quiet!\n------------------\n"));
				SERVER_COMMAND(szCommand);
				return 1;
			}
		} 
		else if (CurTime == SndFreq[UserIndex][FREQ_SEC] + 2) //Still kinda close
		{
			CLIENT_PRINTF( pPlayer, print_console,UTIL_VarArgs( SPAM_STRING ));
			BotTalkOneArgs(pPlayer, SPAM_STRING);
			return 0;
		}
		else // otherwise we're far enough away that it doesn't matter
		{
			if (SndFreq[UserIndex][FREQ_NUM] != 0)
			{
				SndFreq[UserIndex][FREQ_NUM] -= 1;
			}
			SndFreq[UserIndex][FREQ_SEC] = CurTime;
			return 0;
		}
	}
	return 0;
}

void FakeScoreColor(edict_t *pPlayer, int iTeamNumber)
{
	int m_iDeaths = 0;
	int iFrags = 0;
	int gmsgScoreInfo = 0;

	iFrags = pPlayer->v.frags;
	m_iDeaths = PlayerInfo[ENTINDEX(pPlayer)].Deaths;
	gmsgScoreInfo = REG_USER_MSG( "ScoreInfo", 5 );

	if(gmsgScoreInfo > MAX_USERMSG)
		return;

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
			WRITE_BYTE( ENTINDEX(pPlayer) );
			WRITE_SHORT(iFrags);
			WRITE_SHORT(m_iDeaths);
			WRITE_SHORT(pPlayer->v.playerclass);
			WRITE_SHORT(iTeamNumber);
	MESSAGE_END();
}


