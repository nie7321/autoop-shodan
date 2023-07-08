/*****************************************************\
*	Auto-OP Shodan
*
*	File: voting.h
*	Description: AutoOP Voting Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef VOTING_H
#define VOTING_H

//Kickvote stuff
extern void EndTheVote();
extern void CastKickVote(edict_t *pVoter, const char *VoteSpeech);

//Map vote stuff
typedef struct MapVoteItem_s
{
	char		MapName[BUF_SIZE]; //Name of map
	int			Votes;

} MapVoteItem_t;

#define MAX_MAPS 20 //Maximum allowed maps to be in a vote.
extern int CheckMap(const char *MapName2);
extern void VoteCast(edict_t *pEntity, const char *Speech);
extern void ScorchVoteCast(edict_t *pEntity, const char *ScorchMap);
extern void VoteStart(edict_t *pCaller);
extern void VoteEnd( );
extern void CancelMapVote( );
extern void SayWinningMap( );
extern void SayNextMap( );

#endif //VOTING_H