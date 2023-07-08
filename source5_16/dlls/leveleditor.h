/*****************************************************\
*	Auto-OP Shodan
*
*	File: leveleditor.h
*	Description: AutoOP Entity Manipulation Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

//Entity Manipulation Funcs
#define MAX_ENTS 120 //Maximum allowed to be selected at a time per player.
extern void SelectEntity(edict_t *pEntity);
extern int IsValidEnt(edict_t *pEntity);
extern int FindFreeSlot(edict_t *pEntity);
extern void DeselectEntity(edict_t *pEntity);
extern void PointColorEnt(edict_t *pEntity, Vector vColors);
extern void PointTransEnt(edict_t *pPlayer, int iTransVal);
extern void MoveEnt(edict_t *pEntity, int Direction);
extern void CloneEnt(edict_t *pEntity);
extern void DeselectAll(edict_t *pEntity);
extern void DeleteEntity(edict_t *pEntity);
extern void QuickMove(edict_t *pEntity);
extern void QuickMoveX(edict_t *pEntity);
extern void QuickMoveY(edict_t *pEntity);
extern void QuickMoveZ(edict_t *pEntity);
extern void RotatePoint(edict_t *pEntity);
extern void RotateBrush(edict_t *pEntity);
extern void ChangeKeyValue(edict_t *pEntity, const char *Key, const char *KValue);
extern void TriggerEntity(edict_t *pEntity);
extern void SelectClass(edict_t *pEntity, const char *ClassName, const char *Range);
extern void ScanRadius(edict_t *pAdmin, int Info);
extern float FixAngle( float angle );
extern void StartPull(edict_t *pAdmin);
extern void PullThink(edict_t *pAdmin);
extern void EndPull(edict_t *pAdmin);
extern bool SaveEnts(edict_t *pAdmin, const char *FileName);
extern bool LoadEnts(edict_t *pAdmin, const char *FileName);
extern void KillAEFEnts(edict_t *pAdmin, const char *FileName);
extern void SelectLoaded(edict_t *pAdmin, const char *FileName);

typedef struct TFXAEFHeader_s
{
	int		Ver;			//FileVer.
	char	MapName[32];	//Map ent file is for.
	char	AuthorName[MAX_NAME_LENGTH];	//Who Made it.
	int		EntNumber;		//How many entities there are.
} TFXAEFHeader_t;

typedef struct TFXAEFEntity_s
{
	//Rendering stuff
	Vector	rendercolor;	//v.rendercolor
	int	rendermode;
	int renderfx;
	float renderamt;

	Vector	origin;			//v.origin
	Vector	angles;			//v.angles
	char model[64];			//v.model
	char classname[64];		//v.classname
	char targetname[64];
	char target[64];
	float speed;
	int movetype;
	float dmg;
	int effects;

} TFXAEFEntity_t;

#endif //LEVELEDITOR_H