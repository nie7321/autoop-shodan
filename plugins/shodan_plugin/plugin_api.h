/*****************************************************\
*	Auto-OP Shodan Plugin/Extension
*
*   File: plugin_api.h
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

#ifndef PLUGIN_API_H
#define PLUGIN_API_H

//Constants
typedef enum 
{

	BUILDTYPE_SENTRYGUN = 0,
	BUILDTYPE_DISPENSER,
	BUILDTYPE_LASERSENSOR,
	BUILDTYPE_MANUALTURRET

} BUILDTYPE;

typedef enum
{
	plgprint_console,
	plgprint_center,
	plgprint_chat,
} PLGPRINT_TYPE;

#define SWITCH_OFF	0
#define SWITCH_ON	1

#define	PC_SCOUT		1
#define	PC_SNIPER		2
#define	PC_SOLDIER		3
#define	PC_DEMOMAN		4
#define	PC_MEDIC		5
#define	PC_HWGUY		6
#define	PC_PYRO			7
#define	PC_SPY			8
#define	PC_ENGINEER		9
#define	PC_CIVILIAN		11

#define VIEW_1STPERSON	0
#define VIEW_3RDPERSON	1

//Engine functions

typedef struct shodanfuncs_s
{
	float		(*pfnGetPluginInterfaceVer)	( void );

	//Client functions
	char*		(*pfnGetName)				(int iPlIndex);
	char*		(*pfnGetAuthID)				(int iPlIndex);
	int			(*pfnGetUserID)				(int iPlIndex);

	int			(*pfnIndexFromText)			(char *szText);
	
	int			(*pfnGet_Credits)			(int iPlIndex);
	int			(*pfnSet_Credits)			(int iPlIndex, int iAmount);
	long		(*pfnGet_Time)				(int iPlIndex);
	char*		(*pfnGet_OrigName)			(int iPlIndex);
	char*		(*pfnGet_Rank)				(int iPlIndex);

	int			(*pfnSet_Ammo_Shells)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Shells)		(int iPlIndex);
	int			(*pfnSet_Ammo_Nails)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Nails)		(int iPlIndex);
	int			(*pfnSet_Ammo_Rockets)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Rockets)		(int iPlIndex);
	int			(*pfnSet_Ammo_Cells)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Cells)		(int iPlIndex);
	int			(*pfnSet_Ammo_Gren1)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Gren1)		(int iPlIndex);
	int			(*pfnSet_Ammo_Gren2)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Gren2)		(int iPlIndex);
	int			(*pfnSet_Ammo_Proxmine)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Proxmine)		(int iPlIndex);
	int			(*pfnSet_Ammo_Snarks)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_Snarks)		(int iPlIndex);
	int			(*pfnSet_Ammo_C4)			(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_C4)			(int iPlIndex);
	int			(*pfnSet_Ammo_TripMine)		(int iPlIndex, int iAmount);
	int			(*pfnGet_Ammo_TripMine)		(int iPlIndex);

	int			(*pfnReset_Build)			(int iPlIndex, int iBuildType);

	int			(*pfnGag)					(int iPlIndex);
	int			(*pfnUngag)					(int iPlIndex);

	int			(*pfnInvis)					(int iPlIndex, int iSwitch);
	int			(*pfnGodmode)				(int iPlIndex, int iSwitch);
	int			(*pfnNoclip)				(int iPlIndex, int iSwitch);
	int			(*pfnGlow)					(int iPlIndex, int r, int g, int b, int iSize);

	int			(*pfnSet_Playerclass)		(int iPlIndex, int iPlayerclass);
	int			(*pfnGet_Playerclass)		(int iPlIndex);

	int			(*pfnSet_Armor)				(int iPlIndex, int iAmount);
	int			(*pfnGet_Armor)				(int iPlIndex);

	int			(*pfnSet_Health)			(int iPlIndex, int iAmount);
	int			(*pfnGet_Health)			(int iPlIndex);

	int			(*pfnSet_CustomSkin)		(int iPlIndex, int iSkinIndex);
	int			(*pfnGet_CustomSkin)		(int iPlIndex);

	int			(*pfnSet_Admin_Access)		(int iPlIndex, int iAdminLevel);
	int			(*pfnGet_Admin_Access)		(int iPlIndex);

	int			(*pfnSet_View)				(int iPlIndex, int iView);

	int			(*pfnColor_Trail)			(int iPlIndex, int r, int g, int b);

	int			(*pfnClient_Command)		(int iPlIndex, char *szCommand);

	int			(*pfnSet_Origin)			(int iPlIndex, int x, int y, int z);
	int			(*pfnGet_Origin)			(int iPlIndex, int &x, int &y, int &z);

	int			(*pfnHurt)					(int iPlIndex, int iDmg, int iDmgType);

	int			(*pfnSet_Velocity)			(int iPlIndex, float x, float y, float z);

	char*		(*pfnGet_Client_KeyValue)	(int iPlIndex, char *szKeyname);
	int			(*pfnSet_Client_KeyValue)	(int iPlIndex, char *szKeyname, char *szValue);

	BOOL		(*pfnHasFencepost)			(int iPlIndex);
	BOOL		(*pfnHasManualturret)		(int iPlIndex);
	BOOL		(*pfnHasC4)					(int iPlIndex);
	BOOL		(*pfnHasProxmine)			(int iPlIndex);
	BOOL		(*pfnHasTripmine)			(int iPlIndex);
	BOOL		(*pfnInStealth)				(int iPlIndex);
	BOOL		(*pfnHasPackbomb)			(int iPlIndex);
	BOOL		(*pfnHasSuperflame)			(int iPlIndex);
	BOOL		(*pfnHasSentrygun)			(int iPlIndex);
	BOOL		(*pfnHasDispenser)			(int iPlIndex);

	//Server functions
	int			(*pfnTextMessage)			(int iPlIndex, int iPrintType, char *format, ...);
	int			(*pfnColorMessage)			(int iPlIndex, int x, int y, int r1, int g1, int b1, int r2, int g2, int b2, int time, char *format, ...);

	int			(*pfnScreenShake)			(float flAmplitude);
	int			(*pfnScreenFade)			(int r, int g, int b, int alpha);

	void		(*pfnStartMapVote)			( void );
	void		(*pfnEndMapVote)			( void );
	void		(*pfnCancelMapVote)			( void );

	int			(*pfnResponseBotSay)		(char *format, ...);
	int			(*pfnLog_Write)				(char *szLogLine);

	char*		(*pfnGet_Cvar)				(char *szCvarKey);
	void		(*pfnSet_CvarString)		(char *szCvarKey, char *szCvarValue);
	void		(*pfnSet_CvarFloat)			(char *szCvarKey, float flCvarValue);

	char*		(*pfnCurrentMap)			( void );
	char*		(*pfnNextMap)				( void );

	void		(*pfnServer_Command)		(char *szCommand);

	char*		(*pfnGetBotName1)			( void );
	char*		(*pfnGetBotName2)			( void );

	int			(*pfnPlayerCount)			( void );

	int			(*pfnPointTo)				(int iPlIndex, int iRange);

	void		(*pfnSpawnGrunt)			(bool blnCanWalk, int x, int y, int z);
	void		(*pfnSpawnGarg)				(bool blnCanWalk, int x, int y, int z);

	float		(*pfnGet_Server_Time)		( void );

	BOOL		(*pfnIsValidMap)			(char *szMapName);

	void		(*pfnMessageBox)			(char *szMessage);

	void		(*pfnWriteBackAccounts)		( void );

	void		(*pfnCreateLight)			(int x, int y, int z, int r, int g, int b, int brightness);
	void		(*pfnCreateEntity)			(int x, int y, int z, int iType);

	int			(*pfnLoadEntLayout)			(char *szLayoutName);
	int			(*pfnUnloadEntLayout)		(char *szLayoutName);

	void		(*pfnTriggerEntity)			(char *Targetname);

	float		(*pfnRandom_Float)			(float flMin, float flMax);
	long		(*pfnRandom_Long)			(long lngMin, long lngMax);

	char*		(*pfnSkinNameFromIndex)		(int iSkinIndex);

} shodanfuncs_t;

#endif //PLUGIN_API_H