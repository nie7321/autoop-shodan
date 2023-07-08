/*****************************************************\
*	Auto-OP Shodan
*
*	File: util.cpp
*	Description: Utility Code.
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

/***
*
*   Copyright (c) 1999, Valve LLC. All rights reserved.
*   
*   This product contains software technology licensed from Id 
*   Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*   All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
/*

===== util.cpp ========================================================

  Utility code.  Really not optional after all.

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "shake.h"
#include "decals.h"
#include "stripper.h"
#include "autoop.h"
#include "commands.h"
#include <winsock.h>

extern int mod_id;
extern DLL_FUNCTIONS other_gFunctionTable;

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;

extern mapcycle_t mapcycle;

extern team_limits[];
extern team_available[];

extern int message_StatusText;
extern int message_StatusValue;

// returns a CBaseEntity pointer to a player by index.  Only returns if the player is spawned and connected
// otherwise returns NULL
// Index is 1 based
CBaseEntity   *UTIL_PlayerByIndex( int playerIndex )
{
   CBaseEntity *pPlayer = NULL;

   if ( playerIndex > 0 && playerIndex <= gpGlobals->maxClients )
   {
      edict_t *pPlayerEdict = INDEXENT( playerIndex );
      if ( pPlayerEdict && !pPlayerEdict->free )
      {
         pPlayer = CBaseEntity::Instance( pPlayerEdict );
      }
   }
   
   return pPlayer;
}


int gmsgTextMsg = 0;

void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
   if (gmsgTextMsg == 0)
      //gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );
	  gmsgTextMsg = 80;

   MESSAGE_BEGIN( MSG_ONE, gmsgTextMsg, NULL, client );
      WRITE_BYTE( msg_dest );
      WRITE_STRING( msg_name );

      if ( param1 )
         WRITE_STRING( param1 );
      if ( param2 )
         WRITE_STRING( param2 );
      if ( param3 )
         WRITE_STRING( param3 );
      if ( param4 )
         WRITE_STRING( param4 );

   MESSAGE_END();
}

int make_friendly(char *name) {
	int i;

	for(i=0;i<(int)strlen(name);i++) { //(int) stops nasty unsigned warning.
		if (name[i]=='%') {
		//	printf("FOUND BAD BIT!\n");
			strcpy(name+i,name+i+1);
		}
	}
return 1;
}

//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf( char *fmt, ... )
{
        va_list                 argptr;
        static char             string[1024];
        
        va_start ( argptr, fmt );
        vsprintf ( string, fmt, argptr );
        va_end   ( argptr );

        // Print to server console
        ALERT( at_logged, "%s", string );
}

//Easy entity finding
CBaseEntity *UTIL_FindEntityInSphere( CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius )
{
   edict_t   *pentEntity;

   if (pStartEntity)
      pentEntity = pStartEntity->edict();
   else
      pentEntity = NULL;

   pentEntity = FIND_ENTITY_IN_SPHERE( pentEntity, vecCenter, flRadius);

   if (!FNullEnt(pentEntity))
      return CBaseEntity::Instance(pentEntity);
   return NULL;
}

CBaseEntity *UTIL_FindEntityByString( CBaseEntity *pStartEntity, const
char *szKeyword, const char *szValue )
{
        edict_t *pentEntity;

        if (pStartEntity)
pentEntity = pStartEntity->edict();
        else
           pentEntity = NULL;

        pentEntity = FIND_ENTITY_BY_STRING( pentEntity, szKeyword, szValue
);

if (!FNullEnt(pentEntity))
                return CBaseEntity::Instance(pentEntity);
        return NULL;
}                        

CBaseEntity *UTIL_FindEntityByClassname( CBaseEntity *pStartEntity, const
char *szName )
{
        return UTIL_FindEntityByString( pStartEntity, "classname", szName
);    }      

char* UTIL_VarArgs( char *format, ... )
{
        va_list         argptr;
        static char             string[1024];
        
        va_start (argptr, format);
        vsprintf (string, format,argptr);
        va_end (argptr);

        return string;  
}

//USed for moving entities
void UTIL_SetOrigin( entvars_t *pev, const Vector &vecOrigin )
{
	SET_ORIGIN(ENT(pev), vecOrigin );
}

//	float UTIL_MoveToOrigin( edict_t *pent, const Vector vecGoal, float flDist, int iMoveType )
void UTIL_MoveToOrigin( edict_t *pent, const Vector &vecGoal, float flDist, int iMoveType )
{
	float rgfl[3];
	vecGoal.CopyToArray(rgfl);
//		return MOVE_TO_ORIGIN ( pent, rgfl, flDist, iMoveType ); 
	MOVE_TO_ORIGIN ( pent, rgfl, flDist, iMoveType ); 
}

//Sends chat text to all players
void UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
   if (gmsgTextMsg == 0)
      //gmsgTextMsg = REG_USER_MSG( "TextMsg", -1 );
	  gmsgTextMsg = 80;

   MESSAGE_BEGIN( MSG_ALL, gmsgTextMsg );
      WRITE_BYTE( msg_dest );
      WRITE_STRING( msg_name );

      if ( param1 )
         WRITE_STRING( param1 );
      if ( param2 )
         WRITE_STRING( param2 );
      if ( param3 )
         WRITE_STRING( param3 );
      if ( param4 )
         WRITE_STRING( param4 );

   MESSAGE_END();
}

static unsigned short FixedUnsigned16( float value, float scale )
{
	int output;

	output = value * scale;
	if ( output < 0 )
		output = 0;
	if ( output > 0xFFFF )
		output = 0xFFFF;

	return (unsigned short)output;
}

static short FixedSigned16( float value, float scale )
{
	int output;

	output = value * scale;

	if ( output > 32767 )
		output = 32767;

	if ( output < -32768 )
		output = -32768;

	return (short)output;
}

//Sends a big coloured text message
void UTIL_HudMessage( CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage )
{
	if ( !pEntity  )
		return;

	MESSAGE_BEGIN( MSG_ONE, SVC_TEMPENTITY, NULL, pEntity->edict() );
		WRITE_BYTE( TE_TEXTMESSAGE );
		WRITE_BYTE( textparms.channel & 0xFF );

		WRITE_SHORT( FixedSigned16( textparms.x, 1<<13 ) );
		WRITE_SHORT( FixedSigned16( textparms.y, 1<<13 ) );
		WRITE_BYTE( textparms.effect );

		WRITE_BYTE( textparms.r1 );
		WRITE_BYTE( textparms.g1 );
		WRITE_BYTE( textparms.b1 );
		WRITE_BYTE( textparms.a1 );

		WRITE_BYTE( textparms.r2 );
		WRITE_BYTE( textparms.g2 );
		WRITE_BYTE( textparms.b2 );
		WRITE_BYTE( textparms.a2 );

		WRITE_SHORT( FixedUnsigned16( textparms.fadeinTime, 1<<8 ) );
		WRITE_SHORT( FixedUnsigned16( textparms.fadeoutTime, 1<<8 ) );
		WRITE_SHORT( FixedUnsigned16( textparms.holdTime, 1<<8 ) );

		if ( textparms.effect == 2 )
			WRITE_SHORT( FixedUnsigned16( textparms.fxTime, 1<<8 ) );
		
		if ( strlen( pMessage ) < 512 )
		{
			WRITE_STRING( pMessage );
		}
		else
		{
			char tmp[512];
			strncpy( tmp, pMessage, 511 );
			tmp[511] = 0;
			WRITE_STRING( tmp );
		}
	MESSAGE_END();
}

//Sends a big coloured text message to all players
void UTIL_HudMessageAll( const hudtextparms_t &textparms, const char *pMessage )
{
	int			i;

	for ( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
		if ( pPlayer )
			UTIL_HudMessage( pPlayer, textparms, pMessage );
	}
}

/*SCREEN FADE STUFF START*/
static unsigned short FixedUnsigned216( float value, float scale )
{
	int output;

	output = value * scale;
	if ( output < 0 )
		output = 0;
	if ( output > 0xFFFF )
		output = 0xFFFF;

	return (unsigned short)output;
}

