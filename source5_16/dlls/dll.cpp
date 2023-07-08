/*****************************************************\
*	Auto-OP Shodan
*
*	File: dll.cpp
*	Description: Core file.
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*	Based on work by Botman (http://www.planethalflife.com/botman)
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
#include "voting.h"
#include "commands.h"
#include "customweapons.h"
#include "skins.h"
#include "3rdperson.h"
#include "aohgrunt.h"
#include "aogarg.h"
#include "stripper.h"
#include "plugins.h"
#include "decals.h"
#include <direct.h>
#include <stdio.h>

typedef int (FAR *GETENTITYAPI)(DLL_FUNCTIONS *, int);

extern GETENTITYAPI other_GetEntityAPI;
extern enginefuncs_t g_engfuncs;
extern globalvars_t  *gpGlobals;

DLL_FUNCTIONS other_gFunctionTable;

mapcycle_t mapcycle;

DLL_GLOBAL const Vector	g_vecZero = Vector(0,0,0);

AOPlayerInfo_s PlayerInfo[33]; //All variables about a player.
TFXFeatureEnabled_s Features;
FILE *fp; //For debug

//Kickvote stuff
int VoteStatus = 0;
float LastCalled = -600.0;
int iKickUser = 0;

//Mapvote stuff
float MapVoteLast = 600.0;
float MapLastCalled = 0.0;
int UsedSlots = 0;
int MapVoteStat = 0;
float MapChange = 0.0;
char *NextMapChange;

int NightTime = 0; //A Night time map?

//Storing Player LastSsaid info
char LastSaid[200];
char LastPlayer[50];
char LastConnected[50];

int team_allies[4];  // TFC bit mapped allies BLUE, RED, YELLOW, and GREEN
int SniperMap = 0; //Has this map got a sniper only team?

//Are teams admins only?
int team1_admins = 0;
int team2_admins = 0;
int team3_admins = 0;
int team4_admins = 0;

int team_limits[5]; //TFC team player limits
int team_available[5];

int mod_id = 0;
int ConnectCount = 0;

edict_t *pBuilding = NULL;

bool DoneEndMapSound = false;

//Powerup buying
extern int QUAD_CREDS;
extern int GOD_CREDS;
extern int INVIS_CREDS;
extern int GRAV_CREDS;
extern int SPEED_CREDS;
extern int CIV_CREDS;
extern int BUILD_CREDS;
extern int HEAL_CREDS;
extern int MAPVOTE_CREDS;
extern int TRAIL_CREDS;
extern int ADMINSKIN_CREDS;

//Ammo Buying
extern int AMMO_NAILS_CREDS;
extern int AMMO_SHELLS_CREDS;
extern int AMMO_ROCKETS_CREDS;
extern int AMMO_CELLS_CREDS;
extern int AMMO_GREN1_CREDS;
extern int AMMO_GREN2_CREDS;

//Weapon Buying
extern int GUN_CREDS;

//Speech buying
extern int SPEECH_CREDS;

//CVARS START -------------------------------
cvar_t tfx_airstrike		 = {"tfx_airstrikes","1",FCVAR_EXTDLL};
cvar_t tfx_manualsentrys	 = {"tfx_sentrys","1",FCVAR_EXTDLL};
cvar_t tfx_grapplehook		 = {"tfx_grapplehook","1",FCVAR_EXTDLL};
cvar_t tfx_laserfence		 = {"tfx_laserfence","1",FCVAR_EXTDLL};
cvar_t tfx_snarks			 = {"tfx_snarks","1",FCVAR_EXTDLL};
cvar_t tfx_credits			 = {"tfx_credits","1",FCVAR_EXTDLL};
cvar_t tfx_skinshowteam		 = {"tfx_skinshowteam","1",FCVAR_EXTDLL};
cvar_t tfx_c4				 = {"tfx_c4","1",FCVAR_EXTDLL};
cvar_t tfx_tripmines		 = {"tfx_tripmines","1",FCVAR_EXTDLL};
cvar_t tfx_icons			 = {"tfx_icons","1",FCVAR_EXTDLL};
cvar_t tfx_mines			 = {"tfx_mines","1",FCVAR_EXTDLL};
cvar_t tfx_sgcloak			 = {"tfx_sgcloak","0",FCVAR_EXTDLL};
cvar_t tfx_skins			 = {"tfx_skins","1",FCVAR_EXTDLL};
cvar_t tfx_debug			 = {"tfx_debug","0",FCVAR_EXTDLL};
cvar_t tfx_mediflag			 = {"tfx_mediflag","1",FCVAR_EXTDLL};
cvar_t tfx_superflame		 = {"tfx_superflame","1",FCVAR_EXTDLL};
cvar_t tfx_kickvote			 = {"tfx_kickvote","0",FCVAR_EXTDLL};
cvar_t tfx_flashbang		 = {"tfx_flashbang","1",FCVAR_EXTDLL};
cvar_t tfx_mapvote			 = {"tfx_mapvote","1",FCVAR_EXTDLL};
cvar_t tfx_skyhook			 = {"tfx_skyhook","1",FCVAR_EXTDLL};
cvar_t tfx_adminspect		 = {"tfx_adminspect","1",FCVAR_EXTDLL};
cvar_t tfx_stealthcamo		 = {"tfx_stealthcamo","1",FCVAR_EXTDLL};
cvar_t tfx_sword			 = {"tfx_sword","1",FCVAR_EXTDLL};
cvar_t tfx_revival			 = {"tfx_revival","0",FCVAR_EXTDLL};
cvar_t tfx_voteratio		 = {"tfx_voteratio","50",FCVAR_EXTDLL};
cvar_t tfx_antispawncamp	 = {"tfx_antispawncamp","1",FCVAR_EXTDLL};
cvar_t tfx_logging			 = {"tfx_logging","1",FCVAR_EXTDLL};
cvar_t tfx_editorprotection	 = {"tfx_editorprotection","1",FCVAR_EXTDLL};
cvar_t tfx_customweapons	 = {"tfx_customweapons","1",FCVAR_EXTDLL};
cvar_t tfx_bottalk			 = {"tfx_bottalk","1",FCVAR_EXTDLL};
cvar_t tfx_teamgrapple		 = {"tfx_teamgrapple","1",FCVAR_EXTDLL};
cvar_t tfx_snipemapcheck	 = {"tfx_snipemapcheck","2",FCVAR_EXTDLL};
cvar_t tfx_lockdown			 = {"tfx_lockdown","0",FCVAR_EXTDLL};
cvar_t tfx_privmessages		 = {"tfx_privmessages","1",FCVAR_EXTDLL};
cvar_t tfx_adminseepriv		 = {"tfx_adminseepriv","1",FCVAR_EXTDLL};
cvar_t tfx_gundropping		 = {"tfx_gundropping","1",FCVAR_EXTDLL};
cvar_t tfx_vampirism		 = {"tfx_vampirism","1",FCVAR_EXTDLL};
cvar_t tfx_3rdperson		 = {"tfx_3rdperson","1",FCVAR_EXTDLL};
cvar_t tfx_weaponsonskins	 = {"tfx_weaponsonskins","1",FCVAR_EXTDLL};
cvar_t tfx_hwradar			 = {"tfx_hwradar","1",FCVAR_EXTDLL};
cvar_t tfx_adminteam		 = {"tfx_adminteam","1",FCVAR_EXTDLL};
cvar_t tfx_customdispenser	 = {"tfx_customdispenser","1",FCVAR_EXTDLL};
cvar_t tfx_c4onplayers		 = {"tfx_c4onplayers","1",FCVAR_EXTDLL};
cvar_t tfx_c4autodet		 = {"tfx_c4autodet","105",FCVAR_EXTDLL};
cvar_t tfx_forceteambalance	 = {"tfx_forceteambalance","0",FCVAR_EXTDLL};
cvar_t tfx_buying			 = {"tfx_buying","1",FCVAR_EXTDLL};
cvar_t tfx_classicgrapple	 = {"tfx_classicgrapple","0",FCVAR_EXTDLL};
cvar_t tfx_grapplespeed		 = {"tfx_grapplespeed","1500",FCVAR_EXTDLL};
cvar_t nailgun_paintball	 = {"nailgun_paintball","0",FCVAR_EXTDLL};
cvar_t tfx_autodj			 = {"tfx_autodj","0",FCVAR_EXTDLL};
cvar_t tfx_autodjdelay		 = {"tfx_autodjdelay","120",FCVAR_EXTDLL};
cvar_t tfx_icqnotify		 = {"tfx_icqnotify","1",FCVAR_EXTDLL};
cvar_t tfx_messageflags		 = {"tfx_messageflags","1",FCVAR_EXTDLL};
cvar_t tfx_version			 = {"autoop_version_number",MOD_VERSION,FCVAR_SERVER};
cvar_t tfx_revision			 = {"autoop_version_revision",MOD_REVISION,FCVAR_SERVER};

//Item Cost CVARs
cvar_t tfx_cost_quad		 = {"tfx_cost_quad","15",FCVAR_EXTDLL};
cvar_t tfx_cost_god			 = {"tfx_cost_god","200",FCVAR_EXTDLL};
cvar_t tfx_cost_invis		 = {"tfx_cost_invis","100",FCVAR_EXTDLL};
cvar_t tfx_cost_grav		 = {"tfx_cost_grav","15",FCVAR_EXTDLL};
cvar_t tfx_cost_speed		 = {"tfx_cost_speed","30",FCVAR_EXTDLL};
cvar_t tfx_cost_build		 = {"tfx_cost_build","10",FCVAR_EXTDLL};
cvar_t tfx_cost_civilian		 = {"tfx_cost_civilian","0",FCVAR_EXTDLL};
cvar_t tfx_cost_ammo_nails		 = {"tfx_cost_ammo_nails","10",FCVAR_EXTDLL};
cvar_t tfx_cost_ammo_shells		 = {"tfx_cost_ammo_shells","15",FCVAR_EXTDLL};
cvar_t tfx_cost_ammo_rockets	 = {"tfx_cost_ammo_rockets","20",FCVAR_EXTDLL};
cvar_t tfx_cost_ammo_cells		 = {"tfx_cost_ammo_cells","25",FCVAR_EXTDLL};
cvar_t tfx_cost_ammo_gren1		 = {"tfx_cost_ammo_gren1","30",FCVAR_EXTDLL};
cvar_t tfx_cost_ammo_gren2		 = {"tfx_cost_ammo_gren2","30",FCVAR_EXTDLL};
cvar_t tfx_cost_guns	= {"tfx_cost_guns","2",FCVAR_EXTDLL};
cvar_t tfx_cost_speech	= {"tfx_cost_speech","2",FCVAR_EXTDLL};
cvar_t tfx_cost_heal	= {"tfx_cost_heal","7",FCVAR_EXTDLL};
cvar_t tfx_cost_mapvote	= {"tfx_cost_mapvote","30",FCVAR_EXTDLL};
cvar_t tfx_cost_trail	= {"tfx_cost_trail","5",FCVAR_EXTDLL};
cvar_t tfx_cost_adminskin	= {"tfx_cost_adminskin","50",FCVAR_EXTDLL};

//Fake HLGuard
cvar_t hlg_official			= {"hlg_official","1",FCVAR_SERVER};
cvar_t hlguard_version		= {"hlguard_version","1.07",FCVAR_SERVER};
cvar_t hlguard_cfgversion	= {"hlguard_cfgversion","HLG v1.07 (Net)",FCVAR_SERVER};

//CVARS END   -------------------------------

void GameDLLInit( void )
{
	//Add server commands
	ADD_SERVER_COMMAND("admc",HandleServerCommands);

	//Register CVARs
	CVAR_REGISTER (&tfx_airstrike);
	CVAR_REGISTER (&tfx_manualsentrys);
	CVAR_REGISTER (&tfx_grapplehook);
	CVAR_REGISTER (&tfx_laserfence);
	CVAR_REGISTER (&tfx_snarks);
	CVAR_REGISTER (&tfx_credits);
	CVAR_REGISTER (&tfx_c4);
	CVAR_REGISTER (&tfx_tripmines);
	CVAR_REGISTER (&tfx_icons);
	CVAR_REGISTER (&tfx_mines);
	CVAR_REGISTER (&tfx_skins);
	CVAR_REGISTER (&tfx_kickvote);
	CVAR_REGISTER (&tfx_debug);
	CVAR_REGISTER (&tfx_sgcloak);
	CVAR_REGISTER (&tfx_skinshowteam);
	CVAR_REGISTER (&tfx_superflame);
	CVAR_REGISTER (&tfx_mediflag);
	CVAR_REGISTER (&tfx_mapvote);
	CVAR_REGISTER (&tfx_adminspect);
	CVAR_REGISTER (&tfx_flashbang);
	CVAR_REGISTER (&tfx_skyhook);
	CVAR_REGISTER (&tfx_version);
	CVAR_REGISTER (&tfx_revision);
	CVAR_REGISTER (&tfx_stealthcamo);
	CVAR_REGISTER (&tfx_sword);
	CVAR_REGISTER (&tfx_voteratio);
	CVAR_REGISTER (&tfx_logging);
	CVAR_REGISTER (&tfx_editorprotection);
	CVAR_REGISTER (&tfx_antispawncamp);
	CVAR_REGISTER (&tfx_revival);
	CVAR_REGISTER (&tfx_customweapons);
	CVAR_REGISTER (&tfx_bottalk);
	CVAR_REGISTER (&tfx_teamgrapple);
	CVAR_REGISTER (&tfx_snipemapcheck);
	CVAR_REGISTER (&tfx_lockdown);
	CVAR_REGISTER (&tfx_privmessages);
	CVAR_REGISTER (&tfx_adminseepriv);
	CVAR_REGISTER (&tfx_gundropping);
	CVAR_REGISTER (&tfx_vampirism);
	CVAR_REGISTER (&tfx_hwradar);
	CVAR_REGISTER (&tfx_3rdperson);
	CVAR_REGISTER (&tfx_weaponsonskins);
	CVAR_REGISTER (&tfx_adminteam);
	CVAR_REGISTER (&tfx_customdispenser);
	CVAR_REGISTER (&tfx_c4onplayers);
	CVAR_REGISTER (&tfx_c4autodet);
	CVAR_REGISTER (&tfx_forceteambalance);
	CVAR_REGISTER (&tfx_classicgrapple);
	CVAR_REGISTER (&tfx_grapplespeed);
	CVAR_REGISTER (&tfx_autodj);
	CVAR_REGISTER (&tfx_autodjdelay);
	CVAR_REGISTER (&tfx_icqnotify);
	CVAR_REGISTER (&tfx_messageflags);
	CVAR_REGISTER (&nailgun_paintball);

	//Buying
	CVAR_REGISTER (&tfx_buying);
	CVAR_REGISTER (&tfx_cost_quad);
	CVAR_REGISTER (&tfx_cost_god);
	CVAR_REGISTER (&tfx_cost_invis);
	CVAR_REGISTER (&tfx_cost_grav);
	CVAR_REGISTER (&tfx_cost_speed);
	CVAR_REGISTER (&tfx_cost_build);
	CVAR_REGISTER (&tfx_cost_ammo_nails);
	CVAR_REGISTER (&tfx_cost_ammo_shells);
	CVAR_REGISTER (&tfx_cost_ammo_rockets);
	CVAR_REGISTER (&tfx_cost_ammo_cells);
	CVAR_REGISTER (&tfx_cost_ammo_gren1);
	CVAR_REGISTER (&tfx_cost_ammo_gren2);
	CVAR_REGISTER (&tfx_cost_guns);
	CVAR_REGISTER (&tfx_cost_speech);
	CVAR_REGISTER (&tfx_cost_civilian);
	CVAR_REGISTER (&tfx_cost_heal);
	CVAR_REGISTER (&tfx_cost_mapvote);
	CVAR_REGISTER (&tfx_cost_trail);
	CVAR_REGISTER (&tfx_cost_adminskin);

	//Fake HLGuard
	CVAR_REGISTER (&hlg_official);
	CVAR_REGISTER (&hlguard_version);
	CVAR_REGISTER (&hlguard_cfgversion);

	(*other_gFunctionTable.pfnGameInit)();
}

int DispatchSpawn( edict_t *pent )
{
	char *pClassname = (char *)STRING(pent->v.classname);

	if (strcmp(pClassname, "worldspawn") == 0)
    {
		DecalInit();

		pBuilding = NULL;

		DoneEndMapSound = false;

		SniperMap = 0; //Reset this.

		for (int i=0; i <= 4; i++)
		{
			//Reset team allies and limits
			team_allies[i] = 0;
			team_limits[i] = 0;
			team_available[i] = 0;
		}

		//Reset admin only teams
		team1_admins = 0;
		team2_admins = 0;
		team3_admins = 0;
		team4_admins = 0;

		if(Features.Debug >= 1)
		{
			fp=fopen("sdebug.log","a");
			fprintf(fp, "--------------\nAutoOP Shodan v%s Revision %s Active...\n",MOD_VERSION,MOD_REVISION);
			fclose(fp);
		}

		PrecacheResources();
		LoadGameSettings();
		LoadCommandLevels();

		//Plugin stuff
		UnloadPlugins(); //Unload plugins
		InitInterface(); // Initialise interface to plugins
		LoadPlugins();   //Load plugins

		//Exec the autoop.cfg
		SERVER_COMMAND("exec autoop.cfg\n");
	}

	return (*other_gFunctionTable.pfnSpawn)(pent);
}

void DispatchThink( edict_t *pent )
{
if(!TFXDispatchThink(pent))
   (*other_gFunctionTable.pfnThink)(pent);
}

void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
   (*other_gFunctionTable.pfnUse)(pentUsed, pentOther);
}

void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
	if (strcmp(STRING(pentOther->v.classname), "weapon_grapplehook") == 0)
		GrappleTouch(pentOther,pentTouched);

	//Test
	if ((strcmp(STRING(pentTouched->v.classname), "tf_rpg_rocket") == 0) || (strcmp(STRING(pentTouched->v.classname), "tf_nailgun_nail") == 0))
	{
		if(CVAR_GET_FLOAT("nailgun_paintball") == 1)
		{
			TraceResult tr;
			Vector		vecSpot;// trace starts here!
			vecSpot = pentTouched->v.origin - pentTouched->v.velocity.Normalize() * 32;
			UTIL_TraceLine( vecSpot, vecSpot + pentTouched->v.velocity.Normalize() * 64, ignore_monsters, pentTouched, &tr );
			UTIL_DecalTrace( &tr, DECAL_YBLOOD1 + RANDOM_LONG(0,5) );

			/*if(pentTouched->v.owner)
				CLIENT_PRINTF( pentTouched->v.owner, print_console,UTIL_VarArgs( "NAIL OWNER.\n"));

			if(strcmp(STRING(pentOther->v.classname), "player") == 0)
			{
				int Team1 = pentTouched->v.owner->v.team;
				int Team2 = pentOther->v.team;
				if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
					if(pentOther->v.health > 5)
						TakeDamage(pentOther,NULL,5,0);
			}*/
		}
	}

   (*other_gFunctionTable.pfnTouch)(pentTouched, pentOther);
}

