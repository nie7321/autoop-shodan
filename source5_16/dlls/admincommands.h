/*****************************************************\
*	Auto-OP Shodan
*
*	File: admincommands.h
*	Description: Admin Commands Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef ADMINCOMMANDS_H
#define ADMINCOMMANDS_H

typedef struct AdminCommand_s
{
	char	Description[100]; //Description i.e Bans a player
	char	Format[100]; //Format i.e <playername> <time>
	char	Name[64]; //Command name i.e ban
	int		ReqLevel; //Minimum required access level
	int		CheatCommand; //If >0 then this command is a cheat
} AdminCommand_t;

extern bool TFXAdminCommand(edict_t *pEntity, const char *cmd, const char *arg1, const char *arg2, const char *arg3, const char *arg4);
extern int	GetCommandIndex(const char *cmd);
extern bool TFXAdminCommandHelp(edict_t *pEntity, const char *cmd);
extern bool TFXListCommands(edict_t *pEntity, const char *letter);

#endif //ADMINCOMMANDS_H
