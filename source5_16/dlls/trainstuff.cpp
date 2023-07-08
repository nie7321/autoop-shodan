/*****************************************************\
*	Auto-OP Shodan
*
*	File: traincommands.cpp
*	Description: Functions for train and track creation.
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
#include "commands.h"
#include "stripper.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[];
extern TFXFeatureEnabled_s Features;

extern int m_Chain;

extern DLL_FUNCTIONS other_gFunctionTable;

//Red Mem
char *g_RedTargets[]={"rtfxt1", "rtfxt2", "rtfxt3", "rtfxt4", "rtfxt5", "rtfxt6", "rtfxt7" , "rtfxt8" , "rtfxt9" , "rtfxt10" , "rtfxt11" , "rtfxt12" , "rtfxt13" , "rtfxt14" , "rtfxt15" , "rtfxt16" , "rtfxt17" , "rtfxt18" , "rtfxt19" , "rtfxt20" , "rtfxt21" , "rtfxt22" , "rtfxt23" , "rtfxt24" , "rtfxt25" , "rtfxt26" , "rtfxt27" , "rtfxt28" , "rtfxt29" , "rtfxt30" , "rtfxt31" , "rtfxt32" , "rtfxt33" , "rtfxt34" , "rtfxt35" , "rtfxt36" , "rtfxt37" , "rtfxt38" , "rtfxt39" , "rtfxt40" , "rtfxt41" , "rtfxt42" , "rtfxt43" , "rtfxt44" , "rtfxt45" , "rtfxt46" , "rtfxt47" , "rtfxt48" , "rtfxt49" , "rtfxt50" , "rtfxt51" , "rtfxt52" , "rtfxt53" , "rtfxt54" , "rtfxt55" , "rtfxt56" , "rtfxt57" , "rtfxt58" , "rtfxt59" , "rtfxt60" , "rtfxt61" , "rtfxt62" , "rtfxt63" , "rtfxt64" , "rtfxt65" , "rtfxt66" , "rtfxt67" , "rtfxt68" , "rtfxt69" , "rtfxt70" , "rtfxt71" , "rtfxt72" , "rtfxt73" , "rtfxt74" , "rtfxt75" , "rtfxt76" , "rtfxt77" , "rtfxt78" , "rtfxt79" , "rtfxt80" , "rtfxt81" , "rtfxt82" , "rtfxt83" , "rtfxt84" , "rtfxt85" , "rtfxt86" , "rtfxt87" , "rtfxt88" , "rtfxt89" , "rtfxt90" , "rtfxt91" , "rtfxt92" , "rtfxt93" , "rtfxt94" , "rtfxt95" , "rtfxt96" , "rtfxt97" , "rtfxt98" , "rtfxt99" , "rtfxt100", "rtfxt101", "rtfxt102", "rtfxt103", "rtfxt104", "rtfxt105", "rtfxt106", "rtfxt107", "rtfxt108", "rtfxt109", 
"rtfxt110" , "rtfxt111" , "rtfxt112" , "rtfxt113" , "rtfxt114" , "rtfxt115" , "rtfxt116" , "rtfxt117" , "rtfxt118" , "rtfxt119" , "rtfxt120" , "rtfxt121" , "rtfxt122" , "rtfxt123" , "rtfxt124" , "rtfxt125" , "rtfxt126" , "rtfxt127" , "rtfxt128" , "rtfxt129" , "rtfxt130" , "rtfxt131" , "rtfxt132" , "rtfxt133" , "rtfxt134" , "rtfxt135" , "rtfxt136" , "rtfxt137" , "rtfxt138" , "rtfxt139" , "rtfxt140" , "rtfxt141" , "rtfxt142" , "rtfxt143" , "rtfxt144" , "rtfxt145" , "rtfxt146" , "rtfxt147" , "rtfxt148" , "rtfxt149" , "rtfxt150" , "rtfxt151" , "rtfxt152" , "rtfxt153" , "rtfxt154" , "rtfxt155" , "rtfxt156" , "rtfxt157" , "rtfxt158" , "rtfxt159" , "rtfxt160" , "rtfxt161" , "rtfxt162" , "rtfxt163" , "rtfxt164" , "rtfxt165" , "rtfxt166" , "rtfxt167" , "rtfxt168" , "rtfxt169" , "rtfxt170" , "rtfxt171" , "rtfxt172" , "rtfxt173" , "rtfxt174" , "rtfxt175" , "rtfxt176" , "rtfxt177" , "rtfxt178" , "rtfxt179" , "rtfxt180" , "rtfxt181" , "rtfxt182" , "rtfxt183" , "rtfxt184" , "rtfxt185" , "rtfxt186" , "rtfxt187" , "rtfxt188" , "rtfxt189" , "rtfxt190" , "rtfxt191" , "rtfxt192" , "rtfxt193" , "rtfxt194" , "rtfxt195" , "rtfxt196" , "rtfxt197" , "rtfxt198" , "rtfxt199", "rtfxt200", "rtfxt201" , "rtfxt202" , "rtfxt203" , "rtfxt204" , "rtfxt205" , "rtfxt206" , "rtfxt207" , "rtfxt208" , "rtfxt209" , "rtfxt210" , "rtfxt211" , "rtfxt2212" , "rtfxt213" , "rtfxt214" , "rtfxt215" , "rtfxt216" , "rtfxt217" , "rtfxt218" , "rtfxt219" , "rtfxt220" , "rtfxt221" , "rtfxt222" , "rtfxt223" , "rtfxt224" , "rtfxt225" , "rtfxt226" , "rtfxt227" , "rtfxt228" , "rtfxt229" , "rtfxt230" , "rtfxt231" , "rtfxt232" , "rtfxt233" , "rtfxt234" , "rtfxt235" , "rtfxt236" , "rtfxt237" , "rtfxt238" , "rtfxt239" , "rtfxt240"  };

//Blue Mem
char *g_BlueTargets[]={"btfxt1", "btfxt2", "btfxt3", "btfxt4", "btfxt5", "btfxt6", "btfxt7" , "btfxt8" , "btfxt9" , "btfxt10" , "btfxt11" , "btfxt12" , "btfxt13" , "btfxt14" , "btfxt15" , "btfxt16" , "btfxt17" , "btfxt18" , "btfxt19" , "btfxt20" , "btfxt21" , "btfxt22" , "btfxt23" , "btfxt24" , "btfxt25" , "btfxt26" , "btfxt27" , "btfxt28" , "btfxt29" , "btfxt30" , "btfxt31" , "btfxt32" , "btfxt33" , "btfxt34" , "btfxt35" , "btfxt36" , "btfxt37" , "btfxt38" , "btfxt39" , "btfxt40" , "btfxt41" , "btfxt42" , "btfxt43" , "btfxt44" , "btfxt45" , "btfxt46" , "btfxt47" , "btfxt48" , "btfxt49" , "btfxt50" , "btfxt51" , "btfxt52" , "btfxt53" , "btfxt54" , "btfxt55" , "btfxt56" , "btfxt57" , "btfxt58" , "btfxt59" , "btfxt60" , "btfxt61" , "btfxt62" , "btfxt63" , "btfxt64" , "btfxt65" , "btfxt66" , "btfxt67" , "btfxt68" , "btfxt69" , "btfxt70" , "btfxt71" , "btfxt72" , "btfxt73" , "btfxt74" , "btfxt75" , "btfxt76" , "btfxt77" , "btfxt78" , "btfxt79" , "btfxt80" , "btfxt81" , "btfxt82" , "btfxt83" , "btfxt84" , "btfxt85" , "btfxt86" , "btfxt87" , "btfxt88" , "btfxt89" , "btfxt90" , "btfxt91" , "btfxt92" , "btfxt93" , "btfxt94" , "btfxt95" , "btfxt96" , "btfxt97" , "btfxt98" , "btfxt99" , "btfxt100", "btfxt101", "btfxt102", "btfxt103", "btfxt104", "btfxt105", "btfxt106", "btfxt107", "btfxt108", "btfxt109", 
"btfxt110" , "btfxt111" , "btfxt112" , "btfxt113" , "btfxt114" , "btfxt115" , "btfxt116" , "btfxt117" , "btfxt118" , "btfxt119" , "btfxt120" , "btfxt121" , "btfxt122" , "btfxt123" , "btfxt124" , "btfxt125" , "btfxt126" , "btfxt127" , "btfxt128" , "btfxt129" , "btfxt130" , "btfxt131" , "btfxt132" , "btfxt133" , "btfxt134" , "btfxt135" , "btfxt136" , "btfxt137" , "btfxt138" , "btfxt139" , "btfxt140" , "btfxt141" , "btfxt142" , "btfxt143" , "btfxt144" , "btfxt145" , "btfxt146" , "btfxt147" , "btfxt148" , "btfxt149" , "btfxt150" , "btfxt151" , "btfxt152" , "btfxt153" , "btfxt154" , "btfxt155" , "btfxt156" , "btfxt157" , "btfxt158" , "btfxt159" , "btfxt160" , "btfxt161" , "btfxt162" , "btfxt163" , "btfxt164" , "btfxt165" , "btfxt166" , "btfxt167" , "btfxt168" , "btfxt169" , "btfxt170" , "btfxt171" , "btfxt172" , "btfxt173" , "btfxt174" , "btfxt175" , "btfxt176" , "btfxt177" , "btfxt178" , "btfxt179" , "btfxt180" , "btfxt181" , "btfxt182" , "btfxt183" , "btfxt184" , "btfxt185" , "btfxt186" , "btfxt187" , "btfxt188" , "btfxt189" , "btfxt190" , "btfxt191" , "btfxt192" , "btfxt193" , "btfxt194" , "btfxt195" , "btfxt196" , "btfxt197" , "btfxt198" , "btfxt199", "btfxt200", "btfxt201" , "btfxt202" , "btfxt203" , "btfxt204" , "btfxt205" , "btfxt206" , "btfxt207" , "btfxt208" , "btfxt209" , "btfxt210" , "btfxt211" , "btfxt2212" , "btfxt213" , "btfxt214" , "btfxt215" , "btfxt216" , "btfxt217" , "btfxt218" , "btfxt219" , "btfxt220" , "btfxt221" , "btfxt222" , "btfxt223" , "btfxt224" , "btfxt225" , "btfxt226" , "btfxt227" , "btfxt228" , "btfxt229" , "btfxt230" , "btfxt231" , "btfxt232" , "btfxt233" , "btfxt234" , "btfxt235" , "btfxt236" , "btfxt237" , "btfxt238" , "btfxt239" , "btfxt240"  };

//Yellow Mem
char *g_YellowTargets[]={"ytfxt1", "ytfxt2", "ytfxt3", "ytfxt4", "ytfxt5", "ytfxt6", "ytfxt7" , "ytfxt8" , "ytfxt9" , "ytfxt10" , "ytfxt11" , "ytfxt12" , "ytfxt13" , "ytfxt14" , "ytfxt15" , "ytfxt16" , "ytfxt17" , "ytfxt18" , "ytfxt19" , "ytfxt20" , "ytfxt21" , "ytfxt22" , "ytfxt23" , "ytfxt24" , "ytfxt25" , "ytfxt26" , "ytfxt27" , "ytfxt28" , "ytfxt29" , "ytfxt30" , "ytfxt31" , "ytfxt32" , "ytfxt33" , "ytfxt34" , "ytfxt35" , "ytfxt36" , "ytfxt37" , "ytfxt38" , "ytfxt39" , "ytfxt40" , "ytfxt41" , "ytfxt42" , "ytfxt43" , "ytfxt44" , "ytfxt45" , "ytfxt46" , "ytfxt47" , "ytfxt48" , "ytfxt49" , "ytfxt50" , "ytfxt51" , "ytfxt52" , "ytfxt53" , "ytfxt54" , "ytfxt55" , "ytfxt56" , "ytfxt57" , "ytfxt58" , "ytfxt59" , "ytfxt60" , "ytfxt61" , "ytfxt62" , "ytfxt63" , "ytfxt64" , "ytfxt65" , "ytfxt66" , "ytfxt67" , "ytfxt68" , "ytfxt69" , "ytfxt70" , "ytfxt71" , "ytfxt72" , "ytfxt73" , "ytfxt74" , "ytfxt75" , "ytfxt76" , "ytfxt77" , "ytfxt78" , "ytfxt79" , "ytfxt80" , "ytfxt81" , "ytfxt82" , "ytfxt83" , "ytfxt84" , "ytfxt85" , "ytfxt86" , "ytfxt87" , "ytfxt88" , "ytfxt89" , "ytfxt90" , "ytfxt91" , "ytfxt92" , "ytfxt93" , "ytfxt94" , "ytfxt95" , "ytfxt96" , "ytfxt97" , "ytfxt98" , "ytfxt99" , "ytfxt100", "ytfxt101", "ytfxt102", "ytfxt103", "ytfxt104", "ytfxt105", "ytfxt106", "ytfxt107", "ytfxt108", "ytfxt109", 
"ytfxt110" , "ytfxt111" , "ytfxt112" , "ytfxt113" , "ytfxt114" , "ytfxt115" , "ytfxt116" , "ytfxt117" , "ytfxt118" , "ytfxt119" , "ytfxt120" , "ytfxt121" , "ytfxt122" , "ytfxt123" , "ytfxt124" , "ytfxt125" , "ytfxt126" , "ytfxt127" , "ytfxt128" , "ytfxt129" , "ytfxt130" , "ytfxt131" , "ytfxt132" , "ytfxt133" , "ytfxt134" , "ytfxt135" , "ytfxt136" , "ytfxt137" , "ytfxt138" , "ytfxt139" , "ytfxt140" , "ytfxt141" , "ytfxt142" , "ytfxt143" , "ytfxt144" , "ytfxt145" , "ytfxt146" , "ytfxt147" , "ytfxt148" , "ytfxt149" , "ytfxt150" , "ytfxt151" , "ytfxt152" , "ytfxt153" , "ytfxt154" , "ytfxt155" , "ytfxt156" , "ytfxt157" , "ytfxt158" , "ytfxt159" , "ytfxt160" , "ytfxt161" , "ytfxt162" , "ytfxt163" , "ytfxt164" , "ytfxt165" , "ytfxt166" , "ytfxt167" , "ytfxt168" , "ytfxt169" , "ytfxt170" , "ytfxt171" , "ytfxt172" , "ytfxt173" , "ytfxt174" , "ytfxt175" , "ytfxt176" , "ytfxt177" , "ytfxt178" , "ytfxt179" , "ytfxt180" , "ytfxt181" , "ytfxt182" , "ytfxt183" , "ytfxt184" , "ytfxt185" , "ytfxt186" , "ytfxt187" , "ytfxt188" , "ytfxt189" , "ytfxt190" , "ytfxt191" , "ytfxt192" , "ytfxt193" , "ytfxt194" , "ytfxt195" , "ytfxt196" , "ytfxt197" , "ytfxt198" , "ytfxt199", "ytfxt200", "ytfxt201" , "ytfxt202" , "ytfxt203" , "ytfxt204" , "ytfxt205" , "ytfxt206" , "ytfxt207" , "ytfxt208" , "ytfxt209" , "ytfxt210" , "ytfxt211" , "ytfxt2212" , "ytfxt213" , "ytfxt214" , "ytfxt215" , "ytfxt216" , "ytfxt217" , "ytfxt218" , "ytfxt219" , "ytfxt220" , "ytfxt221" , "ytfxt222" , "ytfxt223" , "ytfxt224" , "ytfxt225" , "ytfxt226" , "ytfxt227" , "ytfxt228" , "ytfxt229" , "ytfxt230" , "ytfxt231" , "ytfxt232" , "ytfxt233" , "ytfxt234" , "ytfxt235" , "ytfxt236" , "ytfxt237" , "ytfxt238" , "ytfxt239" , "ytfxt240"  };

//Green Mem
char *g_GreenTargets[]={"gtfxt1", "gtfxt2", "gtfxt3", "gtfxt4", "gtfxt5", "gtfxt6", "gtfxt7" , "gtfxt8" , "gtfxt9" , "gtfxt10" , "gtfxt11" , "gtfxt12" , "gtfxt13" , "gtfxt14" , "gtfxt15" , "gtfxt16" , "gtfxt17" , "gtfxt18" , "gtfxt19" , "gtfxt20" , "gtfxt21" , "gtfxt22" , "gtfxt23" , "gtfxt24" , "gtfxt25" , "gtfxt26" , "gtfxt27" , "gtfxt28" , "gtfxt29" , "gtfxt30" , "gtfxt31" , "gtfxt32" , "gtfxt33" , "gtfxt34" , "gtfxt35" , "gtfxt36" , "gtfxt37" , "gtfxt38" , "gtfxt39" , "gtfxt40" , "gtfxt41" , "gtfxt42" , "gtfxt43" , "gtfxt44" , "gtfxt45" , "gtfxt46" , "gtfxt47" , "gtfxt48" , "gtfxt49" , "gtfxt50" , "gtfxt51" , "gtfxt52" , "gtfxt53" , "gtfxt54" , "gtfxt55" , "gtfxt56" , "gtfxt57" , "gtfxt58" , "gtfxt59" , "gtfxt60" , "gtfxt61" , "gtfxt62" , "gtfxt63" , "gtfxt64" , "gtfxt65" , "gtfxt66" , "gtfxt67" , "gtfxt68" , "gtfxt69" , "gtfxt70" , "gtfxt71" , "gtfxt72" , "gtfxt73" , "gtfxt74" , "gtfxt75" , "gtfxt76" , "gtfxt77" , "gtfxt78" , "gtfxt79" , "gtfxt80" , "gtfxt81" , "gtfxt82" , "gtfxt83" , "gtfxt84" , "gtfxt85" , "gtfxt86" , "gtfxt87" , "gtfxt88" , "gtfxt89" , "gtfxt90" , "gtfxt91" , "gtfxt92" , "gtfxt93" , "gtfxt94" , "gtfxt95" , "gtfxt96" , "gtfxt97" , "gtfxt98" , "gtfxt99" , "gtfxt100", "gtfxt101", "gtfxt102", "gtfxt103", "gtfxt104", "gtfxt105", "gtfxt106", "gtfxt107", "gtfxt108", "gtfxt109", 
"gtfxt110" , "gtfxt111" , "gtfxt112" , "gtfxt113" , "gtfxt114" , "gtfxt115" , "gtfxt116" , "gtfxt117" , "gtfxt118" , "gtfxt119" , "gtfxt120" , "gtfxt121" , "gtfxt122" , "gtfxt123" , "gtfxt124" , "gtfxt125" , "gtfxt126" , "gtfxt127" , "gtfxt128" , "gtfxt129" , "gtfxt130" , "gtfxt131" , "gtfxt132" , "gtfxt133" , "gtfxt134" , "gtfxt135" , "gtfxt136" , "gtfxt137" , "gtfxt138" , "gtfxt139" , "gtfxt140" , "gtfxt141" , "gtfxt142" , "gtfxt143" , "gtfxt144" , "gtfxt145" , "gtfxt146" , "gtfxt147" , "gtfxt148" , "gtfxt149" , "gtfxt150" , "gtfxt151" , "gtfxt152" , "gtfxt153" , "gtfxt154" , "gtfxt155" , "gtfxt156" , "gtfxt157" , "gtfxt158" , "gtfxt159" , "gtfxt160" , "gtfxt161" , "gtfxt162" , "gtfxt163" , "gtfxt164" , "gtfxt165" , "gtfxt166" , "gtfxt167" , "gtfxt168" , "gtfxt169" , "gtfxt170" , "gtfxt171" , "gtfxt172" , "gtfxt173" , "gtfxt174" , "gtfxt175" , "gtfxt176" , "gtfxt177" , "gtfxt178" , "gtfxt179" , "gtfxt180" , "gtfxt181" , "gtfxt182" , "gtfxt183" , "gtfxt184" , "gtfxt185" , "gtfxt186" , "gtfxt187" , "gtfxt188" , "gtfxt189" , "gtfxt190" , "gtfxt191" , "gtfxt192" , "gtfxt193" , "gtfxt194" , "gtfxt195" , "gtfxt196" , "gtfxt197" , "gtfxt198" , "gtfxt199", "gtfxt200", "gtfxt201" , "gtfxt202" , "gtfxt203" , "gtfxt204" , "gtfxt205" , "gtfxt206" , "gtfxt207" , "gtfxt208" , "gtfxt209" , "gtfxt210" , "gtfxt211" , "gtfxt2212" , "gtfxt213" , "gtfxt214" , "gtfxt215" , "gtfxt216" , "gtfxt217" , "gtfxt218" , "gtfxt219" , "gtfxt220" , "gtfxt221" , "gtfxt222" , "gtfxt223" , "gtfxt224" , "gtfxt225" , "gtfxt226" , "gtfxt227" , "gtfxt228" , "gtfxt229" , "gtfxt230" , "gtfxt231" , "gtfxt232" , "gtfxt233" , "gtfxt234" , "gtfxt235" , "gtfxt236" , "gtfxt237" , "gtfxt238" , "gtfxt239" , "gtfxt240"  };

//Train TargetNames
char *g_TrainNames[]={"rtfxtrain", "btfxtrain", "ytfxtrain", "gtfxtrain"};

void AdmShowNodes(edict_t *pAdmin)
{
	//Shows all path nodes.
	CBaseEntity *pNode = NULL;
	while ((pNode = UTIL_FindEntityByClassname(pNode, "path_track")) != NULL)
	{
		if (pNode)
		{
			pNode->pev->effects = 0;

			pNode->Activate();

			//TEST CODE
		}
	}
}

void AdmHideNodes(edict_t *pAdmin)
{
	//Hides all path nodes.
	CBaseEntity *pNode = NULL;
	while ((pNode = UTIL_FindEntityByClassname(pNode, "path_track")) != NULL)
	{
		if (pNode)
		{
			pNode->pev->effects = EF_NODRAW;
		}
	}
}

void AdmStartPath(edict_t *pAdmin, const char *Trackname)
{
	Vector vColors;
	int iDraw = 0;

	if(!FStrEq(Trackname,"red") && !FStrEq(Trackname,"blue") && !FStrEq(Trackname,"yellow") && !FStrEq(Trackname,"green"))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "\"%s\" is an invalid trackname.\n",Trackname));
		return;
	}

	if(CheckTrack(Trackname) == 1)
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Track \"%s\" is currently being drawn.\n",Trackname));
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Track \"%s\" is currently being drawn.\n",Trackname));
		return;
	}

	if(TrackExist(Trackname) == 1)
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Track \"%s\" already exists.\n",Trackname));
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Track \"%s\" already exists.\n",Trackname));
		return;
	}

	PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount = 0;
	edict_t *pPath = NULL;
	pPath = CREATE_NAMED_ENTITY(MAKE_STRING("path_track"));
	pPath->v.origin = pAdmin->v.origin;
	//SET_MODEL( ENT(VARS(pPath)), "models/presentlg.mdl" );
	UTIL_SetOrigin(VARS(pPath),pPath->v.origin);

	strcpy(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,Trackname);

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"red"))
	{
		pPath->v.targetname = MAKE_STRING(g_RedTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_RedTargets[1]);
		vColors = Vector(255,0,0);
		iDraw = 1;
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"blue"))
	{
		pPath->v.targetname = MAKE_STRING(g_BlueTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_BlueTargets[1]);
		vColors = Vector(0,100,255);
		iDraw = 2;
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"yellow"))
	{
		pPath->v.targetname = MAKE_STRING(g_YellowTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_YellowTargets[1]);
		vColors = Vector(255,255,0);
		iDraw = 3;
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"green"))
	{
		pPath->v.targetname = MAKE_STRING(g_GreenTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_GreenTargets[1]);
		vColors = Vector(0,255,0);
		iDraw = 4;
	}
	
	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pAdmin->v.origin);
		WRITE_BYTE(TE_BEAMFOLLOW);
		WRITE_SHORT(ENTINDEX(pAdmin));
		WRITE_SHORT(m_Chain);
		WRITE_BYTE(250);
		WRITE_BYTE(5);
		WRITE_BYTE(vColors.x);
		WRITE_BYTE(vColors.y);
		WRITE_BYTE(vColors.z);
		WRITE_BYTE(255);
	MESSAGE_END();

	pPath->v.nextthink = gpGlobals->time + 0.5;

	DispatchSpawn(pPath);
	//CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "TARGETNAME: %s TARGET: %s\n",STRING(pPath->v.targetname),STRING(pPath->v.target)));
	PlayerInfo[ENTINDEX(pAdmin)].PathBuild.Drawingpath = iDraw;

	PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NextDraw = gpGlobals->time + 0.5;

	CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Now laying \"%s\" track.\n",PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName));

}

void DrawPath(edict_t *pAdmin)
{
	PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount += 1;

	if(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount == 239)
	{
		AdmEndPath(pAdmin,0);
		return;
	}

	edict_t *pPath = NULL;
	pPath = CREATE_NAMED_ENTITY(MAKE_STRING("path_track"));
	pPath->v.origin = pAdmin->v.origin;
	//SET_MODEL( ENT(VARS(pPath)), "models/presentlg.mdl" );
	UTIL_SetOrigin(VARS(pPath),pPath->v.origin);

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"red"))
	{
		pPath->v.targetname = MAKE_STRING(g_RedTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_RedTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount + 1]);
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"blue"))
	{
		pPath->v.targetname = MAKE_STRING(g_BlueTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_BlueTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount + 1]);
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"yellow"))
	{
		pPath->v.targetname = MAKE_STRING(g_YellowTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_YellowTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount + 1]);
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"green"))
	{
		pPath->v.targetname = MAKE_STRING(g_GreenTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_GreenTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount + 1]);
	}	
	pPath->v.nextthink = gpGlobals->time + 0.5;

	DispatchSpawn(pPath);
	//CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "TARGETNAME: %s TARGET: %s\n",STRING(pPath->v.targetname),STRING(pPath->v.target)));
	CLIENT_PRINTF( pAdmin, print_center,UTIL_VarArgs( "Node %d out of 240 placed.\n",PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount));

	PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NextDraw = gpGlobals->time + 0.5;

}

void AdmEndPath(edict_t *pAdmin, int UserEnded)
{
	if(UserEnded == 1)
		PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount += 1;

	MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pAdmin->v.origin);
		WRITE_BYTE(TE_KILLBEAM);
		WRITE_SHORT(ENTINDEX(pAdmin));
	MESSAGE_END();

	PlayerInfo[ENTINDEX(pAdmin)].PathBuild.Drawingpath = 0;
	edict_t *pPath = NULL;
	pPath = CREATE_NAMED_ENTITY(MAKE_STRING("path_track"));
	pPath->v.origin = pAdmin->v.origin;
	//SET_MODEL( ENT(VARS(pPath)), "models/presentlg.mdl" );
	UTIL_SetOrigin(VARS(pPath),pPath->v.origin);

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"red"))
	{
		pPath->v.targetname = MAKE_STRING(g_RedTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_RedTargets[0]);
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"blue"))
	{
		pPath->v.targetname = MAKE_STRING(g_BlueTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_BlueTargets[0]);
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"yellow"))
	{
		pPath->v.targetname = MAKE_STRING(g_YellowTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_YellowTargets[0]);
	}

	if(FStrEq(PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName,"green"))
	{
		pPath->v.targetname = MAKE_STRING(g_GreenTargets[PlayerInfo[ENTINDEX(pAdmin)].PathBuild.NodeCount]);
		pPath->v.target = MAKE_STRING(g_GreenTargets[0]);
	}	

	pPath->v.nextthink = gpGlobals->time + 0.5;

	DispatchSpawn(pPath);
	//CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "TARGETNAME: %s TARGET: %s\n",STRING(pPath->v.targetname),STRING(pPath->v.target)));
	CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "You completed laying the track.\n"));
}

void MakePathTrain(edict_t *pAdmin, const char *TrackColor)
{
	if(!FStrEq(TrackColor,"red") && !FStrEq(TrackColor,"blue") && !FStrEq(TrackColor,"yellow") && !FStrEq(TrackColor,"green"))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "\"%s\" is an invalid track color.\n",PlayerInfo[ENTINDEX(pAdmin)].PathBuild.TrackName));
		return;
	}

	edict_t	*pTrain = NULL;
	edict_t *pSelected = NULL;

	if(PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[1] == 0)
	{
		BotTalkOneArgs(pAdmin,"You must select an entity into slot 1 to use to be the train.\n");
		return;
	}

	pSelected = INDEXENT(PlayerInfo[ENTINDEX(pAdmin)].Selected.SelectedEnts[1]);

	//Make the train
	pTrain = CREATE_NAMED_ENTITY(MAKE_STRING("func_train"));

	//Set the trains parameters
	VARS( pTrain )->origin = pTrain->v.origin;

	if(FStrEq(TrackColor,"red"))
	{
		VARS( pTrain )->target = MAKE_STRING(g_RedTargets[0]);
		VARS( pTrain )->targetname = MAKE_STRING(g_TrainNames[0]);
	}

	if(FStrEq(TrackColor,"blue"))
	{
		VARS( pTrain )->target = MAKE_STRING(g_BlueTargets[0]);
		VARS( pTrain )->targetname = MAKE_STRING(g_TrainNames[1]);
	}

	if(FStrEq(TrackColor,"yellow"))
	{
		VARS( pTrain )->target = MAKE_STRING(g_YellowTargets[0]);
		VARS( pTrain )->targetname = MAKE_STRING(g_TrainNames[2]);
	}

	if(FStrEq(TrackColor,"green"))
	{
		VARS( pTrain )->target = MAKE_STRING(g_GreenTargets[0]);
		VARS( pTrain )->targetname = MAKE_STRING(g_TrainNames[3]);
	}

	VARS( pTrain )->speed		= 128; //128 is a nice default
	VARS( pTrain )->movetype	= MOVETYPE_PUSH;
	VARS( pTrain )->solid		= SOLID_BSP;
	SET_MODEL( ENT(VARS(pTrain)), STRING(pSelected->v.model) );
	UTIL_SetOrigin(VARS( pTrain ), pAdmin->v.origin - (VARS( pTrain )->mins + VARS(pTrain)->maxs) * 0.5 );
	VARS( pTrain )->dmg			= 2;
	VARS( pTrain )->nextthink	= 1;


	DispatchSpawn( pTrain ); // call to the engine
}

void DeleteTrack(edict_t *pAdmin, const char *TrackColor)
{
	CBaseEntity *pPath = NULL;

	if(!FStrEq(TrackColor,"red") && !FStrEq(TrackColor,"blue") && !FStrEq(TrackColor,"yellow") && !FStrEq(TrackColor,"green"))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "\"%s\" is an invalid track color.\n",TrackColor));
		return;
	}

	for (int i = 0; i <= 239; i++)
	{
		if(FStrEq(TrackColor,"red"))
		{
			pPath = UTIL_FindEntityByTargetname( NULL, g_RedTargets[i]);
		}

		if(FStrEq(TrackColor,"blue"))
		{
			pPath = UTIL_FindEntityByTargetname( NULL, g_BlueTargets[i]);
		}

		if(FStrEq(TrackColor,"yellow"))
		{
			pPath = UTIL_FindEntityByTargetname( NULL, g_YellowTargets[i]);
		}

		if(FStrEq(TrackColor,"green"))
		{
			pPath = UTIL_FindEntityByTargetname( NULL, g_GreenTargets[i]);
		}

		if (pPath)
		{
			pPath->pev->flags = FL_KILLME;
		}
	}

	CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "Track \"%s\" deleted.\n",TrackColor));
	CLIENT_PRINTF( pAdmin, print_center,UTIL_VarArgs( "Track \"%s\" deleted.\n",TrackColor));
}

void DrawTrackLines(edict_t *pAdmin, const char *TrackColor)
{
	edict_t *pBeam = NULL;
	CBaseEntity *pPath1 = NULL; //Current Node
	CBaseEntity *pPath2 = NULL; //Next Node
	Vector vColors;

	if(!FStrEq(TrackColor,"red") && !FStrEq(TrackColor,"blue") && !FStrEq(TrackColor,"yellow") && !FStrEq(TrackColor,"green"))
	{
		CLIENT_PRINTF( pAdmin, print_console,UTIL_VarArgs( "\"%s\" is an invalid track color.\n",TrackColor));
		return;
	}

	for (int i = 0; i <= 239; i++)
	{

		if(FStrEq(TrackColor,"red"))
		{
			pPath1 = UTIL_FindEntityByTargetname( NULL, g_RedTargets[i]);
			pPath2 = UTIL_FindEntityByTargetname( NULL, g_RedTargets[i+1]);
			vColors = Vector(255,0,0);
		}

		if(FStrEq(TrackColor,"blue"))
		{
			pPath1 = UTIL_FindEntityByTargetname( NULL, g_BlueTargets[i]);
			pPath2 = UTIL_FindEntityByTargetname( NULL, g_BlueTargets[i+1]);
			vColors = Vector(0,100,255);
		}

		if(FStrEq(TrackColor,"yellow"))
		{
			pPath1 = UTIL_FindEntityByTargetname( NULL, g_YellowTargets[i]);
			pPath2 = UTIL_FindEntityByTargetname( NULL, g_YellowTargets[i+1]);
			vColors = Vector(255,255,0);
		}

		if(FStrEq(TrackColor,"green"))
		{
			pPath1 = UTIL_FindEntityByTargetname( NULL, g_GreenTargets[i]);
			pPath2 = UTIL_FindEntityByTargetname( NULL, g_GreenTargets[i+1]);
			vColors = Vector(0,255,0);
		}

		if(i==239)
			pPath2 = NULL; //This will make it default to the first node.

		if(!pPath2) //If path2 doesnt exist, try to join up with first node.
		{
			if(FStrEq(TrackColor,"red"))
			{
				pPath2 = UTIL_FindEntityByTargetname( NULL, g_RedTargets[0]);
			}

			if(FStrEq(TrackColor,"blue"))
			{
				pPath2 = UTIL_FindEntityByTargetname( NULL, g_BlueTargets[0]);
			}

			if(FStrEq(TrackColor,"yellow"))
			{
				pPath2 = UTIL_FindEntityByTargetname( NULL, g_YellowTargets[0]);
			}

			if(FStrEq(TrackColor,"green"))
			{
				pPath2 = UTIL_FindEntityByTargetname( NULL, g_GreenTargets[0]);
			}
		}

		if (pPath1 && pPath2)
		{
			MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pAdmin->v.origin);
				WRITE_BYTE(TE_BEAMPOINTS);
				WRITE_COORD(pPath1->pev->origin.x);
				WRITE_COORD(pPath1->pev->origin.y);
				WRITE_COORD(pPath1->pev->origin.z);
				WRITE_COORD(pPath2->pev->origin.x);
				WRITE_COORD(pPath2->pev->origin.y);
				WRITE_COORD(pPath2->pev->origin.z);
				WRITE_SHORT(m_Chain); //Sprite
				WRITE_BYTE(0); //starting frame
				WRITE_BYTE(0); //frame rate
				WRITE_BYTE(400); //life
				WRITE_BYTE(15); //width
				WRITE_BYTE(0); //noise
				
				WRITE_BYTE(vColors.x);
				WRITE_BYTE(vColors.y);
				WRITE_BYTE(vColors.z);
				
				WRITE_BYTE(255); //brightness
				WRITE_BYTE(0); //scroll speed
			MESSAGE_END();
		}
	}
}

int CheckTrack(const char *TrackColor)
{
	int iDraw = 0;

	if(FStrEq(TrackColor,"red"))
	{
		iDraw = 1;
	}

	if(FStrEq(TrackColor,"blue"))
	{
		iDraw = 2;
	}

	if(FStrEq(TrackColor,"yellow"))
	{
		iDraw = 3;
	}

	if(FStrEq(TrackColor,"green"))
	{
		iDraw = 4;
	}

	for (int i = 1; i <= gpGlobals->maxClients; i++)
	{
		if(PlayerInfo[i].PathBuild.Drawingpath == iDraw)
			return 1;
	}

	return 0;
}

int TrackExist(const char *TrackColor)
{
	CBaseEntity *pPath1 = NULL;

	if(FStrEq(TrackColor,"red"))
	{
		pPath1 = UTIL_FindEntityByTargetname( NULL, g_RedTargets[0]);
	}

	if(FStrEq(TrackColor,"blue"))
	{
		pPath1 = UTIL_FindEntityByTargetname( NULL, g_BlueTargets[0]);
	}

	if(FStrEq(TrackColor,"yellow"))
	{
		pPath1 = UTIL_FindEntityByTargetname( NULL, g_YellowTargets[0]);
	}

	if(FStrEq(TrackColor,"green"))
	{
		pPath1 = UTIL_FindEntityByTargetname( NULL, g_GreenTargets[0]);
	}

	if(pPath1)
	{
		return 1;
	} else {
		return 0;
	}
}