static short FixedSigned216( float value, float scale )
{
	int output;

	output = value * scale;

	if ( output > 32767 )
		output = 32767;

	if ( output < -32768 )
		output = -32768;

	return (short)output;
}
void UTIL_ScreenFadeBuild( ScreenFade &fade, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
	fade.duration = FixedUnsigned216( fadeTime, 1<<12 );		// 4.12 fixed
	fade.holdTime = FixedUnsigned216( fadeHold, 1<<12 );		// 4.12 fixed
	fade.r = (int)color.x;
	fade.g = (int)color.y;
	fade.b = (int)color.z;
	fade.a = alpha;
	fade.fadeFlags = flags;
}

void UTIL_SetSize( entvars_t *pev, const Vector &vecMin, const Vector &vecMax )
{
	SET_SIZE( ENT(pev), vecMin, vecMax );
}

Vector UTIL_VecToAngles( const Vector &vec )
{
	float rgflVecOut[3];
	VEC_TO_ANGLES(vec, rgflVecOut);
	return Vector(rgflVecOut);
}

void UTIL_ScreenFadeWrite( const ScreenFade &fade, CBaseEntity *pEntity )
{
	if ( !pEntity || !pEntity->IsNetClient() )
		return;

	//Fade stuff
	int gmsgFade = 0;
	gmsgFade = REG_USER_MSG("ScreenFade", sizeof(ScreenFade));

	MESSAGE_BEGIN( MSG_ONE, gmsgFade, NULL, pEntity->edict() );		// use the magic #1 for "one client"
		
		WRITE_SHORT( fade.duration );		// fade lasts this long
		WRITE_SHORT( fade.holdTime );		// fade lasts this long
		WRITE_SHORT( fade.fadeFlags );		// fade type (in / out)
		WRITE_BYTE( fade.r );				// fade red
		WRITE_BYTE( fade.g );				// fade green
		WRITE_BYTE( fade.b );				// fade blue
		WRITE_BYTE( fade.a );				// fade blue

	MESSAGE_END();
}

void UTIL_ScreenFadeAll( const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
	int			i;
	ScreenFade	fade;


	UTIL_ScreenFadeBuild( fade, color, fadeTime, fadeHold, alpha, flags );

	for ( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );
	
		UTIL_ScreenFadeWrite( fade, pPlayer );
	}
}


void UTIL_ScreenFade( CBaseEntity *pEntity, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags )
{
	ScreenFade	fade;

	UTIL_ScreenFadeBuild( fade, color, fadeTime, fadeHold, alpha, flags );
	UTIL_ScreenFadeWrite( fade, pEntity );
}

void UTIL_ScreenShake( const Vector &center, float amplitude, float frequency, float duration, float radius )
{
	int			i;
	float		localAmplitude;
	ScreenShake	shake;

	int gmsgShake = 0;
	gmsgShake = REG_USER_MSG("ScreenShake", sizeof(ScreenShake));

	shake.duration = FixedUnsigned216( duration, 1<<12 );		// 4.12 fixed
	shake.frequency = FixedUnsigned216( frequency, 1<<8 );	// 8.8 fixed

	for ( i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );

		if ( !pPlayer || !(pPlayer->pev->flags & FL_ONGROUND) )	// Don't shake if not onground
			continue;

		localAmplitude = 0;

		if ( radius <= 0 )
			localAmplitude = amplitude;
		else
		{
			Vector delta = center - pPlayer->pev->origin;
			float distance = delta.Length();
	
			// Had to get rid of this falloff - it didn't work well
			if ( distance < radius )
				localAmplitude = amplitude;//radius - distance;
		}
		if ( localAmplitude )
		{
			shake.amplitude = FixedUnsigned216( localAmplitude, 1<<12 );		// 4.12 fixed
			
			MESSAGE_BEGIN( MSG_ONE, gmsgShake, NULL, pPlayer->edict() );		// use the magic #1 for "one client"
				
				WRITE_SHORT( shake.amplitude );				// shake amount
				WRITE_SHORT( shake.duration );				// shake lasts this long
				WRITE_SHORT( shake.frequency );				// shake noise frequency

			MESSAGE_END();
		}
	}
}

void UTIL_ScreenShakeAll( const Vector &center, float amplitude, float frequency, float duration )
{
	UTIL_ScreenShake( center, amplitude, frequency, duration, 0 );
}
/*SCREEN FADE STUFF END*/

void UTIL_MakeVectors( const Vector &vecAngles )
{
	MAKE_VECTORS( vecAngles );
}

char* stristr( const char* haystack, const char* needle ) {
  int iLength = strlen( haystack );
  int iNeedleLen = strlen( needle );
  char* pcBegin = 0;

  int iH = 0;
  int iN = 0;

  if ( iNeedleLen > iLength ) return 0;

  for ( int iStart = 0; iStart < iLength-iNeedleLen+1; iStart++ ) {
    pcBegin = (char*)(haystack + iStart);
    
    iH = iStart;
    iN = 0;
    while ( (iH < iLength) && (iN < iNeedleLen) && (tolower(haystack[iH]) == tolower(needle[iN])) ) {
      iH++;
      iN++;
    }  // while
    // match
    if ( iN == iNeedleLen ) return pcBegin;
    // no match
    if ( iH == iLength ) return 0;
  }  // for

  return 0;
}  // stristr()

int UTIL_GetPlayerIndex( char *PlayerText )
{
	int PlayerNumber = 0;
	int i;
	int found = 0;
	int index = 0;

	if (atoi(PlayerText)!=0)
		PlayerNumber=atoi(PlayerText);

	for (i = 1; i <= gpGlobals->maxClients; i++) {
		CBaseEntity *pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer) {
			//if (PlayerNumber!=0 && (GETPLAYERWONID(pPlayer->edict())==PlayerNumber)) return i;
			if (PlayerNumber!=0 && (GETPLAYERUSERID(pPlayer->edict())==PlayerNumber)) return i;
			if (!stricmp(STRING(pPlayer->pev->netname),PlayerText)) return i;
			
			if ( stristr(STRING(pPlayer->pev->netname), PlayerText) != 0) {
				index = i;
				found++;
			}
		}
	}

	if ( found == 1 ) return index;

	return 0;
}

/*
	UTIL_GiveAllWeapons
	-------------------
	Nice way of giving all guns to a player in TFC.
	This takes into account players class, so they dont get
	guns they already have, as this leaves "leftovers" in
	the game which are ugly.
*/
void UTIL_GiveAllWeapons( CBaseEntity *pEntity )
{
	int i = 0;
	int GunOK = 1;
	for(i = 1; i < 18; i++)
	{
		GunOK = 1;

		if(i == 1 && pEntity->pev->playerclass == 3) //Soldier and RPB
			GunOK = 0;

		if(i == 11 && pEntity->pev->playerclass == 9) //Engy and railgun
			GunOK = 0;
		
		if(i == 7 && pEntity->pev->playerclass == 9) //Engy and spanner
			GunOK = 0;

		if(i == 14 && pEntity->pev->playerclass == 7) //Pyro and flamethrower
			GunOK = 0;

		if(GunOK != 0)
			TFXGiveWeapon(i,pEntity->edict());
	}
}

CBaseEntity *UTIL_FindEntityByTargetname( CBaseEntity *pStartEntity, const char *szName )
{
	return UTIL_FindEntityByString( pStartEntity, "targetname", szName );
}

int UTIL_SpectCheck( edict_t *pPlayer )
{
	//Returns 1 if spectator

	if ((pPlayer->v.team < 1) || (pPlayer->v.team > 4))
	{
		return 1;
	} else {
		if(pPlayer->v.playerclass < 1)
			return 1;
		return 0;
	}
}

int UTIL_PointContents(	const Vector &vec )
{
	return POINT_CONTENTS(vec);
}

void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass?0x100:0), pentIgnore, ptr );
}


void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_LINE( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), pentIgnore, ptr );
}


void UTIL_TraceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr )
{
	TRACE_HULL( vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE), hullNumber, pentIgnore, ptr );
}

void UTIL_TraceModel( const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr )
{
	g_engfuncs.pfnTraceModel( vecStart, vecEnd, hullNumber, pentModel, ptr );
}

