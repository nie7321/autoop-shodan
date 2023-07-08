/*****************************************************\
*	Auto-OP Shodan
*
*	File: dualbar.cpp
*	Description: Heavy Weapon Guy's Dual Crowbar\Sword.
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
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern int team_allies[4];

void SwingDualBar(edict_t *pEntity)
{
	if(PlayerInfo[ENTINDEX(pEntity)].DualBarLastSwing > gpGlobals->time)
		return;

	//EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/cbar_hitbod2.wav", 1, ATTN_NORM, 0, 120);

	Vector v_src, v_dest;
	TraceResult tr;

	UTIL_MakeVectors( pEntity->v.v_angle );

	v_src = pEntity->v.origin + pEntity->v.view_ofs;
	v_dest = v_src + gpGlobals->v_forward * 60; //Normal CB is 32

	UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity, &tr);

	if (tr.flFraction < 1.0)
	{
		if(strcmp("worldspawn", STRING(tr.pHit->v.classname)) == 0)
		{
			EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/xbow_hit1.wav", 1, ATTN_NORM, 0, 100);
		}

		if(strcmp("player", STRING(tr.pHit->v.classname)) == 0)
		{
			int Team1 = tr.pHit->v.team;
			int Team2 = pEntity->v.team;
			if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
			{
				EMIT_SOUND_DYN2(pEntity, CHAN_VOICE, "weapons/cbar_hitbod2.wav", 1, ATTN_NORM, 0, 120);

				if(tr.pHit->v.takedamage != DAMAGE_NO)
				{
					if(tr.pHit->v.health > 20)
						tr.pHit->v.health -= 20;

					if(tr.pHit->v.health < 20)
					{
						tr.pHit->v.health = 1;
						//FakeGib(tr.pHit);

						edict_t *pBomb;
						pBomb = CREATE_NAMED_ENTITY(MAKE_STRING("tf_gl_grenade"));
						DispatchSpawn(pBomb);
						pBomb->v.origin = tr.pHit->v.origin + Vector(0, 0, 32);
						pBomb->v.dmg = 50;
						pBomb->v.owner = pEntity;
						pBomb->v.classname = MAKE_STRING("weapon_ao_sword");
						pBomb->v.effects |= EF_NODRAW;
						pBomb->v.nextthink = gpGlobals->time;
					}
				}
			}
		}
	}
	PlayerInfo[ENTINDEX(pEntity)].DualBarLastSwing = gpGlobals->time + 0.4;
}

				








