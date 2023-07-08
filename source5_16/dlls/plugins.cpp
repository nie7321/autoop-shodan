/*****************************************************\
*	Auto-OP Shodan
*
*	File: plugins.cpp
*	Description: Code for loading/processing the AOX files.
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
#include "plugins.h"
#include "tchar.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern int team_allies[4];
extern TFXFeatureEnabled_s Features;

#define MAX_PLUGINS 50
int iNextFreeSpace = 0;

extern shodanfuncs_t plugin_interface;

HINSTANCE m_dwa[MAX_PLUGINS];

//Gets the next free plugin space, returns -1 if its full
int GetNextFree()
{
	return -1; //No more plugin spaces!
}

BOOL GetProcAddresses( HINSTANCE *hLibrary, 
    LPCSTR lpszLibrary, INT nCount, ... )
{
    va_list va;
    va_start( va, nCount );

    if ( ( *hLibrary = LoadLibrary( lpszLibrary ) ) 
        != NULL )
    {
        FARPROC * lpfProcFunction = NULL;
        LPSTR lpszFuncName = NULL;
        INT nIdxCount = 0;
        while ( nIdxCount < nCount )
        {
            lpfProcFunction = va_arg( va, FARPROC* );
            lpszFuncName = va_arg( va, LPSTR );
            if ( ( *lpfProcFunction = 
                GetProcAddress( *hLibrary, 
                    lpszFuncName ) ) == NULL )
            {
                lpfProcFunction = NULL;
                return FALSE;
            }
            nIdxCount++;
        }
    }
    else
    {
        va_end( va );
        return FALSE;
    }
    va_end( va );
    return TRUE;
}

BOOL GetProcAddresses2( HINSTANCE *hLibrary, 
    INT nCount, ... )
{
    va_list va;
    va_start( va, nCount );

    FARPROC * lpfProcFunction = NULL;
    LPSTR lpszFuncName = NULL;
    INT nIdxCount = 0;
    while ( nIdxCount < nCount )
    {
        lpfProcFunction = va_arg( va, FARPROC* );
        lpszFuncName = va_arg( va, LPSTR );
        if ( ( *lpfProcFunction = 
            GetProcAddress( *hLibrary, 
                lpszFuncName ) ) == NULL )
        {
            lpfProcFunction = NULL;
            return FALSE;
        }
        nIdxCount++;
    }

    va_end( va );
    return TRUE;
}

void LoadPlugins()
{
	char szLineTemp[200];
	int ch,i;

	FILE *Pluginfp;
	Pluginfp=fopen("tfc\\autoop\\plugins.ini","r");

	if(Pluginfp == NULL)
		return;

	ch = fgetc(Pluginfp);

	// skip any leading blanks
	while (ch == ' ')
		ch = fgetc(Pluginfp);

	szLineTemp[0] = '\0';
	i = 0;

	while (ch != EOF)
	{
		if((ch == '\r') || (ch == '\n'))
		{
			if(strlen(szLineTemp) > 1)
			{
				if(szLineTemp[0] != '#')
					LoadAPlugin(szLineTemp);
			}

			memset(szLineTemp, 0, sizeof(szLineTemp));
			szLineTemp[0] = '\0';
			i = 0;
		} else {
			szLineTemp[i] = ch;
			i++;
		}
		ch = fgetc(Pluginfp);
	}
	
	if(strlen(szLineTemp) > 1)
	{
		if(strncmp("\\\\",szLineTemp,2) != 0)
			LoadAPlugin(szLineTemp);
	}

	fclose(Pluginfp);
	return;
}

void LoadAPlugin(char *DLLName)
{
	if(iNextFreeSpace == MAX_PLUGINS)
	{
		UTIL_LogPrintf( "[AUTOOP] Ignoring load of plugin \"%s\", no free plugin space.\n", DLLName);
		return;
	}

	HINSTANCE hm = LoadLibrary(DLLName);

	if ( !hm )
	{
		UTIL_LogPrintf( "[AUTOOP] Tried to load plugin \"%s\", load failed.\n", DLLName);
	}
	else
	{
		//Copy instance to memory
		m_dwa[iNextFreeSpace] = hm;

		//Call the plugins init function
		PFUNCGETINIT pFuncGetInit = NULL;
		PFUNCINIT pFuncInit = NULL;

		if(GetProcAddresses2( &m_dwa[iNextFreeSpace], 2, &pFuncGetInit, "fnPluginGetInterface", &pFuncInit, "fnPluginInit"))
		{
			//Load was successful
			UTIL_LogPrintf( "[AUTOOP] Loaded plugin \"%s\".\n", DLLName);
			pFuncGetInit(&plugin_interface);
			pFuncInit();
			iNextFreeSpace++; //Move the next free space along.
		} else {
			//Could not find the init function, so we unload this plugin
			UTIL_LogPrintf( "[AUTOOP] Plugin \"%s\" seems to not be an AutoOP plugin.\n", DLLName);
			FreeLibrary(m_dwa[iNextFreeSpace]);
		}
	}
}

void UnloadPlugins()
{
	if(iNextFreeSpace == 0)
		return; //No plugins to process

	PFUNCEND pFuncEnd = NULL;

	for(int i = 0; i < iNextFreeSpace; i++)
	{
		if(GetProcAddresses2( &m_dwa[i], 1, &pFuncEnd, "fnPluginEnd"))
			pFuncEnd();
		FreeLibrary(m_dwa[i]);
	}
}

void PluginsThink()
{
	if(iNextFreeSpace == 0)
		return; //No plugins to process

	PFUNCTHINK pFuncThink = NULL;

	for(int i = 0; i < iNextFreeSpace; i++)
	{
		if(GetProcAddresses2( &m_dwa[i], 1, &pFuncThink, "fnPluginThink"))
			pFuncThink();
	}
}

void PluginsClientDisconnect(int iClientIndex)
{
	if(iNextFreeSpace == 0)
		return; //No plugins to process

	PFUNCDISCONNECT pFuncDisconnect = NULL;

	for(int i = 0; i < iNextFreeSpace; i++)
	{
		if(GetProcAddresses2( &m_dwa[i], 1, &pFuncDisconnect, "fnClientDisconnect"))
			pFuncDisconnect(iClientIndex);
	}
}

void PluginsClientConnect(int iClientIndex, const char *szName, const char *szIPAddress)
{
	if(iNextFreeSpace == 0)
		return; //No plugins to process

	PFUNCCONNECT pFuncConnect = NULL;

	for(int i = 0; i < iNextFreeSpace; i++)
	{
		if(GetProcAddresses2( &m_dwa[i], 1, &pFuncConnect, "fnClientConnect"))
			pFuncConnect(iClientIndex,szName,szIPAddress);
	}
}

bool PluginsClientCommand(int iClientIndex, const char *szCommand, const char *szArgs)
{
	if(iNextFreeSpace == 0)
		return 0;

	PFUNCCOMMAND pFunc = NULL;
	int iRetval = 0;

	for(int i = 0; i < iNextFreeSpace; i++)
	{
		if(GetProcAddresses2( &m_dwa[i], 1, &pFunc, "fnClientCommand"))
			iRetval = pFunc(iClientIndex, szCommand, szArgs);
		
		//If plugin wants to block the command
		//Then return 1 now.
		if(iRetval == PLUGIN_BLOCK)
			return 1;
	}
	return 0; //No blocking
}