/*****************************************************\
*	Auto-OP Shodan
*
*	File: aomonsters.h
*	Description: AutoOP Monsters Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef AOMONSTERS_H
#define AOMONSTERS_H

extern float MonsterChangeYaw( edict_t *pEdict, float flspeed );
extern edict_t *MonsterFindEnemy(edict_t *pGrunt);
extern bool MonsterCanSeeEnemy(edict_t *pGrunt);
extern void MonsterMove(edict_t *pGrunt, int iType, int StatSeq, int Multiplier);
extern void MonsterFixIdealYaw(edict_t *pGrunt);
extern void MonsterBleed(edict_t *pGrunt, int BloodColor);

#endif //AOMONSTERS_H