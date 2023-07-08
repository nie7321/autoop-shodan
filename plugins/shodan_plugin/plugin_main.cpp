/*****************************************************\
*	Auto-OP Shodan Plugin/Extension
*
*   File: plugin_main.cpp
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*   This code may be used freely for the purpose of
*   creating plugins/extensions for AutoOP Shodan.
*
*   You may not use this code for commercial purposes
*   without written permission.
*
\*****************************************************/

#include "stdafx.h"
#include "plugin_api.h"
#include "plugin_main.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

//Called when
PLUGIN_API void fnPluginInit()
{

}

PLUGIN_API void fnPluginEnd()
{

}

PLUGIN_API void fnPluginThink()
{

}

PLUGIN_API void fnClientConnect(int iClientIndex, char *szName, char *szIPAddress)
{

}

PLUGIN_API void fnClientDisconnect(int iClientIndex)
{

}

PLUGIN_API int fnClientCommand(int iClientIndex, const char *szCommand, const char *szArgs)
{
	return PLUGIN_CONTINUE;
}