void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
   (*other_gFunctionTable.pfnBlocked)(pentBlocked, pentOther);
}

void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
	//Determine what teams are available via what teams have spawnpoints
	 if (strcmp(STRING(pentKeyvalue->v.classname), "info_player_teamspawn") == 0)
	 {
		if (strcmp(pkvd->szKeyName, "team_no") == 0)
			team_available[atoi(pkvd->szValue)] = 1;
	 }

	 if (strcmp(STRING(pentKeyvalue->v.classname), "info_tfdetect") == 0)
	 {
		if (strcmp(pkvd->szKeyName, "team1_allies") == 0)  // BLUE allies
			team_allies[0] = atoi(pkvd->szValue);
		else if (strcmp(pkvd->szKeyName, "team2_allies") == 0)  // RED allies
			team_allies[1] = atoi(pkvd->szValue);
		else if (strcmp(pkvd->szKeyName, "team3_allies") == 0)  // YELLOW allies
			team_allies[2] = atoi(pkvd->szValue);
		else if (strcmp(pkvd->szKeyName, "team4_allies") == 0)  // GREEN allies
			team_allies[3] = atoi(pkvd->szValue);
		//Yellow team
		else if (strcmp(pkvd->szKeyName, "maxammo_rockets") == 0)
		{
			if(atoi(pkvd->szValue) == 1021)
				SniperMap = 1;
		}
		//Blue
		else if (strcmp(pkvd->szKeyName, "maxammo_shells") == 0)
		{
			if(atoi(pkvd->szValue) == 1021)
				SniperMap = 1;
		}
		//Green
		else if (strcmp(pkvd->szKeyName, "maxammo_cells") == 0)
		{
			if(atoi(pkvd->szValue) == 1021)
				SniperMap = 1;
		}
		//Red
		else if (strcmp(pkvd->szKeyName, "maxammo_nails") == 0)
		{
			if(atoi(pkvd->szValue) == 1021)
				SniperMap = 1;
		}
		//TEAM NAMES
		else if (strcmp(pkvd->szKeyName, "team1_name") == 0)
		{
			if(_stricmp(pkvd->szValue,"ADMINS") == 0)
			{
				team1_admins = 1;
				CVAR_SET_FLOAT ("tfc_autoteam",0);
				CVAR_SET_FLOAT ("tfc_balance_teams",0);
				CVAR_SET_FLOAT ("tfc_balance_scores",0);
			}
		}
		else if (strcmp(pkvd->szKeyName, "team2_name") == 0)
		{
			if(_stricmp(pkvd->szValue,"ADMINS") == 0)
			{
				team2_admins = 1;
				CVAR_SET_FLOAT ("tfc_autoteam",0);
				CVAR_SET_FLOAT ("tfc_balance_teams",0);
				CVAR_SET_FLOAT ("tfc_balance_scores",0);
			}
		}
		else if (strcmp(pkvd->szKeyName, "team3_name") == 0)
		{
			if(_stricmp(pkvd->szValue,"ADMINS") == 0)
			{
				team3_admins = 1;
				CVAR_SET_FLOAT ("tfc_autoteam",0);
				CVAR_SET_FLOAT ("tfc_balance_teams",0);
				CVAR_SET_FLOAT ("tfc_balance_scores",0);
			}
		}
		else if (strcmp(pkvd->szKeyName, "team4_name") == 0)
		{
			if(_stricmp(pkvd->szValue,"ADMINS") == 0)
			{
				team4_admins = 1;
				CVAR_SET_FLOAT ("tfc_autoteam",0);
				CVAR_SET_FLOAT ("tfc_balance_teams",0);
				CVAR_SET_FLOAT ("tfc_balance_scores",0);
			}
		}
		//MAX PLAYERS
		else if (strcmp(pkvd->szKeyName, "ammo_medikit") == 0)
			team_limits[0] = atoi(pkvd->szValue);
		else if (strcmp(pkvd->szKeyName, "ammo_detpack") == 0)
			team_limits[1] = atoi(pkvd->szValue);
		else if (strcmp(pkvd->szKeyName, "maxammo_medikit") == 0)
			team_limits[2] = atoi(pkvd->szValue);
		else if (strcmp(pkvd->szKeyName, "maxammo_detpack") == 0)
			team_limits[3] = atoi(pkvd->szValue);
	 }

	(*other_gFunctionTable.pfnKeyValue)(pentKeyvalue, pkvd);
}

