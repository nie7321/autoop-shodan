/*****************************************************\
*	Auto-OP Shodan
*
*	File: ranks.cpp
*	Description: Rank functions.
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
#include "credits.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;

TFXPlayerRank_t g_Ranks[] = {
	{0,			"N00bie"},
	{40,		"Greenhorn"},
	{70,		"Warrior"},
	{100,		"Beast"},
	{150,		"Fragmeister"},
	{350,		"Elite Fragger"},
	{450,		"Lord Of Death"},
	{600,		"Excellence"},
	{800,		"Prez Bodyguard"},
	{1000,		"HW Slayer"},
	{2000,		"Smooth Operator"},
	{2500,		"Scout Raper"},
	{3000,		"Cleaner"},
	{4000,		"Llama Eliminator"},
	{5000,		"Stealth Shadow"},
	{6000,		"Balls Of Steel"},
	{9000,		"Extreme Power"},
	{15000,		"Credit Junkie"},
	{1000000,	"Immortal"},
	{-1,		""}
};

//Returns the rank the player is at.
int GetRank(long int iCreditValue)
{
	int i = 0;
	int iReturnValue = 0;

	while(g_Ranks[i].Requirement != -1) {

		if(iCreditValue >= g_Ranks[i].Requirement)
			iReturnValue = MAKE_STRING(g_Ranks[i].RankName);
		i++;
	}

	if(iReturnValue == 0)
		iReturnValue = MAKE_STRING("Llama");

	return iReturnValue;
}

//Called when they get a credit, if true then user has
//just moved up a rank.
bool JustChangedRank(long int iCreditValue)
{
	int i = 0;

	while(g_Ranks[i].Requirement != -1) {
		if(iCreditValue == g_Ranks[i].Requirement)
			return TRUE;
		i++;
	}

	return FALSE;
}

//Returns now many credits are needed before player reaches
//the next rank
int NextRankAmount(long int iCreditValue)
{
	int i = 0;

	int CurrentRank = 0;
	int NextRank = 0;

	if(iCreditValue >= 0)
	{
		while(g_Ranks[i].Requirement != -1) {

			if(iCreditValue >= g_Ranks[i].Requirement)
				CurrentRank = i;
			i++;
		}

		if(g_Ranks[CurrentRank + 1].Requirement == -1)
			return -1; //-1 = We are at the top rank.
		else
			NextRank = CurrentRank + 1;
	} else {
		NextRank = 0;
	}

	return (g_Ranks[NextRank].Requirement - iCreditValue);
}