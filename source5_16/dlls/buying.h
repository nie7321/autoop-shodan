/*****************************************************\
*	Auto-OP Shodan
*
*	File: buying.h
*	Description: Prices and function prototypes for buying
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef BUYING_H
#define BUYING_H

//Menu constants
#define MENU_NONE 0
#define MENU_MAIN 1
#define MENU_POWERUPS 2
#define MENU_AMMO 3
#define MENU_WEAPONS 4
#define MENU_SPEECH 5
#define MENU_SPEECH2 6
#define MENU_BUILDING 7
#define MENU_POWERUPS2 8
#define MENU_SGOPTIONS 20

//Item Buying stuff

extern void ShowSGOptions(edict_t *pEntity);
extern void ShowBuyMenu(edict_t *pEntity);
extern void BuyMenuSelect(edict_t *pEntity, int iOptionSelected, int iMenuNumber);

#endif //BUYING_H
