/*****************************************************\
*	AutoOP Shodan
*
*	File: helpsystem.cpp
*	Description: In-Game Help Functions for Shodan
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

//This function responds to what people have said and sends network mesages of help to them.

void ShowHelp(edict_t *pEntity, const char *szCommand)
{
	int gmsgMOTDText = REG_USER_MSG( "MOTD", -1);

	if(gmsgMOTDText > MAX_USERMSG)
		return;

	CBaseEntity *pHelpPlayer = UTIL_PlayerByIndex(ENTINDEX(pEntity));

	if ( FStrEq(szCommand, "!help" )) { //Generic help message

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.1 - Help contents\n--------------------------\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Say the following in game for help on that topic.\n\n" );
		MESSAGE_END();
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help credits - info on the credits system and how to give them.\n");
		MESSAGE_END();
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help buying - info on the buying system\n!help rank - Whats are ranks?\n" );
		MESSAGE_END();
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help commands - other commands available\n!help contact - need to contact us?\n" );
		MESSAGE_END();
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help flashbang - Help with scouts new toy\n" );
		MESSAGE_END();
	
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help speech - Speech commands\n!help skins - complete skin listings.\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help icons - Information on the icons system\n!help sensors - help on sensor array\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help superflame - help with the pyros superflame\n!help tripmine - Help on soldiers tripmine.\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help c4 - help with the medics C4\n!help mediflag - Help with medics mediflag.\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help snarks - help with the snipers snarks.\n!help grapple - how to use the hook\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help prox - help with the proximity mine\n!help airstirke - Airstrike info.\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help sentry - help with the manual sentry gun.\n!help energygun - How to use the energy gun.\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "!help blaster - help with demomans blaster.\n!help private - How to use private messaging." );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\n!help 3rdperson - How to activate 3rdperson mode.\n!help sgradar - HWGuy's Sentry Radar" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\n!help stealth - Spies stealth camo.\n!help messages - How to leave message flags." );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\n!help packbomb - Demomans packbomb." );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\n\nFOR MORE DETAILS SEE www.girlpower2.dhs.org!" );
		MESSAGE_END();
		return;
	}

	if ( FStrEq(szCommand, "!help credits" )) { //Help on credits

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Credits Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "As you play on the server you gain credits. These credits can be used to purchase items. " );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "When you leave the server you credits are retained\nSay '!help buying' for more information on purchasing items.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "use the command givecredits <player> <amount> to give credits to people.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "You get 1 credit for every 10 frags!\n" );
		MESSAGE_END();	

		return;
	}

	if ( FStrEq(szCommand, "!help buying" ) || FStrEq(szCommand, "!help buy" )) { //Help on other items to purchase

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Item buying Help\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "All item buying is done via a menu system\n\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Type 'buy' in the console to bring up the buy menu.\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Prices are displayed next to the item on the menu.\n" );
		MESSAGE_END();		
		return;
	}

	if ( FStrEq(szCommand, "!help rank" )) { //Help on rankage

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Rank List\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Ranks show your progress you have made while playing on GP2.\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "To see how many credits a player must earn before the next rank say \"rank <playername>\"\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Below is a list of ranks in the order which you attain them.\n\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "N00bie\nGreenhorn\n1337 Warrior\nBeast\nFragmeister\nElite Fragger\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Lord Of Death\nExcellence\nPrez Bodyguard\nHW Slayer\nSmooth Operator\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Scout raper\nCleaner\nLlama Eliminator\nShadow\nBalls Of Steel\n..." );
		MESSAGE_END();		
		return;
	}

	if ( FStrEq(szCommand, "!help commands" )) { //Help on other commands

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Information on other commands.\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Use these commands in the server to use them.\n\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "jess make me civilian - makes you play as the civilian class\nrockthevote - starts a map vote\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "(IN CONSOLE) applyskin <code> - skins you (see !help skins) x\ncheck <player> - checks of player is a server op or not\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "time <player> - Tells you how long player has been in the server.\nvotestat - display winning map in a map vote.\n" );
		MESSAGE_END();		

		return;;
	}

	if ( FStrEq(szCommand, "!help contact" )) { //Contact listings

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Server OP Contacts.\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Here is a contact list for main server operators.\n\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Rob - Mail: rob@planetfortress.com - ICQ: 85668646\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "KnightHawk - Mail: knighthawktfc@hotmail.com\n" );
		MESSAGE_END();		
		
		return;
	}
	if ( FStrEq(szCommand, "!help flashbang" )) { //Help on the FB

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - FlashBang grenade.\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "The flashbang is designed to be used as a keybind (Scouts only)\n\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "bind a key to \"flashbang\" to use. Type bind <key> \"flashbang\" in console\n." );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "You get 2 Flashbangs per life." );
		MESSAGE_END();		

		return;
	}
	if ( FStrEq(szCommand, "!help speech" )) { //Help on credits

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Speech commands.\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "If you have a microphone, bind a key to +voicerecord\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "and hold it down and speak to send a voice message!" );
		MESSAGE_END();

		return;
	}

	if ( FStrEq(szCommand, "!help skins" )) { //Help on credits

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Skins list.\n--------------------------\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Heres the current GP2 Skins list. Type 'applyskin <code>' in console to use.\n" );
		MESSAGE_END();		

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "To Take off your skin use the 'killskin' console command\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Use the 'currentskin' console command to see what you are currently wearing.\n\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "A '*' ondicates Admin Only skin\n0 - Homer\n1 - Lego Man\n2 - Trinity\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "3 - Barnes\n4 - Toothpaste\n5 - Skeleton\n6 - Dr.Eastman\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "7 - Alien\n8 - Santa\n9 - SpiderMan\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "10 - Superman\n11 - Quake2 Marine\n12 - G-Man\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "13 - Barney\n14 - Xen Tree\n15 - SentryGun\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "16 - Dispenser\n*17 - RoboRob\n18 - Eyeball\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "19 - Reaper\n20 - Trekker\n21 - Judge Death\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "*22 - Nude\n23 - Yoda\n24 - Kermit\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "25 - Box\n26 - Wolverine\n27 - Rabbit\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "28 - Baboon\n29 - Hanky\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "30 - Slim Shady\n31 - Bender\n32 - Major K\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "33 - Scorpion\n34 - Sniper Wolf\n35 - Onna\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "36 - OBI 07\n37 - Quake\n38 - Slave\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "39 - Spawn\n40 - the Fist\n41 - Tigger\n42 - Ninja Turtle\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "43 - Limp Bizkit\n*44 - Solid Snake\n45 - Scream\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "46 - 7 of 9\n47 - Bad-Day\n48 - Bert\n49 - Boba-Fett\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "50 - Dunbine\n51 - KitKat\n52 - Midgets In Black\n*53 - Spam\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "54 - Thor\n*55 - Chris Redfield\n56 - AeonFlux\n" );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "57 - Daffy Duck\n58 - EVA\n*59 - Megatron\n60 - Rigell\n" );
		MESSAGE_END();
		return;


	}

	if ( FStrEq(szCommand, "!help icons" )) { //Help on credits

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Icons Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Use the new icons to communicate with other players.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "In the console type \"icon\" followed by one of the following commands\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "i.e \"icon happy\" for the happy icon, you can also bind these commands to a key.\n\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "suprise - Exclaimation mark icon\nammo - need ammo icon\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "armour - Need armour icon\nhappy - Happy smiley icon\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "sad - Sad smiley Icon\nfix - Needs fixing icon\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "idea - Light bulb above your head\ncoke - Coke logo\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "disabled - Use if you have foot damage or too lazy to walk\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "infected - Warn teammates you have aids\ngerman - German flag\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "uk - United Kingdom flag\nusa - United States flag\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help sensors" )) { //Help on sensor array

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Sensor Array Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Sensor posts can be built by engineers\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "2 or more sensors must join up to form a working sensor array. Kills enemies that trip the joining laser.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Use the following console commands to use the sensor array technology.\n\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "buildsensor - builds a sensor post\ndetsensor - detonates your sensor post\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nIf another engineer builds near you, your sensors automatically connect.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help prox" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Prox. Mine Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Proximity mines can be dropped by spies.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "You get 2 mines per life, mines are destroyed when you die.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Use the following console command to use the proximity mines.\n\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "dropmine - Drops a proximity mine.\n\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Mines are triggered by sensing nearby enemies.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help airstrike" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Airstrike Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Airstrike markers must be set by scouts\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "A Demoman must launch the airstrike." );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Use the following console commands to use the airstrike system.\n\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "(SCOUT) placemarker - Places the teams marker where the scout is stood.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "(DEMOMAN) +airstrike - Bind to a key and press and HOLD for 40 seconds to launch air strike\n\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "A Demoman cannot move when arming the airstrike.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help superflame" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Superflame Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Superflames can be used by Pyros\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "A Superflame muct be charged for 20 seconds." );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Bind +superflame to a key, and press and hold to charge the superflame.\n" );
		MESSAGE_END();		
		return;
	}

	if ( FStrEq(szCommand, "!help tripmine" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Tripmine Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Tripmines can be used by Soldiers\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Tripmines can only be placed on flat walls. They have a 4 second charge time.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "The console command to deploy a tripmine is \"tripmine\"\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help c4" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - C4 Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "C4 can be used by medics.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "C4 can only be placed on flat walls.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "The console command to deploy a tripmine is \"placec4\" to detonate a C4, use \"detc4\".\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Optionally, the placec4 command can take a parameter, " );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "detonation delay (in seconds). e.g \"placec4 30\"" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help mediflag" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - MediFlag Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Mediflags can be placed by medics.\nThey heal nearby teammates.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Use the command \"placeflag\" to drop your flag at your position\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help snarks" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Snarks Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Snarks can be deployed by snipers, you get 5 per life.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Use the command \"dropsnark\" to toss a snark in your direction.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help sentry" )) { //Help on proximity mine

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Manual Sentry Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Engineers can build manual sentry guns, any class can use them\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Engineers commands:\nbuildturret - builds a manual sentry" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "fixturret - use this when stood close to the turret to fix it.\ndetturret - destroy your turret" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nTo use the manual sentry go up to it and type useturret in the console." );
		MESSAGE_END();	

		return;
	}

	if ( FStrEq(szCommand, "!help energygun" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Energy Gun Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Soldiers can use the energy gun.\n" );
		MESSAGE_END();	
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "It can be fired every 5 seconds, press 6 to select it." );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "The energygun fires a green ball of energy that kills enemies." );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help grapple" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Grapple hook Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Anyone can use the grapple hook\nBind a key to +hook and press and hold to use.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "When you drop from the grapple, theres a 5 sec delay before you can use it again.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help blaster" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Blaster Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Demomen use the blaster\nPress 6 to get the gun out.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help private" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Private messaging Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "To send a private message use this command:\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "private <user> <message> (remember to quote the message).\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "Other commands:\ndisablepm - Disables private messages.\nenablepm - Enables private messages.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help 3rdperson" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - 3rd Person Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "You can activare AutoOPs 3rdperson mode with the following commands:\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "3rdpersonon - Type into console to activate 3rdperson\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "3rdpersonoff - Type into console to go to 1stperson\n" );
		MESSAGE_END();
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nNOTE: As the 3rdperson is serverside it may be only useable to LPBs.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help sgradar" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - SG Radar Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nType 'sgradar' into the console for a 10 second display of Sgs locations.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nCosts 30 armor points to use the radar.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help stealth" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Stealth Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nType 'stealth' into the console to toggle stealth camo.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nStealth drains 2hp per second so use it sparingly!\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nWhen in stealth your view switches to Spytech visual mode\nthis allows you to see other cloaked objects." );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nWhen stood still you are totally invisible, when moving, 20% visible.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help messages" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Message Flag Help\n--------------------------\n\n" );
		MESSAGE_END();			
		
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nType \"dropmessage <message>\" into the console to drop a mesage flag.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nThe message must be max 40 chars length.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nUse the command \"killmessage\" to destroy any message you have dropped." );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nThere is a minimum 15 second delay between message droppings.\n" );
		MESSAGE_END();	
		return;
	}

	if ( FStrEq(szCommand, "!help packbomb" )) {

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "Shodan Help system v3.0 - Packbomb Help\n--------------------------\n\n" );
		MESSAGE_END();				
		
		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nType \"droppackbomb\" to deploy a pack bomb.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nThe packbomb looks almost exactly like a regular backpack.\n" );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (0);
		WRITE_STRING( "\nWhen touched by enemies it will detonate." );
		MESSAGE_END();	

		MESSAGE_BEGIN( MSG_ONE, gmsgMOTDText, NULL, pEntity );
		WRITE_BYTE (1);
		WRITE_STRING( "\nDemomen get 2 packbombs per life, maximum 2 packbombs active at once.\n" );
		MESSAGE_END();	
		return;
	}
	
}//End of help commands