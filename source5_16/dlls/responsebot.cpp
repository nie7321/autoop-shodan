/*****************************************************\
*	Auto-OP Shodan
*
*	File: responsebot.cpp
*	Description: Responsebot interface and sayings.
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
extern DLL_FUNCTIONS other_gFunctionTable;

void BotTalkAll(char *Speech)
{
	UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("[%s]: %s\n",BOT_NAME,Speech));
	UTIL_LogPrintf("[AUTOOP] bot_say \"[%s]: %s\"\n",BOT_NAME,Speech);

}

void BotTalkOneArgs( edict_t *pEntity, char *format, ... )
{
	va_list         argptr;
	static char             string[1024];

	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	if(pEntity == NULL)
		return;

	if(strlen(string) > 0)
	{
		ClientPrint( &pEntity->v, HUD_PRINTTALK,UTIL_VarArgs("[%s]: %s\n",BOT_NAME,string));
		UTIL_LogPrintf("[AUTOOP] bot_say_one \"%s\" \"[%s]: %s\"\n", STRING(pEntity->v.netname), BOT_NAME,string); 
	}
}

void BotTalkAllArgs( char *format, ... )
{
	va_list         argptr;
	static char             string[1024];

	va_start (argptr, format);
	vsprintf (string, format,argptr);
	va_end (argptr);

	if(strlen(string) > 0)
	{
		UTIL_ClientPrintAll( HUD_PRINTTALK, UTIL_VarArgs("[%s]: %s\n",BOT_NAME,string));
		UTIL_LogPrintf("[AUTOOP] bot_say \"[%s]: %s\"\n",BOT_NAME,string); 
	}
}

void BotTalkOne(edict_t *pEntity, char *Speech)
{
	ClientPrint( &pEntity->v, HUD_PRINTTALK, UTIL_VarArgs( "[%s]: %s\n",BOT_NAME,Speech ) );
	UTIL_LogPrintf("[AUTOOP] bot_say_one [%s]: %s\n",BOT_NAME,Speech);
}

void BotCom(edict_t *pEntity, const char *FullSpeech, int iName)
{
	//Firstly rip the bots name from the speech
	char Speech[MAX_TEXT_LENGTH];
	char Response[MAX_TEXT_LENGTH];
	int Length = strlen(FullSpeech);

	int NameLength = 0;

	if(iName == 1)
	{
		NameLength = strlen(BOT_NAME) + 1;
	}

	if (iName == 2)
	{
		NameLength = strlen(BOT_NAME2) + 1;
	}

	int Rand = 0.0;

	//Remove the BOT_NAME+1 from the beginning of the string.
	for( int i=NameLength; i<Length+1; i++)
		Speech[i-NameLength] = FullSpeech[i];
	Speech[i-NameLength] = NULL;

	//At this point, the users remark to the responsebot is in char Speech.
	if(_stricmp( Speech, "hi" ) == 0 || _stricmp( Speech, "hey" ) == 0 || _stricmp( Speech, "hello" ) == 0 || _stricmp( Speech, "yo" ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Hi there ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,"!");
			break;

		case 2:
			strcpy(Response,"Heya.");
			break;

		case 3:
			strcpy(Response,"Hey there, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 4:
			strcpy(Response,"Hello there.");
			break;

		default:
			strcpy(Response,"Hello.");
			break;

		}
		CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "BOTRAND: %d\n",Rand));
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "suck", 4 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Um no thinks, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"Suck what?!?");
			break;

		case 3:
			strcpy(Response,"Whatever turns you on, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 4:
			strcpy(Response,"I think not.");
			break;

		default:
			strcpy(Response,"No thanks.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "thats", 5 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"I agree with you, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"Yeah, I agree.");
			break;

		case 3:
			strcpy(Response,"I don't think so, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 4:
			strcpy(Response,"I think not.");
			break;

		default:
			strcpy(Response,"I'm not sure.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "i am", 4 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"I already knew, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"Yeah, I agree.");
			break;

		case 3:
			strcpy(Response,"I don't you are, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 4:
			strcpy(Response,"I think not.");
			break;

		default:
			strcpy(Response,"Maybe you are.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "i want", 6 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"I want never gets, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"Ok, later though.");
			break;

		case 3:
			strcpy(Response,"Dream on, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 4:
			strcpy(Response,"Not from me.");
			break;

		default:
			strcpy(Response,"erm.. Ask someone else.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "give me", 7 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Sorry, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response," I won't give you that");
			break;

		case 2:
			strcpy(Response,"Then give me money.");
			break;

		case 3:
			strcpy(Response,"I dont want to seperate with that.");
			break;

		case 4:
			strcpy(Response,"I don't think so.");
			break;

		default:
			strcpy(Response,"No.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "do you want", 6 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No I dont.");
			break;

		case 2:
			strcpy(Response,"No thankyou.");
			break;

		case 3:
			strcpy(Response,"Yes I do.");
			break;

		case 4:
			strcpy(Response,"Nope.");
			break;

		default:
			strcpy(Response,"Yep.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "do you like", 6 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No I dont.");
			break;

		case 2:
			strcpy(Response,"Thats ok.");
			break;

		case 3:
			strcpy(Response,"Yes I do.");
			break;

		case 4:
			strcpy(Response,"Nope.");
			break;

		default:
			strcpy(Response,"Yep.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "do you have", 6 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No I dont.");
			break;

		case 2:
			strcpy(Response,"Yes I do, are you interested in it?");
			break;

		case 3:
			strcpy(Response,"Yes I do.");
			break;

		case 4:
			strcpy(Response,"Nope.");
			break;

		default:
			strcpy(Response,"Yep.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "do you", 6 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No I dont.");
			break;

		case 2:
			strcpy(Response,"Yes I do, are you interested in it?");
			break;

		case 3:
			strcpy(Response,"Yes I do.");
			break;

		case 4:
			strcpy(Response,"Nope.");
			break;

		default:
			strcpy(Response,"Yep.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "can i", 5 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No you can't.");
			break;

		case 2:
			strcpy(Response,"Maybe you can.");
			break;

		case 3:
			strcpy(Response,"Yes ok, later.");
			break;

		case 4:
			strcpy(Response,"Nope.");
			break;

		default:
			strcpy(Response,"Yep.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "marry me", 8 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Ok ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response," wheres my ring?");
			break;

		case 2:
			strcpy(Response,"Sorry, im not into commitment.");
			break;

		case 3:
			strcpy(Response,"Sorry but you're not good enough for me.");
			break;

		case 4:
			strcpy(Response,"Of course, wheres the church?");
			break;

		default:
			strcpy(Response,"Hmm, let me think about that one.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "eat", 3 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 4 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No thanks ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", I'm not hungry at the moment.");
			break;

		case 2:
			strcpy(Response,"Got pizza?");
			break;

		case 3:
			strcpy(Response,"mmm.. Im starving, wheres the food?");
			break;

		case 4:
			strcpy(Response,"No thanks, im on a diet.");
			break;

		default:
			strcpy(Response,"What flavour is that?");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "kick me", 7 ) == 0 || _strnicmp( Speech, "boot me", 7 ) == 0)
	{
		strcpy(Response,"lol! Ok then, ");
		strcat(Response,STRING(pEntity->v.netname));
		strcat(Response,".");

		char KickCommand[256];
		_snprintf(KickCommand,256,"kick # %i\n", GETPLAYERUSERID( pEntity ));
		SERVER_COMMAND(KickCommand);

		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "make me civ", 11 ) == 0 || _strnicmp( Speech, "make me a civ", 13 ) == 0)
	{
		if(UTIL_SpectCheck(pEntity) == 0)
		{
			strcpy(Response,"Ok ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", you're a civilian.");

			pEntity->v.playerclass = CLASS_CIVILIAN;

			CLIENT_COMMAND(pEntity, UTIL_VarArgs("kill\n"));

			BotTalkAll(Response);
			return;
		}
	}

	if(_strnicmp( Speech, "help", 4 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"I can't help you ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", I'm not a qualified doctor..");
			break;

		case 2:
			strcpy(Response,"Try saying !help to get help.");
			break;

		default:
			strcpy(Response,"Try saying !help to get help.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	//3rdperson command removed
	/*if(_strnicmp( Speech, "3rdperson", 9 ) == 0 || _strnicmp( Speech, "3rd person", 10 ) == 0 || _strnicmp( Speech, "thirdperson", 11 ) == 0 || _strnicmp( Speech, "third person", 12 ) == 0)
	{
		strcpy(Response,"Ok ");
		strcat(Response,STRING(pEntity->v.netname));
		strcat(Response,", done.");

		CLIENT_COMMAND(pEntity, UTIL_VarArgs("cam_command 1\n"));
		CLIENT_COMMAND(pEntity, UTIL_VarArgs("cam_idealyaw 0\n"));
		CLIENT_COMMAND(pEntity, UTIL_VarArgs("cam_idealdist 100\n"));

		CLIENT_PRINTF(pEntity, print_console, "3RD PERSON MODE ACTIVATED, NOW RECONNECTING...\n");
		CLIENT_PRINTF(pEntity, print_console, "3RD PERSON MODE ACTIVATED, NOW RECONNECTING...\n");
		CLIENT_PRINTF(pEntity, print_console, "3RD PERSON MODE ACTIVATED, NOW RECONNECTING...\n");

		CLIENT_COMMAND(pEntity, UTIL_VarArgs("retry\n"));
		
		BotTalkOne(pEntity,Response);
		return;
	}

	if(_strnicmp( Speech, "1stperson", 9 ) == 0 || _strnicmp( Speech, "1st person", 10 ) == 0 || _strnicmp( Speech, "firstperson", 11 ) == 0 || _strnicmp( Speech, "first person", 12 ) == 0)
	{
		strcpy(Response,"Ok ");
		strcat(Response,STRING(pEntity->v.netname));
		strcat(Response,", done.");

		CLIENT_COMMAND(pEntity, UTIL_VarArgs("firstperson\n"));
		
		BotTalkOne(pEntity,Response);
		return;
	}*/

	if(_strnicmp( Speech, "nigger", 6 ) == 0)
	{
		strcpy(Response,"Thats a bad word, ");
		strcat(Response,STRING(pEntity->v.netname));
		strcat(Response,". I shut you up.");

		PlayerInfo[ENTINDEX(pEntity)].LoadedInfo.CanTalk = 1;
	}

	if(_strnicmp( Speech, "thanks", 6 ) == 0 || _strnicmp( Speech, "thankyou", 8 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Anytime, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"No prob.");
			break;

		default:
			strcpy(Response,"Yea yea I know im the best.");
			break;
		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "how are you", 11 ) == 0 || _strnicmp( Speech, "you ok", 6 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Fine, thanks ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,"!");
			break;

		case 2:
			strcpy(Response,"I'm good.");
			break;

		default:
			strcpy(Response,"No problem here.");
			break;
		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "were you", 8 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"No I wasn't.");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"Definatly not!");
			break;

		default:
			strcpy(Response,"Yeah.");
			break;
		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "are you", 7 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Yes of course ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", why? aren't you?");
			break;

		case 2:
			strcpy(Response,"Definatly not!");
			break;

		default:
			strcpy(Response,"Yep.");
			break;
		}
		BotTalkAll(Response);
		return;
	}
	
	if ( stristr(Speech, "rob") != 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Rob 0wnz my ass ");
			break;

		case 2:
			strcpy(Response,"What about him?");
			break;

		default:
			strcpy(Response,"I know that guy.");
			break;
		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "you are", 7 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 2 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Um.. yeah, sure ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,".");
			break;

		case 2:
			strcpy(Response,"I think not.");
			break;

		default:
			strcpy(Response,"Takes one to know one.");
			break;
		}
		BotTalkAll(Response);
		return;
	}

	// added by some non-coding-nut
	if ( stristr(Speech, "tea") != 0)
	{
		Rand = (int)RANDOM_LONG( 0, 3 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Oh, how unfriendly of me! ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", want a cup, too?");
			break;

		case 2:
			strcpy(Response,"No thanx, I just had a whole teapot ");
			break;

		case 3:
			strcpy(Response,"YEAH! Lets get drunk! ");
			break;

		default:
			strcpy(Response,"I have mine with milk, sugar and jack daniels.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if ( stristr(Speech, "beer") != 0)
	{
		Rand = (int)RANDOM_LONG( 0, 3 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Hey ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response," want to drink one with me?");

		case 2:
			strcpy(Response,"They have huge beer-mugs in germany! ");
			break;

		case 3:
			strcpy(Response,"YEAH! Lets get drunk! ");
			break;

		default:
			strcpy(Response,"If you go to germany, try a beer called Russ. It kicks ass!");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "its me scrappy", 14 ) == 0)
	{
		strcpy(Response,"Oh no, not that mouthy guy from (Lamur)");

		BotTalkAll(Response);
		return;
	}

	if(_strnicmp( Speech, "fuck", 4 ) == 0)
	{
		Rand = (int)RANDOM_LONG( 0, 3 );
		switch (Rand) {
		case 1:
			strcpy(Response,"Oh, come on ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", go fuck a tree, alright?!");

		case 2:
			strcpy(Response,"Better not, your rubber-doll would be jaelous ");
			break;

		case 3:
			strcpy(Response,"I can see ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response," sticking his noodle in his CD-Rom drive :) ");
			break;

		default:
			strcpy(Response,"Watch out, jerk! Your keyboard is getting wet.");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if ( stristr(Speech, "english") != 0)
	{
		Rand = (int)RANDOM_LONG( 0, 3 );
		switch (Rand) {
		case 1:
			strcpy(Response,"well, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", I heard they make crumpets out of people like you over there...");
			break;

		case 2:
			strcpy(Response,"I heard they ate crumpets and drank tea all day ");
			break;

		case 3:
			strcpy(Response,"You see, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", they might drive on the wrong side, but they smoke grass like you and me");
			break;

		default:
			strcpy(Response,"The guys that wear slippers and talk with their chaps about politics while smoking pipe?");
			break;

		}
		BotTalkAll(Response);
		return;
	}

	if ( stristr(Speech, "germans") != 0)
	{
		Rand = (int)RANDOM_LONG( 0, 3 );
		switch (Rand) {
		case 1:
			strcpy(Response,"well, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", I heard they make lederhosen out of the skins of people like you");
			break;

		case 2:
			strcpy(Response,"I heard they have those green, furry hats. Funny things! ");
			break;

		case 3:
			strcpy(Response,"You see, ");
			strcat(Response,STRING(pEntity->v.netname));
			strcat(Response,", they might drive a bit fast on their autobahns, but they take ecstacy like you and me");
			break;

		default:
			strcpy(Response,"The fat guys that drink insane amounts of beer out of huge beer-mugs?");
			break;

		}
		BotTalkAll(Response);
		return;
	}
}



