/*****************************************************\
*	Auto-OP Shodan
*
*	File: ablaster.cpp
*	Description: Demomans Anthrax Blaster Gun.
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
#include "customweapons.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>
#include "player.h"

extern AOPlayerInfo_s PlayerInfo[];
extern int m_Chain;
extern int team_allies[4];

extern TFXFeatureEnabled_s Features;
extern FILE *fp;

void SwitchToBlaster(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_DEMOMAN || pEntity->v.health <= 0 || pEntity->v.weaponmodel == MAKE_STRING(ANTHRAX_PMODEL))
		return;

	CLIENT_COMMAND(pEntity, "tf_weapon_axe\n");
	PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = ANTHRAX_BLASTER_ID;

	CBasePlayer *pPlayer = (CBasePlayer *)pEntity;

	pPlayer->m_pActiveItem = NULL;

	int gmsgCurWeapon = REG_USER_MSG("CurWeapon", 3);

	if(gmsgCurWeapon > MAX_USERMSG)
		return;

	MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pEntity );
		WRITE_BYTE(0);
		WRITE_BYTE(0);
		WRITE_BYTE(0);
	MESSAGE_END();
}

void FireBlaster(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_DEMOMAN || pEntity->v.health <= 0)
		return;

	if(pEntity->v.armorvalue < ANTHRAX_AMMO)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "Not enough armor to fire gun.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Not enough armor to fire gun.\n"));
		PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[ANTHRAX_BLASTER_ID] = gpGlobals->time + ANTHRAX_DELAY; //So the server doesnt spam the message too much.
		return;
	}

	pEntity->v.armorvalue -= ANTHRAX_AMMO;

	PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[ANTHRAX_BLASTER_ID] = gpGlobals->time + ANTHRAX_DELAY;
	
	Vector v_start, v_end;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_start = pEntity->v.origin + pEntity->v.view_ofs;
	v_end = v_start + gpGlobals->v_forward * 2000;

	UTIL_TraceLine( v_start, v_end, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "ABlaster(): Blaster Fired.\n");
		fclose(fp);
	}

	EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/gauss2.wav", 1, ATTN_NORM, 0, 100);

	//Beam for firer to a point
	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin ); //Draw 'chain'
	WRITE_BYTE( TE_BEAMENTPOINT ); 
	WRITE_SHORT( ENTINDEX(pEntity) );
	WRITE_COORD( tr.vecEndPos.x );
	WRITE_COORD( tr.vecEndPos.y );
	WRITE_COORD( tr.vecEndPos.z );
	WRITE_SHORT( m_Chain ); 
	WRITE_BYTE( 1 ); //start frame
	WRITE_BYTE( 0 ); 
	WRITE_BYTE( 5 ); //0.5 Seconds
	WRITE_BYTE( 10 ); //width 
	WRITE_BYTE( 25 ); 
	WRITE_BYTE( 255 ); //r
	WRITE_BYTE( 255 ); //g
	WRITE_BYTE( 255 ); //b
	WRITE_BYTE( 255);
	WRITE_BYTE( 10 );
	MESSAGE_END( );

	//Scan for players in a 500 unit radius.
	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 1000)) != NULL)
	{
		if (strcmp(STRING(pScan->pev->classname), "player") == 0)
		{
			int Team1 = pScan->pev->team;
			int Team2 = pEntity->v.team;
			if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
			{
				if(EnemyIsVisible(pEntity,pScan->edict()) == 1)
				{
					EMIT_SOUND_DYN2(pScan->edict(), CHAN_VOICE, "weapons/gauss2.wav", 1, ATTN_NORM, 0, 100);
					MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, tr.vecEndPos ); //Draw beams
					WRITE_BYTE( TE_BEAMENTPOINT ); 
					WRITE_SHORT( ENTINDEX(pScan->edict()) );
					WRITE_COORD( tr.vecEndPos.x );
					WRITE_COORD( tr.vecEndPos.y );
					WRITE_COORD( tr.vecEndPos.z );
					WRITE_SHORT( m_Chain ); 
					WRITE_BYTE( 1 ); //start frame
					WRITE_BYTE( 0 ); 
					WRITE_BYTE( 10 ); //0.5 Seconds
					WRITE_BYTE( 10 ); //width 
					WRITE_BYTE( 20 ); 
					WRITE_BYTE( 255 ); //r
					WRITE_BYTE( 255 ); //g
					WRITE_BYTE( 255 ); //b
					WRITE_BYTE( 255);
					WRITE_BYTE( 10 );
					MESSAGE_END( );

					edict_t *pBomb;
					pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
					DispatchSpawn(pBomb);
					pBomb->v.origin = tr.pHit->v.origin;
					pBomb->v.dmg = 50;
					pBomb->v.team = pEntity->v.team;
					pBomb->v.owner = pEntity;
					pBomb->v.classname = MAKE_STRING("ao_blasterkill");
					pBomb->v.effects |= EF_NODRAW;
					pBomb->v.nextthink = gpGlobals->time;
				}
			}
		}
	}
}