bool UTIL_CheckBox(Vector Origin, Vector Dimensions)
{
	// This function checks up, down, left, right, front and back in Dimensions/2 units in each direction from the origin. It's for establishing
    // whether it's OK for items to be placed in a certain position, but it isn't foolproof -- doesn't check diagolnals or slope of land etc.

    TraceResult tr;
    UTIL_TraceLine(Origin, Origin + Vector(Dimensions.x/2, 0, 0), dont_ignore_monsters, NULL, &tr);
    if(tr.flFraction!=1.0) return 0;
    UTIL_TraceLine(Origin, Origin - Vector(Dimensions.x/2, 0, 0), dont_ignore_monsters, NULL, &tr);
    if(tr.flFraction!=1.0) return 0;
    UTIL_TraceLine(Origin, Origin + Vector(0, Dimensions.y/2, 0), dont_ignore_monsters, NULL, &tr);
    if(tr.flFraction!=1.0) return 0;
    UTIL_TraceLine(Origin, Origin - Vector(0, Dimensions.y/2, 0), dont_ignore_monsters, NULL, &tr);
    if(tr.flFraction!=1.0) return 0;
    UTIL_TraceLine(Origin, Origin + Vector(0, 0, Dimensions.z/2), dont_ignore_monsters, NULL, &tr);
    if(tr.flFraction!=1.0) return 0;
    UTIL_TraceLine(Origin, Origin - Vector(0, 0, Dimensions.z/2), dont_ignore_monsters, NULL, &tr);
    if(tr.flFraction!=1.0) return 0;

    return 1;
}

/*int UTIL_GetRank( int credits ) //Convert Credit amount into a rank.
{
	int Rank = 0;

	if (credits<0)
	{
		Rank = MAKE_STRING("Llama");
	}
	if(credits>=0)
	{
		Rank = MAKE_STRING("N00bie");
	}
	if(credits>40)
	{
		Rank = MAKE_STRING("Greenhorn");
	}
	if(credits>70)
	{
		Rank = MAKE_STRING("Warrior");
	}
	if(credits>100)
	{
		Rank = MAKE_STRING("Beast");
	}
	if(credits>150)
	{
		Rank = MAKE_STRING("Fragmeister");
	}
	if(credits>350)
	{
		Rank = MAKE_STRING("Elite Fragger");
	}
	if(credits>450)
	{
		Rank = MAKE_STRING("Lord Of Death");
	}
	if(credits>600)
	{
		Rank = MAKE_STRING("Excellence");
	}
	if(credits>800)
	{
		Rank = MAKE_STRING("Prez Bodyguard");
	}
	if(credits>2000)
	{
		Rank = MAKE_STRING("Smooth Operator");
	}
	if(credits>2500)
	{
		Rank = MAKE_STRING("Scout Raper");
	}
	if(credits>3000)
	{
		Rank = MAKE_STRING("Cleaner");
	}
	if(credits>4000)
	{
		Rank = MAKE_STRING("Llama Eliminator");
	}
	if(credits>5000)
	{
		Rank = MAKE_STRING("Shadow");
	}
	if(credits>6000)
	{
		Rank = MAKE_STRING("Balls Of Steel");
	}
	if (credits>9000)
	{
		Rank = MAKE_STRING("Extreme Power");
	}
	if (credits>15000)
	{
		Rank = MAKE_STRING("Credit Junkie");
	}
	if (credits>=1000000)
	{
		Rank = MAKE_STRING("Immortal");
	}
	return Rank;
}*/

//
// BModelOrigin - calculates origin of a bmodel from absmin/size because all bmodel origins are 0 0 0
//
Vector VecBModelOrigin( entvars_t* pevBModel )
{
	return pevBModel->absmin + ( pevBModel->size * 0.5 );
}

int GetPartialPlayerIndex(char *PlayerText, int SelfIndex) {
	int PlayerNumber = 0;
	int i;
	int found = 0;
	int index = 0;
	edict_t *pAdmin = NULL;

	if(SelfIndex > -1)
		pAdmin = INDEXENT(SelfIndex);

	if ( strlen( PlayerText ) == 0 ) return 0;

	if (!stricmp("!self", PlayerText) && (SelfIndex != -1)) return SelfIndex;

	if(pAdmin != NULL)
	{
		if (!stricmp("!point", PlayerText))
		{
			Vector v_src, v_dest;
			TraceResult tr;

			UTIL_MakeVectors( pAdmin->v.v_angle );

			v_src = pAdmin->v.origin + pAdmin->v.view_ofs;
			v_dest = v_src + gpGlobals->v_forward * 9000;

			UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pAdmin->v.pContainingEntity, &tr);

			if (tr.flFraction < 1.0)
			{
				//Ignoring anything except worldspawn, func_wall and func_door.
				if (strcmp("player", STRING(tr.pHit->v.classname)) == 0)
					return ENTINDEX(tr.pHit);
			}
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You are not pointing at a player.\n"));
			return 0;
		}
	}
   
	if (atoi(PlayerText)!=0) {
		PlayerNumber=atoi(PlayerText);
	}
   
	for (i = 1; i <= gpGlobals->maxClients; i++) {
		CBaseEntity *pPlayer = UTIL_PlayerByIndex(i);
		if (pPlayer) {
			
			if (!stricmp(GETPLAYERAUTHID(pPlayer->edict()), PlayerText))
				if(PlayerInfo[i].NoGreen == 0) return i;

			if (PlayerNumber!=0 && (GETPLAYERUSERID(pPlayer->edict())==PlayerNumber)) return i;
			if (!stricmp(STRING(pPlayer->pev->netname), PlayerText)) return i;
			if ( stristr(STRING(pPlayer->pev->netname), PlayerText) != 0) {
				index = i;
				found++;
			}
		}
	}

	if ( found == 1 ) return index;

	if(pAdmin != NULL)
	{
		if ( found > 1 ) {
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Player search \"%s\" returned too many players. Be more specific.\n",PlayerText));
		} else {
			CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "No player match found for \"%s\".\n",PlayerText));
		}
	}
	return 0;
}

void fix_string(char *str,int len)
{

for(int i=0;i<len;i++) {
        if( str[i]=='>' || str[i]=='<' || str[i]=='%' )
strcpy(str+i,str+i+1);
}
 

}

int IsMorePowerful(int Index1, int Index2)
{
	//In the admin functions
	//Index1 = the player
	//Index2 = the admin

	if(Index1 == Index2)
		return 0; //Player calling it on himself.

	//returns 1 if Player 1 is more powerful or equal
	//to player2, else 0.
	int Power1 = PlayerInfo[Index1].IsAdmin;
	int Power2 = PlayerInfo[Index2].IsAdmin;

	if(Power2 >= LEVEL_SUPEROP)
		return 0; //Superops bypass this check.

	if((Power1 >= Power2) || (Power1 == Power2))
	{
		return 1;
	} else {
		return 0; //Admin is more powerful
	}
}

//Weapon giving code for TFC
void TFXGiveWeapon( int code, edict_t *pEntity )
{
	if (code==0)
		return;

	//Select the weapon
	int istr = 0;
	
	switch (code) {
	case 1:
		istr = MAKE_STRING("tf_weapon_rpg");
		break;
	case 2:
		istr = MAKE_STRING("tf_weapon_ac");
		break;
	case 3:
		istr = MAKE_STRING("tf_weapon_medikit");
		break;
	case 4:
		istr = MAKE_STRING("tf_weapon_shotgun");
		break;
	case 5:
		istr = MAKE_STRING("tf_weapon_knife");
		break;
	case 6:
		istr = MAKE_STRING("tf_weapon_supershotgun");
		break;
	case 7:
		istr = MAKE_STRING("tf_weapon_spanner");
		break;
	case 8:
		istr = MAKE_STRING("tf_weapon_ng");
		break;
	case 9:
		istr = MAKE_STRING("tf_weapon_superng");
		break;
	case 10:
		istr = MAKE_STRING("tf_weapon_tranq");
		break;
	case 11:
		istr = MAKE_STRING("tf_weapon_railgun");
		break;
	case 12:
		istr = MAKE_STRING("tf_weapon_sniperrifle");
		break;
	case 13:
		istr = MAKE_STRING("tf_weapon_autorifle");
		break;
	case 14:
		istr = MAKE_STRING("tf_weapon_flamethrower");
		break;
	case 15:
		istr = MAKE_STRING("tf_weapon_ic");
		break;
	case 16:
		istr = MAKE_STRING("tf_weapon_gl");
		break;
	case 17:
		istr = MAKE_STRING("tf_weapon_pl");
		break;
	case 18:
		istr = MAKE_STRING("tf_weapon_axe");
		break;
	default:
		istr = 0;
	}

	if(istr == 0)
		return;

	edict_t	*pent;
	pent = CREATE_NAMED_ENTITY(istr);
	VARS( pent )->origin = pEntity->v.origin;
	pent->v.spawnflags |= SF_NORESPAWN;
	DispatchSpawn( pent );
	DispatchTouch( pent, pEntity );
}

