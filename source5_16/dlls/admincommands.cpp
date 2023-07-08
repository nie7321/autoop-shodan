/*****************************************************\
*	Auto-OP Shodan
*
*	File: admincommands.cpp
*	Description: Parsing of aoc commands.
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
#include "commands.h"
#include "voting.h"
#include "customweapons.h"
#include "leveleditor.h"
#include "admincommands.h"
#include "skins.h"
#include "userstrings.h"
#include "stripper.h"
#include "CIniFile.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;

AdminCommand_t CommandLib[] = {
	{"Bans a player for specified time.","<player> <mins> \"<reason>\"","ban",LEVEL_OP,0},
	{"Changes current map.","<mapname>","map",LEVEL_OP},
	{"Kicks a user from the server.","<player> \"<reason>\"","kick",LEVEL_LOW,0},
	{"Prints text to users com line.","<player> <message>","clientprint",LEVEL_OP,0},
	{"Changes mp_teamplay value on server.","<value>","teamplay",LEVEL_OP,0},
	{"Allows/Disallows spectator chat.","<value>","specttalk",LEVEL_OP,0},
	{"Changes gravity value.","<value>","gravity",LEVEL_OP,0},
	{"Sends a large HUD message to all players.","<message>","godmsg",LEVEL_POWEROP,0},
	{"Toggles players gag value.","<player>","gag",LEVEL_LOW,0},
	{"Send com message to admins only.","(none)","securechat",LEVEL_OP,0},
	{"Adds an admin with specified details.","<wonid> <level>","addadmin",LEVEL_SUPEROP,0},
	{"Deletes specified admin.","<wonid>","deladmin",LEVEL_SUPEROP,0},
	{"Forces all userdata to be written to disk.","(none)","forcewrite",LEVEL_SUPEROP,0},
	{"Prints message to all users com lines.","<message>","say",LEVEL_SUPEROP,0},
	{"Adds specified value to players credits.","<player> <amount>","changecreds",LEVEL_SUPEROP,1},
	{"Enables/Disables god on a player.","<player> <1/0>","god",LEVEL_OP,1},
	{"Enables/Disables hologram mode on a player.","<player> <1/0>","hologram",LEVEL_POWEROP,1},
	{"Enables/Disables invisibility on a player.","<player> <1/0>","invis",LEVEL_OP,1},
	{"Sets players health value.","<player> <value>","health",LEVEL_OP,1},
	{"Enables/Disables noclip on a player.","<player> <1/0>","noclip",LEVEL_OP,1},
	{"Changes players speed (400 = Scout).","<player> <value>","changespeed",LEVEL_OP,1},
	{"Gives all weapons to player.","<player>","giveallguns",LEVEL_SUPEROP,1},
	{"Changes players armor value.","<player> <value>","armor",LEVEL_POWEROP,1},
	{"Creates a light on a player.","<player> <r> <g> <b>","light",LEVEL_OP,0},
	{"Shakes everyones screen.","(none)","screenshake",LEVEL_OP,0},
	{"Coloured screenfade effect.","<r> <g> <b>","screenfade",LEVEL_OP,0},
	{"Puts trail effect on a player.","<player> <r> <g> <b>","trail",LEVEL_OP,0},
	{"Spins a players view.","<player> <angle>","dizzyness",LEVEL_OP,0},
	{"Drops a temporary model into the world.","<player> <modelpath>","dropmodel",LEVEL_SUPEROP,0},
	{"Airstrikes a player.","<player>","adminstrike",LEVEL_OP,0},
	{"Puts an electric ring around 2 players.","<player1> <player2>","ring",LEVEL_OP,0},
	{"Tosses a player in the air.","<player>","toss",LEVEL_OP,0},
	{"Creates specific entity.","<player> <entID>","entcreate",LEVEL_SUPEROP,0},
	{"Shows track nodes.","(none)","shownodes",LEVEL_POWEROP,0},
	{"Hides track nodes.","(none)","hidenodes",LEVEL_POWEROP,0},
	{"Starts drawing specified track.","<color>","startpath",LEVEL_POWEROP,0},
	{"Stops drawing track.","(none)","endpath",LEVEL_POWEROP,0},
	{"Creates train entity out of entity selected in slot 1.","(none)","createtrain",LEVEL_POWEROP,0},
	{"Deletes specified track.","<color>","deletepath",LEVEL_POWEROP,0},
	{"Draws track with lines.","<color>","drawpath",LEVEL_POWEROP,0},
	{"Forces player onto blue team.","<player>","forceblue",LEVEL_OP,0},
	{"Forces player onto red team.","<player>","forcered",LEVEL_OP,0},
	{"Forces player onto yellow team.","<player>","forceyellow",LEVEL_OP,0},
	{"Forces player onto green team.","<player>","forcegreen",LEVEL_OP,0},
	{"Executes command on player.","<player> <command>","clientdo",LEVEL_POWEROP,0},
	{"Plays a VOX sentence to all players.","<VOX sounds>","vox",LEVEL_OP,0},
	{"Executes a command on all players.","<command>","executeall",LEVEL_SUPEROP,0},
	{"Moves player to you.","<player>","movehere",LEVEL_OP,1},
	{"Moves you to player.","<player>","movethere",LEVEL_OP,1},
	{"Changes players class.","<player> <0-9,11>","class",LEVEL_POWEROP,1},
	{"Strips player of their guns.","<player>","takeguns",LEVEL_SUPEROP,0},
	{"Warns a player and adds 1 to their warnings.","<player>","warn",LEVEL_OP,0},
	{"Toggles players ability to vote in map and kick votes.","<player>","allowvote",LEVEL_POWEROP,0},
	{"Toggles players ability to use spraypaint.","<player>","allowspray",LEVEL_OP,0},
	{"Wipes custom spawn point set for player.","<player>","killcustomspawn",LEVEL_POWEROP,0},
	{"Player will always respawn in your current location.","<player>","setcustomspawn",LEVEL_POWEROP,0},
	{"Sets players time to specified number of hours.","<player> <hours>","settime",LEVEL_SUPEROP,0},
	{"Creates teleporter entrance with specified name.","<name>","telestart",LEVEL_POWEROP,0},
	{"Creates teleporter exit with specified name.","<name>","teleend",LEVEL_POWEROP,0},
	{"Deletes all entrances/exits with specified name.","<name>","telekill",LEVEL_POWEROP,0},
	{"Authenticates you as a server op.","(none)","login",LEVEL_NONE,0},
	{"Logs you out of server op mode.","(none)","logout",LEVEL_NONE,0},
	{"Drops a gun on the floor.","<weapon name>","dropweapon",LEVEL_POWEROP,0},
	{"Kills all trail type things on a player.","<player>","killtrail",LEVEL_OP,0},
	{"Lists all admins on server (including stealth).","(none)","adminlist",LEVEL_SUPEROP,0},
	{"Creates a permanent light entity.","<red> <green> <blue>","permlight",LEVEL_POWEROP,0},
	{"Plays a sound without using 'speak' or 'play' console commands.","<filename>","playsound",LEVEL_OP,0},
	{"Makes a player a spectator.","<player>","makespectate",LEVEL_OP,0},
	{"Extends map for 30 minutes.","(none)","extendmap",LEVEL_OP,0},
	{"Puts a black messagebox on all players screens.","<message> or 0","msgbox",LEVEL_SUPEROP,0},
	{"Puts an AutoOP EntEffect on a player.","<player> <effect number>","veffect",LEVEL_SUPEROP,0},
	{"Heals a player from medic infection.","<player>","heal",LEVEL_POWEROP,1},
	{"Spawns a monster_hgrunt into the game.","<canwalk 1 or 0>","spawngrunt",LEVEL_POWEROP,0},
	{"Spawns a gargantua into the game.","<canwalk 1 or 0>","spawngarg",LEVEL_POWEROP,0},
	{"Resets your sensor build flag.","(none)","resetbuild",LEVEL_SUPEROP,1},
	{"Changes player ammo amount.","<ammotype> <amount> <name>","changeammo",LEVEL_SUPEROP,1},
	{"Resets engineers building flag.","<buildtype> <name>","multibuild",LEVEL_SUPEROP,1},
	{"Sends ICQ to admins on the ICQ list.","<message>","sendicq",LEVEL_OP,0}
};
int TotalCommands = 77; //Total number of commands


bool TFXAdminCommand(edict_t *pEntity, const char *cmd, const char *arg1, const char *arg2, const char *arg3, const char *arg4)
{
	if(Features.LockDown == 1)
	{
		if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < LEVEL_HYPEROP)
		{
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Lockdown in progress: Command disabled.\n"));
			return 1;
		}
	}

	int CommandID = GetCommandIndex(cmd);

	if(CommandID == -1)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( CMD_UNKNOWN ));
		return 0;
	}

	//Ok, the command exists, make sure they have access to it
	if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < CommandLib[CommandID].ReqLevel)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( CMD_NO_ACCESS ));
		return 1;
	}

	//if they cannot use cheat commands, and this is a cheat
	//command, stop them here.
	if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.flag4 == 1)
	{
		if(CommandLib[CommandID].CheatCommand == 1)
		{
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( CMD_NO_CHEAT ));
			return 1;
		}
	}

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "TFXAdminCommand(): Player %d executed command \"%s\".\n",ENTINDEX(pEntity),cmd);
		fclose(fp);
	}

	//They have access, so find and run its function

	//Admin Commands
	if(CommandID == 0)
		AdmBan(pEntity,arg1,arg2,arg3);

	if(CommandID == 1)
		AdmMap(pEntity,arg1);

	if(CommandID == 2)
		AdmKick(pEntity,arg1,arg2);

	if(CommandID == 3)
		AdmClientPrint(pEntity,arg1,arg2);

	if(CommandID == 4)
		AdmTeamplay(pEntity,arg1);

	if(CommandID == 5)
		AdmSpecttalk(pEntity,arg1);

	if(CommandID == 6)
		AdmGravity(pEntity,arg1);

	if(CommandID == 7)
		AdmGodMessage(pEntity,arg1);

	if(CommandID == 8)				
		AdmGag(pEntity,arg1);

	if(CommandID == 9)
		AdmSetAdminTalk(pEntity);

	if(CommandID == 10)
		AddAdmin(pEntity,arg1,arg2);

	if(CommandID == 11)
		DelAdmin(pEntity,arg1);

	if(CommandID == 12)
		AdmForceWriteBack(pEntity);

	if(CommandID == 13)
		AdmSay(pEntity,arg1);

	if(CommandID == 14)
		AdmChangeCreds(pEntity,arg1,arg2);

	//Cheat Commands
	if(CommandID == 15)
		AdmGod(pEntity,arg1,arg2);

	if(CommandID == 16)
		AdmHologram(pEntity,arg1,arg2);

	if(CommandID == 17)
		AdmInvis(pEntity,arg1,arg2);

	if(CommandID == 18)
		AdmHealth(pEntity,arg1,arg2);

	if(CommandID == 19)
		AdmNoclip(pEntity,arg1,arg2);

	if(CommandID == 20)
		AdmChangeSpeed(pEntity,arg1,arg2);

	if(CommandID == 21)
		AdmGiveAllGuns(pEntity,arg1);

	if(CommandID == 22)
		AdmArmor(pEntity,arg1,arg2);

	//Gimmik commands
	if(CommandID == 23)
		AdmCreateLight(pEntity,arg1,arg2,arg3,arg4);

	if(CommandID == 24)
		AdmScreenShake(pEntity);

	if(CommandID == 25)
		AdmScreenFade(pEntity,arg1,arg2,arg3);

	if(CommandID == 26)
		AdmTrail(pEntity,arg1,arg2,arg3,arg4);

	if(CommandID == 27)
		AdmDizzyness(pEntity,arg1,arg2);

	if(CommandID == 28)
		AdmDropModel(pEntity,arg1,arg2);

	if(CommandID == 29)
		AdmAdminStrike(pEntity,arg1);

	if(CommandID == 30)
		AdmRing(pEntity,arg1,arg2);

	if(CommandID == 31)
		AdmToss(pEntity,arg1);

	if(CommandID == 32)
		AdmEntCreate(pEntity,arg1);

	//Train Creation Commands
	if(CommandID == 33)
		AdmShowNodes(pEntity);

	if(CommandID == 34)
		AdmHideNodes(pEntity);

	if(CommandID == 35)
		if(PlayerInfo[ENTINDEX(pEntity)].PathBuild.Drawingpath == 0)
			AdmStartPath(pEntity,arg1);

	if(CommandID == 36)
		if(PlayerInfo[ENTINDEX(pEntity)].PathBuild.Drawingpath > 0)
			AdmEndPath(pEntity,1);

	if(CommandID == 37)
		MakePathTrain(pEntity,arg1);

	if(CommandID == 38)
		DeleteTrack(pEntity,arg1);

	if(CommandID == 39)
		DrawTrackLines(pEntity,arg1);

	//Player commands
	if(CommandID == 40)
		AdmForceBlue(pEntity,arg1);

	if(CommandID == 41)
		AdmForceRed(pEntity,arg1);

	if(CommandID == 42)
		AdmForceYellow(pEntity,arg1);

	if(CommandID == 43)
		AdmForceGreen(pEntity,arg1);

	if(CommandID == 44)
		AdmClientExecute(pEntity,arg1,arg2);

	if(CommandID == 45)
		AdmVoxToEveryone(pEntity,arg1);

	if(CommandID == 46)
		AdmExecuteToAll(pEntity,arg1);

	if(CommandID == 47) //Old telehere
		AdmMoveHere(pEntity,arg1);

	if(CommandID == 48) //Reverse telehere
		AdmMoveThere(pEntity,arg1);

	if(CommandID == 49)
		AdmChangeClass(pEntity,arg1,arg2);

	if(CommandID == 50)
		AdmTakeGuns(pEntity,arg1);

	if(CommandID == 51)
		AdmWarn(pEntity,arg1);

	if(CommandID == 52)
		AdmAllowVote(pEntity,arg1);

	if(CommandID == 53)
		AdmSpray(pEntity,arg1);

	if(CommandID == 54)
		AdmKillCustomSpawn(pEntity,arg1);

	if(CommandID == 55)
		AdmSetCustomSpawn(pEntity,arg1);

	if(CommandID == 56)
			AdmSetTime(pEntity,arg1,atoi(arg2));

	//Teleporter commands
	if(CommandID == 57)
		AdmCreateTeleStart(pEntity,arg1);

	if(CommandID == 58)
		AdmCreateTeleEnd(pEntity,arg1);

	if(CommandID == 59)
		AdmKillTeleStructure(pEntity,arg1);

	//Login and out
	if(CommandID == 60)
		AdminAuth(pEntity,0);

	if(CommandID == 61)
		AdminDeAuth(pEntity);

	if(CommandID == 62)
		DropWeapon(pEntity,arg1);

	if(CommandID == 63)
		AdmKillTrail(pEntity,arg1);

	if(CommandID == 64)
		AdmAdminList(pEntity);

	if(CommandID == 65)
		AdmPermLight(pEntity,arg1,arg2,arg3);
	
	if(CommandID == 66)
		AdmPlaySound(pEntity,arg1);

	if(CommandID == 67)
		AdmAllowSpec(pEntity,arg1);

	if(CommandID == 68)
		AdmExtendMap(pEntity);

	if(CommandID == 69)
		AdmMsgBox(pEntity,arg1);

	if(CommandID == 70)
		AdmVEffect(pEntity,arg1,arg2);

	if(CommandID == 71)
		AdmDisinfectPlayer(pEntity,arg1);

	if(CommandID == 72)
		AdmSpawnGrunt(pEntity,arg1);

	if(CommandID == 73)
		AdmSpawnGarg(pEntity,arg1);

	if(CommandID == 74)
		AdmResetBuild(pEntity);

	if(CommandID == 75)
		AdmChangeAmmo(pEntity,arg1,arg2,arg3);

	if(CommandID == 76)
		AdmMultiBuild(pEntity,arg1,arg2);

	if(CommandID == 77)
		AdmICQ(pEntity,arg1);

	return 1;
}

int	GetCommandIndex(const char *cmd)
{
	for( int i = 0; i <= TotalCommands; i++)
	{
		if(FStrEq(cmd,CommandLib[i].Name))
			return i;
	}
	return -1; //Fail
}

bool TFXAdminCommandHelp(edict_t *pEntity, const char *cmd)
{
	int CommandID = GetCommandIndex(cmd);

	if(CommandID == -1)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Unrecognised AOC command.\n"));
		return 0;
	}

	//Ok, the command exists, make sure they have access to it
	if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < CommandLib[CommandID].ReqLevel)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You do not have the access to use the command you want help on.\n"));
		return 1;
	}

	CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "\n\"%s\": %s\n",CommandLib[CommandID].Name,CommandLib[CommandID].Description));
	CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Format: %s\n",CommandLib[CommandID].Format));
	return 1;
}

bool TFXListCommands(edict_t *pEntity, const char *letter)
{
	if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin <= LEVEL_NONE)
	{
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You do not have access to use this command.\n"));
		return 1;
	}

	bool FirstPass = false;
	
	for( int i = 0; i <= TotalCommands; i++)
	{
		if(_strnicmp( CommandLib[i].Name, letter, 1 ) == 0)
			if(!FirstPass)
			{
				FirstPass = true;
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "\nCOMMAND LIST:\n%s: %s\n",CommandLib[i].Name,CommandLib[i].Description));
			} else {
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s: %s\n",CommandLib[i].Name,CommandLib[i].Description));
			}
	}
	return 1;
}

void LoadCommandLevels()
{
	FILE *cLvlFile = fopen("tfc\\autoop\\shodan.ini", "rb");

	if(cLvlFile == NULL)
	{
		//The ini file does not exist
		UTIL_LogPrintf( "[AUTOOP] shodan.ini not found - using default command levels.\n");
		return;
	}

	//Load the access levels from the INI file

	int iTempLvl = 0;

	CIniFile iniFile;
	iniFile.SetFileName("tfc\\autoop\\shodan.ini");

	for(int i = 0; i <= TotalCommands; i++)
	{
		iTempLvl = iniFile.GetInt("Command Levels",CommandLib[i].Name);

		if((iTempLvl >= 0) && (iTempLvl <= 4))
			CommandLib[i].ReqLevel = iTempLvl;
		else
			UTIL_LogPrintf( "[AUTOOP] Error in shodan.ini - invalid access level on command %s, using default.\n", CommandLib[i].Name);
	}
}



