/*****************************************************\
*	Auto-OP Shodan
*
*	File: userfiles.h
*	Description: User Files Header
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
\*****************************************************/

#ifndef USERFILES_H
#define USERFILES_H



typedef struct TFXUserFileHeader_s
{
	int Version; //Version of userfile

} TFXUserFileHeader_t;

typedef struct TFXUserDataFile_s
{
	int CanSpray; //Can he use impulse 201? (spraycan)
	int CanTalk; //Can player talk?
	int CanVote; //Can player vote in a mapvote?
	int Warnings; //Number of warnings player has
	char Name[MAX_NAME_LENGTH]; //Last used name
	
	long int Credits; //Current Number of credits
	long int Time; //Current number of seconds on server

	int flag1; //For expansion
	int flag2; //For expansion
	int flag3; //For expansion
	int flag4; //For expansion

} TFXUserDataFile_t;	

#endif // USERFILES_H