#define ARMOR_RATIO	 0.2	// Armor Takes 80% of the damage
#define ARMOR_BONUS  0.5	// Each Point of Armor is work 1/x points of health

void TakeDamage(edict_t *pEntity, edict_t* pAttacker, float flDamage, int DamageType)
{
	if((pEntity->v.takedamage == DAMAGE_NO) || (pEntity->v.health < flDamage))
		return;

	float flRatio;
	float flBonus;

	flBonus = ARMOR_BONUS;
	flRatio = ARMOR_RATIO;

	int bitsDamage = DamageType;
	
	if ( bitsDamage & DMG_BLAST )
	{
		// blasts damage armor more.
		flBonus *= 2;
	}

	int Huddmg = DamageType |= bitsDamage;

	// Armor. 
	if (pEntity->v.armorvalue && !(bitsDamage & (DMG_FALL | DMG_DROWN)) )// armor doesn't protect against fall or drown damage!
	{
		float flNew = flDamage * flRatio;

		float flArmor;

		flArmor = (flDamage - flNew) * flBonus;

		// Does this use more armor than we have?
		if (flArmor > pEntity->v.armorvalue)
		{
			flArmor = pEntity->v.armorvalue;
			flArmor *= (1/flBonus);
			flNew = flDamage - flArmor;
			pEntity->v.armorvalue = 0;
		}
		else
			pEntity->v.armorvalue -= flArmor;
		
		flDamage = flNew;
	}

	pEntity->v.health -= flDamage;

	if(pEntity->v.health <= 0)
		CLIENT_COMMAND(pEntity, "kill\n");

	pEntity->v.dmg_take += flDamage;

	pEntity->v.dmg_inflictor = pAttacker;

	//if(pEntity->v.health < 20)
	//	FakeGib(pEntity);

	int gmsgDamage = REG_USER_MSG( "Damage", 12 );

	if(gmsgDamage > MAX_USERMSG)
		return;

	if(DamageType > 0)
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgDamage, NULL, &pEntity->v );
			WRITE_BYTE( pEntity->v.dmg_save );
			WRITE_BYTE( pEntity->v.dmg_take );
			WRITE_LONG( Huddmg );
			WRITE_COORD( pEntity->v.origin.x );
			WRITE_COORD( pEntity->v.origin.y );
			WRITE_COORD( pEntity->v.origin.z );
		MESSAGE_END();
	}
}

/*********************************************************************
new map stuff

**********************************************************************/
/*
==============
DestroyMapCycle

Clean up memory used by mapcycle when switching it
==============
*/
void DestroyMapCycle( mapcycle_t *cycle )
{
	mapcycle_item_t *p, *n, *start;
	p = cycle->items;
	if ( p )
	{
		start = p;
		p = p->next;
		while ( p != start )
		{
			n = p->next;
			delete p;
			p = n;
		}
		
		delete cycle->items;
	}
	cycle->items = NULL;
	cycle->next_item = NULL;
}


static char com_token[ 1500 ];
/*
==============
COM_Parse

Parse a token out of a string
==============
*/
char *COM_Parse (char *data)
{
	int             c;
	int             len;
	
	len = 0;
	com_token[0] = 0;
	
	if (!data)
		return NULL;
		
// skip whitespace
skipwhite:
	while ( (c = *data) <= ' ')
	{
		if (c == 0)
			return NULL;                    // end of file;
		data++;
	}
	
// skip // comments
	if (c=='/' && data[1] == '/')
	{
		while (*data && *data != '\n')
			data++;
		goto skipwhite;
	}
	

// handle quoted strings specially
	if (c == '\"')
	{
		data++;
		while (1)
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				com_token[len] = 0;
				return data;
			}
			com_token[len] = c;
			len++;
		}
	}

// parse single characters
	if (c=='{' || c=='}'|| c==')'|| c=='(' || c=='\'' || c == ',' )
	{
		com_token[len] = c;
		len++;
		com_token[len] = 0;
		return data+1;
	}

// parse a regular word
	do
	{
		com_token[len] = c;
		data++;
		len++;
		c = *data;
	if (c=='{' || c=='}'|| c==')'|| c=='(' || c=='\'' || c == ',' )
			break;
	} while (c>32);
	
	com_token[len] = 0;
	return data;
}

/*
==============
COM_TokenWaiting

Returns 1 if additional data is waiting to be processed on this line
==============
*/
int COM_TokenWaiting( char *buffer )
{
	char *p;

	p = buffer;
	while ( *p && *p!='\n')
	{
		if ( !isspace( *p ) || isalnum( *p ) )
			return 1;

		p++;
	}

	return 0;
}

/*
==============
ReloadMapCycleFile


Parses mapcycle.txt file into mapcycle_t structure
==============
*/
int ReloadMapCycleFile( char *filename, mapcycle_t *cycle )
{
	char szBuffer[ MAX_RULE_BUFFER ];
	char szMap[ 32 ];
	int length;
	char *pFileList;
	char *aFileList = pFileList = (char*)LOAD_FILE_FOR_ME( filename, &length );
	int hasbuffer;
	mapcycle_item_s *item, *newlist = NULL, *next;

	if (aFileList == NULL || pFileList == NULL ) return 0;

	if ( pFileList && length )
	{
		// the first map name in the file becomes the default
		while ( 1 )
		{
			hasbuffer = 0;
			memset( szBuffer, 0, MAX_RULE_BUFFER );

			pFileList = COM_Parse( pFileList );
			if ( strlen( com_token ) <= 0 )
				break;

			strcpy( szMap, com_token );

			// Any more tokens on this line?
			if ( COM_TokenWaiting( pFileList ) )
			{
				pFileList = COM_Parse( pFileList );
				if ( strlen( com_token ) > 0 )
				{
					hasbuffer = 1;
					strcpy( szBuffer, com_token );
				}
			}

			// Check map
			if ( IS_MAP_VALID( szMap ) )
			{

				item = new mapcycle_item_s;

				strcpy( item->mapname, szMap );

				item->next = cycle->items;
				cycle->items = item;
			}
			else
			{
				ALERT( at_console, "Skipping %s from mapcycle, not a valid map\n", szMap );
			}


		}

		FREE_FILE( aFileList );
	}

	// Fixup circular list pointer
	item = cycle->items;

	// Reverse it to get original order
	while ( item )
	{
		next = item->next;
		item->next = newlist;
		newlist = item;
		item = next;
	}
	cycle->items = newlist;
	item = cycle->items;

	// Didn't parse anything
	if ( !item )
	{
		return 0;
	}

	while ( item->next )
	{
		item = item->next;
	}
	item->next = cycle->items;
	
	cycle->next_item = item->next;


	

	return 1;
}

mapcycle_item_s *CurrentMap(mapcycle_t *cycle)
{
	mapcycle_item_s *item;
	for ( item = mapcycle.next_item; item->next != mapcycle.next_item; item = item->next )	{
		if (!stricmp(item->mapname , STRING(gpGlobals->mapname)) ) break;
	}

	return(item->next);
}


/*********************************************************************************************
*
*
* check_map - check if we are allowed to vote for that map
*
*
**********************************************************************************************/
int allowed_map(char *map) { //is this map in maps.ini ? 1=yes, 0=no

	char *mapcfile = (char*)CVAR_GET_STRING( "maps_file" );
	if (mapcfile==NULL || FStrEq(mapcfile,"0")) return 0;
	int length;
	char *pFileList;
	char *aFileList = pFileList = (char*)LOAD_FILE_FOR_ME( mapcfile, &length );
	if ( pFileList && length )
	{


		// keep pulling mapnames out of the list until we find "map", else return 0
		while ( 1 )
		{
			while ( *pFileList && isspace( *pFileList ) ) pFileList++; // skip over any whitespace
			if ( !(*pFileList) )
				break;

			char cBuf[32];
			int ret = sscanf( pFileList, " %32s", cBuf );
			// Check the map name is valid
			if ( ret != 1 || *cBuf < 13 )
				break;

			if ( FStrEq( cBuf, map ) )
			{  // we've found our map;  they can do it!
				return 1;
			}

			pFileList += strlen( cBuf );
		}

		FREE_FILE( aFileList );
	}
return 0;

}