void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
   (*other_gFunctionTable.pfnSave)(pent, pSaveData);
}

int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
   return (*other_gFunctionTable.pfnRestore)(pent, pSaveData, globalEntity);
}

void DispatchObjectCollsionBox( edict_t *pent )
{
   (*other_gFunctionTable.pfnSetAbsBox)(pent);
}

void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
   (*other_gFunctionTable.pfnSaveWriteFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
   (*other_gFunctionTable.pfnSaveReadFields)(pSaveData, pname, pBaseData, pFields, fieldCount);
}

void SaveGlobalState( SAVERESTOREDATA *pSaveData )
{
   (*other_gFunctionTable.pfnSaveGlobalState)(pSaveData);
}

void RestoreGlobalState( SAVERESTOREDATA *pSaveData )
{
   (*other_gFunctionTable.pfnRestoreGlobalState)(pSaveData);
}

void ResetGlobalState( void )
{
   (*other_gFunctionTable.pfnResetGlobalState)();
}

BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{
	pEntity->v.team = 0; //Reset their team

	ConnectCount += 1;

	TFXResetPlayerInfo(pEntity);

	if ( FStrEq(GETPLAYERAUTHID(pEntity), "STEAM_ID_PENDING" ))
	{
		//uh oh a fucking steamer in here
		//mr.steamers ID is not available yet so
		//we cant do some init stuff

		//do it in ClientPutInServer instead
		PlayerInfo[ENTINDEX(pEntity)].IsSteamer = 1;
		TFXClientConnectionMsg(pEntity,pszName);

	} else {
		PlayerInfo[ENTINDEX(pEntity)].IsSteamer = 0; //not steam
		
		TFXLoadPlayerInfo(pEntity,pszName);
		TFXClientConnectionMsg(pEntity,pszName);
		UserLogger(pEntity,pszName,pszAddress);
	}

	//Copy players IP to memory
	strcpy(PlayerInfo[ENTINDEX(pEntity)].IPAddress,pszAddress);

	int PlayerCount = 0;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ ) {
		CBaseEntity *pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer) 
			if(!FStrEq(STRING(pPlayer->pev->netname),""))
				PlayerCount++;
	}

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "ClientConnected(): Established connection with player on slot %d\n",ENTINDEX(pEntity));
		fclose(fp);
	}

	if((Features.ReservedSlotTime > gpGlobals->time) && (PlayerInfo[ENTINDEX(pEntity)].IsAdmin < LEVEL_SUPEROP) && (PlayerCount > (gpGlobals->maxClients - 1)))
	{
		sprintf(szRejectReason,"Server is full.\n");
		return FALSE;
	}

	PluginsClientConnect(ENTINDEX(pEntity),pszName,pszAddress);

	return (*other_gFunctionTable.pfnClientConnect)(pEntity, pszName, pszAddress, szRejectReason);
}

void ClientDisconnect( edict_t *pEntity )
{
	PluginsClientDisconnect(ENTINDEX(pEntity));

	TFXPlayerDisconnect(pEntity);
	pEntity->v.team = 0; //Reset their team

	(*other_gFunctionTable.pfnClientDisconnect)(pEntity);
}

void ClientKill( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnClientKill)(pEntity);
}

