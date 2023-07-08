/*****************************************************\
*	Auto-OP Shodan
*
*	File: commands.cpp
*	Description: Parsing of user input.
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
#include "admincommands.h"
#include "leveleditor.h"
#include "aohgrunt.h"
#include "skins.h"
#include "stripper.h"
#include "buying.h"
#include "3rdperson.h"
#include "buying.h"
#include "userstrings.h"
#include "plugins.h"
#include <direct.h>
#include <stdio.h>
#include <winsock.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern DLL_FUNCTIONS other_gFunctionTable;
extern char LastSaid[];
extern char LastPlayer[];
extern int VoteStatus;
extern float LastCalled;
extern int iKickUser;
extern int team_allies[4];

extern int team1_admins;
extern int team2_admins;
extern int team3_admins;
extern int team4_admins;

extern int MapVoteStat;
extern int NightTime;

bool TFXPlayerCommand(edict_t *pEntity, const char *pcmd, const char *pcmd1, const char *pcmd2, const char *pcmd3, const char *pcmd4, const char *pcmd5)
{
	if ( FStrEq(pcmd, "menuselect" ))
	{
		if(PlayerInfo[ENTINDEX(pEntity)].MenuStatus > 0)
		{
			if(UTIL_SpectCheck(pEntity) == 0)
			{
				BuyMenuSelect(pEntity,atoi(pcmd1),PlayerInfo[ENTINDEX(pEntity)].MenuStatus);
			} else {
				PlayerInfo[ENTINDEX(pEntity)].MenuStatus = 0;
			}
			return 1;
		}
	}

	#ifndef GP2_DLL
		
		if ( FStrEq(pcmd, "randloop" ))
		{
			PlayRandomLoop();
		}

		if ( FStrEq(pcmd, "sendicq" ))
		{
			//SendICQ(pEntity,"RobTest","rob@planetfortress.com","Test ICQ","Hello!","85668646");
			SendStandardICQ("ALL","TESTSTANDARD","STANDARD ICQ TEST");
		}

	#endif

	//Speech commands
	if ( FStrEq(pcmd, "say" ) || FStrEq(pcmd, "say_team" )) {

		FreqCheck(pEntity);

		if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 1)
			return 1;

		if(PlayerInfo[ENTINDEX(pEntity)].AdminChat == 1)
		{
			AdmAdminChat(pEntity,pcmd1);
			return 1;
		}

		//COPY WHAT PLAYER SAID AND THEIR NAME TO MEMORY
		strcpy(LastSaid,pcmd1);
		strcpy(LastPlayer,STRING(pEntity->v.netname));

		int HelpCom = 1;

		HelpCom = strncmp( pcmd1, "!help" , 5 );
		if(HelpCom == 0) //Is it a help command?
		{
			ShowHelp(pEntity,pcmd1); //Show the help
			return 1; //returning 1 makes the command never get to the TFC DLL.	
		}

		//Info - Gives credit data on a player
		HelpCom = strncmp( pcmd1, "info " , 5 );
		if(HelpCom == 0)
		{
			PlayerCredInfo(pcmd1);
			return 0;
			
		}

		//Whois
		HelpCom = strncmp( pcmd1, "whois " , 6 );
		if(HelpCom == 0)
		{
			PlayerWhois(pcmd1);
			return 0;
		}

		//Warnings
		HelpCom = strncmp( pcmd1, "warnings " , 9 );
		if(HelpCom == 0)
		{
			PlayerWarnings(pcmd1);
			return 0;
		}

		//Time - Gives time data on a player
		HelpCom = strncmp( pcmd1, "time " , 5 );
		if(HelpCom == 0)
		{
			PlayerTimeInfo(pcmd1);
			return 0;
			
		}

		//Check - Checks admin status of a player
		HelpCom = strncmp( pcmd1, "check " , 6 );
		if(HelpCom == 0)
		{
			PlayerCheck(pcmd1);
			return 0;
			
		}

		//Rank - Get rank info on a player
		HelpCom = strncmp( pcmd1, "rank " , 5 );
		if(HelpCom == 0)
		{
			PlayerRankInfo(pcmd1);
			return 0;
		}

		if(Features.BotTalk == 1)
		{
			//They talking to the responsebot?
			if(_strnicmp( pcmd1, BOT_NAME, strlen(BOT_NAME)) == 0)
			{
				BotCom(pEntity,pcmd1,1);
				return 0;
				
			}

			if(_strnicmp( pcmd1, BOT_NAME2, strlen(BOT_NAME2)) == 0)
			{
				BotCom(pEntity,pcmd1,2);
				return 0;
				
			}
		}

		if(FStrEq(pcmd1, "timeleft"))
		{
			TimeLeft();
		}

		if(FStrEq(pcmd1, "nextmap"))
		{
			SayNextMap();
		}

		//Mapvote
		if((Features.MapVote == 1) || (PlayerInfo[ENTINDEX(pEntity)].IsAdmin >= LEVEL_OP))
		{
			HelpCom = strncmp( pcmd1, "rockthevote" , 11 );
			if(HelpCom == 0)
			{
				if(MapVoteStat == 0)
				{
					VoteStart(pEntity);
					return 0;
				} else {
					BotTalkAllArgs(MV_ALREADYVOTING);
					return 0;
				}				
			}
		}
		
		if(MapVoteStat == 1)
		{
			HelpCom = strncmp( pcmd1, "vote " , 5 );
			if(HelpCom == 0)
			{
				VoteCast(pEntity,pcmd1);
				return 0;
				
			}

			HelpCom = strncmp( pcmd1, "votestat" , 8 );
			if(HelpCom == 0)
			{
				SayWinningMap();
				return 0;
			}

			if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin >= LEVEL_OP)
			{
				HelpCom = strncmp( pcmd1, "endthevote" , 11 );
				if(HelpCom == 0)
				{
					VoteEnd();
					return 0;
					
				}

				HelpCom = strncmp( pcmd1, "fuckthevote" , 11 );
				if(HelpCom == 0)
				{
					CancelMapVote();
					return 0;
					
				}
			}
		}

		//Kickvote
		if(VoteStatus == 1)
		{		
			if(FStrEq(pcmd1, "yes") || FStrEq(pcmd1, "no"))
				CastKickVote(pEntity,pcmd1);
		}
	}
	// END OF SPEECH COMMANDS ------------------------------------

	int Weapon = PlayerCurrentWeapon(pEntity,pcmd);
	if(Weapon > 0)
		PlayerInfo[ENTINDEX(pEntity)].CurrentWeapon = Weapon;

	//Teamchange check
	if(FStrEq(pcmd, "jointeam"))
	{
		if(PlayerInfo[ENTINDEX(pEntity)].CantChangeTeam > gpGlobals->time)
			return 1;
	}
	
	//Impulse 201 check
	if(FStrEq(pcmd, "impulse"))
	{
		if(FStrEq(pcmd1, "201"))
			if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanSpray == 1)
				return 1;
	}

	if(FStrEq(pcmd, "givecredits"))
	{
		GiveCredits(pEntity,pcmd1,atoi(pcmd2));
	}

	if(FStrEq(pcmd, "tell"))
	{
		if(PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 1)
		{
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Can't use TELL, you are gagged."));
			return 1; //Ignore tell message
		}
	}
	
	//Buying
	if(Features.Buying == 1)
	{
		if(UTIL_SpectCheck(pEntity) == 0)
		{
			if(FStrEq(pcmd, "buy"))
				ShowBuyMenu(pEntity);

			if(FStrEq(pcmd, "sgoptions"))
				ShowSGOptions(pEntity);

			if(FStrEq(pcmd, "sgrotate180"))
				UTIL_RotateSG(pEntity,TRUE);

			if(FStrEq(pcmd, "sgrotate45"))
				UTIL_RotateSG(pEntity,FALSE);
		}
	}

	//Kickvote command.
	if(FStrEq(pcmd, "kickvote") && (Features.Kickvote == 1 || PlayerInfo[ENTINDEX(pEntity)].IsAdmin >= LEVEL_LOW))
	{
		if(pcmd1 != NULL)
		{
			if(gpGlobals->time >= (LastCalled + 600) && PlayerInfo[ENTINDEX(pEntity)].StartedAVote == 0)
			{
				char *PlayerText = (char *)pcmd1;
				iKickUser = GetPartialPlayerIndex(PlayerText,ENTINDEX(pEntity));
				if(iKickUser > 0 && PlayerInfo[iKickUser].IsAdmin < 2)
				{
					edict_t *pKickVote = INDEXENT(iKickUser);
					BotTalkAllArgs(KV_VOTESTART,STRING(pEntity->v.netname),STRING(pKickVote->v.netname));
					LastCalled = gpGlobals->time;
					VoteStatus = 1;
					PlayerInfo[ENTINDEX(pEntity)].StartedAVote = 1;
				}
			} else {
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( KV_CANTSTART ));
			}
		}
	}

	//Spectator command
	if(Features.AdminSpect == 1)
	{
		if ( FStrEq(pcmd, "spectate"))
		{
			if(PlayerInfo[ENTINDEX(pEntity)].SpecOverride == 0)
			{
				if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < 1)
				{
					CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"access denied\"\n") );
					return 1;
				}
			} else {
				PlayerInfo[ENTINDEX(pEntity)].SpecOverride = 0;
			}
		}
	}

	if(Features.PrivMessages == 1)
	{
		if ( FStrEq(pcmd, "private"))
		{
			if(strlen(pcmd1) > 0)
			{
				//Only allow PMing if not gagged.
				if((PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk == 0) && (PlayerInfo[ENTINDEX(pEntity)].PMBlock == 0))
				{
					if(strlen(pcmd2) > 0)
					{
						PrivateMessage(pEntity,pcmd1,(CMD_ARGS() + (strlen(pcmd1) + 1)));
					} else {
						CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You must enter a message to send.\n"));
					}
				} else {
					CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You cannot send a private message, you may be gagged or have disabled it.\n"));
				}
			} else {
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Format: private <user> <message>\n"));
			}
		}

		if ( FStrEq(pcmd, "disablepm"))
		{
			PlayerInfo[ENTINDEX(pEntity)].PMBlock = 1;
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Sending and recieveing of private messages is disabled.\n"));
		}

		if ( FStrEq(pcmd, "enablepm"))
		{
			PlayerInfo[ENTINDEX(pEntity)].PMBlock = 0;
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Sending and recieveing of private messages is enabled.\n"));
		}

	}

	if(Features.AllowGunDrop == 1)
	{
		if ( FStrEq(pcmd, "dropgun"))
		{
			DropGun(pEntity);
		}
	}

	//Custom Weapons Commands
	if(Features.CustomWeapons == 1)
	{
		if(FStrEq(pcmd, "tf_weapon_energygun"))
		{
			SwitchToEnergyGun(pEntity);
		}

		if(FStrEq(pcmd, "tf_weapon_tazar"))
		{
			SwitchToTazar(pEntity);
		}

		if(FStrEq(pcmd, "tf_weapon_blaster"))
		{
			SwitchToBlaster(pEntity);
		}

		if(Features.TripMines == 1)
		{
			if(FStrEq(pcmd, "tripmine"))
			{
				PlaceTripMine(pEntity);
			}
		}

		/*if ( FStrEq(pcmd, "tf_weapon_tripmine" ))
		{
			if(PlayerInfo[ENTINDEX(pEntity)].TripMineAmmo > 0)
				SwitchToTripMine(pEntity);
			//CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "tf_weapon_tripmine has been detected."));
		}*/
	}

	//Skins command
	if(Features.Skins == 1)
	{
		if ( FStrEq(pcmd, "applyskin"))
		{
			int iSkin = atoi(pcmd1);
			ApplySkin(pEntity,iSkin);
		}

		if ( FStrEq(pcmd, "killskin"))
		{
			KillSkin(pEntity);
		}

		if ( FStrEq(pcmd, "currentskin"))
		{
			SayCurrentSkin(pEntity);
		}
	}

	//Medi Revive command
	if(Features.Revival == 1)
	{
		if ( FStrEq(pcmd, "revive"))
		{
			if(pEntity->v.playerclass == CLASS_MEDIC)
			{
				MediRevive(pEntity);
			}
		}
	}
	
	//Flashbang command
	if(Features.FlashBangs == 1)
	{
		if ( FStrEq(pcmd, "flashbang"))
		{
			if(pEntity->v.playerclass == CLASS_SCOUT)
			{
				TossFlashBang(pEntity);
			}
		}
	}

	//Message flag
	if(Features.Messageflags == 1)
	{
		if ( FStrEq(pcmd, "dropmessage"))
		{
			if(CMD_ARGC() <= 2)
			{
				if(strlen(pcmd1) > 0)
				{
					DropMailFlag(pEntity,pcmd1);
				} else {
					CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "You must specify a message.\n"));
				}
			} else {
				DropMailFlag(pEntity,CMD_ARGS());
			}

			//CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%d\n",CMD_ARGC()));
		}

		if ( FStrEq(pcmd, "killmessage"))
			DestroyMailFlag(pEntity);
	}

	//Flashlight command
	if(NightTime == 1)
	{
		if ( FStrEq(pcmd, "flashlight"))
		{
			if(pEntity->v.team > 0 && pEntity->v.team < 5)
			{
				if(PlayerInfo[ENTINDEX(pEntity)].FlashLightOff == 0)
				{
					CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "FlashLight OFF\n"));
					PlayerInfo[ENTINDEX(pEntity)].FlashLightOff = 1;
					pEntity->v.effects = 0;
				} else {
					CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "FlashLight ON\n"));
					PlayerInfo[ENTINDEX(pEntity)].FlashLightOff = 0;
					pEntity->v.effects = EF_DIMLIGHT;
				}
			}
		}
	}

	//Icon command
	if(Features.Icons == 1)
	{
		if ( FStrEq(pcmd, "icon" ) && PlayerInfo[ENTINDEX(pEntity)].IconTime <= gpGlobals->time && UTIL_SpectCheck(pEntity) == 0)
		{			
			PlayerInfo[ENTINDEX(pEntity)].IconTime = (gpGlobals->time + ICON_DELAY); //Set the icon delay
			ApplyIcon(pEntity,pcmd1);
		}
	}
	
	//Grapple Hook commands
	if (Features.Grapple == 1)
	{
		//Command executed when player drops off the hook.
		if ( FStrEq(pcmd, "-hook"))
		{
			if(Features.ClassicGrapple == 1)
			{
				if(PlayerInfo[ENTINDEX(pEntity)].GrapStat >= 1)
					PlayerDrops(pEntity,1);
			} else {
				KillGrapple(pEntity);
			}
		}

		if ( FStrEq(pcmd, "+hook"))
		{
			if(UTIL_SpectCheck(pEntity) == 0 && pEntity->v.health > 0 && (pEntity->v.deadflag == DEAD_NO))
			{
				if((!(pEntity->v.button & IN_ATTACK)) || (!FStrEq(STRING(pEntity->v.viewmodel),"models/v_tfc_sniper.mdl")))
				{
					if(Features.ClassicGrapple == 1)
					{
						if(PlayerInfo[ENTINDEX(pEntity)].GrapTime <= gpGlobals->time && PlayerInfo[ENTINDEX(pEntity)].GrapStat == 0)
							TossGrapple(pEntity);
	
					} else {
						ThrowGrapple(pEntity);
					}
				}
			}
		}
	}

	//Manual Sentry commands
	if(Features.Biggun == 1)
	{
		if(pEntity->v.playerclass == CLASS_ENGINEER)
		{
			if(FStrEq(pcmd, "buildturret"))
			{
				BuildGun(pEntity);
			}

			/*if(FStrEq(pcmd, "fixturret"))
			{
				FixSentry(pEntity);
			}*/

			if(FStrEq(pcmd, "detturret"))
			{
				if(pEntity->v.playerclass == CLASS_ENGINEER && pEntity->v.health > 0)
				{
					PlayerInfo[ENTINDEX(pEntity)].CantChangeTeam = gpGlobals->time + 2;
					DestroyPlayersGun(pEntity);
				}
			}
		}

		if(FStrEq(pcmd, "useturret"))
		{
			UseGun(pEntity);
		}
	}

	//Airstrike commands
	if(Features.Airstrikes == 1)
	{
		if ( FStrEq(pcmd, "+airstrike"))
		{
			if(pEntity->v.playerclass == CLASS_DEMOMAN && pEntity->v.health > 10)
			{
				SendStrike(pEntity);
			}
		}

		if ( FStrEq(pcmd, "-airstrike"))
		{
			if(pEntity->v.playerclass == CLASS_DEMOMAN && pEntity->v.health > 0)
			{
				CancelLaunch(pEntity);
			}
		}

		if ( FStrEq(pcmd, "placemarker"))
		{
			if(pEntity->v.playerclass == CLASS_SCOUT && pEntity->v.health > 0)
			{
				PlantMarker(pEntity);
			}
		}
	}

	/*//Holoduke
	if ( FStrEq(pcmd, "holoon"))
	{
		DeployHoloDuke(pEntity);
	}

	if ( FStrEq(pcmd, "holooff"))
	{
		DeactivateHoloDuke(pEntity);
	}*/

	//RestoreGreen command
	if ( FStrEq(pcmd, "unstealth"))
	{
		if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin > 1)
		{
			PlayerInfo[ENTINDEX(pEntity)].NoGreen = 0;
		}
	}

	//Snarks
	if(Features.Snarks == 1)
	{
		if(pEntity->v.playerclass == CLASS_SNIPER)
		{
			if(FStrEq(pcmd, "+gren2"))
			{		
				ThrowSnark(pEntity);
				return 1;
			}
		}
	}

	if(Features.Vampirism == 1)
	{
		if(pEntity->v.playerclass == CLASS_SPY)
		{
			if(FStrEq(pcmd, "+suckblood"))
				PlayerInfo[ENTINDEX(pEntity)].IsSucking = 1;

			if(FStrEq(pcmd, "-suckblood"))
				PlayerInfo[ENTINDEX(pEntity)].IsSucking = 0;
		}
	}

	//Level Editor commands
	if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin >= LEVEL_POWEROP)
	{
		int iIndex = ENTINDEX(pEntity);
		//ENTITY MANIPULATION COMMANDS
		if(FStrEq(pcmd, "select"))
		{
			SelectEntity(pEntity);
		}

		if(FStrEq(pcmd, "deselect"))
		{
			DeselectEntity(pEntity);
		}

		if(FStrEq(pcmd, "transent"))
		{
			int TransVal = atoi(pcmd1);
			PointTransEnt(pEntity,TransVal);
		}

		if(FStrEq(pcmd, "colorent"))
		{
			int Col1 = atoi(pcmd1);
			int Col2 = atoi(pcmd2);
			int Col3 = atoi(pcmd3);

			Vector vColor = Vector(Col1,Col2,Col3);
			PointColorEnt(pEntity,vColor);
		}

		if(FStrEq(pcmd, "moveentity"))
		{
			int Dir = atoi(pcmd1);
			MoveEnt(pEntity,Dir);
		}

		if(FStrEq(pcmd, "clone"))
		{
			CloneEnt(pEntity);
		}

		if(FStrEq(pcmd, "deselectall"))
		{
			DeselectAll(pEntity);
		}

		if(FStrEq(pcmd, "delentity"))
		{
			DeleteEntity(pEntity);
		}

		if(FStrEq(pcmd, "+quickmove"))
		{
			PlayerInfo[iIndex].MovingEnt = 1;
		}

		if(FStrEq(pcmd, "-quickmove"))
		{
			PlayerInfo[iIndex].MovingEnt = 0;
		}

		if(FStrEq(pcmd, "+lockx"))
		{
			PlayerInfo[iIndex].Selected.LockX = 1;
		}

		if(FStrEq(pcmd, "-lockx"))
		{
			PlayerInfo[iIndex].Selected.LockX = 0;
		}

		if(FStrEq(pcmd, "+locky"))
		{
			PlayerInfo[iIndex].Selected.LockY = 1;
		}

		if(FStrEq(pcmd, "-locky"))
		{
			PlayerInfo[iIndex].Selected.LockY = 0;
		}

		if(FStrEq(pcmd, "+lockz"))
		{
			PlayerInfo[iIndex].Selected.LockZ = 1;
		}

		if(FStrEq(pcmd, "-lockz"))
		{
			PlayerInfo[iIndex].Selected.LockZ = 0;
		}

		if(FStrEq(pcmd, "rotate"))
		{
			RotatePoint(pEntity);
		}
		
		if(FStrEq(pcmd, "rotatebrush"))
		{
			RotateBrush(pEntity);
		}

		if(FStrEq(pcmd, "chkv"))
		{
			ChangeKeyValue(pEntity,pcmd1,pcmd2);
		}

		if(FStrEq(pcmd, "triggerent"))
		{
			TriggerEntity(pEntity);
		}

		if(FStrEq(pcmd, "selectclass"))
		{
			SelectClass(pEntity,pcmd1,pcmd2);
		}

		if(FStrEq(pcmd, "entscan"))
		{
			ScanRadius(pEntity,0);
		}

		if(FStrEq(pcmd, "renderscan"))
		{
			ScanRadius(pEntity,1);
		}

		if(FStrEq(pcmd, "+pull"))
		{
			StartPull(pEntity);
		}

		if(FStrEq(pcmd, "-pull"))
		{
			EndPull(pEntity);
		}

		if(FStrEq(pcmd, "saveents"))
		{
			SaveEnts(pEntity,pcmd1);
		}

		if(FStrEq(pcmd, "loadents"))
		{
			LoadEnts(pEntity,pcmd1);
		}

		if(FStrEq(pcmd, "deloadents"))
		{
			KillAEFEnts(pEntity,pcmd1);
		}

		if(FStrEq(pcmd, "selectloaded"))
		{
			SelectLoaded(pEntity,pcmd1);
		}
	}

	if(Features.Debug >= 1)
	{
		if(FStrEq(pcmd, "allyinfo"))
		{
			int Team1 = atoi(pcmd1);
			int Team2 = atoi(pcmd2);
			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "BLUE:%d  RED:%d  YELLOW:%d  GREEN:%d)\n",team_allies[0],team_allies[1],team_allies[2],team_allies[3]));
			if(Team1 != Team2)
			{
				if(!team_allies[Team1-1] & (1<<(Team2-1)))
				{
					CLIENT_PRINTF( pEntity, print_console, "Enemy");
				} else {
					CLIENT_PRINTF( pEntity, print_console, "Friend");
				}
			}
		}

		if(FStrEq(pcmd, "allyinfo2"))
		{
			int Team1 = atoi(pcmd1);
			int Team2 = atoi(pcmd2);

			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%d and %d are ",Team1,Team2));

			if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
			{
				CLIENT_PRINTF( pEntity, print_console, "enemies\n");
			} else {
				CLIENT_PRINTF( pEntity, print_console, "friends\n");
			}
		}
		
		if(FStrEq(pcmd, "traceinfo"))
		{
			Vector v_src, v_dest;
			TraceResult tr;

			UTIL_MakeVectors( pEntity->v.v_angle );

			v_src = pEntity->v.origin + pEntity->v.view_ofs;
			v_dest = v_src + gpGlobals->v_forward * 2000;

			UTIL_TraceLine( v_src, v_dest, ignore_monsters, pEntity->v.pContainingEntity, &tr);

			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "vecPlaneNormal(%f,%f,%f)\n",tr.vecPlaneNormal.x,tr.vecPlaneNormal.y,tr.vecPlaneNormal.z));
		}
  
		if(FStrEq(pcmd, "entinfo"))
		{
    		char tString[8192];
			CBaseEntity *pCEntity = NULL;
    		pCEntity = NULL;
    		while(pCEntity = UTIL_FindEntityByClassname(pCEntity, pcmd1))
			{
        		sprintf(tString, "[AOD] <%s ent index: %d> <owner ent index: %d> <controller: %d/%d/%d/%d>\n", STRING(pCEntity->pev->classname), ENTINDEX(ENT(pCEntity->pev)), pCEntity->pev->owner ? ENTINDEX(pCEntity->pev->owner) : -1, pCEntity->pev->controller[0], pCEntity->pev->controller[1], pCEntity->pev->controller[2], pCEntity->pev->controller[3]);
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s\n",tString));
				sprintf(tString, "[AOD] <iuser: %d/%d/%d/%d> <fuser: %f/%f/%f/%f>\n", pCEntity->pev->iuser1, pCEntity->pev->iuser2, pCEntity->pev->iuser3, pCEntity->pev->iuser4, pCEntity->pev->fuser1, pCEntity->pev->fuser2, pCEntity->pev->fuser3, pCEntity->pev->fuser4);
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s\n",tString));
				sprintf(tString, "[AOD] <euser: %d/%d/%d/%d> <chain: %d>\n", pCEntity->pev->euser1 ? ENTINDEX(pCEntity->pev->euser1) : -1, pCEntity->pev->euser2 ? ENTINDEX(pCEntity->pev->euser2) : -1, pCEntity->pev->euser3 ? ENTINDEX(pCEntity->pev->euser3) : -1, pCEntity->pev->euser4 ? ENTINDEX(pCEntity->pev->euser4) : -1, pCEntity->pev->chain ? ENTINDEX(pCEntity->pev->chain) : -1);
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s\n",tString));
				sprintf(tString, "[AOD] <team: %d> <colormap: %d> <health: %f> <dmg: %f> <armorvalue: %f>\n", pCEntity->pev->team, pCEntity->pev->colormap, pCEntity->pev->health, pCEntity->pev->dmg, pCEntity->pev->armorvalue);
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s\n",tString));
				sprintf(tString, "[AOD] <angles: (%f, %f, %f)> <origin: (%f, %f, %f)>\n", pCEntity->pev->angles.x, pCEntity->pev->angles.y, pCEntity->pev->angles.z, pCEntity->pev->origin.x, pCEntity->pev->origin.y, pCEntity->pev->origin.z);
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s\n",tString));
				sprintf(tString, "[AOD] <model: %s> <skin: %d> <sequence: %d>\n", STRING(pCEntity->pev->model), pCEntity->pev->skin, pCEntity->pev->sequence);
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "%s\n",tString));
			}
		}
	}

	//Stealth command
	if(Features.StealthCamo == 1)
	{
		if(FStrEq(pcmd, "stealth"))
		{
			if(PlayerInfo[ENTINDEX(pEntity)].StealthOn == 0)
			{
				ActivateStealth(pEntity);
			} else {
				DeactivateStealth(pEntity);
			}
		}
	}

	//HWRadar
	if(Features.HWRadar == 1)
	{
		if(FStrEq(pcmd, "sgradar"))
			if(pEntity->v.playerclass == CLASS_HWGUY)
				DoRadar(pEntity);
	}

	//Laser Fence commands
	if(Features.Biggun == 1)
	{
		if ( FStrEq(pcmd, "buildsensor"))
		{
			if(pEntity->v.playerclass == CLASS_ENGINEER && pEntity->v.health > 0)
			{
				BuildLaserFence(pEntity);
			}
		}

		if ( FStrEq(pcmd, "detsensor"))
		{
			if(pEntity->v.playerclass == CLASS_ENGINEER && pEntity->v.health > 0)
			{
				PlayerInfo[ENTINDEX(pEntity)].CantChangeTeam = gpGlobals->time + 2;
				
				CBaseEntity *pScan = NULL;
				while(pScan = UTIL_FindEntityByClassname(pScan, "weapon_ao_fencepost"))
				{
					if (pScan->pev->euser1 == pEntity)
						DetonateFencePost(pScan->edict());
				}
			}
		}
	}

	//Proximity Mines
	if(Features.ProxMines == 1)
	{
		if ( FStrEq(pcmd, "dropmine"))
		{
			if(pEntity->v.playerclass == CLASS_SPY && pEntity->v.health > 0)
			{
				DropMine(pEntity);
			}
		}
	}

	//C4
	if(Features.C4 == 1)
	{
		if(FStrEq(pcmd, "placec4"))
		{
			if(pcmd1 != NULL)
			{
				PlaceC4(pEntity,atoi(pcmd1));
			} else {
				PlaceC4(pEntity,0);
			}
		}

		if(FStrEq(pcmd, "detc4"))
		{
			if(pEntity->v.playerclass != CLASS_MEDIC)
				return 1;

			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Detonating...\n"));

			CBaseEntity *pMine = NULL;
			while(pMine = UTIL_FindEntityByClassname(pMine, "weapon_ao_c4"))
			{
				if(pMine->pev->euser1 == pEntity)
				{
					if(pMine->pev->iuser3 == 0)
					{
						pMine->pev->fuser2 = gpGlobals->time + C4_DETDELAY;
						CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Detonating...\n"));
					} else {
						CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "C4 is set on a timer, cannot be detonated.\n"));
					}
				}
			}
			//DetonateC4(pEntity,1);
		}
	}

	//Superflame
	if(Features.SuperFlame == 1)
	{
		if ( FStrEq(pcmd, "+superflame"))
		{
			if(pEntity->v.playerclass == CLASS_PYRO && pEntity->v.health > 10)
			{
				ChargeFlame(pEntity);
			}
		}

		if ( FStrEq(pcmd, "-superflame"))
		{
			if(pEntity->v.playerclass == CLASS_PYRO && pEntity->v.health > 0)
			{
				CancelFlame(pEntity);
			}
		}
	}

	//MediFlag
	if(Features.MediFlag == 1)
	{
		if(FStrEq(pcmd, "placeflag"))
		{
			DropFlag(pEntity);
		}
	}

	//Admin commands
	if(FStrEq(pcmd, "aoc"))
		if(TFXAdminCommand(pEntity,pcmd1,pcmd2,pcmd3,pcmd4,pcmd5))
			return 1;

	if(FStrEq(pcmd, "aochelp"))
		if(TFXAdminCommandHelp(pEntity,pcmd1))
			return 1;

	if(FStrEq(pcmd, "aoclist"))
		if(TFXListCommands(pEntity,pcmd1))
			return 1;

	//3rd Person
	if(Features.Thirdperson == 1)
	{
		if(FStrEq(pcmd, "3rdpersonon"))
			Start3rdPerson(pEntity);

		if(FStrEq(pcmd, "3rdpersonoff"))
			End3rdPerson(pEntity);
	}

	if(Features.ForceTeamBalance == 1)
	{
		if ( FStrEq(pcmd, "jointeam"))
		{
			if(TeamChangeValid(atoi(pcmd1),pEntity->v.team) == false)
			{
				CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"team switch denied\"\n") );
				CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Too many players on selected team.\n"));
				CLIENT_PRINTF( pEntity, print_center,UTIL_VarArgs( "Too many players on selected team.\n"));
				return 1;
			}
		}
	}

	//Admin only team blocking
	if(Features.AdminTeam == 1)
	{
		if ( FStrEq(pcmd, "jointeam"))
		{
			if ( FStrEq(pcmd1, "1"))
			{
				if(team1_admins == 1)
				{
					if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < 1)
					{
						CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"access denied\"\n") );
						return 1;
					}
				}
			}
			if ( FStrEq(pcmd1, "2"))
			{
				if(team2_admins == 1)
				{
					if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < 1)
					{
						CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"access denied\"\n") );
						return 1;
					}
				}
			}
			if ( FStrEq(pcmd1, "3"))
			{
				if(team3_admins == 1)
				{
					if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < 1)
					{
						CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"access denied\"\n") );
						return 1;
					}
				}
			}
			if ( FStrEq(pcmd1, "4"))
			{
				if(team4_admins == 1)
				{
					if(PlayerInfo[ENTINDEX(pEntity)].IsAdmin < 1)
					{
						CLIENT_COMMAND ( pEntity, UTIL_VarArgs("speak \"access denied\"\n") );
						return 1;
					}
				}
			}
		}
	}

	return 0;
}