int listmaps(edict_t *pAdminEnt) {
    		mapcycle_item_s *item;
		char *mapcfile = (char*)CVAR_GET_STRING( "mapcyclefile" );
		DestroyMapCycle( &mapcycle );
		ReloadMapCycleFile( mapcfile, &mapcycle );

	for ( item = mapcycle.next_item; item->next!= mapcycle.next_item; item = item->next )	{
		 CLIENT_PRINTF( pAdminEnt, print_console,UTIL_VarArgs("%s\n",item->mapname));
	}
	    CLIENT_PRINTF( pAdminEnt, print_console,UTIL_VarArgs("%s\n",item->mapname));




return 0;
}

int check_map(char *map)
{

	char *mapcfile = (char*)CVAR_GET_STRING( "maps_file" );

	if ( FStrEq(map,"next_map")) { // next map in the cycle

		mapcycle_item_s *item;
		char *mapcfile = (char*)CVAR_GET_STRING( "mapcyclefile" );
		DestroyMapCycle( &mapcycle );
		ReloadMapCycleFile( mapcfile, &mapcycle );
		item=CurrentMap(&mapcycle);
		strcpy(map,item->mapname);
		return (IS_MAP_VALID(item->mapname));


	} else { // they have defined the maps.ini file for us

	char *mapcfile = (char*)CVAR_GET_STRING( "maps_file" );
	if (mapcfile==NULL || FStrEq(mapcfile,"0") ) { // no maps.ini file defined, just check it is good
		return (IS_MAP_VALID(map));
	}
	if (!IS_MAP_VALID(map)) return 0; // bad map name
	return(allowed_map(map));

	} // end of maps.ini else check



return 0;

}

int GetSGTeam(edict_t *pEntity)
{   
	int ColourMap;

    ColourMap = pEntity->v.colormap;

    if(ColourMap == 0xA096)
    	return 1;
    if(ColourMap == 0x04FA)
    	return 2;
    if(ColourMap == 0x372D)
    	return 3;
    if(ColourMap == 0x6E64)
    	return 4;
    return 0;
}

int GetNumberOFFreeEnts()
{
	return (gpGlobals->maxEntities - NUMBER_OF_ENTITIES());
}

int GetEntAmountState()
{
	if(GetNumberOFFreeEnts() <= KEEP_CLEAR)
		return 1;

	return 0;
}

// ripped this out of the engine
float	UTIL_AngleMod(float a)
{
	if (a < 0)
	{
		a = a + 360 * ((int)(a / 360) + 1);
	}
	else if (a >= 360)
	{
		a = a - 360 * ((int)(a / 360));
	}
	// a = (360.0/65536) * ((int)(a*(65536/360.0)) & 65535);
	return a;
}

void UTIL_BloodDecalTrace( TraceResult *pTrace, int bloodColor )
{
	if ( UTIL_ShouldShowBlood( bloodColor ) )
	{
		if ( bloodColor == BLOOD_COLOR_RED )
			UTIL_DecalTrace( pTrace, DECAL_BLOOD1 + RANDOM_LONG(0,5) );
		else
			UTIL_DecalTrace( pTrace, DECAL_YBLOOD1 + RANDOM_LONG(0,5) );
	}
}


void UTIL_DecalTrace( TraceResult *pTrace, int decalNumber )
{
	short entityIndex;
	int index;
	int message;

	if ( decalNumber < 0 )
		return;

	index = gDecals[ decalNumber ].index;

	if ( index < 0 )
		return;

	if (pTrace->flFraction == 1.0)
		return;

	// Only decal BSP models
	if ( pTrace->pHit )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( pTrace->pHit );
		if ( pEntity && !pEntity->IsBSPModel() )
			return;
		entityIndex = ENTINDEX( pTrace->pHit );
	}
	else 
		entityIndex = 0;

	message = TE_DECAL;
	if ( entityIndex != 0 )
	{
		if ( index > 255 )
		{
			message = TE_DECALHIGH;
			index -= 256;
		}
	}
	else
	{
		message = TE_WORLDDECAL;
		if ( index > 255 )
		{
			message = TE_WORLDDECALHIGH;
			index -= 256;
		}
	}
	
	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( message );
		WRITE_COORD( pTrace->vecEndPos.x );
		WRITE_COORD( pTrace->vecEndPos.y );
		WRITE_COORD( pTrace->vecEndPos.z );
		WRITE_BYTE( index );
		if ( entityIndex )
			WRITE_SHORT( entityIndex );
	MESSAGE_END();
}

BOOL UTIL_ShouldShowBlood( int color )
{
	if ( color != DONT_BLEED )
	{
		if ( color == BLOOD_COLOR_RED )
		{
			if ( CVAR_GET_FLOAT("violence_hblood") != 0 )
				return TRUE;
		}
		else
		{
			if ( CVAR_GET_FLOAT("violence_ablood") != 0 )
				return TRUE;
		}
	}
	return FALSE;
}

float UTIL_Approach( float target, float value, float speed )
{
	float delta = target - value;

	if ( delta > speed )
		value += speed;
	else if ( delta < -speed )
		value -= speed;
	else 
		value = target;

	return value;
}

int UTIL_CountTeam(int iTeam)
{
	edict_t *PlEdict = NULL;
	int iCount = 0;

	for(int i = 1; i <= gpGlobals->maxClients; i++)
	{
		PlEdict = INDEXENT(i);
		if(PlEdict)
			if(PlEdict->v.team == iTeam)
				iCount++;
	}
	return iCount;
}

bool TeamChangeValid(int iTeam, int iCurrent)
{
	/*
	This function will work like this:

	Firstly check to see if any of the teams are full
	according to their player restriction value
	set by the mapper (if there is one), if the team
	is then it is ignored from the rest of the function
	if its not full then its included.

	After this its a basic player counting thing

	I've reset the player edict team to 0 on connect and
	disconnect to hopefully stop problems of counting
	some spectators as players.

	*/

	int i = 0;
	bool ignore_team[5];
	int LeastPlayers = 0;
	bool FirstTeam = true;

	for(i = 1; i <= 4; i++)
		ignore_team[i] = false;

	//See if any teams are full
	for(i = 1; i <= 4; i++)
	{
		if(team_available[i] == 0)
		{
			ignore_team[i] = true;
		}

		if(team_limits[i-1] > 0)
		{
			if(UTIL_CountTeam(i) >= team_limits[i-1])
				ignore_team[i] = true;
		}
	}

	for(i = 1; i <= 4; i++)
	{
		if(ignore_team[i] == false)
		{
			if(FirstTeam == true)
			{
				FirstTeam = false;

				LeastPlayers = UTIL_CountTeam(i);
			}
			else
			{
				if(UTIL_CountTeam(i) < LeastPlayers)
				{
					LeastPlayers = UTIL_CountTeam(i);
				}
			}
		}
	}

	if(iCurrent > 0)
		if(UTIL_CountTeam(iCurrent) == LeastPlayers)
			return false;
	
	if(UTIL_CountTeam(iTeam) == LeastPlayers)
		return true;
	else
		return false;
}

int UTIL_CountPlayers()
{
	edict_t *pEntity = NULL;
	int PlCount = 0;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pEntity = INDEXENT(i);

		if(pEntity)
		{
			if(!FStrEq(STRING(pEntity->v.netname),""))
			{
				PlCount++;
			}
		}
	}
	return PlCount;
}

