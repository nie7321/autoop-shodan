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

#ifndef WEAPONS_H
#define WEAPONS_H



typedef struct TFXWeapon_s
{
	int		ID;				//AutoOP Weapon ID
	char	Name[64];		//Name of weapon (and command it uses)
	int		Bucket;			//Bucket it uses
	int		BucketPos;		//Position in the bucket
	int		PlayerClass;	//Class that player must be to get this gun
	
} TFXWeapon_t;

#endif //WEAPONS_H
