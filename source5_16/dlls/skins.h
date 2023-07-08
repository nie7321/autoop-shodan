/*****************************************************\
*	Auto-OP Shodan
*
*	File: skins.h
*	Description: AutoOP Skins Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef SKINS_H
#define SKINS_H

//Skins Functions
extern void InitSkins( );
extern void ApplySkin(edict_t *pEntity, int iSkinNo);
extern bool SkinThink(edict_t *pEntity);
extern void KillSkin(edict_t *pEntity);
extern void SayCurrentSkin(edict_t *pEntity);
extern int GetPlayerSkinIndex(edict_t *pEntity);

#define TOTALSKINS 60

#define MAX_SKINS 32 //Max skin entities required at a time.
#define SKIN_MDL	"models/autoop/gp2data1.mdl"
#define SKIN_MDL2	"models/autoop/gp2data2.mdl"
#define SKIN_MDL3	"models/autoop/gp2data3.mdl"
#define SKIN_MDL4	"models/autoop/gp2data4.mdl"
#define SKIN_MDL5	"models/autoop/gp2data5.mdl"
#define SKIN_MDL6	"models/autoop/gp2data6.mdl"
#define SKIN_MDL7	"models/autoop/gp2data7.mdl"

typedef struct TFXSkin_s
{
	int		ID;				//Unique Skin ID
	int		BodyNumber;		//Studio body in MDL
	char	MDLName[64];	//MDL File the Skin uses
	int		ReqAccess;		//Access required to use skin
	char	SkinName[64];	//Name of skin model (i.e Homer)
} TFXSkin_t;

#endif //SKINS_H