void UTIL_PlayerStatusTracelines()
{
	edict_t *PlEdict = NULL;
	int iCount = 0;

	for(int i = 1; i <= gpGlobals->maxClients; i++)
	{
		PlEdict = INDEXENT(i);
		if(PlEdict)
		{
			if(UTIL_SpectCheck(PlEdict) == 0)
			{
				//Shoot a traceline out of the player for 9000 units.
				Vector v_src, v_dest;
				TraceResult tr;

				UTIL_MakeVectors( PlEdict->v.v_angle );

				v_src = PlEdict->v.origin + PlEdict->v.view_ofs;
				v_dest = v_src + gpGlobals->v_forward * 9000;

				UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, PlEdict->v.pContainingEntity, &tr);

				if (tr.flFraction < 1.0)
				{
					//Ignoring anything except weapon_ao_fencepost and weapon_ao_guncontrol.
					if (strcmp("weapon_ao_fencepost", STRING(tr.pHit->v.classname)) == 0)
					{
						if(tr.pHit->v.team == PlEdict->v.team)
						{
							MESSAGE_BEGIN( MSG_ONE, message_StatusText, NULL, PlEdict );
								WRITE_BYTE (1);
								WRITE_STRING( "1 Sensor built by: %p1\n2    Health: %i2%%\n3  Armor: %i3%%\n" );
							MESSAGE_END();

							MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
								WRITE_BYTE (1);
								WRITE_SHORT(ENTINDEX(tr.pHit->v.euser1));
							MESSAGE_END();

							MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
								WRITE_BYTE (2);
								WRITE_SHORT(UTIL_BuildingPercentageHealth(tr.pHit->v.health,tr.pHit->v.max_health));
							MESSAGE_END();

							MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
								WRITE_BYTE (3);
								WRITE_SHORT(0);
							MESSAGE_END();
						}

						PlayerInfo[ENTINDEX(PlEdict)].StatusReset = gpGlobals->time + 1;
					}

					if (strcmp("weapon_ao_guncontrol", STRING(tr.pHit->v.classname)) == 0)
					{
						MESSAGE_BEGIN( MSG_ONE, message_StatusText, NULL, PlEdict );
							WRITE_BYTE (1);
							WRITE_STRING( "1 Turret built by: %p1\n2    Health: %i2%%\n3  Armor: %i3%%\n" );
						MESSAGE_END();

						MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
							WRITE_BYTE (1);
							WRITE_SHORT(ENTINDEX(tr.pHit->v.euser1));
						MESSAGE_END();

						if(tr.pHit->v.team == PlEdict->v.team)
						{
							MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
								WRITE_BYTE (2);
								WRITE_SHORT(UTIL_BuildingPercentageHealth(tr.pHit->v.health,tr.pHit->v.max_health));
							MESSAGE_END();
						} else {
							MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
								WRITE_BYTE (2);
								WRITE_SHORT(0);
							MESSAGE_END();
						}

						MESSAGE_BEGIN( MSG_ONE, message_StatusValue, NULL, PlEdict );
							WRITE_BYTE (3);
							WRITE_SHORT(0);
						MESSAGE_END();

						PlayerInfo[ENTINDEX(PlEdict)].StatusReset = gpGlobals->time + 1;
					}
				}
			}
		}
	}
}

int UTIL_BuildingPercentageHealth(float iHealth, float iMaximum)
{
	return (int)(((iMaximum - (100000 - iHealth)) / iMaximum) * 100);
}

void UTIL_PlayerRepairTracelines(edict_t *PlEdict)
{
	if(PlEdict)
	{
		//Return if not engy
		if(PlEdict->v.playerclass != CLASS_ENGINEER)
			return;

		//Return if spanner not out
		if(!FStrEq(STRING(PlEdict->v.viewmodel),"models/v_tfc_spanner.mdl"))
			return;

		if((PlEdict->v.button & IN_ATTACK) && (UTIL_SpectCheck(PlEdict) == 0))
		{
			//Shoot a traceline out of the player for 9000 units.
			Vector v_src, v_dest;
			TraceResult tr;

			UTIL_MakeVectors( PlEdict->v.v_angle );

			v_src = PlEdict->v.origin + PlEdict->v.view_ofs;
			v_dest = v_src + gpGlobals->v_forward * 42;

			UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, PlEdict->v.pContainingEntity, &tr);

			if (tr.flFraction < 1.0)
			{
				//Ignoring anything except weapon_ao_fencepost and weapon_ao_guncontrol.
				if (strcmp("weapon_ao_fencepost", STRING(tr.pHit->v.classname)) == 0)
				{
					if((tr.pHit->v.health < 100000) && (tr.pHit->v.team == PlEdict->v.team))
					{
						if(GetpvPrivateInt(PlEdict,208) /*PlEdict->v.armorvalue*/ >= FENCEPOST_FIX_REQ)
						{
							tr.pHit->v.health = 100000;
							//PlEdict->v.armorvalue -= FENCEPOST_FIX_REQ;
							PatchpvPrivateInt(PlEdict,208,(GetpvPrivateInt(PlEdict,208) - FENCEPOST_FIX_REQ));
						}							
					}
				}

				if (strcmp("weapon_ao_guncontrol", STRING(tr.pHit->v.classname)) == 0)
				{
					if((tr.pHit->v.health < 100000) && (tr.pHit->v.team == PlEdict->v.team))
					{
						if(GetpvPrivateInt(PlEdict,208) /*PlEdict->v.armorvalue*/ >= BIGGUN_FIX_REQ)
						{
							tr.pHit->v.health = 100000;
							//PlEdict->v.armorvalue -= BIGGUN_FIX_REQ;
							PatchpvPrivateInt(PlEdict,208,(GetpvPrivateInt(PlEdict,208) - BIGGUN_FIX_REQ));
						}							
					}
				}
			}
		}
	}
}

void UTIL_SendCustomBucketMessages(edict_t *pEntity)
{
	int gmsgWeaponList = REG_USER_MSG( "WeaponList", -1 );

	if(gmsgWeaponList > MAX_USERMSG)
		return;

	if(pEntity->v.playerclass == CLASS_SOLDIER)
	{
		//Energy gun
		MESSAGE_BEGIN( MSG_ONE, gmsgWeaponList, NULL, pEntity );
			WRITE_STRING( "tf_weapon_energygun" ); //Name (command)
			WRITE_BYTE (-1); //Ammo1 Type
			WRITE_BYTE (-1); //Ammo1 Max
			WRITE_BYTE (-1); //Ammo2 Type
			WRITE_BYTE (-1); //Ammo2 Max
			WRITE_BYTE (5); //Slot
			WRITE_BYTE (0); //Slot Position
			WRITE_BYTE (18); //ID
			WRITE_BYTE (0); //Flags
		MESSAGE_END();

		pEntity->v.weapons |= (1<<18);
	}

	if(pEntity->v.playerclass == CLASS_DEMOMAN)
	{
		//Blaster
		MESSAGE_BEGIN( MSG_ONE, gmsgWeaponList, NULL, pEntity );
			WRITE_STRING( "tf_weapon_blaster" ); //Name (command)
			WRITE_BYTE (-1); //Ammo1 Type
			WRITE_BYTE (-1); //Ammo1 Max
			WRITE_BYTE (-1); //Ammo2 Type
			WRITE_BYTE (-1); //Ammo2 Max
			WRITE_BYTE (5); //Slot
			WRITE_BYTE (0); //Slot Position
			WRITE_BYTE (18); //ID
			WRITE_BYTE (0); //Flags
		MESSAGE_END();

		pEntity->v.weapons |= (1<<18);
	}

	if(pEntity->v.playerclass == CLASS_CIVILIAN)
	{
		//Blaster
		MESSAGE_BEGIN( MSG_ONE, gmsgWeaponList, NULL, pEntity );
			WRITE_STRING( "tf_weapon_tazar" ); //Name (command)
			WRITE_BYTE (-1); //Ammo1 Type
			WRITE_BYTE (-1); //Ammo1 Max
			WRITE_BYTE (-1); //Ammo2 Type
			WRITE_BYTE (-1); //Ammo2 Max
			WRITE_BYTE (5); //Slot
			WRITE_BYTE (0); //Slot Position
			WRITE_BYTE (18); //ID
			WRITE_BYTE (0); //Flags
		MESSAGE_END();

		pEntity->v.weapons |= (1<<18);
	}
}

