/*****************************************************\
*	Auto-OP Shodan
*
*	File: credits.cpp
*	Description: Main credits modules & commands.
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
#include "credits.h"
#include "skins.h"
#include "stripper.h"
#include "3rdperson.h"
#include "userstrings.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;
extern DLL_FUNCTIONS other_gFunctionTable;

char *g_LevelNames[]={"None", "LowOP", "ServerOP", "PowerOP", "SuperOP", "Administrator"};

extern char LastSaid[];
extern char LastPlayer[];
extern char LastConnected[];

extern float SndFreq[][2];

extern int NightTime;

void TFXLoadPlayerInfo(edict_t *pEntity, const char *PlayerName)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXLoadPlayerInfo(): Initializing player %d.\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_rpg \"m\";alias tf_weapon_sniperrifle \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_flamethrower \"m\";alias tf_weapon_ac \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_gl \"m\";alias tf_weapon_pl \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_ic \"m\";alias tf_weapon_tranq \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_railgun \"m\";alias tf_weapon_superng \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_supershotgun \"m\";alias tf_weapon_autorifle \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tf_weapon_shotgun \"m\";alias tf_weapon_ng \"m\"\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias lastinv \"m\";r_drawviewmodel 1\n") );

	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("exec gp2_clientpack.cfg\n") );

	_mkdir( "tfc\\autoop" );
	_mkdir( "tfc\\autoop\\userinfo" );

	//Get Index value of player
	int JoinedIndex = ENTINDEX(pEntity);

	//Reset players admin value (safety)
	PlayerInfo[JoinedIndex].IsAdmin = LEVEL_NONE;
	
	int AllowCreds = (int)CVAR_GET_FLOAT("tfx_credits");

	if (AllowCreds == 1 )
	{	
		char UserFileName[200];
		strcpy(UserFileName, "tfc\\autoop\\userinfo\\");
		strcat(UserFileName,GETPLAYERAUTHID(pEntity));
		strcat(UserFileName, ".dat");

		//replace any colons ':' with an 'x'
		//i.e STEAM_0:123456 -> STEAM_0x123456
		for(int i = 0;i<(int)strlen(UserFileName);i++)
		{
			if(UserFileName[i] == ':')
				UserFileName[i] = 'x';
		}

		FILE *cUserFile = fopen(UserFileName, "rb");

			if(Features.Debug >= 1)
			{
				fp=fopen("sdebug.log","a");
				fprintf(fp, "22Reading user data from %s\n",UserFileName);
				fclose(fp);
			}

		if(cUserFile != NULL)
		{
			if(Features.Debug >= 1)
			{
				fp=fopen("sdebug.log","a");
				fprintf(fp, "Reading user data from %s\n",UserFileName);
				fclose(fp);
			}

			//Load Players Data
			TFXUserFileHeader_s Header;

			fread(&Header, sizeof(Header), 1, cUserFile);

			if(Header.Version > USERFILE_VERSION)
			{
				if(Features.Debug >= 1)
				{
					fp=fopen("sdebug.log","a");
					fprintf(fp, "LoadPlayer(): PLAYER DATAFILE VERSION MISMATCH!\n");
					fclose(fp);
				}

				return;
			}

			Header.OrigName[MAX_NAME_LENGTH-1] = 0;
			strcpy(PlayerInfo[JoinedIndex].OrigName,Header.OrigName);
			if(FStrEq(PlayerInfo[JoinedIndex].OrigName,""))
			{
				strcpy(PlayerInfo[JoinedIndex].OrigName,PlayerName);
			}

			//Support for Old userfile system.
			if(Header.Version == USERFILE_V2)
			{
				//LOAD VERSION 2 USERFILE

				TFXUserDataFile_s FileData;

				fread(&FileData, sizeof(FileData), 1, cUserFile);

				PlayerInfo[JoinedIndex].LoadedInfo.Credits = FileData.Credits;
				PlayerInfo[JoinedIndex].LoadedInfo.CanSpray = FileData.CanSpray;
				PlayerInfo[JoinedIndex].LoadedInfo.CanTalk = FileData.CanTalk;
				PlayerInfo[JoinedIndex].LoadedInfo.CanVote = FileData.CanVote;
				PlayerInfo[JoinedIndex].LoadedInfo.flag1 = FileData.flag1;
				PlayerInfo[JoinedIndex].LoadedInfo.flag2 = FileData.flag2;
				PlayerInfo[JoinedIndex].LoadedInfo.flag3 = FileData.flag3;
				PlayerInfo[JoinedIndex].LoadedInfo.flag4 = FileData.flag4;
				PlayerInfo[JoinedIndex].LoadedInfo.TimeLen = FileData.TimeLen;
				PlayerInfo[JoinedIndex].LoadedInfo.Warnings = FileData.Warnings;

				FileData.Name[MAX_NAME_LENGTH-1] = 0;
				strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Name,FileData.Name);
				if(FStrEq(PlayerInfo[JoinedIndex].LoadedInfo.Name,""))
					strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Name,PlayerName);

				fclose(cUserFile);
			} else if(Header.Version == USERFILE_VERSION) {
				//LOAD VERSION 3 USERFILE

				TFXUserDataFileV3_s FileData;

				fread(&FileData, sizeof(FileData), 1, cUserFile);

				PlayerInfo[JoinedIndex].LoadedInfo.Credits = FileData.Credits;
				PlayerInfo[JoinedIndex].LoadedInfo.CanSpray = FileData.CanSpray;
				PlayerInfo[JoinedIndex].LoadedInfo.CanTalk = FileData.CanTalk;
				PlayerInfo[JoinedIndex].LoadedInfo.CanVote = FileData.CanVote;
				PlayerInfo[JoinedIndex].LoadedInfo.TimeLen = FileData.TimeLen;
				PlayerInfo[JoinedIndex].LoadedInfo.Warnings = FileData.Warnings;
				PlayerInfo[JoinedIndex].LoadedInfo.flag1 = FileData.flag1;
				PlayerInfo[JoinedIndex].LoadedInfo.flag2 = FileData.flag2;
				PlayerInfo[JoinedIndex].LoadedInfo.flag3 = FileData.flag3;
				PlayerInfo[JoinedIndex].LoadedInfo.flag4 = FileData.flag4;
				PlayerInfo[JoinedIndex].LoadedInfo.flag5 = FileData.flag5;
				PlayerInfo[JoinedIndex].LoadedInfo.flag6 = FileData.flag6;
				PlayerInfo[JoinedIndex].LoadedInfo.flag7 = FileData.flag7;
				PlayerInfo[JoinedIndex].LoadedInfo.flag8 = FileData.flag8;
				PlayerInfo[JoinedIndex].LoadedInfo.flag9 = FileData.flag9;
				PlayerInfo[JoinedIndex].LoadedInfo.flag10 = FileData.flag10;

				FileData.Name[MAX_NAME_LENGTH-1] = 0;
				strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Name,FileData.Name);
				if(FStrEq(PlayerInfo[JoinedIndex].LoadedInfo.Name,""))
					strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Name,PlayerName);

				FileData.Char1[MAX_NAME_LENGTH-1] = 0;
				strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Char1,FileData.Char1);

				FileData.Char2[MAX_NAME_LENGTH-1] = 0;
				strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Char2,FileData.Char2);

				FileData.Char3[MAX_NAME_LENGTH-1] = 0;
				strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Char3,FileData.Char3);

				FileData.Char4[MAX_NAME_LENGTH-1] = 0;
				strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Char4,FileData.Char4);

				fclose(cUserFile);
			}

			if(PlayerInfo[JoinedIndex].LoadedInfo.TimeLen >= 32400000)
			{
				//If greater than 9000 hours default to 0 hours
				PlayerInfo[JoinedIndex].LoadedInfo.TimeLen = 0;
			}

		} else {
			//No userfile, so create one.
			strcpy(PlayerInfo[JoinedIndex].LoadedInfo.Name,PlayerName);
			strcpy(PlayerInfo[JoinedIndex].OrigName,PlayerName);
		}
	}

	if(PlayerInfo[JoinedIndex].LoadedInfo.flag1 == 0)
	{
		AdminAuth(pEntity,1); //Auth player as server op
		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "TFXLoadPlayerInfo(): Attempted to authenticate.\n");
			fclose(fp);
		}
	} else {
		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "TFXLoadPlayerInfo(): Player's flag1 is not 0, authing skipped.\n");
			fclose(fp);
		}
	}

	
	if(PlayerInfo[JoinedIndex].IsAdmin > LEVEL_NONE)
	{
		//Adminstealth check
		extern enginefuncs_t  g_engfuncs;
		char *infobuffer=g_engfuncs.pfnGetInfoKeyBuffer(pEntity);
		char *nogreen=g_engfuncs.pfnInfoKeyValue( infobuffer, "_adminstealth");
		char *nologin=g_engfuncs.pfnInfoKeyValue( infobuffer, "_nologin");
		
		if(PlayerInfo[JoinedIndex].IsAdmin >= LEVEL_LOW)
		{
			if(strcmp(nogreen,"1") == 0) //If it is, set their green variable to 1.
			{
				if(Features.Debug >= 1)
				{
					fp=fopen("sdebug.log","a");
					fprintf(fp, "Player using admin stealth\n");
					fclose(fp);
				}
				PlayerInfo[JoinedIndex].NoGreen = 1;

				//Give the player a random fake credits & time value
				PlayerInfo[JoinedIndex].FakeCreds = (int)RANDOM_LONG( 0, 300 );
				PlayerInfo[JoinedIndex].FakeTime = RANDOM_LONG( 0, 360000 );

			}
		}

		if(strcmp(nologin,"1") == 0) //If it is, set their green variable to 1.
		{
			if(Features.Debug >= 1)
			{
				fp=fopen("sdebug.log","a");
				fprintf(fp, "Player using nologin\n");
				fclose(fp);
			}
			PlayerInfo[JoinedIndex].IsAdmin = LEVEL_NONE;
		}
	}

}

void TFXResetPlayerInfo(edict_t *pEntity)
{
	int i;
	int JoinedIndex = ENTINDEX(pEntity);
	
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXResetPlayerInfo(): Resetting player info on player %d\n", JoinedIndex);
		fclose(fp);
	}

	PlayerInfo[JoinedIndex].GotFrags = 0;
	PlayerInfo[JoinedIndex].NeedFrags = CREDIT_GAP;
	PlayerInfo[JoinedIndex].Deaths = 0;
	
	PlayerInfo[JoinedIndex].LoadedInfo.Credits = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.TimeLen = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.CanSpray = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.CanTalk = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.CanVote = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag1 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag2 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag3 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag4 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag5 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag6 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag7 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag8 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag9 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.flag10 = 0;
	PlayerInfo[JoinedIndex].LoadedInfo.Warnings = 0;
	memset(PlayerInfo[JoinedIndex].LoadedInfo.Name, 0, sizeof(PlayerInfo[JoinedIndex].LoadedInfo.Name));
	memset(PlayerInfo[JoinedIndex].LoadedInfo.Char1, 0, sizeof(PlayerInfo[JoinedIndex].LoadedInfo.Char1));
	memset(PlayerInfo[JoinedIndex].LoadedInfo.Char2, 0, sizeof(PlayerInfo[JoinedIndex].LoadedInfo.Char2));
	memset(PlayerInfo[JoinedIndex].LoadedInfo.Char3, 0, sizeof(PlayerInfo[JoinedIndex].LoadedInfo.Char3));
	memset(PlayerInfo[JoinedIndex].LoadedInfo.Char4, 0, sizeof(PlayerInfo[JoinedIndex].LoadedInfo.Char4));

	PlayerInfo[JoinedIndex].OrigName[0] = 0;
	memset(PlayerInfo[JoinedIndex].OrigName, 0, sizeof(PlayerInfo[JoinedIndex].OrigName));

	PlayerInfo[JoinedIndex].FBAmmo = 2;
	PlayerInfo[JoinedIndex].FBTime = 0;
	PlayerInfo[JoinedIndex].IconTime = 0;

	PlayerInfo[JoinedIndex].RemoveDisguise = 0.0;

	PlayerInfo[JoinedIndex].GrapStat = 0;
	PlayerInfo[JoinedIndex].GrapOrig = Vector(0,0,0);
	PlayerInfo[JoinedIndex].GrapTime = 0.0;
	PlayerInfo[JoinedIndex].LineTime = 0.0;
	PlayerInfo[JoinedIndex].HangTime = 0.0;

	PlayerInfo[JoinedIndex].HasFencePost = 0;
	PlayerInfo[JoinedIndex].LastBuilt = 0.0;
	PlayerInfo[JoinedIndex].CantChangeTeam = 0.0;
	PlayerInfo[JoinedIndex].FlashLightOff = 0;
	PlayerInfo[JoinedIndex].PlacedPoint = 0.0;
	PlayerInfo[JoinedIndex].KickVote = 0;
	PlayerInfo[JoinedIndex].StartedAVote = 0;
	PlayerInfo[JoinedIndex].HasBigGun = 0;

	PlayerInfo[JoinedIndex].ProxMineAmmo = PROX_MINE_AMMO;
	PlayerInfo[JoinedIndex].TripMineAmmo = TRIPMINE_AMMO;
	PlayerInfo[JoinedIndex].C4Ammo = C4_AMMO;
	PlayerInfo[JoinedIndex].PackBombAmmo = 2;
	PlayerInfo[JoinedIndex].SnarkAmmo = SNARK_AMMO;
	PlayerInfo[JoinedIndex].SnarkLastThrow = 0.0;
	PlayerInfo[JoinedIndex].DualBarLastSwing = 0;
	PlayerInfo[JoinedIndex].IsUsingTurret = 0;
	PlayerInfo[JoinedIndex].LastSuckTime = 0.0;
	PlayerInfo[JoinedIndex].IsSucking = 0;
	PlayerInfo[JoinedIndex].LastMovedFlag = 0.0;
	PlayerInfo[JoinedIndex].StealthOn = 0;

	PlayerInfo[JoinedIndex].Selected.LockX = 0;
	PlayerInfo[JoinedIndex].Selected.LockY = 0;
	PlayerInfo[JoinedIndex].Selected.LockZ = 0;

	PlayerInfo[JoinedIndex].CustomSpawnPoint = Vector(0,0,0);

	PlayerInfo[JoinedIndex].MovingEnt = 0;

	PlayerInfo[JoinedIndex].PathBuild.Drawingpath = 0;
	PlayerInfo[JoinedIndex].PathBuild.NextDraw = 0;
	PlayerInfo[JoinedIndex].PathBuild.NodeCount = 0;

	PlayerInfo[JoinedIndex].AdminChat = 0;

	SndFreq[JoinedIndex][FREQ_NUM] = 0.0;
	SndFreq[JoinedIndex][FREQ_SEC] = 0.0;

	for (i = 1; i <= MAX_ENTS; i++)
	{
		PlayerInfo[JoinedIndex].Selected.SelectedEnts[i] = 0;
	}

	PlayerInfo[JoinedIndex].VotedMapIndex = 0;

	for (i = 1; i <= MAX_CUST_GUNS; i++)
	{
		PlayerInfo[JoinedIndex].LastUsedEnergy[i] = 0;
	}

	PlayerInfo[JoinedIndex].SwitchingToCustomGun = 0;
	PlayerInfo[JoinedIndex].CurrentWeapon = 0;

	PlayerInfo[JoinedIndex].HasDied = 0;
	PlayerInfo[JoinedIndex].InvulnEnd = 0.0;

	PlayerInfo[JoinedIndex].VisualEffect = 0;
	PlayerInfo[JoinedIndex].EffectNextThink = 0.0;
	PlayerInfo[JoinedIndex].VisualCounter = 0.0;
	PlayerInfo[JoinedIndex].VisualThinkDelay = 0.0;
	PlayerInfo[JoinedIndex].IsSteamer = 0;
	memset(PlayerInfo[JoinedIndex].IPAddress, 0, sizeof(PlayerInfo[JoinedIndex].IPAddress));

	PlayerInfo[JoinedIndex].PMBlock = 0;
	memset(PlayerInfo[JoinedIndex].LastStatusText, 0, sizeof(PlayerInfo[JoinedIndex].LastStatusText));
	PlayerInfo[JoinedIndex].StatusReset = 0.0;

	PlayerInfo[JoinedIndex].MenuStatus = 0;
	PlayerInfo[JoinedIndex].AbilityOverride = 0;

	memset(PlayerInfo[JoinedIndex].GaggedName, 0, sizeof(PlayerInfo[JoinedIndex].GaggedName));

	PlayerInfo[JoinedIndex].TimerActive = 0;
	PlayerInfo[JoinedIndex].LastMovedMessage = 0.0;

	memset(PlayerInfo[JoinedIndex].ClientPackVer, 0, sizeof(PlayerInfo[JoinedIndex].ClientPackVer));
	
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("tfc_newmodels 0\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("console 1\n") );
}

void TFXPlayerDisconnect(edict_t *pEntity)
{
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("tfc_newmodels 1\n") );
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("scr_connectmsg \"0\"\n") );

	int cdIndex = 0;
	cdIndex = ENTINDEX(pEntity);

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXPlayerDisconnect(): User on slot %d left the game.\n", cdIndex);
		fclose(fp);
	}

	int AllowCreds = (int)CVAR_GET_FLOAT("tfx_credits");

	if (AllowCreds == 1 )
	{
		char UserFileName[200];
		strcpy(UserFileName, "tfc\\autoop\\userinfo\\");
		strcat(UserFileName,GETPLAYERAUTHID(pEntity));
		strcat(UserFileName, ".dat");

		//replace any colons ':' with an 'x'
		//i.e STEAM_0x123456
		for(int i = 0;i<(int)strlen(UserFileName);i++)
		{
			if(UserFileName[i] == ':')
				UserFileName[i] = 'x';
		}

		FILE *cUserFile = fopen(UserFileName, "wb");

		TFXUserFileHeader_s FileHeader;

		//Write Version
		FileHeader.Version = USERFILE_VERSION;

		//Write Original Name
		memset(FileHeader.OrigName, 0, sizeof(FileHeader.OrigName));
		if(FStrEq(PlayerInfo[cdIndex].OrigName,""))
		{
			strncpy(FileHeader.OrigName, STRING(pEntity->v.netname), (MAX_NAME_LENGTH-1));
			
		} else {
			strncpy(FileHeader.OrigName, PlayerInfo[cdIndex].OrigName, (MAX_NAME_LENGTH-1));
		}

		FileHeader.OrigName[MAX_NAME_LENGTH-1] = 0;

		//Write header to disk
		fwrite(&FileHeader, sizeof(FileHeader), 1, cUserFile);

		TFXUserDataFileV3_s FileData;

		FileData.Credits = PlayerInfo[cdIndex].LoadedInfo.Credits;
		FileData.CanSpray = PlayerInfo[cdIndex].LoadedInfo.CanSpray;
		FileData.CanTalk = PlayerInfo[cdIndex].LoadedInfo.CanTalk;
		FileData.CanVote = PlayerInfo[cdIndex].LoadedInfo.CanVote;
		FileData.flag1 = PlayerInfo[cdIndex].LoadedInfo.flag1;
		FileData.flag2 = PlayerInfo[cdIndex].LoadedInfo.flag2;
		FileData.flag3 = PlayerInfo[cdIndex].LoadedInfo.flag3;
		FileData.flag4 = PlayerInfo[cdIndex].LoadedInfo.flag4;
		FileData.flag5 = PlayerInfo[cdIndex].LoadedInfo.flag5;
		FileData.flag6 = PlayerInfo[cdIndex].LoadedInfo.flag6;
		FileData.flag7 = PlayerInfo[cdIndex].LoadedInfo.flag7;
		FileData.flag8 = PlayerInfo[cdIndex].LoadedInfo.flag8;
		FileData.flag9 = PlayerInfo[cdIndex].LoadedInfo.flag9;
		FileData.flag10 = PlayerInfo[cdIndex].LoadedInfo.flag10;
		FileData.TimeLen = PlayerInfo[cdIndex].LoadedInfo.TimeLen;
		FileData.Warnings = PlayerInfo[cdIndex].LoadedInfo.Warnings;

		//Write back the last name used.
		memset(FileData.Name, 0, sizeof(FileData.Name));
		strncpy(FileData.Name, STRING(pEntity->v.netname), (MAX_NAME_LENGTH-1));
		FileData.Name[MAX_NAME_LENGTH-1] = 0;

		memset(FileData.Char1, 0, sizeof(FileData.Char1));
		strncpy(FileData.Char1, PlayerInfo[cdIndex].LoadedInfo.Char1, (MAX_NAME_LENGTH-1));
		FileData.Char1[MAX_NAME_LENGTH-1] = 0;

		memset(FileData.Char2, 0, sizeof(FileData.Char2));
		strncpy(FileData.Char2, PlayerInfo[cdIndex].LoadedInfo.Char2, (MAX_NAME_LENGTH-1));
		FileData.Char2[MAX_NAME_LENGTH-1] = 0;

		memset(FileData.Char3, 0, sizeof(FileData.Char3));
		strncpy(FileData.Char3, PlayerInfo[cdIndex].LoadedInfo.Char3, (MAX_NAME_LENGTH-1));
		FileData.Char3[MAX_NAME_LENGTH-1] = 0;

		memset(FileData.Char4, 0, sizeof(FileData.Char4));
		strncpy(FileData.Char4, PlayerInfo[cdIndex].LoadedInfo.Char4, (MAX_NAME_LENGTH-1));
		FileData.Char4[MAX_NAME_LENGTH-1] = 0;

		fwrite(&FileData, sizeof(FileData), 1, cUserFile);

		fclose(cUserFile);

	}

	//Clear admin variables
	PlayerInfo[cdIndex].NoGreen = 0;
	PlayerInfo[cdIndex].IsAdmin = LEVEL_NONE;

	DeselectAll(pEntity);
	KillSkin(pEntity);
	KillGrapple(pEntity);
	End3rdPerson(pEntity);
	TFXResetPlayerInfo(pEntity);

}

//Client Connection message
void TFXClientConnectionMsg(edict_t *pEntity, const char *pszName)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXClientConnectionMsg(): Sending connection message to player %d.\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	char name[BUF_SIZE];
	strncpy(name, pszName,BUF_SIZE);
	make_friendly(name);

	int JoinedIndex = ENTINDEX(pEntity);
	char *servername=(char *) CVAR_GET_STRING("hostname");
	char *maxpeople=(char *) CVAR_GET_STRING("maxplayers");
	float RemoteXVer = CVAR_GET_FLOAT( "rx_infopulse" );
	int AllowCreds = (int)CVAR_GET_FLOAT("tfx_credits");
	int AllowDownload = (int)CVAR_GET_FLOAT("sv_allowdownload");

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXClientConnectionMsg(): Retrived CVARs from server.\n");
		fclose(fp);
	}

	int Seconds = (int)CVAR_GET_FLOAT("mp_timeleft");
	int Minutes = (int)CVAR_GET_FLOAT("mp_timeleft");
	int Hours = (int)CVAR_GET_FLOAT("mp_timeleft");

	Hours = Hours / 3600;
	Minutes = Seconds / 60 - (Hours * 60);
	Seconds = Seconds - (Minutes * 60) - (Hours * 3600);

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXClientConnectionMsg(): Calculated player time\n");
		fclose(fp);
	}
	
	//INTRO MESSAGE
	CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("Protected by HLGuard %s - http://www.HLGuard.com/\n",CVAR_GET_STRING("hlguard_version")));
	CLIENT_PRINTF(pEntity, print_console, "\n");
	CLIENT_PRINTF(pEntity, print_console, "-------------------------------------------------------------\n");
	CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("Welcome to %s\n",servername));
	CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("This server is using AutoOP Shodan v%s (Rev %s)\n",MOD_VERSION,MOD_REVISION));
	CLIENT_PRINTF(pEntity, print_console, "--------------------------------------------------------------\n");
	if(RemoteXVer != 0) //Is Remote-X on this server?
	{
		CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("and also running Remote-X (www.planetfortress.com/remote-x).\n"));// v%0.0f.\n",RemoteXVer));
	}
	CLIENT_PRINTF(pEntity, print_console, "\n");
	CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("The current map being played is %s\n",STRING(gpGlobals->mapname)));
	CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("Time left on current map is %dh:%dm:%ds.\n",Hours,Minutes,Seconds));

	if(FStrEq(LastSaid,"") || FStrEq(LastPlayer,"")) {
		CLIENT_PRINTF(pEntity, print_console, "Nothing has yet been said in the server.\n");
	} else {
		CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("The last thing said was \"%s\" by %s\n",LastSaid, LastPlayer));
	}

	if(FStrEq(LastConnected,"")) {
		CLIENT_PRINTF(pEntity, print_console, "You are the first to connect to the server.\n");
	} else {
		CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("The last person to connect was %s\n",LastConnected));
	}

	int qi;
	int qPlayerCount;

	qPlayerCount = 0;
	for ( qi = 1; qi <= gpGlobals->maxClients; qi++ ) {
		CBaseEntity *pPlayer = UTIL_PlayerByIndex(qi);
		if (pPlayer) 
			if(!FStrEq(STRING(pPlayer->pev->netname),""))
				qPlayerCount++;
	}
	char cgstr[256];
	
	sprintf(cgstr, "There is currently %d out of %d players on the server.\n", qPlayerCount, gpGlobals->maxClients);

	CLIENT_PRINTF(pEntity, print_console, cgstr);	
	CLIENT_PRINTF(pEntity, print_console, "\n");	
	if(AllowDownload == 1)
	{
		CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("You need the latest GP2 Clientpack (%s) before playing.\n",CLIENTPACK_VERSION));
		CLIENT_PRINTF(pEntity, print_console, "It is recommended that you get the client pack from our website\n");
		CLIENT_PRINTF(pEntity, print_console, "http://www.girlpower2.dhs.org\n");
		CLIENT_PRINTF(pEntity, print_console, "However you may stay connected and download the files from the server\n");
	} else {
		CLIENT_PRINTF(pEntity, print_console, "We require you have the latest GP2 Client pack before playing.\n");
		CLIENT_PRINTF(pEntity, print_console, "You can download the client pack from our website\n");
		CLIENT_PRINTF(pEntity, print_console, "http://www.girlpower2.dhs.org\n");
		CLIENT_PRINTF(pEntity, print_console, "Downloading straight from the server is currently disabled.\n");
	}

	if(NightTime == 1)
		CLIENT_PRINTF(pEntity, print_console, "An NAO map is currently playing, downloading is disabled during this session.\n");


	CLIENT_PRINTF(pEntity, print_console, "\n");
	CLIENT_PRINTF(pEntity, print_console, "When in the server say '!help' for instructions on server features.\n");
	if (AllowCreds == 0 || PlayerInfo[JoinedIndex].LoadedInfo.TimeLen < 1 || PlayerInfo[ENTINDEX(pEntity)].IsSteamer == 1) //If Credits are off or we have never joined before do the normal message
	{
		CLIENT_PRINTF(pEntity, print_console, "Please wait.. The download will finish shortly, thankyou for waiting.\n");
		CLIENT_PRINTF(pEntity, print_console, "\n");
		CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("Welcome to the server %s, we hope you have a good game.\n",name));
	}
	if(PlayerInfo[ENTINDEX(pEntity)].IsSteamer == 0)
	{
		//Credits are on and they have visited before so do the personalised messages.
		if (AllowCreds == 1 && PlayerInfo[JoinedIndex].LoadedInfo.TimeLen > 0)
		{
			//START OF PERSONALISED BIT
			CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("Welcome back to the server %s, have a nice game\n",name));
			//Print the number of credits thay have
			int Rank = 0;
			Rank = GetRank(PlayerInfo[JoinedIndex].LoadedInfo.flag5);
			CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("At the moment you have %d credits in your piggybank, your current rank is %s.\n",PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.Credits,STRING(Rank)));
		
			//Print out their total time on server :)
			long int iHours = PlayerInfo[JoinedIndex].LoadedInfo.TimeLen / 3600;
			long int iMins = (PlayerInfo[JoinedIndex].LoadedInfo.TimeLen - (iHours * 3600)) / 60;
			long int iSecs = PlayerInfo[JoinedIndex].LoadedInfo.TimeLen % 60;
			CLIENT_PRINTF(pEntity, print_console, UTIL_VarArgs("You have wasted %d hours, %d mins and %d seconds of your life on this server.\n",iHours, iMins, iSecs));
		}
	}

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXClientConnectionMsg(): Message sent to client %d\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	strncpy(LastConnected,name,49); //Copy name to last person connected
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXClientConnectionMsg(): Copied name to memeory\n");
		fclose(fp);
	}
}

void CreditThink()
{
	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "CreditThink(): Called\n");
		fclose(fp);
	}

	CBaseEntity *pPlayer = NULL;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) 
	{
		pPlayer = UTIL_PlayerByIndex(i);

		if(pPlayer)
		{
			if(PlayerInfo[i].TimerActive == 1)
			{
				PlayerInfo[i].LoadedInfo.TimeLen = PlayerInfo[i].LoadedInfo.TimeLen + 1;
				//Keep an admins FAKE time counting.
				PlayerInfo[i].FakeTime = PlayerInfo[i].FakeTime + 1;
			}

			if (pPlayer->pev->frags >= PlayerInfo[i].NeedFrags)
			{
				//He has gotten another credit, update his info.
				PlayerInfo[i].NeedFrags = PlayerInfo[i].NeedFrags + CREDIT_GAP;
				PlayerInfo[i].LoadedInfo.Credits = PlayerInfo[i].LoadedInfo.Credits + 1;
				
				//Update rank credits to keep rank up to date
				PlayerInfo[i].LoadedInfo.flag5 = PlayerInfo[i].LoadedInfo.flag5 + 1;
				
				//Update stealth admins FAKE credits.
				PlayerInfo[i].FakeCreds = PlayerInfo[i].FakeCreds + 1;

				//Also notify the player.
				ClientPrint( pPlayer->pev, HUD_PRINTTALK, UTIL_VarArgs( "You just earned another credit, you now have %d credits.\n", PlayerInfo[i].LoadedInfo.Credits) );

				if(JustChangedRank(PlayerInfo[i].LoadedInfo.flag5))
					ClientPrint( pPlayer->pev, HUD_PRINTTALK, UTIL_VarArgs( "*** You have just earned the rank \"%s\".\n", STRING(GetRank(PlayerInfo[i].LoadedInfo.flag5))));

			}
		}
	}

	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "CreditThink(): Completed\n");
		fclose(fp);
	}
}

//Info command meat.
void PlayerCredInfo(const char *Speech)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Playercredinfo(): Called\n");
		fclose(fp);
	}

	char info_user[MAX_TEXT_LENGTH];
	int Length = strlen(Speech);
	int Index = 0;

	//Remove "info " from the beginning of the string.
	for( int i=5; i<Length+1; i++)
		info_user[i-5] = Speech[i];
	info_user[i-5] = NULL;

	Index = GetPartialPlayerIndex(info_user,-1);
	if(Index == 0) return;
	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	//Print out the info to the chat area
	int CreditsOn = CVAR_GET_FLOAT("tfx_credits");

	if (CreditsOn == 1) {
		if(PlayerInfo[Index].NoGreen == 0)
		{
			BotTalkAllArgs("%s currently has %d credits, %d frags and has a rank of %s.\n",STRING(pPlayer->pev->netname),PlayerInfo[Index].LoadedInfo.Credits,(int)pPlayer->pev->frags,STRING(GetRank(PlayerInfo[Index].LoadedInfo.flag5)));
		} else {
			BotTalkAllArgs("%s currently has %d credits, %d frags and has a rank of %s.\n",STRING(pPlayer->pev->netname),PlayerInfo[Index].FakeCreds,(int)pPlayer->pev->frags,STRING(GetRank(PlayerInfo[Index].FakeCreds)));
		}

	} else {
		BotTalkAllArgs("%s currently has %d frags.\n",STRING(pPlayer->pev->netname),(int)pPlayer->pev->frags);
	}
	return;
}

//Time command meat.
void PlayerTimeInfo(const char *Speech)
{
	char info_user[MAX_TEXT_LENGTH];
	int Length = strlen(Speech);
	int Index = 0;

	long int iHours = 0;
	long int iMins = 0;
	long int iSecs = 0;

	//Remove "time " from the beginning of the string.
	for( int i=5; i<Length+1; i++)
		info_user[i-5] = Speech[i];
	info_user[i-5] = NULL;

	Index = GetPartialPlayerIndex(info_user,-1);
	if(Index == 0) return;
	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	if(PlayerInfo[Index].NoGreen == 0)
		//Determine whether to falsify time or not.
	{
		iHours = PlayerInfo[Index].LoadedInfo.TimeLen / 3600;
		iMins = (PlayerInfo[Index].LoadedInfo.TimeLen - (iHours * 3600)) / 60;
		iSecs = PlayerInfo[Index].LoadedInfo.TimeLen % 60;
	} else {
		iHours = PlayerInfo[Index].FakeTime / 3600;
		iMins = (PlayerInfo[Index].FakeTime - (iHours * 3600)) / 60;
		iSecs = PlayerInfo[Index].FakeTime % 60;
	}

	//Print out the info to the chat area
	int CreditsOn = CVAR_GET_FLOAT("tfx_credits");

	if (CreditsOn == 1) {
		BotTalkAllArgs("%s has been on this server for %d hours, %d minutes and %d seconds.\n",STRING(pPlayer->pev->netname),iHours,iMins,iSecs);
	} else {
		BotTalkAllArgs("Players accounts are frozen at the moment, time counter not active.\n");
	}
	return ;
}

//Check command meat.
void PlayerCheck(const char *Speech)
{
	char info_user[MAX_TEXT_LENGTH];
	int Length = strlen(Speech);
	int Index = 0;

	//Remove "check " from the beginning of the string.
	for( int i=6; i<Length+1; i++)
		info_user[i-6] = Speech[i];
	info_user[i-6] = NULL;

	Index = GetPartialPlayerIndex(info_user,-1);
	if(Index == 0) return;
	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	if (PlayerInfo[Index].IsAdmin > LEVEL_NONE && PlayerInfo[Index].NoGreen == 0) {
		BotTalkAllArgs("%s is a level %d server operator.\n",STRING(pPlayer->pev->netname),PlayerInfo[Index].IsAdmin);
	} else {
		BotTalkAllArgs("%s is NOT an admin.\n",STRING(pPlayer->pev->netname));
	}
	return;
}

//Whois command meat.
void PlayerWhois(const char *Speech)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Playerwhois(): Function called\n");
		fclose(fp);
	}

	char info_user[MAX_TEXT_LENGTH];
	int Length = strlen(Speech);
	int Index = 0;

	//Remove "whois " from the beginning of the string.
	for( int i=6; i<Length+1; i++)
		info_user[i-6] = Speech[i];
	info_user[i-6] = NULL;

	Index = GetPartialPlayerIndex(info_user,-1);
	if(Index == 0) return;
	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	if(PlayerInfo[Index].NoGreen == 0) //Determine whether to falsify the info or not.
	{
		BotTalkAllArgs("%s was last known as %s, first known as %s.\n",STRING(pPlayer->pev->netname),PlayerInfo[Index].LoadedInfo.Name,PlayerInfo[Index].OrigName);
	} else {
		BotTalkAllArgs("%s was last known as %s, first known as %s.\n",STRING(pPlayer->pev->netname),STRING(pPlayer->pev->netname),STRING(pPlayer->pev->netname));
	}

	return;
}

//PlayerWarnings command meat.
void PlayerWarnings(const char *Speech)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Playerwarnings(): Called function\n");
		fclose(fp);
	}

	char info_user[MAX_TEXT_LENGTH];
	int Length = strlen(Speech);
	int Index = 0;

	//Remove "warnings " from the beginning of the string.
	for( int i=9; i<Length+1; i++)
		info_user[i-9] = Speech[i];
	info_user[i-9] = NULL;

	Index = GetPartialPlayerIndex(info_user,-1);
	if(Index == 0) return;
	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	BotTalkAllArgs("%s has had %d warnings from admins.\n",STRING(pPlayer->pev->netname),PlayerInfo[Index].LoadedInfo.Warnings);
	return;
}

//Rank command meat.
void PlayerRankInfo(const char *Speech)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Playerrankinfo(): Called\n");
		fclose(fp);
	}

	char info_user[MAX_TEXT_LENGTH];
	int Length = strlen(Speech);
	int Index = 0;

	//Remove "rank " from the beginning of the string.
	for( int i=5; i<Length+1; i++)
		info_user[i-5] = Speech[i];
	info_user[i-5] = NULL;

	Index = GetPartialPlayerIndex(info_user,-1);
	if(Index == 0) return;
	CBaseEntity *pPlayer = UTIL_PlayerByIndex(Index);

	//Print out the info to the chat area
	int CreditsOn = CVAR_GET_FLOAT("tfx_credits");

	if (CreditsOn == 1) {
		if(PlayerInfo[Index].NoGreen == 0)
		{
			BotTalkAllArgs("%s has a rank of %s and must earn %d credits for the next rank.\n",STRING(pPlayer->pev->netname),STRING(GetRank(PlayerInfo[Index].LoadedInfo.flag5)),NextRankAmount(PlayerInfo[Index].LoadedInfo.flag5));
		} else {
			BotTalkAllArgs("%s has a rank of %s and must earn %d credits for the next rank.\n",STRING(pPlayer->pev->netname),STRING(GetRank(PlayerInfo[Index].FakeCreds)),NextRankAmount(PlayerInfo[Index].FakeCreds));
		}
	}
	return;
}

void GiveCredits(edict_t *pGiver, const char *Player, int iAmount)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Player %d gave credits away\n",ENTINDEX(pGiver));
		fclose(fp);
	}

	if(PlayerInfo[ENTINDEX(pGiver)].LoadedInfo.Credits <= 0)
	{
		CLIENT_PRINTF( pGiver, print_console, UTIL_VarArgs( "You don't have any credits to give!\n"));
		return;
	}
	
	if((iAmount <= 0) || iAmount > PlayerInfo[ENTINDEX(pGiver)].LoadedInfo.Credits)
	{
		CLIENT_PRINTF( pGiver, print_console, UTIL_VarArgs( "Invalid credit amount!\n"));
		return;
	}

	char *PlayerInf = (char *)Player;
	int Index = GetPartialPlayerIndex(PlayerInf,-1);

	if(Index == 0)
		return;

	edict_t *pReciever = INDEXENT(Index);

	PlayerInfo[ENTINDEX(pGiver)].LoadedInfo.Credits -= iAmount;
	PlayerInfo[ENTINDEX(pReciever)].LoadedInfo.Credits += iAmount;

	CLIENT_PRINTF( pReciever, print_console, UTIL_VarArgs( "%s gave you %d credits!\n",STRING(pGiver->v.netname),iAmount));
	CLIENT_PRINTF( pReciever, print_center, UTIL_VarArgs( "%s gave you %d credits!\n",STRING(pGiver->v.netname),iAmount));

	CLIENT_PRINTF( pGiver, print_console, UTIL_VarArgs( "%d credits given to %s.\n",iAmount,STRING(pReciever->v.netname)));
	WriteBackFiles();
}

void WriteBackFiles( )
{
	if(CVAR_GET_FLOAT("mp_timeleft") < 5)
		return;

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "WriteBackFiles(): Writing player data.\n");
		fclose(fp);
	}

	edict_t *pTime = NULL;

	_mkdir( "tfc\\autoop" );
	_mkdir( "tfc\\autoop\\userinfo" );

	UTIL_LogPrintf( "[AUTOOP] Writing back everyones playerfiles and stuff.\n");

	//Write everyones files here
	//Cycle thru all players
	for ( int vi = 1; vi <= gpGlobals->maxClients; vi++ ) 
	{
		pTime = INDEXENT( vi );
		if (pTime) //They exist?
		{
			if(!FStrEq(STRING(pTime->v.netname),"") && (FStrEq(STRING(pTime->v.classname),"player")))
			{
				///They exist, update their file(s).
				int iPIndex = vi;
				//int iWONID = GETPLAYERWONID(pTime);

				int AllowCreds = (int)CVAR_GET_FLOAT("tfx_credits");

				if (AllowCreds == 1 )
				{
					//char szWONID[30];
					//_ltoa(iWONID,szWONID,10);

					char UserFileName[200];
					strcpy(UserFileName, "tfc\\autoop\\userinfo\\");
					strcat(UserFileName,GETPLAYERAUTHID(pTime));
					strcat(UserFileName, ".dat");

					//replace any colons ':' with an 'x'
					//i.e STEAM_0x123456
					for(int i = 0;i<(int)strlen(UserFileName);i++)
					{
						if(UserFileName[i] == ':')
							UserFileName[i] = 'x';
					}

					FILE *cUserFile = fopen(UserFileName, "wb");

					TFXUserFileHeader_s FileHeader;

					//Write Version
					FileHeader.Version = USERFILE_VERSION;

					//Write Original Name
					memset(FileHeader.OrigName, 0, sizeof(FileHeader.OrigName));
					if(FStrEq(PlayerInfo[iPIndex].OrigName,""))
					{
						strncpy(FileHeader.OrigName, STRING(pTime->v.netname), (MAX_NAME_LENGTH-1));
						
					} else {
						strncpy(FileHeader.OrigName, PlayerInfo[iPIndex].OrigName, (MAX_NAME_LENGTH-1));
					}

					FileHeader.OrigName[MAX_NAME_LENGTH-1] = 0;

					//Write header to disk
					fwrite(&FileHeader, sizeof(FileHeader), 1, cUserFile);

					TFXUserDataFileV3_s FileData;

					FileData.Credits = PlayerInfo[iPIndex].LoadedInfo.Credits;
					FileData.CanSpray = PlayerInfo[iPIndex].LoadedInfo.CanSpray;
					FileData.CanTalk = PlayerInfo[iPIndex].LoadedInfo.CanTalk;
					FileData.CanVote = PlayerInfo[iPIndex].LoadedInfo.CanVote;
					FileData.flag1 = PlayerInfo[iPIndex].LoadedInfo.flag1;
					FileData.flag2 = PlayerInfo[iPIndex].LoadedInfo.flag2;
					FileData.flag3 = PlayerInfo[iPIndex].LoadedInfo.flag3;
					FileData.flag4 = PlayerInfo[iPIndex].LoadedInfo.flag4;
					FileData.flag5 = PlayerInfo[iPIndex].LoadedInfo.flag5;
					FileData.flag6 = PlayerInfo[iPIndex].LoadedInfo.flag6;
					FileData.flag7 = PlayerInfo[iPIndex].LoadedInfo.flag7;
					FileData.flag8 = PlayerInfo[iPIndex].LoadedInfo.flag8;
					FileData.flag9 = PlayerInfo[iPIndex].LoadedInfo.flag9;
					FileData.flag10 = PlayerInfo[iPIndex].LoadedInfo.flag10;
					FileData.TimeLen = PlayerInfo[iPIndex].LoadedInfo.TimeLen;
					FileData.Warnings = PlayerInfo[iPIndex].LoadedInfo.Warnings;

					//Write back the last name used.
					memset(FileData.Name, 0, sizeof(FileData.Name));
					strncpy(FileData.Name, STRING(pTime->v.netname), (MAX_NAME_LENGTH-1));
					FileData.Name[MAX_NAME_LENGTH-1] = 0;

					memset(FileData.Char1, 0, sizeof(FileData.Char1));
					strncpy(FileData.Char1, PlayerInfo[iPIndex].LoadedInfo.Char1, (MAX_NAME_LENGTH-1));
					FileData.Char1[MAX_NAME_LENGTH-1] = 0;

					memset(FileData.Char2, 0, sizeof(FileData.Char2));
					strncpy(FileData.Char2, PlayerInfo[iPIndex].LoadedInfo.Char2, (MAX_NAME_LENGTH-1));
					FileData.Char2[MAX_NAME_LENGTH-1] = 0;

					memset(FileData.Char3, 0, sizeof(FileData.Char3));
					strncpy(FileData.Char3, PlayerInfo[iPIndex].LoadedInfo.Char3, (MAX_NAME_LENGTH-1));
					FileData.Char3[MAX_NAME_LENGTH-1] = 0;

					memset(FileData.Char4, 0, sizeof(FileData.Char4));
					strncpy(FileData.Char4, PlayerInfo[iPIndex].LoadedInfo.Char4, (MAX_NAME_LENGTH-1));
					FileData.Char4[MAX_NAME_LENGTH-1] = 0;

					fwrite(&FileData, sizeof(FileData), 1, cUserFile);

					fclose(cUserFile);
				}
			}
		}
	}
}

//Authenticates a server op
void AdminAuth(edict_t *pEntity, int iConnecting)
{
	int JoinedIndex = ENTINDEX(pEntity);

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Player %d called AdminAuth()\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	PlayerInfo[JoinedIndex].IsAdmin = PlayerInfo[JoinedIndex].LoadedInfo.flag2;

	if(PlayerInfo[JoinedIndex].IsAdmin > 0)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have been authed as a level %d server op.\n",PlayerInfo[JoinedIndex].IsAdmin));
		PlayerInfo[JoinedIndex].LoadedInfo.flag1 = 0;
		if(iConnecting == 0)
			BotTalkAllArgs("%s logged in as a level %d server op.\n",STRING(pEntity->v.netname),PlayerInfo[JoinedIndex].IsAdmin);
	} else {
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have not been authenticated as a server op.\n"));
	}		
}

void AdminDeAuth(edict_t *pEntity)
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Player %d called AdminDeAuth()\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin > 0)
	{
		PlayerInfo[ENTINDEX(pEntity)].IsAdmin = 0;
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You have logged out of server op mode.\n"));
		PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.flag1 = 1;
		BotTalkAllArgs("%s logged out of server op mode.\n",STRING(pEntity->v.netname));
		FakeScoreColor(pEntity,pEntity->v.team);

		//Knock him out of stealth if hes in.
		PlayerInfo[ENTINDEX(pEntity)].NoGreen = 0;
	} else {
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You are not currently authed!\n"));
	}
}

void CheckClientPack(edict_t *pEntity)
{
	if((strcmp(PlayerInfo[ENTINDEX(pEntity)].ClientPackVer,"32fd49b5") != 0) && (strcmp(PlayerInfo[ENTINDEX(pEntity)].ClientPackVer,"29fa51c8") != 0))
		BotTalkOne(pEntity,CLIENTPACK_STRING);

	//Clear it on client
	CLIENT_COMMAND ( pEntity, UTIL_VarArgs("setinfo \"_gp2clpckver\" \"\"\n") );
}
