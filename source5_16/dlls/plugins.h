/*****************************************************\
*	Auto-OP Shodan
*
*	File: plugins.h
*	Description: AutoOP plugin header
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef PLUGINS_H
#define PLUGINS_H

#include "plugin_api.h"

#define PLUGIN_CONTINUE 0
#define PLUGIN_BLOCK 1

typedef int (*PFUNC)(void);
typedef void (*PFUNCINIT)(void);
typedef void (*PFUNCEND)(void);
typedef void (*PFUNCTHINK)(void);
typedef void (*PFUNCDISCONNECT)(int iClientIndex);
typedef void (*PFUNCCONNECT)(int iClientIndex, const char *szName, const char *szIPAddress);
typedef int (*PFUNCCOMMAND)(int iClientIndex, const char *szCommand, const char *szArgs);
typedef void (*PFUNCGETINIT)(shodanfuncs_t *pEngine);

extern void LoadPlugins();
extern void LoadAPlugin(char *DLLName);
extern void PluginsThink();
extern bool PluginsClientCommand(int iClientIndex, const char *szCommand, const char *szArgs);
extern void UnloadPlugins();
extern void PluginsClientConnect(int iClientIndex, const char *szName, const char *szIPAddress);
extern void PluginsClientDisconnect(int iClientIndex);
extern void InitInterface();

#endif //PLUGINS_H