void FormatBuffer(char *pData, int length, char *pBuffer)
{
	int index;
	char *pDataPrev;

	*pBuffer = 0;
	pDataPrev = pData;

	for (index = 0; index < length; index++)
	{
		if (index % 16 == 8)
			sprintf(pBuffer, "-%02X", (unsigned char)*pData);
		else
			sprintf(pBuffer, " %02X", (unsigned char)*pData);

		pBuffer += 3;
		pData++;

		if ((index+1) % 16 == 0)
		{
			*pBuffer++ = ' ';
			*pBuffer++ = ' ';
			*pBuffer++ = '|';

			while (pDataPrev < pData)
			{
				if ((*pDataPrev >= ' ') && (*pDataPrev <= '~'))
				   *pBuffer++ = *pDataPrev++;
				else
				{
					*pBuffer++ = '.';
					pDataPrev++;
				}
			}

			*pBuffer++ = '|';
			*pBuffer++ = '\n';
		}
	}

	if ((length % 16) != 0)
	{
		for (index=(length % 16); index < 16; index++)
		{
			*pBuffer++ = ' ';
			*pBuffer++ = ' ';
			*pBuffer++ = ' ';
		}

		*pBuffer++ = ' ';
		*pBuffer++ = ' ';
		*pBuffer++ = '|';

		while (pDataPrev < pData)
		{
			if ((*pDataPrev >= ' ') && (*pDataPrev <= '~'))
				*pBuffer++ = *pDataPrev++;
			else
			{
				*pBuffer++ = '.';
				pDataPrev++;
			}
		}

		for (index=(length % 16); index < 16; index++)
			*pBuffer++ = ' ';

		*pBuffer++ = '|';
		*pBuffer++ = '\n';
	}

	*pBuffer = 0;
}

void UTIL_ShowMenu(edict_t *pEntity, int bitsValidSlots, int nDisplayTime, BOOL fNeedMore, char *pszText)
{
	int gmsgShowMenu = REG_USER_MSG( "ShowMenu", -1 );
	
	if(gmsgShowMenu > MAX_USERMSG)
		return;

    MESSAGE_BEGIN( MSG_ONE, gmsgShowMenu, NULL, pEntity);
        WRITE_SHORT( bitsValidSlots);
        WRITE_CHAR( nDisplayTime );
        WRITE_BYTE( fNeedMore );
        WRITE_STRING (pszText);
    MESSAGE_END();
}


void PatchpvPrivateInt(edict_t *pEntity, int offset, int value)
{
	/*

	Known Offsets
	
	Player Class
	Offset 200	 = Nails amount (int)
	Offset 216	 = Rocket amount (int)
	Offset 192	 = Shell amount (int)
	Offset 208	 = Cell amount (int)
	Offset 60	 = Gren1 amount (int)
	Offset 56	 = Gren2 amount (int)	  
	Offset 326	 = Can build SG? Set to 00 for yes (2 bytes)
	Offset 322	 = Can build Disp? Set to 00 for yes (2 bytes)
	Offset 288	 = Internal Frags value (int)

	Monster_miniturret + Monster_turret (+ Monster_sentry?)
	Offset 276	 = Team number (int)

	Building_sentrygun
	Offset 148	 = Angle 1 (int)
	Offset 152	 = Angle 2 (int)
	Offset 192	 = Ammo percentage (int)
	Offset 128	 = SentryGun Level (int)
	Offset 216	 = Number of rockets (int)

	*/

	*(int *)((char *)pEntity->pvPrivateData + offset) = value;
}

int GetpvPrivateInt(edict_t *pEntity, int offset)
{
	return *(int *)((char *)pEntity->pvPrivateData + offset);
}

void UTIL_RotateSG(edict_t *pPlayer, BOOL Rotate180)
{
	//Get pointer to SG
	edict_t *pSentryGun = NULL;

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pPlayer->v.v_angle );

	v_src = pPlayer->v.origin + pPlayer->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 1000;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pPlayer->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if(strcmp("building_sentrygun", STRING(tr.pHit->v.classname)) == 0)
		{
			//does it belong to us?
			//if(*(int *)((char *)tr.pHit->pvPrivateData + 144) == ENTINDEX(pPlayer))
			if(tr.pHit->v.euser4 == pPlayer)
			{
				pSentryGun = tr.pHit;
			} else {
				CLIENT_PRINTF( pPlayer, print_console,"That sentrygun does not belong to you.\n");
				CLIENT_PRINTF( pPlayer, print_center,"That sentrygun does not belong to you.\n");
				return;
			}
		}
	}

	if(pSentryGun == NULL)
	{
		CLIENT_PRINTF( pPlayer, print_console,"You must look at the sentrygun\nyou want to rotate.\n");
		CLIENT_PRINTF( pPlayer, print_center,"You must look at the sentrygun\nyou want to rotate.\n");
		return;
	}

	//Rotate
	int Angle1 = 0;
	int Angle2 = 0;

	Angle1 = *(int *)((char *)pSentryGun->pvPrivateData + 148);
	Angle2 = *(int *)((char *)pSentryGun->pvPrivateData + 152);

	if(Rotate180)
	{
		Angle1 += 180; //Add 180 degrees
		if(Angle1>=360)
			Angle1 -=360;

		Angle2 += 180;
		if(Angle2>=360)
			Angle2 -=360;
	} else {
		Angle1 += 45; //Minus 135 degrees
		if(Angle1>=360)
			Angle1 -=360;

		Angle2 += 45;
		if(Angle2>=360)
			Angle2 -=360;
	}

	//Write new angles to sentrygun.
	*(int *)((char *)pSentryGun->pvPrivateData + 148) = Angle1;
	*(int *)((char *)pSentryGun->pvPrivateData + 152) = Angle2;

	CLIENT_PRINTF( pPlayer, print_console,"Sentrygun rotated.\n");
	CLIENT_PRINTF( pPlayer, print_center,"Sentrygun rotated.\n");
}

void UTIL_Sparks( const Vector &position )
{
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, position );
		WRITE_BYTE( TE_SPARKS );
		WRITE_COORD( position.x );
		WRITE_COORD( position.y );
		WRITE_COORD( position.z );
	MESSAGE_END();
}

//=========================================================================
// Returns 0 if the area around obj is safe to build in
int CheckArea( edict_t *pEntity, edict_t *pIgnore )
{
	TraceResult tr;
	Vector vecOrg = pEntity->v.origin;

	// Check the origin
	int iContents = UTIL_PointContents(vecOrg);
	if ( iContents != CONTENT_EMPTY && iContents != CONTENT_WATER ) 
		return CAREA_BLOCKED;

	Vector vecIgnoreOrg = pIgnore->v.origin;
	// Get the player's origin irrelevant of crouching
	if ( pIgnore->v.flags & FL_DUCKING )
	{
		vecIgnoreOrg = vecIgnoreOrg + (VEC_DUCK_HULL_MIN - VEC_HULL_MIN);
	}
	// Trace a hull
	UTIL_TraceHull( vecIgnoreOrg, pEntity->v.origin, ignore_monsters, large_hull, pEntity, &tr );
	edict_t *pTraced = tr.pHit;
	if (tr.flFraction != 1 || tr.fAllSolid == 1)
		return CAREA_BLOCKED;

	// Check for solid entities in the area
	CBaseEntity *pEnt = NULL;
	edict_t *pEnt2 = NULL;
	while ( (pEnt = UTIL_FindEntityInSphere( pEnt, pEntity->v.origin, 48 )) != NULL )
	{
		pEnt2 = pEnt->edict();
		// If it's not the engineer, and its a solid entity, fail
		if (pEnt2 != pIgnore && pEnt2 != pEntity && pEnt2->v.solid > SOLID_TRIGGER)
		return CAREA_BLOCKED;
	}

	// Cycle through all the Nobuild zones in the map and make sure this isn't in one of them
	CBaseEntity *pNoBuild = UTIL_FindEntityByClassname( NULL, "func_nobuild" );
	while ( pNoBuild )
	{
		// Check to see if we're building in this zone
		if ( vecOrg.x >= pNoBuild->pev->mins.x && vecOrg.y >= pNoBuild->pev->mins.y && vecOrg.z >= pNoBuild->pev->mins.z && vecOrg.x <= pNoBuild->pev->maxs.x && vecOrg.y <= pNoBuild->pev->maxs.y && vecOrg.z <= pNoBuild->pev->maxs.z )
			return CAREA_NOBUILD;
		pNoBuild = UTIL_FindEntityByClassname( pNoBuild, "func_nobuild" );
	}

	// Check below
	UTIL_TraceLine( vecOrg, vecOrg + Vector(0,0,-64), dont_ignore_monsters, pEntity, &tr );
		if ( tr.flFraction == 1.0 )
			return CAREA_BLOCKED;

	return CAREA_CLEAR;
}

