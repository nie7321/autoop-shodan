/*****************************************************\
*	Auto-OP Shodan
*
*	File: tazar.cpp
*	Description: Prezes Anti Sniper Tazar.
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

void SwitchToTazar(edict_t *pEntity)
{
	if(pEntity->v.playerclass != CLASS_CIVILIAN || pEntity->v.health <= 0 || pEntity->v.weaponmodel == MAKE_STRING(TAZAR_PMODEL))
		return;

	CLIENT_COMMAND(pEntity, "tf_weapon_axe\n");
	PlayerInfo[ENTINDEX(pEntity)].SwitchingToCustomGun = TAZAR_ID;

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

void FireTazar(edict_t *pEntity)
{
	if(Features.Debug >= 2)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "FireTazar(): Player fired tazar.\n");
		fclose(fp);
	}

	if(pEntity->v.playerclass != CLASS_CIVILIAN || pEntity->v.health <= 0)
		return;

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 400;

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if ( (strcmp("player", STRING(tr.pHit->v.classname)) == 0) && UTIL_SpectCheck(tr.pHit) == 0)
		{
			int Team1 = tr.pHit->v.team;
			int Team2 = pEntity->v.team;
			if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
			{
				PlayerInfo[ENTINDEX(pEntity)].LastUsedEnergy[TAZAR_ID] = gpGlobals->time + TAZAR_DELAY;
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/electro4.wav", 1, ATTN_NORM, 0, 100);

				MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
					WRITE_BYTE(TE_BEAMENTS);
					WRITE_SHORT(ENTINDEX(pEntity));
					WRITE_SHORT(ENTINDEX(tr.pHit));
					WRITE_SHORT(m_Chain);
					WRITE_BYTE(1);// byte (starting frame) 
					WRITE_BYTE(0);// byte (frame rate in 0.1's) 
					WRITE_BYTE(10);// byte (life in 0.1's) 
					WRITE_BYTE(10);// byte (line width in 0.1's) 
					WRITE_BYTE(10);// byte (noise amplitude in 0.01's) 
					WRITE_BYTE(0);// byte,byte,byte (color)
					WRITE_BYTE(100);
					WRITE_BYTE(200);
					WRITE_BYTE(200);// byte (brightness)
					WRITE_BYTE(10);// byte (scroll speed in 0.1's)
				MESSAGE_END();

				if(tr.pHit->v.health > TAZAR_DAMAGE)
				{
					TakeDamage(tr.pHit,NULL,TAZAR_DAMAGE,256);
				} else {
					edict_t *pBomb;
					pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
					DispatchSpawn(pBomb);
					pBomb->v.origin = tr.vecEndPos;
					pBomb->v.dmg = 100;
					pBomb->v.team = pEntity->v.team;
					pBomb->v.owner = pEntity;
					pBomb->v.classname = MAKE_STRING("ao_tazar_kill");
					pBomb->v.effects |= EF_NODRAW;
					pBomb->v.nextthink = gpGlobals->time;
				}
			}
		}
	}
}
