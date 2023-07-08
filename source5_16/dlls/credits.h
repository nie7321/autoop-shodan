/*****************************************************\
*	Auto-OP Shodan
*
*	File: credits.h
*	Description: AutoOP Credits Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef CREDITS_H
#define CREDITS_H

extern void CreditThink();
extern void WriteBackFiles( );
extern int CREDIT_GAP;

//Rank functions
extern int GetRank(long int iCreditValue);
extern bool JustChangedRank(long int iCreditValue);
extern int NextRankAmount(long int iCreditValue);

//Player Connect/Disconnect stuff
extern void TFXLoadPlayerInfo(edict_t *pEntity, const char *PlayerName);
extern void TFXResetPlayerInfo(edict_t *pEntity);
extern void TFXPlayerDisconnect(edict_t *pEntity);
extern void TFXClientConnectionMsg(edict_t *pEntity, const char *pszName);


#endif //CREDITS_H