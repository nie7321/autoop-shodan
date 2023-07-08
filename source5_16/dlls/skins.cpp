/*****************************************************\
*	Auto-OP Shodan
*
*	File: skins.cpp
*	Description: Skin Changing Functions for Shodan
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
#include "skins.h"
#include "stripper.h"
#include "userstrings.h"
#include <direct.h>
#include <stdio.h>

extern DLL_FUNCTIONS other_gFunctionTable;
extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;


char *g_SkinNames[]={"aoskin000", "aoskin001", "aoskin002", "aoskin003", "aoskin004", "aoskin005", "aoskin006" , "aoskin007", "aoskin008", "aoskin009", "aoskin010", "aoskin011", "aoskin012", "aoskin013", "aoskin014", "aoskin015", "aoskin016", "aoskin017", "aoskin018", "aoskin019", "aoskin020", "aoskin021", "aoskin022", "aoskin023", "aoskin024", "aoskin025", "aoskin026", "aoskin027", "aoskin028", "aoskin029", "aoskin030", "aoskin031", "aoskin032", "aoskin033" };
char *g_WeaponSkinNames[]={"aowskin000", "aowskin001", "aowskin002", "aowskin003", "aowskin004", "aowskin005", "aowskin006" , "aowskin007", "aowskin008", "aowskin009", "aowskin010", "aowskin011", "aowskin012", "aowskin013", "aowskin014", "aowskin015", "aowskin016", "aowskin017", "aowskin018", "aowskin019", "aowskin020", "aowskin021", "aowskin022", "aowskin023", "aowskin024", "aowskin025", "aowskin026", "aowskin027", "aowskin028", "aowskin029", "aowskin030", "aowskin031", "aowskin032", "aowskin033" };

//The skin library keeps all required data on skins
//including model name, body number and more.

TFXSkin_t SkinLibrary[] = {
	{0,0,SKIN_MDL,0,"Homer"}, 
	{1,1,SKIN_MDL,0,"LegoMan"}, 
	{2,2,SKIN_MDL,0,"Trinity"},
	{3,3,SKIN_MDL,0,"Giraffe"},
	{4,4,SKIN_MDL,0,"Toothpaste"}, 
	{5,5,SKIN_MDL,0,"Skeleton"}, 
	{6,6,SKIN_MDL,0,"Dr.Eastman"}, 
	{7,7,SKIN_MDL,0,"Alien"}, 
	{8,8,SKIN_MDL,0,"Santa"}, 
	{9,9,SKIN_MDL,0,"Spiderman"}, 
	{10,10,SKIN_MDL,0,"Superman"}, 
	{11,11,SKIN_MDL,0,"Quake2"}, 
	{12,12,SKIN_MDL,0,"G-Man"}, 
	{13,13,SKIN_MDL,0,"Barney"}, 
	{14,14,SKIN_MDL,0,"Xen Tree"}, 
	{15,15,SKIN_MDL,0,"SentryGun"}, 
	{16,16,SKIN_MDL,0,"Dispenser"}, 
	{17,17,SKIN_MDL,4,"RoboRob"},
	{18,0,SKIN_MDL2,0,"Eyeball"}, 
	{19,1,SKIN_MDL2,0,"Reaper"}, 
	{20,2,SKIN_MDL2,0,"Trekker"}, 
	{21,3,SKIN_MDL2,0,"Judge Death"}, 
	{22,4,SKIN_MDL2,3,"Nude Woman"}, 
	{23,5,SKIN_MDL2,0,"Yoda"}, 
	{24,6,SKIN_MDL2,0,"Kermit"},
	{25,0,SKIN_MDL3,0,"Box"},
	{26,1,SKIN_MDL3,0,"Wolverine"},
	{27,2,SKIN_MDL3,0,"Cyber Rabbit"},
	{28,3,SKIN_MDL3,0,"Baboon"},
	{29,4,SKIN_MDL3,0,"Hanky"},
	{30,0,SKIN_MDL4,0,"Slim"},
	{31,1,SKIN_MDL4,0,"Bender"},
	{32,2,SKIN_MDL4,0,"Major K"},
	{33,3,SKIN_MDL4,0,"Scorpion"},
	{34,4,SKIN_MDL4,0,"Sniper Wolf"},
	{35,5,SKIN_MDL4,0,"Onna"},
	{36,0,SKIN_MDL5,0,"OBI 07"},
	{37,1,SKIN_MDL5,0,"Quake"},
	{38,2,SKIN_MDL5,0,"Slave"},
	{39,3,SKIN_MDL5,0,"Spawn"},
	{40,4,SKIN_MDL5,0,"The Fist"},
	{41,5,SKIN_MDL5,0,"Tigger"},
	{42,6,SKIN_MDL5,0,"Ninja Turtle"},
	{43,0,SKIN_MDL6,0,"Limp Bizkit"},
	{44,1,SKIN_MDL6,2,"Solid Snake"},
	{45,2,SKIN_MDL6,0,"Scream"},
	{46,3,SKIN_MDL6,0,"7 of 9"},
	{47,4,SKIN_MDL6,0,"Bad-Day"},
	{48,5,SKIN_MDL6,0,"Bert"},
	{49,6,SKIN_MDL6,0,"Boba-Fett"},
	{50,7,SKIN_MDL6,0,"Dunbine"},
	{51,8,SKIN_MDL6,0,"KitKat"},
	{52,9,SKIN_MDL6,0,"Midgets in Black"},
	{53,10,SKIN_MDL6,2,"Spam"},
	{54,11,SKIN_MDL6,0,"Thor"},
	{55,12,SKIN_MDL6,2,"Chris Redfield"},
	{56,0,SKIN_MDL7,0,"AeonFlux"},
	{57,1,SKIN_MDL7,0,"Daffy Duck"},
	{58,2,SKIN_MDL7,0,"EVA"},
	{59,3,SKIN_MDL7,2,"MegaTron"},
	{60,4,SKIN_MDL7,0,"Rigell"}
};

//Applies a Skin to a player.
void ApplySkin(edict_t *pEntity, int iSkinNo)
{
	int iIndex = ENTINDEX(pEntity);

	if ( (pEntity->v.deadflag) != DEAD_NO )
		return;

	if ((pEntity->v.health < 1) || (pEntity->v.effects == EF_NODRAW) || (pEntity->v.movetype != MOVETYPE_WALK))
		return;

	if(pEntity->v.playerclass == CLASS_SNIPER)
	{
		if((iSkinNo == 5) || (iSkinNo == 25))
		{
			BotTalkOneArgs(pEntity,SK_NOSNIPERS);
			CLIENT_PRINTF( pEntity, print_console,SK_NOSNIPERS);
			return;
		}
	}

	//Get a pointer for the skin entity
	CBaseEntity *pSkinEnt = NULL;
	CBaseEntity *pWeaponSkinEnt = NULL;

	pSkinEnt = UTIL_FindEntityByTargetname( NULL, g_SkinNames[iIndex]);
	pWeaponSkinEnt = UTIL_FindEntityByTargetname( NULL, g_WeaponSkinNames[iIndex]);
	
	if ((pSkinEnt != NULL) && FStrEq(STRING(pSkinEnt->pev->classname),"tfx_skin"))
	{
		//Depending upon the param 2 change to the correct skin.
		if ( (iSkinNo < 0) || (iSkinNo > TOTALSKINS) ) {
			BotTalkOne(pEntity,"Skin number invalid!\n");
			return;
		} else {
			
			if(SkinLibrary[iSkinNo].ReqAccess > PlayerInfo[ENTINDEX(pEntity)].IsAdmin)
			{
				//Override this check is client has bought an admin skin.
				if(PlayerInfo[ENTINDEX(pEntity)].AbilityOverride & ABILITY_ADMINSKIN)
				{
					PlayerInfo[ENTINDEX(pEntity)].AbilityOverride &= ~ABILITY_ADMINSKIN;
				} else {
					BotTalkOne(pEntity,SK_NOACCESS);
					CLIENT_PRINTF( pEntity, print_console,SK_NOACCESS);
					return;
				}
			}

			//Change the skin's body and model
			pSkinEnt->pev->body = SkinLibrary[iSkinNo].BodyNumber;
			SET_MODEL(pSkinEnt->edict(), SkinLibrary[iSkinNo].MDLName);

			//Set the SkinID to the skin entity for reference
			pSkinEnt->pev->iuser1 = SkinLibrary[iSkinNo].ID;
	
			//Make it visible
			pSkinEnt->pev->iuser3 = 1;
			pSkinEnt->pev->rendermode = kRenderNormal;
			pSkinEnt->pev->renderamt = 255;
			pSkinEnt->pev->renderfx = 0;
			
			pSkinEnt->pev->movetype = MOVETYPE_FOLLOW;
			pSkinEnt->pev->aiment = pEntity;
			
			//Make sure the skin is at the players location before skinning
			if(pSkinEnt->pev->origin == pEntity->v.origin)
			{
				pEntity->v.rendermode = 1;
				pEntity->v.renderamt = 0;
			}

			//Set a glowshell on the skin if the tfx_skinshowteam CVAR is 1
			if (Features.SkinsShowTeam == 1)
			{
				pSkinEnt->pev->rendermode = kRenderNormal;
				pSkinEnt->pev->renderfx = kRenderFxGlowShell;
				pSkinEnt->pev->renderamt = 5;
				
				//Now color the glow depending upon team
				if (pEntity->v.team == 1)
				{
					pSkinEnt->pev->rendercolor = Vector(0,0,100);
				}
				else if (pEntity->v.team == 2)
				{
					pSkinEnt->pev->rendercolor = Vector(100,0,0);
				}
				else if (pEntity->v.team == 3)
				{
					pSkinEnt->pev->rendercolor = Vector(100,100,0);
				}
				else if (pEntity->v.team == 4)
				{
					pSkinEnt->pev->rendercolor = Vector(0,100,0);
				}
			}

			//If ths skin was successful, then we see if the weapon skin
			//is available and if so, apply it.
			if ((pWeaponSkinEnt != NULL) && FStrEq(STRING(pWeaponSkinEnt->pev->classname),"tfx_skin"))
			{
				pWeaponSkinEnt->pev->iuser3 = 1;
				pWeaponSkinEnt->pev->rendermode = kRenderNormal;
				pWeaponSkinEnt->pev->renderamt = 255;
				pWeaponSkinEnt->pev->renderfx = 0;
				
				pWeaponSkinEnt->pev->movetype = MOVETYPE_FOLLOW;
				pWeaponSkinEnt->pev->aiment = pEntity;
				
				//Set the SkinID to the skin entity for reference
				pWeaponSkinEnt->pev->iuser1 = SkinLibrary[iSkinNo].ID;
			}

			//Inform the player they have been skinned
			BotTalkOneArgs(pEntity,"Skin %d applied (%s)\n",iSkinNo,SkinLibrary[iSkinNo].SkinName);

			CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( SK_APPLIED ));

			UTIL_LogPrintf( "[AUTOOP] %s put on a skin.\n", STRING(pEntity->v.netname));


		}
	} else {
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( SK_FATALERROR ));
	}
	return;
}

//Takes skin off player
void KillSkin(edict_t *pEntity)
{
	int iIndex = ENTINDEX(pEntity);

	if ( (pEntity->v.deadflag) != DEAD_NO )
		return;

	if ((pEntity->v.health < 1) || (pEntity->v.effects == EF_NODRAW) || (pEntity->v.movetype != MOVETYPE_WALK))
		return;

	//Get a pointer for the skin entity
	CBaseEntity *pSkinEnt = NULL;
	CBaseEntity *pWeaponSkinEnt = NULL;

	pSkinEnt = UTIL_FindEntityByTargetname( NULL, g_SkinNames[iIndex]);
	pWeaponSkinEnt = UTIL_FindEntityByTargetname( NULL, g_WeaponSkinNames[iIndex]);
	
	if (pSkinEnt && pSkinEnt->pev->iuser3 == 1)
	{
		pSkinEnt->pev->iuser3 = 0;
		pSkinEnt->pev->iuser1 = -1;
		pSkinEnt->pev->rendermode = 1;
		pSkinEnt->pev->renderamt = 0;
		pEntity->v.rendermode = 0;
		pEntity->v.renderamt = 255;
	}

	//Do the weapon skin if applicable
	if (pWeaponSkinEnt && pWeaponSkinEnt->pev->iuser3 == 1 )
	{
		pWeaponSkinEnt->pev->iuser3 = 0;
		pWeaponSkinEnt->pev->iuser1 = -1;
		pWeaponSkinEnt->pev->rendermode = 1;
		pWeaponSkinEnt->pev->renderamt = 0;
	}

	CLIENT_PRINTF(pEntity, print_console, SK_REMOVED);
	CLIENT_PRINTF(pEntity, print_center, SK_REMOVED);
}

//Makes sure all the skins are kept alive in the server.
void InitSkins( )
{
	if(gpGlobals->maxClients > MAX_SKINS)
	{
		//Uh oh, not enough skins to go around!
		//WriteToDebug(1,"Initskins(): FAILED! Maxplayers is greater than MAX_SKINS!!! Skins disabled.");

		Features.Skins = 0; //Disable Skins
		return;
	}

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		CBaseEntity *pSkin = NULL;
		pSkin = UTIL_FindEntityByTargetname( NULL, g_SkinNames[i]);
		
		if (pSkin == NULL)
		{
			edict_t	*pSkin;
			pSkin = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
			DispatchSpawn(pSkin);
			pSkin->v.origin = Vector(-4000,-4000,-4000);
			pSkin->v.velocity = Vector(0, 0, 0);
			pSkin->v.takedamage = DAMAGE_NO;
			pSkin->v.health = 1;
			pSkin->v.movetype = MOVETYPE_FOLLOW;
			pSkin->v.targetname = MAKE_STRING(g_SkinNames[i]);
			pSkin->v.aiment = INDEXENT(i);
			pSkin->v.iuser2 = i;
			pSkin->v.body = 0;
			pSkin->v.effects = 0;
			pSkin->v.iuser3 = 0;
			pSkin->v.solid = SOLID_NOT;
			SET_MODEL(pSkin, SKIN_MDL);
			pSkin->v.classname = MAKE_STRING("tfx_skin");
			pSkin->v.nextthink = gpGlobals->time + 0.1;
			DispatchSpawn( pSkin );
		}
	}

	if(Features.WeaponsOnSkins == 1)
	{
		for (int i = 1; i <= gpGlobals->maxClients; i++)
		{
			CBaseEntity *pSkin = NULL;
			pSkin = UTIL_FindEntityByTargetname( NULL, g_WeaponSkinNames[i]);
			
			if (pSkin == NULL)
			{
				edict_t	*pSkin;
				pSkin = CREATE_NAMED_ENTITY(MAKE_STRING("info_target"));
				DispatchSpawn(pSkin);
				pSkin->v.origin = Vector(-4000,-4000,-4000);
				pSkin->v.velocity = Vector(0, 0, 0);
				pSkin->v.takedamage = DAMAGE_NO;
				pSkin->v.health = 1;
				pSkin->v.movetype = MOVETYPE_FOLLOW;
				pSkin->v.targetname = MAKE_STRING(g_WeaponSkinNames[i]);
				pSkin->v.aiment = INDEXENT(i);
				pSkin->v.iuser2 = i;
				pSkin->v.iuser4 = 1; //Indicates this is a weapon skin
				pSkin->v.body = 0;
				pSkin->v.effects = 0;
				pSkin->v.iuser3 = 0;
				pSkin->v.solid = SOLID_NOT;
				SET_MODEL(pSkin, SKIN_MDL);
				pSkin->v.classname = MAKE_STRING("tfx_skin");
				pSkin->v.nextthink = gpGlobals->time + 0.1;
				DispatchSpawn( pSkin );
			}
		}
	}
	return;
}

bool SkinThink(edict_t *pEntity)
{
	if(pEntity->v.aiment != NULL)
	{
		if(UTIL_SpectCheck(pEntity->v.aiment) == 1)
		{
			//If player is spec, hide skin always.
			pEntity->v.iuser3 = 0;
			pEntity->v.nextthink = gpGlobals->time + 0.1;
			return 0;
		}

		if(pEntity->v.aiment->v.playerclass == CLASS_SNIPER)
		{
			if((pEntity->v.iuser1 == 5) || (pEntity->v.iuser1 == 25))
			{
				KillSkin(pEntity->v.aiment);
				return 0;
			}
		}

		if(pEntity->v.iuser3 == 1)
		{
			//If skin is visible, keep player invisible.
			if(pEntity->v.origin == pEntity->v.aiment->v.origin)
			{
				pEntity->v.aiment->v.rendermode = 1;
				pEntity->v.aiment->v.renderamt = 0;
			}
	
			if (Features.SkinsShowTeam == 1)
			{
				pEntity->v.rendermode = kRenderNormal;
				pEntity->v.renderfx = kRenderFxGlowShell;
				pEntity->v.renderamt = 5;
				
				//Now color the glow depending upon team
				if (pEntity->v.aiment->v.team == 1)
				{
					pEntity->v.rendercolor = Vector(0,0,100);
				}
				else if (pEntity->v.aiment->v.team == 2)
				{
					pEntity->v.rendercolor = Vector(100,0,0);
				}
				else if (pEntity->v.aiment->v.team == 3)
				{
					pEntity->v.rendercolor = Vector(100,100,0);
				}
				else if (pEntity->v.aiment->v.team == 4)
				{
					pEntity->v.rendercolor = Vector(0,100,0);
				}

				//If the player is a spy color off the topcolor, not the team
				if(pEntity->v.aiment->v.playerclass == CLASS_SPY)
				{
				   char *infobuffer;
				   char color[32];
				   int color_player;

				   infobuffer = (*g_engfuncs.pfnGetInfoKeyBuffer)( pEntity->v.aiment );
				   strcpy(color, (g_engfuncs.pfnInfoKeyValue(infobuffer, "topcolor")));
				   sscanf(color, "%d", &color_player);

				   if ((color_player==140) || (color_player==148) || (color_player==150) || (color_player==153))
					  pEntity->v.rendercolor = Vector(0,0,100);

				   if ((color_player == 5) || (color_player == 250) || (color_player==255))
					  pEntity->v.rendercolor = Vector(100,0,0);

				   if ((color_player == 45) && (color_player == 45))
					  pEntity->v.rendercolor = Vector(100,100,0);

				   if ((color_player == 80) || (color_player == 100))
					  pEntity->v.rendercolor = Vector(0,100,0);
				}
			} else {
				pEntity->v.euser1->v.rendermode = 1;
				pEntity->v.euser1->v.renderamt = 0;
				pEntity->v.renderfx = 0;
				pEntity->v.rendercolor = Vector(0,0,0);
			}

			if(PlayerInfo[ENTINDEX(pEntity->v.aiment)].StealthOn == 1)
			{
				pEntity->v.renderfx = 0;
				pEntity->v.rendercolor = Vector(0,0,0);

				//The player has stealth on, so the skin must
				//go stealth like the player model would do

				if(pEntity->v.aiment->v.velocity == Vector(0,0,0))
				{
					pEntity->v.rendermode = 1;
					pEntity->v.renderamt = 0;
				} else {
					pEntity->v.rendermode = 1;
					pEntity->v.renderamt = 20;
				}
			}


			//if skin is a weaponskin, make sure it stays the weapon model
			if(pEntity->v.iuser4 == 1)
			{
				SET_MODEL(pEntity, STRING(pEntity->v.aiment->v.weaponmodel));
			}
		} else {
			pEntity->v.rendermode = 1;
			pEntity->v.renderamt = 0;
		}
	}
	pEntity->v.nextthink = gpGlobals->time + 0.1;
	return 0;
}

void SayCurrentSkin(edict_t *pEntity)
{
	char SayText[MAX_TEXT_LENGTH];
	int FoundSkinIndex = -1;

	//Find this players skin entity
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Look for tfx_skin
		if (strcmp(STRING(pScan->pev->classname), "tfx_skin") == 0)
		{
			if(pScan->pev->aiment == pEntity && pScan->pev->iuser3 == 1)
			{
				FoundSkinIndex = pScan->pev->iuser1;
				break;
			}
		}
	}

	if(FoundSkinIndex != -1)
	{
		_snprintf(SayText,MAX_TEXT_LENGTH,SK_CURRENT,FoundSkinIndex,SkinLibrary[FoundSkinIndex].SkinName);
	} else {
		_snprintf(SayText,MAX_TEXT_LENGTH,SK_NOCURRENT);
	}

	CLIENT_PRINTF(pEntity, print_console, SayText);

	return;
}

int GetPlayerSkinIndex(edict_t *pEntity)
{
	int FoundSkinIndex = -1;

	//Find this players skin entity
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 100)) != NULL)
	{
		//Look for tfx_skin
		if (strcmp(STRING(pScan->pev->classname), "tfx_skin") == 0)
		{
			if(pScan->pev->aiment == pEntity && pScan->pev->iuser3 == 1)
			{
				FoundSkinIndex = pScan->pev->iuser1;
				return FoundSkinIndex;
				break;
			}
		}
	}
	return FoundSkinIndex;
}