/***
*
*	Copyright (c) 1999, 2000 Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef EXTDLL_H
#define EXTDLL_H


//
// Global header file for extension DLLs
//

// Allow "DEBUG" in addition to default "_DEBUG"
#ifdef _DEBUG
#define DEBUG 1
#endif

// Silence certain warnings
#pragma warning(disable : 4244)		// int or float down-conversion
#pragma warning(disable : 4305)		// int or float data truncation
#pragma warning(disable : 4201)		// nameless struct/union
#pragma warning(disable : 4514)		// unreferenced inline function removed
#pragma warning(disable : 4100)		// unreferenced formal parameter

#ifdef _WIN32
// Prevent tons of unused windows definitions
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#include "WINDOWS.H"
//#include <afxsock.h>
 
// Misc C-runtime library headers
#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"
 
#else
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
 
#define MAX_PATH PATH_MAX
#define ULONG ulong
#define FALSE 0
#define TRUE  1
 
#ifndef max
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#endif
 
#ifndef min
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif
 
#define itoa(a,b,c) sprintf(b, "%d", a)
 
typedef unsigned char BYTE;
#endif

// Header file containing definition of globalvars_t and entvars_t
typedef int	func_t;					//
typedef int	string_t;				// from engine's pr_comp.h;
typedef float vec_t;				// needed before including progdefs.h

// Vector class
#include "vector.h"

// Defining it as a (bogus) struct helps enforce type-checking
#define vec3_t Vector

// Shared engine/DLL constants
#include "const.h"
#include "progdefs.h"
#include "edict.h"

// Shared header describing protocol between engine and DLLs
#include "eiface.h"

// Shared header between the client DLL and the game DLLs
#include "cdll_dll.h"

// Shared engine/DLL constants

#define DLL_GLOBAL    
#define CMD_ARGV                              (*g_engfuncs.pfnCmd_Argv)  
#define CMD_ARGS                                       (*g_engfuncs.pfnCmd_Args)
#define SERVER_COMMAND (*g_engfuncs.pfnServerCommand)  
#define ALERT                  (*g_engfuncs.pfnAlertMessage)
#define FREE_FILE                              (*g_engfuncs.pfnFreeFile)
#define LOAD_FILE_FOR_ME               (*g_engfuncs.pfnLoadFileForMe)
#define CVAR_GET_STRING        (*g_engfuncs.pfnCVarGetString) 

#define ASSERT(f)
#define ASSERTSZ(f, sz)    
#define STRING(offset)           (char *)(gpGlobals->pStringBase + (int)offset) 
#define MESSAGE_END            (*g_engfuncs.pfnMessageEnd)  
#define CLIENT_PRINTF             (*g_engfuncs.pfnClientPrintf) 
#define IS_MAP_VALID                   (*g_engfuncs.pfnIsMapValid)
#define CHANGE_LEVEL   (*g_engfuncs.pfnChangeLevel) 
#define WRITE_BYTE             (*g_engfuncs.pfnWriteByte)
#define WRITE_CHAR             (*g_engfuncs.pfnWriteChar)
#define WRITE_SHORT            (*g_engfuncs.pfnWriteShort)
#define WRITE_LONG             (*g_engfuncs.pfnWriteLong)
#define WRITE_ANGLE            (*g_engfuncs.pfnWriteAngle)
#define WRITE_COORD            (*g_engfuncs.pfnWriteCoord)
#define WRITE_STRING   (*g_engfuncs.pfnWriteString)
#define WRITE_ENTITY   (*g_engfuncs.pfnWriteEntity)  
#define CVAR_SET_FLOAT (*g_engfuncs.pfnCVarSetFloat) 
#define CVAR_SET_STRING        (*g_engfuncs.pfnCVarSetString)  
#define ALLOC_STRING   (*g_engfuncs.pfnAllocString)  
#define CVAR_REGISTER  (*g_engfuncs.pfnCVarRegister) 
#define REG_USER_MSG                           (*g_engfuncs.pfnRegUserMsg)
#define GET_GAME_DIR                   (*g_engfuncs.pfnGetGameDir) 
#define FIND_ENTITY_BY_STRING  (*g_engfuncs.pfnFindEntityByString)  
#define GETPLAYERUSERID (*g_engfuncs.pfnGetPlayerUserId) 
#define CMD_ARGC            (*g_engfuncs.pfnCmd_Argc)
#define CVAR_GET_FLOAT (*g_engfuncs.pfnCVarGetFloat)
#define GETPLAYERWONID (*g_engfuncs.pfnGetPlayerWONId)
#define GETPLAYERAUTHID (*g_engfuncs.pfnGetPlayerAuthId)
#define NUMBER_OF_ENTITIES             (*g_engfuncs.pfnNumberOfEntities)  
#define PLAYER_CNX_STATS            ( *g_engfuncs.pfnGetPlayerStats ) 
#define ADD_SERVER_COMMAND (*g_engfuncs.pfnAddServerCommand )

extern void PrecacheResources();

//MAPCYCLE Stuff
#define MAX_RULE_BUFFER 1024

typedef struct mapcycle_item_s
{
	struct mapcycle_item_s *next;

	char mapname[ 32 ];
	int  minplayers, maxplayers;
	char rulebuffer[ MAX_RULE_BUFFER ];
} mapcycle_item_t;

typedef struct mapcycle_s
{
	struct mapcycle_item_s *items;
	struct mapcycle_item_s *next_item;
} mapcycle_t;


void DestroyMapCycle( mapcycle_t *cycle );
int ReloadMapCycleFile( char *filename, mapcycle_t *cycle );
int allowed_map(char *map);
int check_map(char *map);
int listmaps(edict_t *);
mapcycle_item_s *CurrentMap(mapcycle_t *cycle);

int GetSGTeam(edict_t *pEntity);

extern void FormatBuffer(char *pData, int length, char *pBuffer);
extern void PatchpvPrivateInt(edict_t *pEntity, int offset, int value);
extern int GetpvPrivateInt(edict_t *pEntity, int offset);

extern void DecalInit();
extern int UTIL_CountTeam(int iTeam);
extern bool TeamChangeValid(int iTeam, int iCurrent);
extern int UTIL_CountPlayers();
extern void UTIL_PlayerStatusTracelines();
extern int UTIL_BuildingPercentageHealth(float iHealth, float iMaximum);
extern void UTIL_PlayerRepairTracelines(edict_t *PlEdict);
extern void UTIL_SendCustomBucketMessages(edict_t *pEntity);
extern void UTIL_ShowMenu(edict_t *pEntity, int bitsValidSlots, int nDisplayTime, BOOL fNeedMore, char *pszText);
extern void UTIL_RotateSG(edict_t *pPlayer, BOOL Rotate180);
extern void PlayRandomLoop();
extern int SendICQ(edict_t *pCaller, char *From, char *FromEmail, char *Subject, char *Body, char *To);
extern void SendStandardICQ(char *To, char *Subject, char *Body);
extern void LoadGameSettings();
extern void LoadCommandLevels();

extern void strreplace(char* source, char* replace, char* with);
extern int ifindstr(int startx,char* body,char* search);
extern void leftcopy(char* input,char* output,int pos);
extern void rightcopy(char* input,char* output,int pos);

#endif //EXTDLL_H
