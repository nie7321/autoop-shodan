/*****************************************************\
*	Auto-OP Shodan
*
*	File: inisettings.cpp
*	Description: Game settings loading
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

#include "CIniFile.h"
#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include <direct.h>
#include <stdio.h>
#include <iostream>

#define INI_TEXT_LENGTH 150

int MAX_USERMSG = 115;
char BOT_NAME[INI_TEXT_LENGTH];
char BOT_NAME2[INI_TEXT_LENGTH];
int ICON_DELAY = 5;
int KEEP_CLEAR = 40;
int CREDIT_GAP = 10;

int GUN_RESPAWN_DELAY = 20;
int GRAPPLE_DROPTIME = 10;
int GRAPPLE_HEALTH = 200;
int FENCEPOST_HEALTH = 500;
int FENCEPOST_FIX_REQ = 5;
int BIGGUN_REQUIRED = 50;
int BIGGUN_FIX_REQ = 5;
int BIGGUN_HEALTH = 350;
int AIRSTRIKE_RANDOMNESS = 300;
int AIRSTRIKE_CHARGE_TIME = 15;
int AIRSTRIKE_AMOUNT = 7;
int SUPERFLAME_CHARGE_TIME = 10;
int TRIPMINE_HEALTH = 100;
int TRIPMINE_DAMAGE = 200;
int C4_HEALTH = 500;
float C4_DETDELAY = 1.5;
int MEDI_FLAG_HEALTH = 200;
int MEDI_FLAG_RADIUS = 100;
int STEALTH_DRAIN = 2;
int SNARK_HEALTH = 50;
int ENERGY_BALL_SPEED = 150;
int ENERGY_AMMO = 100;
int ENERGY_DELAY = 15;
int TAZAR_DELAY = 2;
int TAZAR_DAMAGE = 70;
float TRIPMINE_DELAY = 1.5;
int ANTHRAX_DELAY = 5;
int ANTHRAX_AMMO = 10;
int ANTHRAX_DAMAGE = 50;
int PROX_MINE_AMMO = 4;
int TRIPMINE_AMMO = 3;
int TRIPMINE_MAX = 3;
int C4_AMMO = 2;
int SNARK_AMMO = 10;

char TIMELEFT_STRING[INI_TEXT_LENGTH];
char NEXTMAP_STRING[INI_TEXT_LENGTH];
char CLIENTPACK_STRING[INI_TEXT_LENGTH];
char SPAM_STRING[INI_TEXT_LENGTH];

char CMD_UNKNOWN[INI_TEXT_LENGTH];
char CMD_NO_ACCESS[INI_TEXT_LENGTH];
char CMD_NO_CHEAT[INI_TEXT_LENGTH];

char MV_ALREADYVOTING[INI_TEXT_LENGTH];
char MV_NOTFOUND[INI_TEXT_LENGTH];
char MV_MAXED[INI_TEXT_LENGTH];
char MV_REMOVEDVOTE[INI_TEXT_LENGTH];
char MV_CASTVOTE[INI_TEXT_LENGTH];
char MV_CANTSTART[INI_TEXT_LENGTH];
char MV_NOVOTES[INI_TEXT_LENGTH];
char MV_EXTENDWIN[INI_TEXT_LENGTH];
char MV_EXTENDLEAD[INI_TEXT_LENGTH];
char MV_WINNER[INI_TEXT_LENGTH];
char MV_NOWINNER[INI_TEXT_LENGTH];
char MV_CANCELLED[INI_TEXT_LENGTH];
char MV_NOTENOUGHVOTES[INI_TEXT_LENGTH];

char KV_VOTESTART[INI_TEXT_LENGTH];
char KV_VOTEYES[INI_TEXT_LENGTH];
char KV_VOTENO[INI_TEXT_LENGTH];
char KV_VOTEEND[INI_TEXT_LENGTH];
char KV_CANTSTART[INI_TEXT_LENGTH];

char SK_NOSNIPERS[INI_TEXT_LENGTH];
char SK_NOACCESS[INI_TEXT_LENGTH];
char SK_APPLIED[INI_TEXT_LENGTH];
char SK_FATALERROR[INI_TEXT_LENGTH];
char SK_REMOVED[INI_TEXT_LENGTH];
char SK_CURRENT[INI_TEXT_LENGTH];
char SK_NOCURRENT[INI_TEXT_LENGTH];

void LoadINITextString(CIniFile iniFile, char *keyname, char *buffer);

void LoadGameSettings()
{
	FILE *cLvlFile = fopen("tfc\\autoop\\shodan.ini", "rb");

	if(cLvlFile == NULL)
	{
		//The ini file does not exist
		UTIL_LogPrintf( "[AUTOOP] shodan.ini not found - using default game settings.\n");
		return;
	}

	//Load the access levels from the INI file
	CIniFile iniFile;
	iniFile.SetFileName("tfc\\autoop\\shodan.ini");

	//[General]
	MAX_USERMSG = iniFile.GetIntD("General","maxusermsg",115);
	iniFile.GetStringD("General","botname1",BOT_NAME,200,"Jessica");
	iniFile.GetStringD("General","botname2",BOT_NAME2,200,"Jess");
	ICON_DELAY = iniFile.GetIntD("General","icondelay",5);
	KEEP_CLEAR = iniFile.GetIntD("General","minfreeents",40);
	CREDIT_GAP = iniFile.GetIntD("General","creditgap",10);

	//[Special Weapons]
	GUN_RESPAWN_DELAY = iniFile.GetIntD("Special Weapons","gunrespawndelay",20);
	GRAPPLE_DROPTIME = iniFile.GetIntD("Special Weapons","grappledroptime",10);
	GRAPPLE_HEALTH = iniFile.GetIntD("Special Weapons","grapplehealth",200);
	FENCEPOST_HEALTH = iniFile.GetIntD("Special Weapons","fenceposthealth",500);
	FENCEPOST_FIX_REQ = iniFile.GetIntD("Special Weapons","fencepostfixreq",5);
	BIGGUN_REQUIRED = iniFile.GetIntD("Special Weapons","turretrequired",50);
	BIGGUN_FIX_REQ = iniFile.GetIntD("Special Weapons","turretfixreq",5);
	BIGGUN_HEALTH = iniFile.GetIntD("Special Weapons","turrethealt",350);
	AIRSTRIKE_RANDOMNESS = iniFile.GetIntD("Special Weapons","airstrikerandomness",300);
	AIRSTRIKE_CHARGE_TIME = iniFile.GetIntD("Special Weapons","airstrikechargetime",15);
	AIRSTRIKE_AMOUNT = iniFile.GetIntD("Special Weapons","airstrikeamount",7);
	SUPERFLAME_CHARGE_TIME = iniFile.GetIntD("Special Weapons","superflamechargetime",10);
	TRIPMINE_HEALTH = iniFile.GetIntD("Special Weapons","tripminehealth",100);
	TRIPMINE_DAMAGE = iniFile.GetIntD("Special Weapons","tripminedamage",200);
	C4_HEALTH = iniFile.GetIntD("Special Weapons","c4health",500);
	C4_DETDELAY = iniFile.GetFloatD("Special Weapons","c4detdelay","1.5");
	MEDI_FLAG_HEALTH = iniFile.GetIntD("Special Weapons","mediflaghealth",200);
	MEDI_FLAG_RADIUS = iniFile.GetIntD("Special Weapons","mediflagradius",100);
	STEALTH_DRAIN = iniFile.GetIntD("Special Weapons","stealthdrain",2);
	SNARK_HEALTH = iniFile.GetIntD("Special Weapons","snarkhealth",50);
	ENERGY_BALL_SPEED = iniFile.GetIntD("Special Weapons","energyballspeed",150);
	ENERGY_AMMO = iniFile.GetIntD("Special Weapons","energyammo",100);
	ENERGY_DELAY = iniFile.GetIntD("Special Weapons","energygundelay",15);
	TAZAR_DELAY = iniFile.GetIntD("Special Weapons","tazardelay",2);
	TAZAR_DAMAGE = iniFile.GetIntD("Special Weapons","tazardamage",70);
	TRIPMINE_DELAY = iniFile.GetFloatD("Special Weapons","tripminedelay","1.5");
	ANTHRAX_DELAY = iniFile.GetIntD("Special Weapons","blasterdelay",5);
	ANTHRAX_AMMO = iniFile.GetIntD("Special Weapons","blasterammo",10);
	ANTHRAX_DAMAGE = iniFile.GetIntD("Special Weapons","blasterdamage",50);
	PROX_MINE_AMMO = iniFile.GetIntD("Special Weapons","proxmineammo",4);
	TRIPMINE_AMMO = iniFile.GetIntD("Special Weapons","tripmineammo",3);
	TRIPMINE_MAX = iniFile.GetIntD("Special Weapons","tripminemax",3);
	C4_AMMO = iniFile.GetIntD("Special Weapons","c4ammo",2);
	SNARK_AMMO = iniFile.GetIntD("Special Weapons","snarkammo",10);

	
	//[Text Strings]
	LoadINITextString(iniFile,"timeleft",TIMELEFT_STRING);
	LoadINITextString(iniFile,"nextmap",NEXTMAP_STRING);
	LoadINITextString(iniFile,"clientpack",CLIENTPACK_STRING);
	LoadINITextString(iniFile,"spam",SPAM_STRING);

	LoadINITextString(iniFile,"badcommand",CMD_UNKNOWN);
	LoadINITextString(iniFile,"noaccess",CMD_NO_ACCESS);
	LoadINITextString(iniFile,"nocheat",CMD_NO_CHEAT);

	LoadINITextString(iniFile,"alreadyvoting",MV_ALREADYVOTING);
	LoadINITextString(iniFile,"mapnotfound",MV_NOTFOUND);
	LoadINITextString(iniFile,"mapmax",MV_MAXED);
	LoadINITextString(iniFile,"removevote",MV_REMOVEDVOTE);
	LoadINITextString(iniFile,"mapvotecast",MV_CASTVOTE);
	LoadINITextString(iniFile,"mapstartfail",MV_CANTSTART);
	LoadINITextString(iniFile,"novotes",MV_NOVOTES);
	LoadINITextString(iniFile,"extendwin",MV_EXTENDWIN);
	LoadINITextString(iniFile,"extendlead",MV_EXTENDLEAD);
	LoadINITextString(iniFile,"mapwinner",MV_WINNER);
	LoadINITextString(iniFile,"nowinner",MV_NOWINNER);
	LoadINITextString(iniFile,"cancelled",MV_CANCELLED);
	LoadINITextString(iniFile,"notenoughvotes",MV_NOTENOUGHVOTES);
	
	LoadINITextString(iniFile,"kvvotestart",KV_VOTESTART);
	LoadINITextString(iniFile,"kvvoteyes",KV_VOTEYES);
	LoadINITextString(iniFile,"kvvoteno",KV_VOTENO);
	LoadINITextString(iniFile,"kvvoteend",KV_VOTEEND);
	LoadINITextString(iniFile,"kvcantstart",KV_CANTSTART);

	LoadINITextString(iniFile,"sknosnipers",SK_NOSNIPERS);
	LoadINITextString(iniFile,"sknoaccess",SK_NOACCESS);
	LoadINITextString(iniFile,"skapplied",SK_APPLIED);
	LoadINITextString(iniFile,"skfatalerror",SK_FATALERROR);
	LoadINITextString(iniFile,"skremoved",SK_REMOVED);
	LoadINITextString(iniFile,"skcurrent",SK_CURRENT);
	LoadINITextString(iniFile,"sknocurrent",SK_NOCURRENT);

	//UTIL_LogPrintf( "[AUTOOP] DEBUG: C4_DETDELAY = %f\n",C4_DETDELAY);

	UTIL_LogPrintf( "[AUTOOP] shodan.ini found - loaded game settings.\n");
}

void LoadINITextString(CIniFile iniFile, char *keyname, char *buffer)
{
	iniFile.GetStringD("Text Strings",keyname,buffer,INI_TEXT_LENGTH,"\0");
	strreplace(buffer,"\\n","\n");
}

