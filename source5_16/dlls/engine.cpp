/*****************************************************\
*	Auto-OP Shodan
*
*	File: engine.cpp
*	Description: Message Hooking.
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*   Based on work by Botman (http://www.planethalflife.com/botman)
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
#include "util.h"

#include "engine.h"
#include "autoop.h"

extern enginefuncs_t g_engfuncs;
extern int mod_id;
extern FILE *fp;
extern TFXFeatureEnabled_s Features;

extern edict_t *pBuilding;

int message_CurWeapon = 0;
int message_DeathMsg = 0;
int message_StatusText = 0;
int message_StatusValue = 0;

void (*AoMsgFunction)(void *, int) = NULL;
void (*AoMsgEndFunction)(void *, int) = NULL;
int AoMsgIndex;
int AOMsgType;

int debug_engine = 0;

extern AOPlayerInfo_s PlayerInfo[];

int pfnRegUserMsg(const char *pszName, int iSize)
{
	int msg;

	msg = (*g_engfuncs.pfnRegUserMsg)(pszName, iSize);

	/*if (strcmp(pszName, "CurWeapon") == 0)
		message_CurWeapon = msg;*/

	if (strcmp(pszName, "DeathMsg") == 0)
		message_DeathMsg = msg;

	if (strcmp(pszName, "StatusText") == 0)
		message_StatusText = msg;

	if (strcmp(pszName, "StatusValue") == 0)
		message_StatusValue = msg;
	
	if(debug_engine) {fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnRegUserMsg: pszName=%s msg=%d\n",pszName,msg); fclose(fp);}
	
	return msg;
}

void pfnMessageBegin(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed)
{
	AOMsgType = msg_type;

	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnMessageBegin: edict=%x dest=%d type=%d\n",ed,msg_dest,msg_type); fclose(fp); }

	if(ed)
	{
		if(!FStrEq(STRING(ed->v.netname),""))
		{
			AoMsgFunction = NULL;     // no msg function until known otherwise
			AoMsgEndFunction = NULL;  // no msg end function until known otherwise
			AoMsgIndex = ENTINDEX(ed);      
			
			//if (msg_type == message_CurWeapon)
				//AoMsgFunction = MsgCurrentWeapon;

			if(msg_type == message_StatusText)
				AoMsgFunction = StatusText;
		}
	}
	else if (msg_dest == MSG_ALL)
	{
		AoMsgFunction = NULL;  // no msg function until known otherwise
		AoMsgIndex = -1;       // index of player receiving message (none)

		if (msg_type == message_DeathMsg)
		{
			AoMsgFunction = MsgDeath;
			if(Features.Debug >= 2)
			{
				fp=fopen("sdebug.log","a");
				fprintf(fp, "pfnMessageBegin(): Hooked Message.\n");
				fclose(fp);
			}
		}
	}

	if(AOMsgType > 116)
	{
		if(Features.Debug >= 0) {fp=fopen("sdebug.log","a"); fprintf(fp,"INVALID USER MESSAGE DETECTED FROM GAME DLL! %d\n",AOMsgType); fclose(fp);}
		return;
	}

	(*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ed);
}
void pfnMessageEnd(void)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnMessageEnd:\n"); fclose(fp); }

	if (AoMsgEndFunction)
		(*AoMsgEndFunction)(NULL, AoMsgIndex);  // NULL indicated msg end

	// clear out the bot message function pointers...
	AoMsgFunction = NULL;
	AoMsgEndFunction = NULL;

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnMessageEnd)();
}
void pfnWriteByte(int iValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteByte: %d\n",iValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&iValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteByte)(iValue);
}
void pfnWriteChar(int iValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteChar: %d\n",iValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&iValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteChar)(iValue);
}
void pfnWriteShort(int iValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteShort: %d\n",iValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&iValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteShort)(iValue);
}
void pfnWriteLong(int iValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteLong: %d\n",iValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&iValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteLong)(iValue);
}
void pfnWriteAngle(float flValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteAngle: %f\n",flValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&flValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteAngle)(flValue);
}
void pfnWriteCoord(float flValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteCoord: %f\n",flValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&flValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteCoord)(flValue);
}
void pfnWriteString(const char *sz)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteString: %s\n",sz); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)sz, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteString)(sz);
}
void pfnWriteEntity(int iValue)
{
	if (debug_engine) { fp=fopen("clmsgs.txt","a"); fprintf(fp,"pfnWriteEntity: %d\n",iValue); fclose(fp); }

	if (AoMsgFunction)
		(*AoMsgFunction)((void *)&iValue, AoMsgIndex);

	if(AOMsgType > 115)
		return;

   (*g_engfuncs.pfnWriteEntity)(iValue);
}

void MsgDeath(void *p, int bot_index)
{
	static int state = 0;   // current state machine state
	static int killer_index;
	static int victim_index;
	static edict_t *victim_edict;
	static edict_t *killer_edict;
	static int index;

	if (state == 0)
	{
		state++;
		killer_index = *(int *)p;  // ENTINDEX() of killer
	}
	else if (state == 1)
	{
		state++;
		victim_index = *(int *)p;  // ENTINDEX() of victim
	}
	else if (state == 2)
	{
		state = 0;

		victim_edict = INDEXENT(victim_index);
		PlayerInfo[victim_index].Deaths++;

		// is this message about a player being killed?

		if ((killer_index == 0) || (killer_index == victim_index))
		{
			// killed by world (worldspawn) or killed self...
			killer_edict = NULL;
		}
		else
		{
			// store edict of player that killed this person...
			killer_edict = INDEXENT(killer_index);
		}
   }
}

void StatusText(void *p, int bot_index)
{
	static int state = 0;   // current state machine state
	static int StatusType = 0;

	if (p == NULL)  // handle pfnMessageEnd case
	{
		state = 0;
		return;
	}

	if(state == 0)
	{
		StatusType = *(int *)p;
		state ++;
		return;
	}

	if(state == 1)
	{
		edict_t *pSend = NULL;

		pSend = INDEXENT(bot_index);

		if(pSend != NULL)
		{
			if(stristr((char *)p, "#Engineer_building") == 0)
				pBuilding = pSend;
			//Ignore messages with a byte of 0 or respawn timer msgs.
				if(StatusType != 0)
					if(stristr((char *)p, "#Sbar_dead") == 0)
						strcpy(PlayerInfo[bot_index].LastStatusText,(char *)p);
		}
		state = 0;
	}
}