int PlayerCurrentWeapon(edict_t *pEntity, const char *pcmd)
{
	int Weapon = 0;

	if(FStrEq(pcmd, "tf_weapon_axe"))
		Weapon = WEAPON_AXE;

	if(FStrEq(pcmd, "tf_weapon_knife"))
		Weapon = WEAPON_KNIFE;

	if(FStrEq(pcmd, "tf_weapon_spanner"))
		Weapon = WEAPON_SPANNER;

	if(FStrEq(pcmd, "tf_weapon_rpg"))
		Weapon = WEAPON_ROCKET_LAUNCHER;

	if(FStrEq(pcmd, "tf_weapon_sniperrifle"))
		Weapon = WEAPON_SNIPER_RIFLE;

	if(FStrEq(pcmd, "tf_weapon_medikit"))
		Weapon = WEAPON_MEDIKIT;

	if(FStrEq(pcmd, "tf_weapon_flamethrower"))
		Weapon = WEAPON_FLAMETHROWER;

	if(FStrEq(pcmd, "tf_weapon_ac"))
		Weapon = WEAPON_ASSAULT_CANNON;

	if(FStrEq(pcmd, "tf_weapon_gl"))
		Weapon = WEAPON_GRENADE_LAUNCHER;

	if(FStrEq(pcmd, "tf_weapon_pl"))
		Weapon = WEAPON_PIPEBOMB_LAUNCHER;

	if(FStrEq(pcmd, "tf_weapon_ic"))
		Weapon = WEAPON_INCENDIARY;

	if(FStrEq(pcmd, "tf_weapon_tranq"))
		Weapon = WEAPON_TRANQ;

	if(FStrEq(pcmd, "tf_weapon_railgun"))
		Weapon = WEAPON_LASER;

	if(FStrEq(pcmd, "tf_weapon_superng"))
		Weapon = WEAPON_SUPER_NAILGUN;

	if(FStrEq(pcmd, "tf_weapon_supershotgun"))
		Weapon = WEAPON_SUPER_SHOTGUN;

	if(FStrEq(pcmd, "tf_weapon_autorifle"))
		Weapon = WEAPON_AUTO_RIFLE;

	if(FStrEq(pcmd, "tf_weapon_shotgun"))
		Weapon = WEAPON_TF_SHOTGUN;

	if(FStrEq(pcmd, "tf_weapon_ng"))
		Weapon = WEAPON_NAILGUN;

	return Weapon;
}