void ClientPutInServer( edict_t *pEntity )
{
	UTIL_ShowMenu(pEntity,0,1,0," ");

	if(PlayerInfo[ENTINDEX(pEntity)].IsSteamer == 1)
	{
		TFXLoadPlayerInfo(pEntity,STRING(pEntity->v.netname));
		UserLogger(pEntity,STRING(pEntity->v.netname),PlayerInfo[ENTINDEX(pEntity)].IPAddress);
	}

	//Put clientpack info into players store.
	extern enginefuncs_t  g_engfuncs;
	char *infobuffer=g_engfuncs.pfnGetInfoKeyBuffer(pEntity);
	char *szPackVer=g_engfuncs.pfnInfoKeyValue( infobuffer, "_gp2clpckver");
	//Copy to memory on server
	strcpy(PlayerInfo[ENTINDEX(pEntity)].ClientPackVer,szPackVer);
	CheckClientPack(pEntity);

	//Activate clients timer
	PlayerInfo[ENTINDEX(pEntity)].TimerActive = 1;

   (*other_gFunctionTable.pfnClientPutInServer)(pEntity);
}

void ClientCommand( edict_t *pEntity )
{
	const char *pcmd = CMD_ARGV(0);
	const char *arg1 = CMD_ARGV(1);
	const char *arg2 = CMD_ARGV(2);
	const char *arg3 = CMD_ARGV(3);
	const char *arg4 = CMD_ARGV(4);
	const char *arg5 = CMD_ARGV(5);

	if(PluginsClientCommand(ENTINDEX(pEntity),pcmd,(CMD_ARGS() + (strlen(pcmd) + 1))))
		return;

	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a"); fprintf(fp,"Command(\"%s\") : %s",STRING(pEntity->v.netname),pcmd);
			if ((arg1 != NULL) && (*arg1 != 0))
				fprintf(fp," %s", arg1);
			if ((arg2 != NULL) && (*arg2 != 0))
				fprintf(fp," %s", arg2);
			if ((arg3 != NULL) && (*arg3 != 0))
				fprintf(fp," %s", arg3);
			if ((arg4 != NULL) && (*arg4 != 0))
				fprintf(fp," %s", arg4);
			fprintf(fp, "\n");
		fclose(fp);
	}

	int iIndex = ENTINDEX(pEntity);

	if(!TFXPlayerCommand(pEntity,pcmd,arg1,arg2,arg3,arg4,arg5))	
		(*other_gFunctionTable.pfnClientCommand)(pEntity);
}

void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
	//If gagged.
	if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 1)
		if(g_engfuncs.pfnInfoKeyValue(infobuffer, "name") != NULL)
			return;

   (*other_gFunctionTable.pfnClientUserInfoChanged)(pEntity, infobuffer);
}

void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
   (*other_gFunctionTable.pfnServerActivate)(pEdictList, edictCount, clientMax);
}

void ServerDeactivate( void )
{
   (*other_gFunctionTable.pfnServerDeactivate)();
}

void PlayerPreThink( edict_t *pEntity )
{
	if(!TFXPlayerPreThink(pEntity))
	   (*other_gFunctionTable.pfnPlayerPreThink)(pEntity);
}

void PlayerPostThink( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnPlayerPostThink)(pEntity);
}

