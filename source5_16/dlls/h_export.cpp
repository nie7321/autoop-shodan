/*****************************************************\
*	Auto-OP Shodan
*
*	File: h_export.cpp
*	Description: Core file.
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*   Based on work by Botman (http://www.planethalflife.com/botman)
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

#include <direct.h>

#include "extdll.h"

#include "stripper.h"

#include "util.h"

#include "CIniFile.h"

#include "engine.h"


HINSTANCE h_Library = NULL;


// Required DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
   if (fdwReason == DLL_PROCESS_ATTACH)
   {
   }
   else if (fdwReason == DLL_PROCESS_DETACH)
   {
      if (h_Library)
         FreeLibrary(h_Library);
   }

   return TRUE;
}


enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;


typedef int (FAR *GETENTITYAPI)(DLL_FUNCTIONS *, int);
typedef void (DLLEXPORT *GIVEFNPTRSTODLL)(enginefuncs_t *, globalvars_t *);

GETENTITYAPI other_GetEntityAPI;
GIVEFNPTRSTODLL other_GiveFnptrsToDll;

extern int mod_id;


void DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals )
{
   int pos;
   char game_dir[256];
   char mod_name[32];

   // get the engine functions from the engine...

   memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
   gpGlobals = pGlobals;

   // find the directory name of the currently running MOD...
   (*g_engfuncs.pfnGetGameDir)(game_dir);

   pos = strlen(game_dir) - 1;

   // scan backwards till first directory separator...
   while ((pos) && (game_dir[pos] != '/'))
      pos--;

   if (pos == 0)
   {
      // FIGURE OUT HOW TO HANDLE ERRORS HERE!!!
   }

   pos++;
   strcpy(mod_name, &game_dir[pos]);

   /*if (strcmpi(mod_name, "valve") == 0)
   {
      mod_id = VALVE_DLL;
      h_Library = LoadLibrary("valve\\dlls\\hl.dll");
   }*/

   if (strcmpi(mod_name, "tfc") == 0)
   {
	   	//Load the gamedll from the INI file
		CIniFile iniFile;
		iniFile.SetFileName("tfc\\autoop\\shodan.ini");
		char iniGameDLL[150];
		iniFile.GetStringD("General","gamedll",iniGameDLL,sizeof(iniGameDLL),"tfc\\dlls\\tfc.dll");

      mod_id = TFC_DLL;

	  //h_Library = LoadLibrary("tfc\\dlls\\tfc.dll");
	  h_Library = LoadLibrary(iniGameDLL);
   }

   /*else if (strcmpi(mod_name, "cstrike") == 0)
   {
      mod_id = CS_DLL;
      h_Library = LoadLibrary("cstrike\\dlls\\mp.dll");
   }

   else if (strcmpi(mod_name, "svencoop") == 0)
   {
      mod_id = SVEN_DLL;
      h_Library = LoadLibrary("svencoop\\dlls\\hl.dll");
   }

   else if (strcmpi(mod_name, "firearms") == 0)
   {
      mod_id = FA_DLL;
      h_Library = LoadLibrary("firearms\\dlls\\firearms.dll");
   }*/

   if (h_Library == NULL)
   {
      // FIGURE OUT HOW TO HANDLE ERRORS HERE!!!
   }


   other_GetEntityAPI = (GETENTITYAPI)GetProcAddress(h_Library, "GetEntityAPI");

   if (other_GetEntityAPI == NULL)
   {
      // FIGURE OUT HOW TO HANDLE ERRORS HERE!!!
   }

   other_GiveFnptrsToDll = (GIVEFNPTRSTODLL)GetProcAddress(h_Library, "GiveFnptrsToDll");

   if (other_GiveFnptrsToDll == NULL)
   {
      // FIGURE OUT HOW TO HANDLE ERRORS HERE!!!
   }

   // give the engine functions to the other DLL...
   pengfuncsFromEngine->pfnRegUserMsg = pfnRegUserMsg;
   pengfuncsFromEngine->pfnMessageBegin = pfnMessageBegin;
   pengfuncsFromEngine->pfnMessageEnd = pfnMessageEnd;
   pengfuncsFromEngine->pfnWriteByte = pfnWriteByte;
   pengfuncsFromEngine->pfnWriteChar = pfnWriteChar;
   pengfuncsFromEngine->pfnWriteShort = pfnWriteShort;
   pengfuncsFromEngine->pfnWriteLong = pfnWriteLong;
   pengfuncsFromEngine->pfnWriteAngle = pfnWriteAngle;
   pengfuncsFromEngine->pfnWriteCoord = pfnWriteCoord;
   pengfuncsFromEngine->pfnWriteString = pfnWriteString;
   pengfuncsFromEngine->pfnWriteEntity = pfnWriteEntity;

   (*other_GiveFnptrsToDll)(pengfuncsFromEngine, pGlobals);
}

