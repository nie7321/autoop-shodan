/*****************************************************\
*	Auto-OP Shodan
*
*	File: icons.cpp
*	Description: Icon Functions.
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

void ApplyIcon(edict_t *pEntity, const char *szIcon)
{
	//Exclaimation mark !!
	if ( FStrEq(szIcon, "suprise" )) {
		short sprExclamation = PRECACHE_MODEL( "sprites/autoop/exclamation.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprExclamation);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Ammo Icon
	if ( FStrEq(szIcon, "ammo" )) {
		short sprAmmo = PRECACHE_MODEL( "sprites/autoop/ammo.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprAmmo);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Armor Icon
	if ( FStrEq(szIcon, "armor" )) {
		short sprArmour = PRECACHE_MODEL( "sprites/autoop/armour.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprArmour);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Happy smiley
	if ( FStrEq(szIcon, "smiley" ) || FStrEq(szIcon, "happy" )) {
		short sprSmiley = PRECACHE_MODEL( "sprites/autoop/smiley.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprSmiley);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Sad smiley
	if ( FStrEq(szIcon, "sad" )) {
		short sprSad = PRECACHE_MODEL( "sprites/autoop/sad.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprSad);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Fix it!
	if ( FStrEq(szIcon, "fix" )) {
		short sprFix = PRECACHE_MODEL( "sprites/autoop/fixit.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprFix);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Bulb (Idea!)
	if ( FStrEq(szIcon, "idea" )) {
		short sprBulb = PRECACHE_MODEL( "sprites/autoop/bulb.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprBulb);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Coke (Drink!)
	if ( FStrEq(szIcon, "coke" )) {
		short sprCoke = PRECACHE_MODEL( "sprites/autoop/coke.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprCoke);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Disabled
	if ( FStrEq(szIcon, "disabled" )) {
		short sprDisabled = PRECACHE_MODEL( "sprites/autoop/disabled.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprDisabled);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Germany Flag
	if ( FStrEq(szIcon, "german" )) {
		short sprGermany = PRECACHE_MODEL( "sprites/autoop/germany.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprGermany);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Infected
	if ( FStrEq(szIcon, "infected" )) {
		short sprInfected = PRECACHE_MODEL( "sprites/autoop/infected.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprInfected);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//UK Flag
	if ( FStrEq(szIcon, "uk" )) {
		short sprUK = PRECACHE_MODEL( "sprites/autoop/uk.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprUK);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//USA Flag
	if ( FStrEq(szIcon, "usa" )) {
		short sprUSA = PRECACHE_MODEL( "sprites/autoop/usa.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprUSA);
			WRITE_SHORT(50);
		MESSAGE_END();
	}

	//Canada Flag
	if ( FStrEq(szIcon, "canada" )) {
		short sprCanada = PRECACHE_MODEL( "sprites/autoop/canada.spr");
		MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pEntity->v.origin);
			WRITE_BYTE(TE_PLAYERATTACHMENT);
			WRITE_BYTE(ENTINDEX(pEntity));
			WRITE_COORD(60);
			WRITE_SHORT(sprCanada);
			WRITE_SHORT(50);
		MESSAGE_END();
	}
}