void StartFrame( void )
{
	static float previous_time = 0.0;
	static float update_time = 0.0;
	static float green_time = 0.0;
	static float logo_time = 0.0;
	static float cred_time = 0.0;
	static float clientpack_time = 0.0;
	static float trace_time = 0.0;
	static float dj_time = 0.0;
	static float sniper_gren2_time = 0.0;
	static float generic_think = 0.0;

	if ((gpGlobals->time + 0.1) < previous_time)
	{
		update_time = 0.0;
		previous_time = 0.0;
		generic_think = 0.0;
		clientpack_time = 0.0;
		cred_time = 0.0;
		logo_time = 0.0;
		green_time = 0.0;
		trace_time = 0.0;
		dj_time = 0.0;
		
		//Reset map vote stuff
		MapVoteLast = 600.0;
		MapVoteStat = 0;
		MapLastCalled = 0.0;
		UsedSlots = 0;
		
		//Reset Kickvote Stuff
		VoteStatus = 0;
		LastCalled = -600.0;
		iKickUser = 0;

		Features.ReservedSlotTime = 0;

	}

	if(Features.Snarks == 1)
	{
		if(sniper_gren2_time < gpGlobals->time)
		{
			sniper_gren2_time = gpGlobals->time + 1.5;
			
			edict_t *pEntity = NULL;

			for ( int i = 1; i <= gpGlobals->maxClients; i++ )
			{
				pEntity = INDEXENT(i);

				if(pEntity)
				{
					if(!FStrEq(STRING(pEntity->v.netname),""))
					{
						if(pEntity->v.playerclass == CLASS_SNIPER)
						{
							if(PlayerInfo[ENTINDEX(pEntity)].SnarkAmmo >= SNARK_AMMO)
							{
								int gmsgSecAmmoVal = REG_USER_MSG("SecAmmoVal", 3);

								if(gmsgSecAmmoVal < MAX_USERMSG)
								{
									MESSAGE_BEGIN( MSG_ONE, gmsgSecAmmoVal, NULL, pEntity );
										WRITE_BYTE(1);
										WRITE_BYTE((int)PlayerInfo[ENTINDEX(pEntity)].SnarkAmmo);
									MESSAGE_END();
								}
							}

							int gmsgSecAmmoIcon = REG_USER_MSG("SecAmmoIcon", -1);
							if(gmsgSecAmmoIcon < MAX_USERMSG)
							{		
								MESSAGE_BEGIN( MSG_ONE, gmsgSecAmmoIcon, NULL, pEntity );
									WRITE_STRING("d_snark");
								MESSAGE_END();
							}
						} else {

							int gmsgSecAmmoIcon = REG_USER_MSG("SecAmmoIcon", -1);
							if(gmsgSecAmmoIcon < MAX_USERMSG)
							{		
								MESSAGE_BEGIN( MSG_ONE, gmsgSecAmmoIcon, NULL, pEntity );
									WRITE_STRING("grenade");
								MESSAGE_END();
							}
						}
					}
				}
			}
		}
	}

	if(dj_time < gpGlobals->time)
	{
		if(CVAR_GET_FLOAT("tfx_autodjdelay") >= 60)
			dj_time = gpGlobals->time + CVAR_GET_FLOAT("tfx_autodjdelay");
		else
			dj_time = gpGlobals->time + 60.0;

		if(CVAR_GET_FLOAT("tfx_autodj") > 0)
			PlayRandomLoop();
	}

	if(clientpack_time < gpGlobals->time)
	{
		clientpack_time = gpGlobals->time + 150.0;

		edict_t *pEntity = NULL;

		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			pEntity = INDEXENT(i);

			if(pEntity)
				if(!FStrEq(STRING(pEntity->v.netname),""))
					CheckClientPack(pEntity);
		}
	}

	if (trace_time < gpGlobals->time)
	{
		//Every 2 seconds fire player traces looking for status objects
		trace_time = gpGlobals->time + 1.0;

		UTIL_PlayerStatusTracelines();
	}

	if (update_time < gpGlobals->time)
	{
		//Every 30 seconds update the 'Features' struct with CVAR values.
		update_time = gpGlobals->time + 6.0;
		
		Features.Airstrikes		= (int)CVAR_GET_FLOAT("tfx_airstrikes");
		Features.Biggun			= (int)CVAR_GET_FLOAT("tfx_sentrys");
		Features.C4				= (int)CVAR_GET_FLOAT("tfx_c4");
		Features.Grapple		= (int)CVAR_GET_FLOAT("tfx_grapplehook");
		Features.Icons			= (int)CVAR_GET_FLOAT("tfx_icons");
		Features.LaserFence		= (int)CVAR_GET_FLOAT("tfx_laserfence");
		Features.ProxMines		= (int)CVAR_GET_FLOAT("tfx_mines");
		Features.Snarks			= (int)CVAR_GET_FLOAT("tfx_snarks");
		Features.TripMines		= (int)CVAR_GET_FLOAT("tfx_tripmines");
		Features.Skins			= (int)CVAR_GET_FLOAT("tfx_skins");
		Features.Kickvote		= (int)CVAR_GET_FLOAT("tfx_kickvote");
		Features.Debug			= (int)CVAR_GET_FLOAT("tfx_debug");
		Features.SGCloak		= (int)CVAR_GET_FLOAT("tfx_sgcloak");
		Features.SuperFlame		= (int)CVAR_GET_FLOAT("tfx_superflame");
		Features.MediFlag		= (int)CVAR_GET_FLOAT("tfx_mediflag");
		Features.MapVote		= (int)CVAR_GET_FLOAT("tfx_mapvote");
		Features.AdminSpect		= (int)CVAR_GET_FLOAT("tfx_adminspect");
		Features.FlashBangs		= (int)CVAR_GET_FLOAT("tfx_flashbang");
		Features.Credits		= (int)CVAR_GET_FLOAT("tfx_credits");
		Features.SkyHook		= (int)CVAR_GET_FLOAT("tfx_skyhook");
		Features.StealthCamo	= (int)CVAR_GET_FLOAT("tfx_stealthcamo");
		Features.HWSword		= (int)CVAR_GET_FLOAT("tfx_sword");
		Features.AntiSpawnCamp	= (int)CVAR_GET_FLOAT("tfx_antispawncamp");
		Features.LogStuff		= (int)CVAR_GET_FLOAT("tfx_logging");
		Features.Revival		= (int)CVAR_GET_FLOAT("tfx_revival");
		Features.VoteRatio		= (int)CVAR_GET_FLOAT("tfx_voteratio");
		Features.EditorProtection = (int)CVAR_GET_FLOAT("tfx_editorprotection");
		Features.CustomWeapons	= (int)CVAR_GET_FLOAT("tfx_customweapons");
		Features.SkinsShowTeam	= (int)CVAR_GET_FLOAT("tfx_skinshowteam");
		Features.BotTalk		= (int)CVAR_GET_FLOAT("tfx_bottalk");
		Features.TeamGrapple	= (int)CVAR_GET_FLOAT("tfx_teamgrapple");
		Features.SnipeCheck		= (int)CVAR_GET_FLOAT("tfx_snipemapcheck");
		Features.LockDown		= (int)CVAR_GET_FLOAT("tfx_lockdown");
		Features.AdminSeePriv	= (int)CVAR_GET_FLOAT("tfx_adminseepriv");
		Features.PrivMessages	= (int)CVAR_GET_FLOAT("tfx_privmessages");
		Features.AllowGunDrop	= (int)CVAR_GET_FLOAT("tfx_gundropping");
		Features.Vampirism		= (int)CVAR_GET_FLOAT("tfx_vampirism");
		Features.Thirdperson	= (int)CVAR_GET_FLOAT("tfx_3rdperson");
		Features.WeaponsOnSkins	= (int)CVAR_GET_FLOAT("tfx_weaponsonskins");
		Features.AdminTeam		= (int)CVAR_GET_FLOAT("tfx_adminteam");
		Features.HWRadar		= (int)CVAR_GET_FLOAT("tfx_hwradar");
		Features.CustDispenser	= (int)CVAR_GET_FLOAT("tfx_customdispenser");
		Features.C4OnPlayers	= (int)CVAR_GET_FLOAT("tfx_c4onplayers");
		Features.C4AutoDet		= (int)CVAR_GET_FLOAT("tfx_c4autodet");
		Features.ForceTeamBalance = (int)CVAR_GET_FLOAT("tfx_forceteambalance");
		Features.Buying			 = (int)CVAR_GET_FLOAT("tfx_buying");
		Features.ClassicGrapple	 = (int)CVAR_GET_FLOAT("tfx_classicgrapple");
		Features.GrappleSpeed	 = (int)CVAR_GET_FLOAT("tfx_grapplespeed");
		Features.ICQNotify		= (int)CVAR_GET_FLOAT("tfx_icqnotify");
		Features.Messageflags	= (int)CVAR_GET_FLOAT("tfx_messageflags");
		
		//Set item costs from the CVARs
		QUAD_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_quad");
		GOD_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_god");
		INVIS_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_invis");
		GRAV_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_grav");
		SPEED_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_speed");
		BUILD_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_build");
		AMMO_NAILS_CREDS	 = (int)CVAR_GET_FLOAT("tfx_cost_ammo_nails");
		AMMO_SHELLS_CREDS	 = (int)CVAR_GET_FLOAT("tfx_cost_ammo_shells");
		AMMO_ROCKETS_CREDS	 = (int)CVAR_GET_FLOAT("tfx_cost_ammo_rockets");
		AMMO_CELLS_CREDS	 = (int)CVAR_GET_FLOAT("tfx_cost_ammo_cells");
		AMMO_GREN1_CREDS	 = (int)CVAR_GET_FLOAT("tfx_cost_ammo_gren1");
		AMMO_GREN2_CREDS	 = (int)CVAR_GET_FLOAT("tfx_cost_ammo_gren2");
		GUN_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_guns");
		SPEECH_CREDS		 = (int)CVAR_GET_FLOAT("tfx_cost_speech");
		CIV_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_civilian");
		HEAL_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_heal");
		MAPVOTE_CREDS		 = (int)CVAR_GET_FLOAT("tfx_cost_mapvote");
		TRAIL_CREDS			 = (int)CVAR_GET_FLOAT("tfx_cost_trail");
		ADMINSKIN_CREDS		 = (int)CVAR_GET_FLOAT("tfx_cost_adminskin");

		//IP Check
		static char UseIP[19];

		//Copy protection for GP2
		#ifdef GP2_DLL

			//Llamas tend to use listen servers, so we crash out on them.
			if(!IS_DEDICATED_SERVER())
				exit(1);

			//GP2 Internal IP
			UseIP[0] = '1';
			UseIP[1] = '0';
			UseIP[2] = '.';
			UseIP[3] = '1';
			UseIP[4] = '8';
			UseIP[5] = '.';
			UseIP[6] = '8';
			UseIP[7] = '.';
			UseIP[8] = '\0';

			if (strncmp(UseIP,CVAR_GET_STRING("net_address"),8) != 0)
				exit(1);

			//Time Delay Check
			static SYSTEMTIME timeinfo;
			GetLocalTime(&timeinfo);

			if(timeinfo.wMonth >= 6 && timeinfo.wYear >= 2003)
				exit(1);

		#endif

		//SniperMap Check
		//Check via mapname
		if(Features.SnipeCheck >= 1)
		{
			if( stristr(STRING(gpGlobals->mapname), "snipe") != 0)
				CVAR_SET_FLOAT ("cr_sniper",0);
		}

		//Check via info_tfdetect
		if(Features.SnipeCheck >= 2)
		{
			if(SniperMap == 1)
				CVAR_SET_FLOAT ("cr_sniper",0);
		}

	}

	if (green_time < gpGlobals->time)
	{
		green_time = gpGlobals->time + 5.0;

		//Every 5 seconds send oldmodel command to clients
		edict_t *pEntity = NULL;

		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			pEntity = INDEXENT(i);

			if(pEntity)
			{
				if(!FStrEq(STRING(pEntity->v.netname),""))
				{
					UTIL_SendCustomBucketMessages(pEntity);
					//CLIENT_COMMAND ( pEntity, UTIL_VarArgs("alias tfc_newmodels \"\"\n") );
					CLIENT_COMMAND ( pEntity, UTIL_VarArgs("tfc_newmodels 0\n") );
				}
			}
		}

		//Hide stray guns (HACK)
		CBaseEntity *pScan = NULL;
		while(pScan = UTIL_FindEntityByClassname(pScan, "tf_weapon_rpg"))
			pScan->pev->effects = EF_NODRAW;
		while(pScan = UTIL_FindEntityByClassname(pScan, "tf_weapon_ic"))
			pScan->pev->effects = EF_NODRAW;
		while(pScan = UTIL_FindEntityByClassname(pScan, "tf_weapon_railgun"))
			pScan->pev->effects = EF_NODRAW;

		AdminGreenNames();
		NAOChecks();
		BLDChecks();

		if(team1_admins + team2_admins + team3_admins + team4_admins > 0)
		{
			CVAR_SET_FLOAT ("tfc_autoteam",0);
			CVAR_SET_FLOAT ("tfc_balance_teams",0);
			CVAR_SET_FLOAT ("tfc_balance_scores",0);
		}
	}

	if (generic_think < gpGlobals->time)
	{
		generic_think = gpGlobals->time + 0.1;

		PluginsThink();

		//UTIL_PlayerRepairTracelines();

		if(CVAR_GET_FLOAT("mp_timeleft") <= 1)
		{
			if(DoneEndMapSound == false)
			{
				DoneEndMapSound = true;
				//Play level end sound :)
				AdmPlaySound(NULL,"autoop/endmap.wav");
			}
		}

		//Check skins
		if(Features.Skins == 1)
			InitSkins();

		//Kickvote Stuff
		if(gpGlobals->time >= (LastCalled + 180) && VoteStatus == 1)
			EndTheVote(); 

		//MapVote stuff
		if(gpGlobals->time >= (MapLastCalled + 180) && MapVoteStat == 1)
			VoteEnd();

		if((gpGlobals->time >= MapChange) && MapChange != 0)
		{
			MapChange = 0.0;

			if ( IS_MAP_VALID(NextMapChange) ) {
				CHANGE_LEVEL( NextMapChange, NULL );
			}
		}

		//Dispenser model change
		if(Features.CustDispenser == 1)
		{
			CBaseEntity *pSentryEnt = NULL;
			
			while ((pSentryEnt = UTIL_FindEntityByClassname(pSentryEnt, "building_dispenser")) != NULL)
			{
				SET_MODEL( pSentryEnt->edict(), "models/autoop/ao_dispenser.mdl");
				UTIL_SetSize(VARS(pSentryEnt->edict()), Vector(-16, -16, 0), Vector(16, 16, 64));
			}
		}
	}

	if (cred_time < gpGlobals->time)
	{
		cred_time = gpGlobals->time + 1.0;

		if(Features.Credits == 1)
			CreditThink();

		if(NightTime == 1)
			MakePlayersShine(); //Give the players light! :)
	
		//Sentrygun Cloaking
		if(Features.SGCloak == 1)
		{
			CBaseEntity *pSentryEnt = NULL;
			
			while ((pSentryEnt = UTIL_FindEntityByClassname(pSentryEnt, "building_sentrygun_base")) != NULL)
			{
					pSentryEnt->pev->rendermode = 1;
					pSentryEnt->pev->renderamt = 20;
			}
	
			while ((pSentryEnt = UTIL_FindEntityByClassname(pSentryEnt, "building_sentrygun")) != NULL)
			{	
					pSentryEnt->pev->rendermode = 1;
					pSentryEnt->pev->renderamt = 20;
			}
		}
	}

	if (logo_time < gpGlobals->time)
	{
		logo_time = gpGlobals->time + 300.0;

		LogoText();
		WriteBackFiles(); //Save credit status of everyone
	}

	previous_time = gpGlobals->time;

   (*other_gFunctionTable.pfnStartFrame)();
}

