/*****************************************************\
*	Auto-OP Shodan
*
*	File: testentity.cpp
*	Description: TEST CODE.
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
#include "testentity.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];

LINK_ENTITY_TO_CLASS( ao_tfc_gun, CAutoOPGun );

void CAutoOPGun :: Spawn(void) {

	SET_MODEL(ENT(pev), "models/autoop/w_guns.mdl");
	m_fRespawnTime = 0.0;
	m_iGunType = 1; //Default
	SetThink(&CAutoOPGun::Think);

	pev->effects		= 0;
	pev->health			= 100;
	pev->nextthink		+= 1.0;
	pev->takedamage		= DAMAGE_NO;
	pev->solid			= SOLID_BBOX;
	pev->movetype		= MOVETYPE_TOSS;
}

void CAutoOPGun::Precache(void) {    
  // Precaching done in modelprecache.cpp (So clients get it)	
}

void CAutoOPGun::KeyValue( KeyValueData *pkvd ) {
	/*if (FStrEq(pkvd->szKeyName, "guntype"))//Which gun are we?
	{
		m_iGunType = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}*/
}

void CAutoOPGun::Think(void) {

}


