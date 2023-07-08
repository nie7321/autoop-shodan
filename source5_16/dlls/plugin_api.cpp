/*****************************************************\
*	Auto-OP Shodan
*
*	File: plugin_api.cpp
*	Description: Plugin API class.
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
#include "3rdperson.h"
#include "stripper.h"
#include "voting.h"
#include "leveleditor.h"
#include "aohgrunt.h"
#include "aogarg.h"
#include "userstrings.h"
#include "plugins.h"
#include "plugin_api.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern int team_allies[4];
extern TFXFeatureEnabled_s Features;
extern char *g_SkinNames[];
extern  DLL_GLOBAL mapcycle_t mapcycle;
extern TFXSkin_t SkinLibrary[];

//--- PLUGIN API FUNCTION PROTOTYPES
float GetPluginInterfaceVer();
char* GetName(int iPlIndex);
char* GetAuthID(int iPlIndex);
int GetUserID(int iPlIndex);
int	IndexFromText(char *szName);
int GetCredits(int iPlIndex);
int SetCredits(int iPlIndex, int iAmount);
long GetTime(int iPlIndex);
char* GetOrigName(int iPlIndex);
char* PluginGetRank(int iPlIndex);
int	Set_Ammo_Shells(int iPlIndex, int iAmount);
int Get_Ammo_Shells(int iPlIndex);
int	Set_Ammo_Nails(int iPlIndex, int iAmount);
int Get_Ammo_Nails(int iPlIndex);
int	Set_Ammo_Rockets(int iPlIndex, int iAmount);
int Get_Ammo_Rockets(int iPlIndex);
int	Set_Ammo_Cells(int iPlIndex, int iAmount);
int Get_Ammo_Cells(int iPlIndex);
int	Set_Ammo_Gren1(int iPlIndex, int iAmount);
int Get_Ammo_Gren1(int iPlIndex);
int	Set_Ammo_Gren2(int iPlIndex, int iAmount);
int Get_Ammo_Gren2(int iPlIndex);
int Get_Ammo_Proxmine(int iPlIndex);
int	Set_Ammo_Proxmine(int iPlIndex, int iAmount);
int Get_Ammo_Snarks(int iPlIndex);
int	Set_Ammo_Snarks(int iPlIndex, int iAmount);
int Get_Ammo_C4(int iPlIndex);
int	Set_Ammo_C4(int iPlIndex, int iAmount);
int Get_Ammo_Tripmine(int iPlIndex);
int	Set_Ammo_Tripmine(int iPlIndex, int iAmount);
int ResetBuild(int iPlIndex, int iBuildType);
int Plugin_Gag(int iPlIndex);
int Plugin_UnGag(int iPlIndex);
int	Plugin_Invis(int iPlIndex, int iSwitch);
int	Plugin_Godmode(int iPlIndex, int iSwitch);
int	Plugin_Noclip(int iPlIndex, int iSwitch);
int Plugin_Glow(int iPlIndex, int r, int b, int g, int iSize);
int	Set_Playerclass(int iPlIndex, int iPlayerclass);
int	Get_Playerclass(int iPlIndex);
int	Set_Armor(int iPlIndex, int iAmount);
int Get_Armor(int iPlIndex);
int	Set_Health(int iPlIndex, int iAmount);
int Get_Health(int iPlIndex);
int	Set_CustomSkin(int iPlIndex, int iSkinIndex);
int	Get_CustomSkin(int iPlIndex);
int	Set_AdminAccess(int iPlIndex, int iAdminLevel);
int	Get_AdminAccess(int iPlIndex);
int Set_View(int iPlIndex, int iView);
int Plugin_Color_Trail(int iPlIndex, int r, int g, int b);
int Plugin_Client_Command(int iPlIndex, char *szCommand);
int Plugin_Set_Origin(int iPlIndex, int x, int y, int z);
int Plugin_Get_Origin(int iPlIndex, int &x, int &y, int &z);
int Plugin_Hurt(int iPlIndex, int iDmg, int iDmgType);
int Plugin_Set_Velocity(int iPlIndex, float x, float y, float z);
char* Get_Client_KeyValue(int iPlIndex, char *szKeyname);
int	Set_Client_KeyValue(int iPlIndex, char *szKeyname, char *szValue);
BOOL HasFencepost (int iPlIndex);
BOOL HasManualTurret (int iPlIndex);
BOOL HasC4 (int iPlIndex);
BOOL HasProxmine (int iPlIndex);
BOOL HasTripmine (int iPlIndex);
BOOL InStealth (int iPlIndex);
BOOL HasPackBomb (int iPlIndex);
BOOL HasSuperFlame (int iPlIndex);
BOOL HasSentryGun (int iPlIndex);
BOOL HasDispenser (int iPlIndex);

//Server function prototypes
int TextMessage(int iPlIndex, int iPrintType, char *format, ...);
int ColorMessage(int iPlIndex, int x, int y, int r1, int g1, int b1, int r2, int g2, int b2, int time, char *format, ...);
int Plugin_ScreenShake(float flAmplitude);
int Plugin_ScreenFade(int r, int g, int b, int alpha);
void Plugin_StartMapVote();
void Plugin_EndMapVote();
void Plugin_CancelMapVote();
int Plugin_ResponseBotSay(char *format, ...);
int Plugin_LogWrite(char *szLogLine);
char* Get_Cvar(char *szCvarKey);
void Set_CvarString(char *szCvarKey, char *szCvarValue);
void Set_CvarFloat(char *szCvarKey, float flCvarValue);
char* Get_Currentmap();
char* Get_Nextmap();
void Plugin_Server_Command(char *szCommand);
char* Get_BotName1();
char* Get_BotName2();
int Plugin_PlayerCount();
int PointTo(int iPlIndex, int iRange);
void Plugin_Grunt(bool blnCanWalk, int x, int y, int z);
void Plugin_Garg(bool blnCanWalk, int x, int y, int z);
float Plugin_Get_Time();
BOOL Plugin_ValidMap(char *szMapName);
void Plugin_MessageBox(char *szMessage);
void Plugin_WriteBack();
void Plugin_CreateLight(int r, int g, int b, int x, int y, int z);
void Plugin_CreateEntity(int x, int y, int z, int anglex, int angley, int anglez, char *szClassname);
int	Plugin_LoadEntLayout(char *szLayoutName);
int	Plugin_UnloadEntLayout(char *szLayoutName);
void Plugin_TriggerEntity(char *szTargetname);
float Plugin_RandomFloat(float flMin, float flMax);
long Plugin_RandomLong(long lngMin, long lngMax);
char* SkinNameFromIndex(int iSkinIndex);
int	Get_TotalSkins();

// --- END PROTOTYPES

shodanfuncs_t plugin_interface;

//Puts the pointers into the function table.
void InitInterface()
{
	plugin_interface.pfnGetPluginInterfaceVer = GetPluginInterfaceVer;
	plugin_interface.pfnGetName = GetName;
	plugin_interface.pfnGetAuthID = GetAuthID;
	plugin_interface.pfnGetUserID = GetUserID;
	plugin_interface.pfnIndexFromText = IndexFromText;
	plugin_interface.pfnGet_Credits = GetCredits;
	plugin_interface.pfnSet_Credits = SetCredits;
	plugin_interface.pfnGet_Time = GetTime;
	plugin_interface.pfnGet_OrigName = GetOrigName;
	plugin_interface.pfnGet_Rank = PluginGetRank;
	plugin_interface.pfnSet_Ammo_Shells = Set_Ammo_Shells;
	plugin_interface.pfnGet_Ammo_Shells = Get_Ammo_Shells;
	plugin_interface.pfnSet_Ammo_Nails = Set_Ammo_Nails;
	plugin_interface.pfnGet_Ammo_Nails = Get_Ammo_Nails;
	plugin_interface.pfnSet_Ammo_Rockets = Set_Ammo_Rockets;
	plugin_interface.pfnGet_Ammo_Rockets = Get_Ammo_Rockets;
	plugin_interface.pfnSet_Ammo_Cells = Set_Ammo_Cells;
	plugin_interface.pfnGet_Ammo_Cells = Get_Ammo_Cells;
	plugin_interface.pfnSet_Ammo_Gren1 = Set_Ammo_Gren1;
	plugin_interface.pfnGet_Ammo_Gren1 = Get_Ammo_Gren1;
	plugin_interface.pfnSet_Ammo_Gren2 = Set_Ammo_Gren2;
	plugin_interface.pfnGet_Ammo_Gren2 = Get_Ammo_Gren2;
	plugin_interface.pfnSet_Ammo_Proxmine = Set_Ammo_Proxmine;
	plugin_interface.pfnGet_Ammo_Proxmine = Get_Ammo_Proxmine;
	plugin_interface.pfnSet_Ammo_Snarks = Set_Ammo_Snarks;
	plugin_interface.pfnGet_Ammo_Snarks = Get_Ammo_Snarks;
	plugin_interface.pfnSet_Ammo_C4 = Set_Ammo_C4;
	plugin_interface.pfnGet_Ammo_C4 = Get_Ammo_C4;
	plugin_interface.pfnSet_Ammo_TripMine = Set_Ammo_Tripmine;
	plugin_interface.pfnGet_Ammo_TripMine = Get_Ammo_Tripmine;
	plugin_interface.pfnReset_Build = ResetBuild;
	plugin_interface.pfnGag = Plugin_Gag;
	plugin_interface.pfnUngag = Plugin_UnGag;
	plugin_interface.pfnInvis = Plugin_Invis;
	plugin_interface.pfnGodmode = Plugin_Godmode;
	plugin_interface.pfnNoclip = Plugin_Noclip;
	plugin_interface.pfnGlow = Plugin_Glow;
	plugin_interface.pfnSet_Playerclass = Set_Playerclass;
	plugin_interface.pfnGet_Playerclass = Get_Playerclass;
	plugin_interface.pfnSet_Armor = Set_Armor;
	plugin_interface.pfnGet_Armor = Get_Armor;
	plugin_interface.pfnSet_Health = Set_Health;
	plugin_interface.pfnGet_Health = Get_Health;
	plugin_interface.pfnSet_CustomSkin = Set_CustomSkin;
	plugin_interface.pfnGet_CustomSkin = Get_CustomSkin;
	plugin_interface.pfnSet_Admin_Access = Set_AdminAccess;
	plugin_interface.pfnGet_Admin_Access = Get_AdminAccess;
	plugin_interface.pfnSet_View = Set_View;
	plugin_interface.pfnColor_Trail = Plugin_Color_Trail;
	plugin_interface.pfnClient_Command = Plugin_Client_Command;
	plugin_interface.pfnSet_Origin = Plugin_Set_Origin;
	plugin_interface.pfnGet_Origin = Plugin_Get_Origin;
	plugin_interface.pfnHurt = Plugin_Hurt;
	plugin_interface.pfnSet_Velocity = Plugin_Set_Velocity;
	plugin_interface.pfnGet_Client_KeyValue = Get_Client_KeyValue;
	plugin_interface.pfnSet_Client_KeyValue = Set_Client_KeyValue;
	plugin_interface.pfnHasFencepost = HasFencepost;
	plugin_interface.pfnHasManualturret = HasManualTurret;
	plugin_interface.pfnHasC4 = HasC4;
	plugin_interface.pfnHasProxmine = HasProxmine;
	plugin_interface.pfnHasTripmine = HasTripmine;
	plugin_interface.pfnInStealth = InStealth;
	plugin_interface.pfnHasPackbomb = HasPackBomb;
	plugin_interface.pfnHasSuperflame = HasSuperFlame;
	plugin_interface.pfnHasSentrygun = HasSentryGun;
	plugin_interface.pfnHasDispenser = HasDispenser;
	
	//Server funcs
	plugin_interface.pfnTextMessage = TextMessage;
	plugin_interface.pfnColorMessage = ColorMessage;
	plugin_interface.pfnScreenShake = Plugin_ScreenShake;
	plugin_interface.pfnScreenFade = Plugin_ScreenFade;
	plugin_interface.pfnStartMapVote = Plugin_StartMapVote;
	plugin_interface.pfnEndMapVote = Plugin_EndMapVote;
	plugin_interface.pfnCancelMapVote = Plugin_CancelMapVote;
	plugin_interface.pfnResponseBotSay = Plugin_ResponseBotSay;
	plugin_interface.pfnLog_Write = Plugin_LogWrite;
	plugin_interface.pfnGet_Cvar = Get_Cvar;
	plugin_interface.pfnSet_CvarString = Set_CvarString;
	plugin_interface.pfnSet_CvarFloat = Set_CvarFloat;
	plugin_interface.pfnCurrentMap = Get_Currentmap;
	plugin_interface.pfnNextMap = Get_Nextmap;
	plugin_interface.pfnServer_Command = Plugin_Server_Command;
	plugin_interface.pfnGetBotName1 = Get_BotName1;
	plugin_interface.pfnGetBotName2 = Get_BotName2;
	plugin_interface.pfnPlayerCount = Plugin_PlayerCount;
	plugin_interface.pfnPointTo = PointTo;
	plugin_interface.pfnSpawnGrunt = Plugin_Grunt;
	plugin_interface.pfnSpawnGarg = Plugin_Garg;
	plugin_interface.pfnGet_Server_Time = Plugin_Get_Time;
	plugin_interface.pfnMessageBox = Plugin_MessageBox;
	plugin_interface.pfnWriteBackAccounts = Plugin_WriteBack;
	plugin_interface.pfnCreateLight = Plugin_CreateLight;
	plugin_interface.pfnCreateEntity = Plugin_CreateEntity;
	plugin_interface.pfnLoadEntLayout = Plugin_LoadEntLayout;
	plugin_interface.pfnUnloadEntLayout = Plugin_UnloadEntLayout;
	plugin_interface.pfnTriggerEntity = Plugin_TriggerEntity;
	plugin_interface.pfnRandom_Float = Plugin_RandomFloat;
	plugin_interface.pfnRandom_Long = Plugin_RandomLong;
	plugin_interface.pfnSkinNameFromIndex = SkinNameFromIndex;
	plugin_interface.pfnGet_TotalSkins = Get_TotalSkins;
}

// --- Generic functions
edict_t *GetPlayerFromIndex(int iPlIndex)
{
	if((iPlIndex < 1) || (iPlIndex > gpGlobals->maxClients))
		return NULL;

	return INDEXENT(iPlIndex);
}

// --- START PLUGIN FUNCTIONS
float GetPluginInterfaceVer()
{
	return PLUGIN_INTERFACE_VER;
}

char* GetName(int iPlIndex)
{
	static char szPlayerName[50];

	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		strcpy(szPlayerName,STRING(pPlayer->v.netname));
		return szPlayerName;
	} else {
		return "";
	}
}

char* GetAuthID(int iPlIndex)
{
	static char szAuthID[50];

	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		strcpy(szAuthID,GETPLAYERAUTHID(pPlayer));
		return szAuthID;
	} else {
		return "";
	}
}

int GetUserID(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);

	int iRetVal = 0;
	
	if(pPlayer != NULL)
	{
		iRetVal = GETPLAYERUSERID(pPlayer);
		return iRetVal;
	} else {
		return 0;
	}
}

int	IndexFromText(char *szName)
{
	return GetPartialPlayerIndex(szName,-1);
}

int GetCredits(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
		return PlayerInfo[iPlIndex].LoadedInfo.Credits;

	return -1;
}

int SetCredits(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PlayerInfo[iPlIndex].LoadedInfo.Credits = iAmount;
		return 0;
	}

	return -1;
}

long GetTime(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
		return PlayerInfo[iPlIndex].LoadedInfo.TimeLen ;

	return -1;
}

char* GetOrigName(int iPlIndex)
{
	static char szOrigName[50];

	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		strcpy(szOrigName,PlayerInfo[iPlIndex].OrigName);
		return szOrigName;
	} else {
		return "";
	}
}

char* PluginGetRank(int iPlIndex)
{
	static char szRank[50];

	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		strcpy(szRank,STRING(GetRank(PlayerInfo[iPlIndex].LoadedInfo.Credits)));
		return szRank;
	} else {
		return "";
	}
}

int	Set_Ammo_Shells(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PatchpvPrivateInt(pPlayer,192,iAmount);
		return 0;
	}
	return -1;
}

int Get_Ammo_Shells(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetpvPrivateInt(pPlayer,192);
	}
	return -1;
}

int	Set_Ammo_Nails(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PatchpvPrivateInt(pPlayer,200,iAmount);
		return 0;
	}
	return -1;
}

int Get_Ammo_Nails(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetpvPrivateInt(pPlayer,200);
	}
	return -1;
}

int	Set_Ammo_Rockets(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PatchpvPrivateInt(pPlayer,216,iAmount);
		return 0;
	}
	return -1;
}

int Get_Ammo_Rockets(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetpvPrivateInt(pPlayer,216);
	}
	return -1;
}

int	Set_Ammo_Cells(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PatchpvPrivateInt(pPlayer,208,iAmount);
		return 0;
	}
	return -1;
}

int Get_Ammo_Cells(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetpvPrivateInt(pPlayer,208);
	}
	return -1;
}

int	Set_Ammo_Gren1(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PatchpvPrivateInt(pPlayer,56,iAmount);
		return 0;
	}
	return -1;
}

int Get_Ammo_Gren1(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetpvPrivateInt(pPlayer,56);
	}
	return -1;
}

int	Set_Ammo_Gren2(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PatchpvPrivateInt(pPlayer,60,iAmount);
		return 0;
	}
	return -1;
}

int Get_Ammo_Gren2(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetpvPrivateInt(pPlayer,60);
	}
	return -1;
}

int	Set_Ammo_Proxmine(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PlayerInfo[iPlIndex].ProxMineAmmo = iAmount;
		return 0;
	}
	return -1;
}

int Get_Ammo_Proxmine(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return PlayerInfo[iPlIndex].ProxMineAmmo;
	}
	return -1;
}

int	Set_Ammo_Snarks(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PlayerInfo[iPlIndex].SnarkAmmo = iAmount;
		return 0;
	}
	return -1;
}

int Get_Ammo_Snarks(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return PlayerInfo[iPlIndex].SnarkAmmo;
	}
	return -1;
}

int	Set_Ammo_C4(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PlayerInfo[iPlIndex].C4Ammo = iAmount;
		return 0;
	}
	return -1;
}

int Get_Ammo_C4(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return PlayerInfo[iPlIndex].C4Ammo;
	}
	return -1;
}

int	Set_Ammo_Tripmine(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PlayerInfo[iPlIndex].TripMineAmmo = iAmount;
		return 0;
	}
	return -1;
}

int Get_Ammo_Tripmine(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return PlayerInfo[iPlIndex].TripMineAmmo;
	}
	return -1;
}

int ResetBuild(int iPlIndex, int iBuildType)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	switch (iBuildType) {
	
	case BUILDTYPE_SENTRYGUN:
		*(word *)((char *)pPlayer->pvPrivateData + 326) = 0x0000;
		return 0;
		break;

	case BUILDTYPE_DISPENSER:
		*(word *)((char *)pPlayer->pvPrivateData + 322) = 0x0000;
		return 0;
		break;

	case BUILDTYPE_LASERSENSOR:
		PlayerInfo[iPlIndex].HasFencePost = 0;
		return 0;
		break;

	case BUILDTYPE_MANUALTURRET:
		PlayerInfo[iPlIndex].HasBigGun = 0;
		return 0;
		break;

	default:
		return -1;
	}

	return -1;
}

int Plugin_Gag(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	PlayerInfo[iPlIndex].LoadedInfo.CanTalk = 1;
	return 0;
}

int Plugin_UnGag(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	PlayerInfo[iPlIndex].LoadedInfo.CanTalk = 0;
	return 0;
}

int	Plugin_Invis(int iPlIndex, int iSwitch)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	if(GetPlayerSkinIndex(pPlayer) > -1)
		return -2;
	
	if(iSwitch == SWITCH_ON)
	{
		pPlayer->v.rendermode = 1;
		pPlayer->v.renderamt = 0;
	} else {
		pPlayer->v.rendermode = 0;
		pPlayer->v.renderamt = 255;
	}
	return 0;
}

int	Plugin_Godmode(int iPlIndex, int iSwitch)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	if(iSwitch == SWITCH_ON)
	{
		pPlayer->v.takedamage = DAMAGE_NO;
	} else {
		pPlayer->v.takedamage = DAMAGE_AIM;
	}
	return 0;
}

int Plugin_Noclip(int iPlIndex, int iSwitch)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	if(iSwitch == SWITCH_ON)
	{
		pPlayer->v.movetype = MOVETYPE_NOCLIP;
	} else {
		pPlayer->v.movetype = MOVETYPE_WALK;
	}
	return 0;
}

int Plugin_Glow(int iPlIndex, int r, int b, int g, int iSize)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	pPlayer->v.rendermode = kRenderNormal;
	pPlayer->v.renderfx = kRenderFxGlowShell;
	pPlayer->v.renderamt = iSize;  
	pPlayer->v.rendercolor.x = r;
	pPlayer->v.rendercolor.y = g;
	pPlayer->v.rendercolor.z = b;

	return 0;
}

int	Set_Playerclass(int iPlIndex, int iPlayerclass)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		pPlayer->v.playerclass = iPlayerclass;
		return 0;
	}
	return -1;
}

int Get_Playerclass(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return pPlayer->v.playerclass;
	}
	return -1;
}

int	Set_Armor(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		pPlayer->v.armorvalue = iAmount;
		return 0;
	}
	return -1;
}

int Get_Armor(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return pPlayer->v.armorvalue;
	}
	return -1;
}

int	Set_Health(int iPlIndex, int iAmount)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		pPlayer->v.health = iAmount;
		return 0;
	}
	return -1;
}

int Get_Health(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return pPlayer->v.health;
	}
	return -1;
}

int	Set_CustomSkin(int iPlIndex, int iSkinIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		ApplySkin(pPlayer, iSkinIndex);
		return 0;
	}
	return -1;
}

int Get_CustomSkin(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return GetPlayerSkinIndex(pPlayer);
	}
	return -1;
}

int	Set_AdminAccess(int iPlIndex, int iAdminLevel)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		PlayerInfo[iPlIndex].IsAdmin = iAdminLevel;
		return 0;
	}
	return -1;
}

int Get_AdminAccess(int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer != NULL)
	{
		return PlayerInfo[iPlIndex].IsAdmin;
	}
	return -1;
}

int Set_View(int iPlIndex, int iView)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	if(iView == VIEW_3RDPERSON)
		Start3rdPerson(pPlayer);
	else
		End3rdPerson(pPlayer);
	return 0;
}

int Plugin_Color_Trail(int iPlIndex, int r, int g, int b)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	//Find index to players skin entity
	CBaseEntity *pSkinEnt = NULL;
	pSkinEnt = UTIL_FindEntityByTargetname( NULL, g_SkinNames[iPlIndex]);

	if(pSkinEnt)
	{
		short sprite;
		sprite = PRECACHE_MODEL("sprites/laserbeam.spr");
		MESSAGE_BEGIN(MSG_ALL, SVC_TEMPENTITY);
			WRITE_BYTE(TE_BEAMFOLLOW);
			WRITE_SHORT(pSkinEnt->entindex());
			WRITE_SHORT(sprite);
			WRITE_BYTE(100);
			WRITE_BYTE(25);
			WRITE_BYTE(r);
			WRITE_BYTE(g);
			WRITE_BYTE(b);
			WRITE_BYTE(150);
		MESSAGE_END();	
		return 0;
	}
	return -1;
}

int Plugin_Client_Command(int iPlIndex, char *szCommand)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	CLIENT_COMMAND(pPlayer, szCommand);
	return 0;
}

int Plugin_Set_Origin(int iPlIndex, int x, int y, int z)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	pPlayer->v.origin.x = x;
	pPlayer->v.origin.y = y;
	pPlayer->v.origin.z = z;
	return 0;
}

int Plugin_Get_Origin(int iPlIndex, int &x, int &y, int &z)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	x = pPlayer->v.origin.x;
	y = pPlayer->v.origin.y;
	z = pPlayer->v.origin.z;
	return 0;
}

int Plugin_Hurt(int iPlIndex, int iDmg, int iDmgType)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	TakeDamage(pPlayer,NULL,(float)iDmg,iDmgType);
	return 0;
}

int Plugin_Set_Velocity(int iPlIndex, float x, float y, float z)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	pPlayer->v.velocity.x = x;
	pPlayer->v.velocity.y = y;
	pPlayer->v.velocity.z = z;
	return 0;
}

char* Get_Client_KeyValue(int iPlIndex, char *szKeyname)
{
	static char string[1024];

	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return "";

	char *infobuffer=g_engfuncs.pfnGetInfoKeyBuffer(pPlayer);
	strcpy(string,g_engfuncs.pfnInfoKeyValue(infobuffer, szKeyname));

	return string;
}

int	Set_Client_KeyValue(int iPlIndex, char *szKeyname, char *szValue)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	char *infobuffer=g_engfuncs.pfnGetInfoKeyBuffer(pPlayer);
	g_engfuncs.pfnSetClientKeyValue(ENTINDEX(pPlayer), infobuffer, szKeyname, szValue);

	return 0;
}

BOOL HasFencepost (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	if(PlayerInfo[iPlIndex].HasFencePost == 1)
		return TRUE;
	return FALSE;
}

BOOL HasManualTurret (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	if(PlayerInfo[iPlIndex].HasBigGun == 1)
		return TRUE;
	return FALSE;
}

BOOL HasC4 (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_c4"))
	{
        if(pMine->pev->euser1 == pPlayer)
			return TRUE;
	}
	return FALSE;
}

BOOL HasProxmine (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_proximity"))
	{
        if(pMine->pev->euser1 == pPlayer)
			return TRUE;
	}
	return FALSE;
}

BOOL HasTripmine (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_tripmine"))
	{
        if(pMine->pev->euser1 == pPlayer)
			return TRUE;
	}
	return FALSE;
}

BOOL InStealth (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	if(PlayerInfo[iPlIndex].StealthOn == 1)
			return TRUE;
	return FALSE;
}

BOOL HasPackBomb (int iPlIndex)
{
	return FALSE;
}

BOOL HasSuperFlame (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	CBaseEntity *pMine = NULL;
    while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_superflame"))
	{
        if(pMine->pev->euser1 == pPlayer)
			return TRUE;
	}
	return FALSE;
}

BOOL HasSentryGun (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	if(*(byte *)((char *)pPlayer->pvPrivateData + 326) == 0x00)
		return FALSE;
	return TRUE;
}

BOOL HasDispenser (int iPlIndex)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return FALSE;

	if(*(byte *)((char *)pPlayer->pvPrivateData + 322) == 0x00)
		return FALSE;
	return TRUE;
}

int TextMessage(int iPlIndex, int iPrintType, char *format, ...)
{
	va_list         argptr;
	static char		string[512];

	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	if(strlen(string) > 0)
	{
		int HUD_TEMP = 0;
		PRINT_TYPE print_temp;

		edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);

		if(iPrintType == plgprint_console)
		{
			HUD_TEMP = 2;
			print_temp = print_console;
		}
		if(iPrintType == plgprint_center)
		{
			HUD_TEMP = 4;
			print_temp = print_center;
		}
		if(iPrintType == plgprint_chat)
		{
			HUD_TEMP = 3;
			print_temp = print_chat;
		}
	
		if(pPlayer != NULL)
			CLIENT_PRINTF( pPlayer, print_temp, string);
		else
			UTIL_ClientPrintAll( HUD_TEMP, string);

		return 0;
	}
	return -1;
}

int ColorMessage(int iPlIndex, int x, int y, int r1, int g1, int b1, int r2, int g2, int b2, int time, char *format, ...)
{
	va_list         argptr;
	static char		string[256];

	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	if(strlen(string) > 0)
	{
		hudtextparms_s m_textParms;

		m_textParms.r1 = r1;
		m_textParms.g1 = g1;
		m_textParms.b1 = b1;
		m_textParms.a1 = 0;
		m_textParms.r2 = r2;
		m_textParms.g2 = g2;
		m_textParms.b2 = b2;
		m_textParms.a2 = 0;
		m_textParms.fadeinTime = 0.02;
		m_textParms.fadeoutTime = 1.0;
		m_textParms.holdTime = time;   
		m_textParms.fxTime = (time / 3);
		m_textParms.x = x;
		m_textParms.y = y;
		m_textParms.effect= 2;
		m_textParms.channel=1;

		edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
		if(pPlayer != NULL)
		{
			CBaseEntity *pPlayerBase = UTIL_PlayerByIndex(iPlIndex);
			UTIL_HudMessage(pPlayerBase,m_textParms,string);
		} else {
			UTIL_HudMessageAll( m_textParms,string );
		}
		return 0;
	}
	return -1;
}

int Plugin_ScreenShake(float flAmplitude)
{
	UTIL_ScreenShakeAll( Vector(0,0,0), flAmplitude, 3.0, 5.0);
	return 0;
}

int Plugin_ScreenFade(int r, int g, int b, int alpha)
{
	UTIL_ScreenFadeAll( Vector(r,g,b), 8, 2, alpha, FFADE_IN );
	return 0;
}

void Plugin_StartMapVote()
{
	VoteStart(NULL);
}

void Plugin_EndMapVote()
{
	VoteEnd();
}

void Plugin_CancelMapVote()
{
	CancelMapVote();
}

int Plugin_ResponseBotSay(char *format, ...)
{
	va_list         argptr;
	static char             string[512];

	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	if(strlen(string) > 0)
	{
		UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("[%s]: %s\n",BOT_NAME,string));
		UTIL_LogPrintf("[AUTOOP] bot_say \"[%s]: %s\"\n",BOT_NAME,string); 
		return 0;
	}
	return -1;
}

int Plugin_LogWrite(char *szLogLine)
{
	UTIL_LogPrintf(szLogLine);	
	return 0;
}

char* Get_Cvar(char *szCvarKey)
{
	static char string[512];

	strcpy(string,CVAR_GET_STRING(szCvarKey));

	return string;
}

void Set_CvarString(char *szCvarKey, char *szCvarValue)
{
	CVAR_SET_STRING(szCvarKey, szCvarValue);
}

void Set_CvarFloat(char *szCvarKey, float flCvarValue)
{
	CVAR_SET_FLOAT(szCvarKey, flCvarValue);
}

char* Get_Currentmap()
{
	static char string[128];

	strcpy(string,STRING(gpGlobals->mapname));

	return string;
}

char* Get_Nextmap()
{
	static char string[128];

	mapcycle_item_s *item;
	char *mapcfile = (char*)CVAR_GET_STRING( "mapcyclefile" );
	DestroyMapCycle( &mapcycle );
	ReloadMapCycleFile( mapcfile, &mapcycle );
	item=CurrentMap(&mapcycle);

	strcpy(string,item->mapname);

	return string;
}

void Plugin_Server_Command(char *szCommand)
{
	SERVER_COMMAND(szCommand);
	return;
}

char* Get_BotName1()
{
	return BOT_NAME;
}

char* Get_BotName2()
{
	return BOT_NAME2;
}

int Plugin_PlayerCount()
{
	edict_t *pEntity = NULL;
	int iRetVal = 0;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pEntity = INDEXENT(i);

		if(pEntity)
			if(!FStrEq(STRING(pEntity->v.netname),""))
				iRetVal++;
	}

	return iRetVal;
}

int PointTo(int iPlIndex, int iRange)
{
	edict_t *pPlayer = GetPlayerFromIndex(iPlIndex);
	
	if(pPlayer == NULL)
		return -1;

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pPlayer->v.v_angle );

	v_src = pPlayer->v.origin + pPlayer->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * iRange;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pPlayer->v.pContainingEntity, &tr);

	if((tr.flFraction < 1.0) && (tr.pHit != NULL))
	{	
		int iHitIndex = ENTINDEX(tr.pHit);

		edict_t *pPlayer2 = GetPlayerFromIndex(iHitIndex);

		if(pPlayer2 == NULL)
		{
			return -2;
		} else {
			return iHitIndex;
		}
	}
	return -1;
}

void Plugin_Grunt(bool blnCanWalk, int x, int y, int z)
{
	CreateGruntCoords(blnCanWalk,x,y,z);
}

void Plugin_Garg(bool blnCanWalk, int x, int y, int z)
{
	CreateGargCoords(blnCanWalk,x,y,z);
}

float Plugin_Get_Time()
{
	return gpGlobals->time;
}

BOOL Plugin_ValidMap(char *szMapName)
{
	if(IS_MAP_VALID(szMapName))
		return TRUE;
	return FALSE;
}

void Plugin_MessageBox(char *szMessage)
{
	int gmsgMOTDText = REG_USER_MSG( "MOTD", -1);

	if(gmsgMOTDText > MAX_USERMSG)
		return;

	char chTemp[51];

	int iCount = 0;

	for(int i = 0; i <= (int)strlen(szMessage); i++)
	{
		chTemp[iCount] = szMessage[i];

		if(iCount == 50)
		{
			chTemp[51] = '\0';

			MESSAGE_BEGIN( MSG_ALL, gmsgMOTDText );

			if(i < (int)strlen(szMessage))
				WRITE_BYTE (0);
			else
				WRITE_BYTE (1);

			WRITE_STRING( chTemp );
			MESSAGE_END();

			for(int j = 0; j <= 51; j++)
				chTemp[j] = '\0';

			iCount = 0;
		} else {
			iCount++;
		}
	}

	if((int)strlen(chTemp) > 0)
	{
		MESSAGE_BEGIN( MSG_ALL, gmsgMOTDText );
		WRITE_BYTE (1);
		WRITE_STRING( chTemp );
		MESSAGE_END();
	}
}

void Plugin_WriteBack()
{
	WriteBackFiles();
}

void Plugin_CreateLight(int r, int g, int b, int x, int y, int z)
{
	MESSAGE_BEGIN(MSG_ALL, SVC_TEMPENTITY);
		WRITE_BYTE(TE_DLIGHT);
		WRITE_COORD(x);
		WRITE_COORD(y);
		WRITE_COORD(z);
		WRITE_BYTE(25);
		WRITE_BYTE(r);
		WRITE_BYTE(g);
		WRITE_BYTE(b);
		WRITE_BYTE(255);
		WRITE_BYTE(0);
	MESSAGE_END();
}

void Plugin_CreateEntity(int x, int y, int z, int anglex, int angley, int anglez, char *szClassname)
{
	//Now spawn the thing
	edict_t	*pent;

	pent = CREATE_NAMED_ENTITY(ALLOC_STRING(szClassname));
	if ( FNullEnt( pent ) )
	{
		ALERT ( at_console, "NULL Ent in UTIL_PrecacheOther\n" );
		return;
	}

	VARS( pent )->origin = Vector(x,y,z);
	VARS( pent )->angles = Vector(anglex,angley,anglez);

	DispatchSpawn( pent );

}

int	Plugin_LoadEntLayout(char *szLayoutName)
{
	if(!LoadEnts(NULL,szLayoutName))
		return -1;
	return 0;
}

int	Plugin_UnloadEntLayout(char *szLayoutName)
{
	KillAEFEnts(NULL,szLayoutName);
	return 0;
}

void Plugin_TriggerEntity(char *szTargetname)
{
	edict_t	*pTrigger = NULL;
	pTrigger = CREATE_NAMED_ENTITY(MAKE_STRING("trigger_auto"));
	pTrigger->v.origin = Vector(0,0,-4000);
	pTrigger->v.target = MAKE_STRING(szTargetname);
	DispatchSpawn(pTrigger);
}

float Plugin_RandomFloat(float flMin, float flMax)
{
	return RANDOM_FLOAT(flMin, flMax);
}

long Plugin_RandomLong(long lngMin, long lngMax)
{
	return RANDOM_LONG(lngMin, lngMax);
}

char* SkinNameFromIndex(int iSkinIndex)
{
	if((iSkinIndex < 0) || (iSkinIndex > TOTALSKINS))
		return "";

	return SkinLibrary[iSkinIndex].SkinName;
}

int	Get_TotalSkins()
{
	return TOTALSKINS;
}