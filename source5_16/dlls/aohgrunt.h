/*****************************************************\
*	Auto-OP Shodan
*
*	File: aohgrunt.h
*	Description: AutoOP HGRUNT Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef AOHGRUNT_H
#define AOHGRUNT_H

extern void CreateGrunt(edict_t *pPlayer, bool CanWalk);
extern bool GruntThink(edict_t *pGrunt);
extern void GruntAttackEnemy(edict_t *pGrunt);
extern float GruntChangeYaw( edict_t *pEdict, float flspeed );
extern void CreateGruntCoords(bool CanWalk, int x, int y, int z);

#define HGRUNT_HEALTH 250

#endif //AOHGRUNT_H