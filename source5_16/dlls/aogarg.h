/*****************************************************\
*	Auto-OP Shodan
*
*	File: aohgrunt.h
*	Description: AutoOP AOGARG Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef AOGARG_H
#define AOGARG_H

extern void CreateGarg(edict_t *pPlayer, bool CanWalk);
extern bool GargThink(edict_t *pGarg);
extern edict_t *GetEyeGlow(edict_t *pGarg);
extern void GargAttackEnemy(edict_t *pGarg);
extern void CreateGargCoords(bool CanWalk, int x, int y, int z);

#define GARG_HEALTH 2000

#endif //AOGARG_H