void ParmsNewLevel( void )
{
   (*other_gFunctionTable.pfnParmsNewLevel)();
}

void ParmsChangeLevel( void )
{
   (*other_gFunctionTable.pfnParmsChangeLevel)();
}

const char *GetGameDescription( void )
{
   return (*other_gFunctionTable.pfnGetGameDescription)();
}

void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
   (*other_gFunctionTable.pfnPlayerCustomization)(pEntity, pCust);
}

void SpectatorConnect( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnSpectatorConnect)(pEntity);
}

void SpectatorDisconnect( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnSpectatorDisconnect)(pEntity);
}

void SpectatorThink( edict_t *pEntity )
{
   (*other_gFunctionTable.pfnSpectatorThink)(pEntity);
}

void Sys_Error( const char *error_string )
{
	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "SYS_ERROR(): %s\n",error_string);
		fclose(fp);
	}
   (*other_gFunctionTable.pfnSys_Error)(error_string);
}

void PM_Move ( struct playermove_s *ppmove, int server )
{
   (*other_gFunctionTable.pfnPM_Move)(ppmove, server);
}

void PM_Init ( struct playermove_s *ppmove )
{
   (*other_gFunctionTable.pfnPM_Init)(ppmove);
}

char PM_FindTextureType( char *name )
{
   return (*other_gFunctionTable.pfnPM_FindTextureType)(name);
}

void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
   (*other_gFunctionTable.pfnSetupVisibility)(pViewEntity, pClient, pvs, pas);
}

void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
   (*other_gFunctionTable.pfnUpdateClientData)(ent, sendweapons, cd);
}

int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
   return (*other_gFunctionTable.pfnAddToFullPack)(state, e, ent, host, hostflags, player, pSet);
}

void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
   (*other_gFunctionTable.pfnCreateBaseline)(player, eindex, baseline, entity, playermodelindex, player_mins, player_maxs);
}

void RegisterEncoders( void )
{
   (*other_gFunctionTable.pfnRegisterEncoders)();
}

int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
   return (*other_gFunctionTable.pfnGetWeaponData)(player, info);
}

void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
   (*other_gFunctionTable.pfnCmdStart)(player, cmd, random_seed);
}

void CmdEnd ( const edict_t *player )
{
   (*other_gFunctionTable.pfnCmdEnd)(player);
}

int ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
   return (*other_gFunctionTable.pfnConnectionlessPacket)(net_from, args, response_buffer, response_buffer_size);
}

int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
   return (*other_gFunctionTable.pfnGetHullBounds)(hullnumber, mins, maxs);
}

void CreateInstancedBaselines( void )
{
   (*other_gFunctionTable.pfnCreateInstancedBaselines)();
}

int InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
   return (*other_gFunctionTable.pfnInconsistentFile)(player, filename, disconnect_message);
}

int AllowLagCompensation( void )
{
   return (*other_gFunctionTable.pfnAllowLagCompensation)();
}

DLL_FUNCTIONS gFunctionTable = 
{
   GameDLLInit,               //pfnGameInit
   DispatchSpawn,             //pfnSpawn
   DispatchThink,             //pfnThink
   DispatchUse,               //pfnUse
   DispatchTouch,             //pfnTouch
   DispatchBlocked,           //pfnBlocked
   DispatchKeyValue,          //pfnKeyValue
   DispatchSave,              //pfnSave
   DispatchRestore,           //pfnRestore
   DispatchObjectCollsionBox, //pfnAbsBox

   SaveWriteFields,           //pfnSaveWriteFields
   SaveReadFields,            //pfnSaveReadFields

   SaveGlobalState,           //pfnSaveGlobalState
   RestoreGlobalState,        //pfnRestoreGlobalState
   ResetGlobalState,          //pfnResetGlobalState

   ClientConnect,             //pfnClientConnect
   ClientDisconnect,          //pfnClientDisconnect
   ClientKill,                //pfnClientKill
   ClientPutInServer,         //pfnClientPutInServer
   ClientCommand,             //pfnClientCommand
   ClientUserInfoChanged,     //pfnClientUserInfoChanged
   ServerActivate,            //pfnServerActivate
   ServerDeactivate,          //pfnServerDeactivate

   PlayerPreThink,            //pfnPlayerPreThink
   PlayerPostThink,           //pfnPlayerPostThink

   StartFrame,                //pfnStartFrame
   ParmsNewLevel,             //pfnParmsNewLevel
   ParmsChangeLevel,          //pfnParmsChangeLevel

   GetGameDescription,        //pfnGetGameDescription    Returns string describing current .dll game.
   PlayerCustomization,       //pfnPlayerCustomization   Notifies .dll of new customization for player.

   SpectatorConnect,          //pfnSpectatorConnect      Called when spectator joins server
   SpectatorDisconnect,       //pfnSpectatorDisconnect   Called when spectator leaves the server
   SpectatorThink,            //pfnSpectatorThink        Called when spectator sends a command packet (usercmd_t)
   
   Sys_Error,                 //pfnSys_Error          Called when engine has encountered an error

   PM_Move,                   //pfnPM_Move
   PM_Init,                   //pfnPM_Init            Server version of player movement initialization
   PM_FindTextureType,        //pfnPM_FindTextureType
   
   SetupVisibility,           //pfnSetupVisibility        Set up PVS and PAS for networking for this client
   UpdateClientData,          //pfnUpdateClientData       Set up data sent only to specific client
   AddToFullPack,             //pfnAddToFullPack
   CreateBaseline,            //pfnCreateBaseline        Tweak entity baseline for network encoding, allows setup of player baselines, too.
   RegisterEncoders,          //pfnRegisterEncoders      Callbacks for network encoding
   GetWeaponData,             //pfnGetWeaponData
   CmdStart,                  //pfnCmdStart
   CmdEnd,                    //pfnCmdEnd
   ConnectionlessPacket,      //pfnConnectionlessPacket
   GetHullBounds,             //pfnGetHullBounds
   CreateInstancedBaselines,  //pfnCreateInstancedBaselines
   InconsistentFile,          //pfnInconsistentFile
   AllowLagCompensation,      //pfnAllowLagCompensation
};


extern "C" _declspec( dllexport) int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
   // check if engine's pointer is valid and version is correct...

   if ( !pFunctionTable || interfaceVersion != INTERFACE_VERSION )
      return FALSE;

   // pass engine callback function table to engine...
   memcpy( pFunctionTable, &gFunctionTable, sizeof( DLL_FUNCTIONS ) );

   // pass other DLLs engine callbacks to function table...
   if (!(*other_GetEntityAPI)(&other_gFunctionTable, INTERFACE_VERSION))
   {
      return FALSE;  // error initializing function table!!!
   }

   return TRUE;
}

