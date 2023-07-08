/*****************************************************\
*	Auto-OP Shodan Plugin/Extension
*
*   File: plugin_main.h
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

#ifdef PLUGIN_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif

#define PLUGIN_CONTINUE 0
#define PLUGIN_BLOCK 1

static shodanfuncs_t *pEng = NULL;

//Exported functions called from the engine
extern "C" PLUGIN_API void fnPluginGetInterface(shodanfuncs_t *pEngine) { pEng = pEngine; }
extern "C" PLUGIN_API void fnPluginInit();
extern "C" PLUGIN_API void fnPluginEnd();
extern "C" PLUGIN_API void fnPluginThink();

extern "C" PLUGIN_API void fnClientConnect(int iClientIndex, const char *szName, const char *szIPAddress);
extern "C" PLUGIN_API void fnClientDisconnect(int iClientIndex);
extern "C" PLUGIN_API int fnClientCommand(int iClientIndex, const char *szCommand, const char *szArgs);


