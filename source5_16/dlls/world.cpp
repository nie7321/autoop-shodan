/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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

//===== world.cpp ========================================================


#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "shake.h"
#include "autoop.h"
#include "decals.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

//
// This must match the list in util.h
//
DLL_DECALLIST gDecals[] = {
	{ "{shot1",	0 },		// DECAL_GUNSHOT1 
	{ "{shot2",	0 },		// DECAL_GUNSHOT2
	{ "{shot3",0 },			// DECAL_GUNSHOT3
	{ "{shot4",	0 },		// DECAL_GUNSHOT4
	{ "{shot5",	0 },		// DECAL_GUNSHOT5
	{ "{lambda01", 0 },		// DECAL_LAMBDA1
	{ "{lambda02", 0 },		// DECAL_LAMBDA2
	{ "{lambda03", 0 },		// DECAL_LAMBDA3
	{ "{lambda04", 0 },		// DECAL_LAMBDA4
	{ "{lambda05", 0 },		// DECAL_LAMBDA5
	{ "{lambda06", 0 },		// DECAL_LAMBDA6
	{ "{scorch1", 0 },		// DECAL_SCORCH1
	{ "{scorch2", 0 },		// DECAL_SCORCH2
	{ "{blood1", 0 },		// DECAL_BLOOD1
	{ "{blood2", 0 },		// DECAL_BLOOD2
	{ "{blood3", 0 },		// DECAL_BLOOD3
	{ "{blood4", 0 },		// DECAL_BLOOD4
	{ "{blood5", 0 },		// DECAL_BLOOD5
	{ "{blood6", 0 },		// DECAL_BLOOD6
	{ "{yblood1", 0 },		// DECAL_YBLOOD1
	{ "{yblood2", 0 },		// DECAL_YBLOOD2
	{ "{yblood3", 0 },		// DECAL_YBLOOD3
	{ "{yblood4", 0 },		// DECAL_YBLOOD4
	{ "{yblood5", 0 },		// DECAL_YBLOOD5
	{ "{yblood6", 0 },		// DECAL_YBLOOD6
	{ "{break1", 0 },		// DECAL_GLASSBREAK1
	{ "{break2", 0 },		// DECAL_GLASSBREAK2
	{ "{break3", 0 },		// DECAL_GLASSBREAK3
	{ "{bigshot1", 0 },		// DECAL_BIGSHOT1
	{ "{bigshot2", 0 },		// DECAL_BIGSHOT2
	{ "{bigshot3", 0 },		// DECAL_BIGSHOT3
	{ "{bigshot4", 0 },		// DECAL_BIGSHOT4
	{ "{bigshot5", 0 },		// DECAL_BIGSHOT5
	{ "{spit1", 0 },		// DECAL_SPIT1
	{ "{spit2", 0 },		// DECAL_SPIT2
	{ "{bproof1", 0 },		// DECAL_BPROOF1
	{ "{gargstomp", 0 },	// DECAL_GARGSTOMP1,	// Gargantua stomp crack
	{ "{smscorch1", 0 },	// DECAL_SMALLSCORCH1,	// Small scorch mark
	{ "{smscorch2", 0 },	// DECAL_SMALLSCORCH2,	// Small scorch mark
	{ "{smscorch3", 0 },	// DECAL_SMALLSCORCH3,	// Small scorch mark
	{ "{mommablob", 0 },	// DECAL_MOMMABIRTH		// BM Birth spray
	{ "{mommablob", 0 },	// DECAL_MOMMASPLAT		// BM Mortar spray?? need decal
};

void DecalInit()
{
	for ( int i = 0; i < ARRAYSIZE(gDecals); i++ )
		gDecals[i].index = DECAL_INDEX( gDecals[i].name );
}
