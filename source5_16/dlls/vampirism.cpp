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

extern AOPlayerInfo_s PlayerInfo[];
extern int team_allies[4];

void SuckBlood(edict_t *pEntity)
{
	if(PlayerInfo[ENTINDEX(pEntity)].LastSuckTime < gpGlobals->time)
	{
		PlayerInfo[ENTINDEX(pEntity)].LastSuckTime = gpGlobals->time + 1;
		//Draw traceline out of player to find tasty victim
		Vector v_src, v_dest;
		TraceResult tr;

		UTIL_MakeVectors( pEntity->v.v_angle );

		v_src = pEntity->v.origin + pEntity->v.view_ofs;
		v_dest = v_src + gpGlobals->v_forward * 75;

		UTIL_TraceLine( v_src, v_dest, dont_ignore_monsters, pEntity->v.pContainingEntity, &tr);

		if (tr.flFraction < 1.0)
		{
			//CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "flFraction < 1\n"));
			if ( (strcmp("player", STRING(tr.pHit->v.classname)) == 0) && UTIL_SpectCheck(tr.pHit) == 0)
			{
				//CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Found player\n"));

				int Team1 = tr.pHit->v.team;
				int Team2 = pEntity->v.team;
				if ((Team1 != Team2) && !(team_allies[(Team1-1)] & (1<<(Team2-1))))
				{
					if(tr.pHit->v.takedamage != DAMAGE_NO)
					{
						//CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "Victim is an enemy\n"));
						if(tr.pHit->v.health > 3)
						{
							tr.pHit->v.health -= 3;
							pEntity->v.health += 3;
						} else {
							CLIENT_COMMAND(tr.pHit, UTIL_VarArgs("kill\n"));
							pEntity->v.health += 3;
						}
					}
				}
			} else {
				PlayerInfo[ENTINDEX(pEntity)].IsSucking = 0;
			}
		}
	}
}