void PlayRandomLoop()
{
	int iMax = 30;

	int iRand = (int)RANDOM_LONG(1,30);

	char strRand[50];
	_ltoa(iRand,strRand,10);

	char UserFileName[200];
	strcpy(UserFileName, "autoop/loop");
	strcat(UserFileName, strRand);
	strcat(UserFileName, ".wav");

	AdmPlaySound(NULL,UserFileName);

	BotTalkAllArgs("AutoDJ - Playing loop number %d\n",iRand);
}

int SendICQ(edict_t *pCaller, char *From, char *FromEmail, char *Subject, char *Body, char *To)
{
	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "SendICQ()\n");
		fclose(fp);
	}

	WSADATA wsa;							
	
	if (WSAStartup(MAKEWORD(1,1), &wsa))	// if error
	{
		if(pCaller != NULL)
			CLIENT_PRINTF( pCaller, print_console, UTIL_VarArgs( "[AUTOOP] ICQ: Unable to open socket.\n"));
		return -1;
	}

	char szData [500];			// buffer for string to send
	char szDataF[500];			// temporary buffer
	char *pszDataF = szDataF;	// pointer to temporary buffer
	char *pszData  = szData;	// pointer to stringbuffer
	char szFrom[50];			// name of sender
	char szFromEmail[50];		// email-address of sender
	char szTo[15];				// UIN of recipient
	char szSubject[100];		// subject
	char szBody[256];			// message-text
	
	SOCKET s;					// declarations of socket,
	struct hostent *host;		// HOSTENT and
	struct sockaddr_in addr;	// SOCKADDR_IN
	
	strncpy(szFrom,From,49);
	szFrom[50] = '\0';

	strncpy(szFromEmail,FromEmail,49);
	szFromEmail[50] = '\0';

	strncpy(szSubject,Subject,99);
	szSubject[100] = '\0';

	strncpy(szBody,Body,255);
	szBody[256] = '\0';

	strncpy(szTo,To,14);
	szTo[15] = '\0';
	
	// --------------------------------------------------
	// builds the unformatted string to send
	// --------------------------------------------------
	strcpy(szData,szFrom);
	strcat(szData,"&fromemail=");
	strcat(szData,szFromEmail);
	strcat(szData,"&subject=");
	strcat(szData,szSubject);
	strcat(szData,"&body=");
	strcat(szData,szBody);
	strcat(szData,"&to=");
	strcat(szData,szTo);
		

	// -------------------------------------------------
	// Parsing. All blanks are replaced by '%20'
	// -------------------------------------------------
	while(*pszData != '\0')			// until string end is
	{								// not reached
		if(*pszData == ' ')			// compare to blanks
		{
			*(pszDataF++) = '%';	// replacement-chars in temp.
			*(pszDataF++) = '2';	// buffer + increase pointer
			*(pszDataF++) = '0';
			pszData++;				// increase pointer to string
		}
		else
			*(pszDataF++) = *(pszData++);	// if no blank, copy
		}									// without changes
	
	*pszDataF = '\0';				// string-end
	
	
	strcpy(szData,"GET /scripts/WWPMsg.dll?from="); // add header
	strcat(szData,szDataF);							// in front of string
	strcat(szData,"\n");							// LF for end
	
	host = gethostbyname("wwp.icq.com");	// find IP-address
	if(host == 0)							// if not found, 
	{										// error
		if(pCaller != NULL)
			CLIENT_PRINTF( pCaller, print_console, UTIL_VarArgs( "[AUTOOP] ICQ: Unable to resolve ICQ server IP.\n"));
		WSACleanup();
		return -1;
	}

	// ---------------------------------------------------------
	// copy IP, protocol-family and portnumber	
	// to sockaddr_in
	// ---------------------------------------------------------
	addr.sin_addr	= *(struct in_addr*)host->h_addr; // IP
	addr.sin_family = AF_INET;						  // protocol-f.
	addr.sin_port	= htons(80);					  // portnumber

	s = socket(AF_INET, SOCK_STREAM,0);	// create socket
	if (s == -1)						// if problems, error-message
	{
		if(pCaller != NULL)
			CLIENT_PRINTF( pCaller, print_console, UTIL_VarArgs( "[AUTOOP] ICQ: Unable to create socket.\n"));
		WSACleanup();
		return -1;
	}
	
	// ------------------------------------------------
	// Connect socket with host (wwp.icq.com')
	// ------------------------------------------------
	connect(s,(struct sockaddr*)&addr,sizeof(addr));
	
	// ------------------------------------------------
	// send the built, formatted string
	// ------------------------------------------------	
	send(s,szData,strlen(szData),0);

	// ------------------------------------------------
	// Message of success
	// ------------------------------------------------
	if(pCaller != NULL)
		CLIENT_PRINTF( pCaller, print_console, UTIL_VarArgs( "[AUTOOP] ICQ: Sent sucessfully.\n"));

	WSACleanup();

	return 0;
}

void SendStandardICQ(char *To, char *Subject, char *Body)
{
	char From[50];

	strncpy(From,"Shodan - ",49);
	strcat(From,CVAR_GET_STRING("hostname"));
	From[50] = '\0';

	if(strcmp(To,"ALL") == 0)
	{
		const int iListMax = 75;
		char szICQTemp[15];
		int ch,i;

		FILE *ICQfp;
		ICQfp=fopen("tfc\\autoop\\icqnotify.txt","r");

		if(ICQfp == NULL)
			return;

		ch = fgetc(ICQfp);

		// skip any leading blanks
		while (ch == ' ')
			ch = fgetc(ICQfp);

		szICQTemp[0] = '\0';
		i = 0;

		while (ch != EOF)
		{
			if((ch == '\r') || (ch == '\n'))
			{
				if(strlen(szICQTemp) > 5)
					SendICQ(NULL,From,"N/A",Subject,Body,szICQTemp);
				szICQTemp[0] = '\0';
				i = 0;
			} else {
				szICQTemp[i] = ch;
				i++;
			}
			ch = fgetc(ICQfp);
		}
		
		if(strlen(szICQTemp) > 5)
			SendICQ(NULL,From,"N/A",Subject,Body,szICQTemp);

		fclose(ICQfp);
		return;
	} else {
		SendICQ(NULL,From,"N/A",Subject,Body,To);
	}
}

//String parsing functions
void strreplace(char* source, char* replace, char* with)
{
	int pos;
	pos = ifindstr(0,source,replace);
	
	if(pos == -1)
	{
		return;
	}
	else
	{
		char* left = new char [ strlen(source) + 1 ];
		char* right = new char [ strlen(source) + 1 ];
		leftcopy(source,left,pos);
		rightcopy(source,right,pos+strlen(replace));
		strcpy(source,left);
		strcat(source,with);
		strcat(source,right);
		delete [] left;
		delete [] right;
		strreplace(source,replace,with);
	}
}

// find string, starting from 
int ifindstr(int startx,char* body,char* search)
{
	int len = strlen(body);
	int len2 = strlen(search); // search len
       
	for(int i = startx+1; i < len; i++)
	{
		if(body[i] == search[0])
		{
			bool ichk = true;
                    	
			for(int z = 0; z < len2; z++)
			{
				if(body[i+z] == search[z])
				{
				}
				else
				{
					ichk = false;
				}
			}
                               	
			if(ichk == true)
			{
				return i;
			}
		}
	}
                               
	return -1; // failure
}

// left copy
void leftcopy(char* input,char* output,int pos)
{
	int index = 0;
	for(int i = 0; i < pos; i++)
	{
		output[index] = input[i];
		index++;
	}
		output[index] = 0;
}

// right copy
void rightcopy(char* input,char* output,int pos)
{
	int index = 0;
	int len = strlen(input);
	for(int i = pos; i < len; i++)
	{
		output[index] = input[i];
		index++;
	}

	output[index] = 0;
}