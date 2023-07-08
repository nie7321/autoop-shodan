/*****************************************************\
*	Auto-OP Shodan
*
*	File: hwradar.cpp
*	Description: HWGuy radar functions.
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

extern TFXFeatureEnabled_s Features;
extern FILE *fp;

extern int m_Chain;			//Chain sprite

void DoRadar(edict_t *pEntity)
{
	if(pEntity->v.armorvalue >= 30)
	{
		pEntity->v.armorvalue -= 30;
	} else {
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "SG Radar requires 30 armor points.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "SG Radar requires 30 armor points.\n"));
		return;
	}

	bool FoundSomething = false;

	CBaseEntity *pScan = NULL;
	while ((pScan = UTIL_FindEntityInSphere(pScan, pEntity->v.origin, 1000)) != NULL)
	{
		//Is it an entity we are bothered about? (Players and grens)
		if (strcmp(STRING(pScan->pev->classname), "building_sentrygun") == 0)
		{
			FoundSomething = true;

			Vector vecColor;

			vecColor = Vector(255,255,255); //Default

			if(GetSGTeam(pScan->edict()) == 1)
			{
				vecColor = Vector(0,0,255);
			}

			if(GetSGTeam(pScan->edict()) == 2)
			{
				vecColor = Vector(255,0,0);
			}

			if(GetSGTeam(pScan->edict()) == 3)
			{
				vecColor = Vector(255,255,0);
			}
			
			if(GetSGTeam(pScan->edict()) == 4)
			{
				vecColor = Vector(0,255,0);
			}

			MESSAGE_BEGIN( MSG_ONE, SVC_TEMPENTITY, NULL, pEntity );
			WRITE_BYTE( TE_BEAMENTPOINT ); 
			WRITE_SHORT( ENTINDEX(pEntity) );
			WRITE_COORD( pScan->pev->origin.x );
			WRITE_COORD( pScan->pev->origin.y );
			WRITE_COORD( pScan->pev->origin.z );
			WRITE_SHORT( m_Chain ); 
			WRITE_BYTE( 1 ); //start frame
			WRITE_BYTE( 0 ); 
			WRITE_BYTE( 80 ); //8 Seconds
			WRITE_BYTE( 6 ); //width 
			WRITE_BYTE( 0 ); 
			WRITE_BYTE( vecColor.x ); //r
			WRITE_BYTE( vecColor.y ); //g
			WRITE_BYTE( vecColor.z ); //b
			WRITE_BYTE( 255 );
			WRITE_BYTE( 10 );
			MESSAGE_END( );
		}
	}

	if(FoundSomething == false)
	{
		CLIENT_PRINTF( pEntity, print_center, UTIL_VarArgs( "No sentryguns in this area.\n"));
		CLIENT_PRINTF( pEntity, print_console,UTIL_VarArgs( "No sentryguns in this area.\n"));
	}
}
	