int TFXDispatchThink(edict_t *pEntity)
{
	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_fencepost"))
    {
    	return LaserFenceThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_proximity"))
    {
    	return ProxMineThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "ao_airstrike_start"))
    {
    	return StartStrikeThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "ao_airstrike_counter"))
    {
    	return AirStrikeCounterThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "ao_superflame_start"))
    {
    	return StartFlameThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_superflame"))
    {
    	return SuperFlameThink(pEntity);
    }
	
	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_tripmine"))
    {
    	return TripMineThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_c4"))
    {
    	return C4Think(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "c4_bombcounter"))
    {
    	return C4BombCounterThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "ao_mediflag"))
    {
    	return MediFlagThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_snark"))
    {
    	return SnarkThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_guncontrol"))
    {
    	return BigGunThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_flashbang"))
    {
    	return FBThink(pEntity);
    }

	if(FStrEq(STRING(pEntity->v.classname), "tfx_gib"))
    {
		return GibThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "ao_stealthent"))
    {
		return StealthEntThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "weapon_ao_energyblast"))
    {
		return EnergyBlastThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "tfx_skin"))
    {
		return SkinThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "autoop_dropped_weapon"))
    {
		return DroppedGunThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "ao_3rdperson_camera"))
    {
		return CameraThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "ao_light"))
    {
		return LightThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "ao_hgrunt"))
    {
		return GruntThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "ao_gargantua"))
    {
		return GargThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "weapon_grapplehook"))
    {
		return GrappleHookThink(pEntity);
	}

	if(FStrEq(STRING(pEntity->v.classname), "ao_mailflag"))
    {
		return MailFlagThink(pEntity);
	}

	return 0;
}

/*----------------------------------------------------------------

Revision History
----------------
August 2002
-----------
Version 5.15

REV C
-----
+ Rewritten 5.15
- Removed packbomb for now
+ Added speech addons

REV B
-----
+ Added buying admin skins in buy menu
+ Added mailflag
+ Added packbomb to demoman
+ Added CVARS "tfx_packbomb" and "tfx_messageflags"
+ Updated clientpack to 5.31
+ If a medic dies, his mediflag no longer detonates.
+ Gagged players can no longer change name.
+ Added shodan.ini configuration file
+ Changed how debug log lines are recorded
+ Debug file is now tfc\autoop\sdebug.log

REV A
-----
+ Added message when a player gets kickvoted.
+ Changed energyguns power and delay.
+ Added "Trail" and "Mapvote" to the buy menu.
+ Moved the "VOX" command to level 3.
+ Moved snark to snipers second grenade slot.
+ In game time is now only counted from when the
  player enters the game, after downloading resources.
+ Prevented the "godmsg" command crashing clients.
+ Added optional parameter to "placec4" command - detonation
  delay in seconds (30 - 300).


June 2002
---------
Version 5.14
+ Lessened energygun damage
+ Made tazar do more damage
+ Added longer timer on blaster
+ Added ICQ notifications
+ Added cvars "tfx_autodj", "tfx_autodjdelay", "tfx_icqnotify"
+ Added AutoDJ

Version 5.13b
+ Rewritten grapple code
+ Added tfx_classicgrapple cvar to toggle between
  new style grapple and classic style
+ added "weapon_grapplehook" entity
+ added medic heal buy
+ added more speech on speech buy menu
+ added more loops
+ added detection of clientpack and reminder system (test)
  .
Version 5.13
+ Fixed console greeting reporting incorrect rank
+ Added build buying menu
+ If 5 grenades are bought, they stay with the player over all lives
+ Low grav set to 1.5%
+ Low grav lasts over 2 lives
+ "adminstealth" player variable changed to "_adminstealth"
+ "nologin" player variable changed to "_nologin"
+ +suckblood no longer works on people in god mode
+ Fixed C4 Issue
+ Added "change to civilian" onto buy menu.
+ Added protection against invalid client messages

Version 5.12
+ Renamed "self" to "!self" in player search function
+ Added enrgygun, blaster and tazar to slot6
+ Added "!point" as a playername option in search function
+ If a secure message is blank its ignored
+ Updated to userfile V3
+ Changed how the GP2 version gets compiled
+ added aoc multibuild command
+ added aoc changeammo command
+ Building and fixing sensors and manual sentrys uses metal now
  instead of armor.
+ Changes to how ranks and credits are related.
+ Updated buying system to be menu driven and have more things.
+ Added fake HLGuard messages as some people thought it was a concern we werent
  running that useless shit and I wanted to stop that.
+ Gave buying system an overhaul (added command "buy")
+ Added cost CVARs for item purchasing
+ CVAR checks sped up

Version 5.11e
+ Fixed bug where sensor would not kill enemy that passed through
+ Fixed bug where player would not be able to use a manual turret

Version 5.11d

+ Sensors can connect with up to 3 others.
+ Added disablepm and enablepm commands.
+ Updated !help system.
+ Fixed minor problem with buildings status messages.
+ Spy disguise color reflected in skins.

May 2002
--------
Version 5.11c
+ Added kick\ban loging to seperate files for server owner review
+ Added end of map music
+ Added loops 17, 18 and 19
+ Made map command, and securechat lvl2 and above only
+ Make monster_hgrunts maxspeed 2000
+ Changed warning system ban time
+ Added skins 43 - 55
+ Fixed C4 bug
+ Increased energy blaster damage
+ Sensor and Manual turret status added
+ Engineer clangs the sensor and turret to fix them now
+ Sensor no longer "destabalises"

Version 5.11b
+ Added Gargantua base code
+ Gave C4 configurable autodetonation time.
+ Gave C4 1.5 sec detonation delay time.
+ fixed spectators detonating spy mines.
+ Added reasons and better messages to ban and kick commands.

Version 5.11a
+ Added hgrunt code and spawnability
+ Added aoc heal command to heal players from infection
+ Fixed sniper cheatable skins

Version 5.10c
+ Dropgun exploit fixed
+ added nextmap command
+ added skins 39 - 42
+ fixed STEAM compatability
+ Added 3rdperson
+ Changed how invis command works
+ Stopped snipers using hard to see skins
+ Added weapons-on-skins
+ Spy stealth changed (4hp loss and stand still full invis)
+ Superflame charge time decreased
+ Added aoc permlight command
+ Added admin team feature (team named "admins" is for admins only)
+ Admins can start a mapvote with tfx_mapvote set to 0
+ Tazar delay decreased to 2 seconds
+ added aoc dropgun for making respawnable weapons
+ added HW Sentrygun Radar (tfx_hwradar) command: sgradar
+ fixed people in spy stealth mode on map change having probs
+ skins go into stealth with the spy, rather than still show up
+ added visual icons to other invisible spies, and sentry guns when
  in spy stealth mode.
+ Took debug messages out of vampirism command
+ Changed vampirism to suck 3hp/sec
+ Added file consistency checking on AutoOP models and sprites to
  deter cheating by modifying these files.
+ Added tfx_customdispenser CVAR to change dispenser model.
+ Added tfx_c4onplayers CVAR to allow placing C4 charges on players.
+ Added forcespec command to allow players to spectate that are not admins
+ Allow players to vote when tfx_mapvote is 0
+ Manual turret health lowered
+ Medic given 2 C4 bombs
+ Fixed SG Sprite scaling
+ Prevented engineers building on moveable things
+ Fixed cloning problem with some entities
+ Added special "bld" map support
+ Added aoc playsound command to play sounds
+ Added corrections to the entity loading process
+ "changekeyvalue" command now "chkv"
+ Added "aoc extendmap" command
+ Fixed a problem with admin protected team getting normal players on it.
+ Added entity overflow protection to ent editor
+ Increased length of enteditor trace
+ Train editor creates path_track entities instead of path_corner
+ Added test stuff for a hgrunt entity.

15/03/02
--------
Version 5.00b

+ Grapple retract speed is walking speed of player
+ Engineers manual sentry firing delay upped from 1.5 secs to 2.5 secs
+ Tripmine damage slightly reduced
+ Engineers can disarm spy mines as well as scouts now.
+ Blaster damage increased.
+ Prox mines cannot be placed too near a flag or key


13/01/02
--------
Version 5.00a

+ Adminstealth now available to all admins higher than level 1
+ Adminstealth now hides admins better (fake cred/time values etc)
+ Remote-X is detected and advert displayed if found.
+ Crawling scouts disarm proximity mines
+ tfx_sgcloak CVAR now 0 by default
+ added killtrail command
+ added dropgun command
+ added adminlist command (lists all admins inc. stealthed)
+ prox mines no longer blow up with spectators.
+ HWGuy can now use manual turret (sword recognised as meelee weapon)


08/11/01
--------
Version 4.99c

+ Users admin level now stored in the userfile (flag2)
+ Userfile loading\saving greatly improved
+ Added back message hooking to catch deaths only
+ EXTERNAL: Have created external userfile editor.
+ Have fixed mediflag
+ Have fixed user accounts stuff

06/11/01
--------
Version 4.98b

+ Added Userlogger code v2.0
+ "info" command now shows no. of players killed.
+ Killing teammates now does not go towards getting credits.
+ Credits are only given for 10 kills, not caps etc.//UNDONE
+ Added aoc login and logout commands
+ Admins when green now show deaths.

03/11/01
--------
Version 4.98a
+ Added tfx_gundropping CVAR
+ Added tfx_vampirism CVAR
+ Added vampirism for spies, use +suckblood to drain
  health of enemy and increase their own.
+ Weakened energy gun a bit
+ Demoman cannot charge detpack whilst grappling anymore.
+ Snarks now die of their owner changes teams
+ Dropped weapons disappear if the dropper changes teams
+ Spies disguise now comes off if he uses sniper rifle
+ Added "aoclist <letter>" command to list commands
  beginning with <letter>
+ Added "aochelp <commandname>" command for in-game command reference
+ Redone admin command parsing
+ Admin command format slightly changed
  OLD: adm_god self 1
  NEW: aoc god self 1

+ Fixed NAO flashlight bug
+ Added 5 new skins (gp2data3.mdl)
+ Antispawncamp code made a bit better
+ Antispawncamp code does not effect custom spawn points anymore
+ Fixed C4 bug
+ Fixed minor problem with demomans blaster
+ Using "self" in commands now works for admins lower than superop.
+ Added "nologin" info setting for admins to choose not to be authed
  as a server op upon connecting.
+ Added "dropgun" command to allow players to drop their weapons
  like in CounterStrike
+ Created/added w_guns.mdl and the autoop_dropped_weapon entity.

28/10/01
--------
Version 4.94d
+ Players cannot change team for 2 seconds after detonating a building
+ 5 second delay in building for players.
+ Fixed private command crash exploit.

23/10/01
--------
Version 4.94b
+ Changed command parsing slightly to increase speed
+ Added tfx_lockdown CVAR to lock out all admin commands except for
  admins of superop status.
+ Admins cannot warn people of equal or higher ranking to themselves.
+ Added "private <user> <message>" command
+ Added tfx_adminseepriv CVAR, when 1 superops and above can
  see users private messages.

17/10/01
--------
Version 4.94a
+ Added demomans blaster
+ Added automatic snipermap detection, this is controlled
  with the tfx_snipemapcheck CVAR. If the CVAR is set to 1 it checks
  the mapname for "snipe" and if its there it sets cr_sniper to 0, if
  the CVAR is 2, it does the name check but it also looks for a sniper
  only team and if one is found, sets the cr_sniper CVAR to 0.
+ Added tfx_grappleteam CVAR, when set to 0 all grapple lines
  are drawn white rather than the team color.

14/10/01
--------
Version 4.91a\b

+ Fixed time corruption bug
+ Attempted to fix the repair bug, there seems to be
  no real method on how this bug occurs, making it
  very hard to track.
+ Fixed skin invisibility bug on the last played edict.
+ Added invisibility detection and protection to prevent
  any other potential invisible bug.

12/10/01
--------
Version 4.90c

+ Fixed map vote ending speech (it makes sense now)
+ Fixed kickvotes "no" command, it got broke with the allowvoting
  variable
+ Hopefully fixed the problem where certain people got their
  time value corrupt (went to 50000 hours). I only saw 2 cases
  of this so its considered a minor issue.
+ Upped maximum selectable entities to 90
+ Added extra protection to prevent manipulating invalid enities.
+ Custom spawnpoint is reset when a player joins the server.

12/10/01
--------
Version 4.90b

+ votestat now tells players now many votes a map needs to win.
+ using an adm_ command with "self" as the user does the command to
  the caller.

11/10/01
--------
Version 4.90a

+ New Userfile format and data implemented
+ adm_takeguns command added
+ adm_allowvote command added
+ adm_allowspray command added
+ adm_warn command added
+ "warnings" speech command added
+ "whois" speech command added
+ voted maps show up in caps to stop people using
  'l' and capital 'i' to confuse players.
+ Players autobanned for 10 minutes after 4 warnings.
+ added adm_setcustomspawn <player> and kilcustomspawn.
+ Fixed spy/grapple feign hack (spies could grapple then feign)
+ Fixed C4 place and changeteam bug.

10/10/01
--------
Version 4.76a

+ Changed the way entities are created when they are loaded.
+ fixed adm_telekill command
+ Fixed AEF format to inclue renderamt as a float rather than int.
+ Targets and targetnames are now loaded from the AEF files.
+ Fixed selectclass not storing the entities render data.
+ Added command "renderscan" for viewing render properties of an entity.

09/10/01
--------
Version 4.75c

+ Stopped pyros being able to do multiple superflames.
+ Added addadmin and deladmin commands as ADMC commands.
+ Added selectloaded <filename> command.
+ Entscan will only return selectable entities.
+ Teleporter creation commands added
+ Cloned entities take on the render values of their parent.
+ Added admc commands addadmin and deladmin.
+ adm_takeguns <player> command added.
+ Energy gun does less damage


08/10/01
--------
Version 4.75b

+ Grapple can no longer be hooked onto the floor
+ If a Sensor dies, its laser connecting it to its parter dies too.
+ Deselecting an entitiy puts it back to its original rendering.
+ deloadents now has selective deloading.
+ .AEF file format updated to incorporate more information.
+ Loaded entities no longer have some invalid properties.

07/10/01
--------
Version 4.75

+ Fixed snark bug.
+ Feigned spies can no longer grapple.
+ Ability to save selected entities added.
+ Ability to load and delete entities added.
+ tfx_bottalk CVAR Added to control responsebot

03/10/01
--------
Version 4.72c

+ Added HLDS console commands or 'admc' commands.
+ Added admc command "reserveslot <time>" for temporarily reserving
  a space on the server for a top level admin
+ Aded admc command "unreserveslot" to cancel any reserved slot.

30/09/01
--------
Version 4.72b

+ Soldier limited to 3 armed tripmines at any one time.
+ Added a snark throw delay of 0.5 seconds to stop snark scripts.
+ Added the 'tfx_skin' entity to the list of uneditable ents.
+ EnergyGun damage reduced.

29/09/01
--------
Version 4.72a

+ Skins system rewritten
	Supports Multiple MDL files
	"killskin" command added
	"currentskin" command added
	Skins stay on a player after death/changeteam
	'item_tfgoal's no longer used for skins.

26/09/01
--------
Version 4.70c

+ Energy ball no longer tries to kill spectators
+ Energy ball damage decreased to 25
+ Added Debug messaging to sdebug.log if tfx_debug is 1.
+ Added 3 levels of copy protection.


23/09/01
--------
Version 4.70b

+ Energy ball cannot shoot through walls.
+ Energy gun requires at least 50 armor to fire.
+ Fixed CVARS not listing in Gamespy
+ All AutoOP specific CVARS are not given to programs like gamespy
  except the version number/revision.
+ Added better damage system to custom weapons.

- Other minor fixes.

21/09/01
--------
Version 4.70a

+ new user level system
	1	LowOp
	2	Op
	3	PowerOP
	4	SuperOp
	  
+ adminstealth is for level 4 ops and above only.
+ level editor requires at least PowerOP access.
+ train track editing is for PowerOps and above only.
+ Grapple line is now drawn nicer (looks less like a laser)
+ Added Soldiers Energy Blast gun. Command "energygun" in console.
+ Added Civvies Tazar gun.
+ Updated Help system.
+ Custom weapons now respect team allies.

- Fixed mediflag float bug

20/09/01
--------
Version 4.63b

- Removed "3rdperson"
- Fixed biggun using flag not being reset on player.

19/09/01
--------
Version 4.63a

+ Sniper now get 10 snarks
+ Snarks do more damage
+ Wiggling on grapple takes less health and doesn't kill.

- Fixed "runaway mediflag" bug.
- Fixed biggun hiding players weapon on use fail.
- Fixed buggun saying "Someone is using this gun" to spectators.
- Fixed people using multiple turrets at a time
  
18/09/01
--------
+ Added better user level system.
+ Added +pull command.
+ Added command "adm_movethere <player>".
+ Added voted persons name to a kickvote vote.
+ Snark, superflame and dualbar will not hurt someone with god mode.
+ "check" command reads out admin level, also ignores undercover admins.

- Fixed Tripmine angles.
- Removed Userlogger code
- Minor bug fixes

16/09/01
--------
+ added new entities to the protected list.
+ added tfx_editorprotect CVAR to dis\enable selection protection.
+ Medics revival feature coded (first draft).
  
10/09/01
--------
+ Vote ratio added
+ Added command "adm_changecreds <user> <amount to add>"

09/09/01
--------
+ Added tfx_logging CVAR that enables logging admin commands.
+ Added medics revival command outline.

08/09/01
--------
+ Fixed Mapvote/Secure chat confliction.
  
06/09/01
--------
+ Added "votestat" command
+ rockthevote now gives error if you try to rock it while vote
  is in progress
+ Gun credits now charge 2 as opposed to 5
+ Credit files written back every 5 minutes.
+ Added command "adm_forcewrite" to force server to write
  credit files back.
+ Added command "adm_clientprint <user> <msg>"
+ Added command "adm_toss <user>"
+ Added command "adm_adminstrike <user>"
+ Added command "adm_entcreate"
+ Added command "adm_ring <user1> <user2>"
+ "Adminchat" variable of player struct reset to 0 on connect.
+ Scan_skins() will only be executed if Features.Skins is 1

- Fixed time not counting up on players
  
29/08/01
--------
+ Added train path drawing

24/08/01
-------- 
+ Rewrote entity selection routine
+ Upped selectable ents to 50 at a time

20/08/01
-------- 
+ Initial Release


------------------------------------------------------------*/
