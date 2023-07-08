/*****************************************************\
*	Auto-OP Shodan
*
*	File: commands.h
*	Description: AutoOP Commands Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef COMMANDS_H
#define COMMANDS_H

//Speech Commands
extern void PlayerCredInfo(const char *Speech);
extern void PlayerTimeInfo(const char *Speech);
extern void PlayerCheck(const char *Speech);
extern void PlayerWhois(const char *Speech);
extern void PlayerWarnings(const char *Speech);
extern void PlayerRankInfo(const char *Speech);

//ADM Basic Commands
extern void AdmBan(edict_t *pAdmin, const char *Player, const char *Time, const char *Reason);
extern void AdmKick(edict_t *pAdmin, const char *Player, const char *Reason);
extern void AdmSay(edict_t *pAdmin, const char *Text);
extern void AdmTeamplay(edict_t *pAdmin, const char *Value);
extern void AdmSpecttalk(edict_t *pAdmin, const char *Value);
extern void AdmGravity(edict_t *pAdmin, const char *Value);
extern void AdmGodMessage(edict_t *pAdmin, const char *Text);
extern void AdmGag(edict_t *pAdmin, const char *Player);
extern void AdmMap(edict_t *pAdmin, const char *MapName);
extern void TimeLeft( );
extern void AdmSetAdminTalk(edict_t *pAdmin);
extern void AdmAdminChat(edict_t *pAdmin, const char *Speech);
extern void AdmForceWriteBack(edict_t *pAdmin);
extern void AdmClientPrint(edict_t *pAdmin, const char *User, const char *Message);
extern void AdmChangeCreds(edict_t *pAdmin, const char *User, const char *Credits);
extern void AdmAdminList(edict_t *pAdmin);
extern void AdmExtendMap(edict_t *pAdmin);
extern void AdmDisinfectPlayer(edict_t *pAdmin, const char *User);
extern void AdmICQ(edict_t *pAdmin, const char *Message);

//ADM Cheat Commands
extern void AdmGod(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmNoclip(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmHologram(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmInvis(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmHealth(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmChangeSpeed(edict_t *pAdmin, const char *Player, const char *Speed);
extern void AdmGiveAllGuns(edict_t *pAdmin, const char *Player);
extern void AdmArmor(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmChangeAmmo(edict_t *pAdmin, const char *AmmoType, const char *Amount, const char *Player);
extern void AdmMultiBuild(edict_t *pAdmin, const char *Type, const char *Player);

//ADM Gimmik Commands
extern void AdmCreateLight(edict_t *pAdmin, const char *Player, const char *Red, const char *Green, const char *Blue);
extern void AdmRing(edict_t *pAdmin, const char *Player1, const char *Player2);
extern void AdmScreenShake(edict_t *pAdmin);
extern void AdmScreenFade(edict_t *pAdmin, const char *Red, const char *Green, const char *Blue);
extern void AdmTrail(edict_t *pAdmin, const char *Player, const char *Red, const char *Green, const char *Blue);
extern void AdmDizzyness(edict_t *pAdmin, const char *Player, const char *Value);
extern void AdmDropModel(edict_t *pAdmin,const char *Player, const char *Model);
extern void AdmAdminStrike(edict_t *pAdmin, const char *Player);
extern void AdmEntCreate(edict_t *pAdmin, const char *Value);
extern void AdmToss(edict_t *pAdmin, const char *Player);
extern void DropWeapon(edict_t *pAdmin,const char *WeaponName);
extern void AdmKillTrail(edict_t *pAdmin, const char *Player);
extern void AdmPermLight(edict_t *pAdmin, const char *Red, const char *Green, const char *Blue);
extern void AdmPlaySound(edict_t *pAdmin, const char *FileName);
extern void AdmVEffect(edict_t *pAdmin, const char *Player, const char *EffectCode);
extern void AdmSpawnGrunt(edict_t *pAdmin, const char *CanWalk);
extern void AdmSpawnGarg(edict_t *pAdmin, const char *CanWalk);
extern void AdmResetBuild(edict_t *pAdmin);

//ADM Train Commands
extern void AdmShowNodes(edict_t *pAdmin);
extern void AdmHideNodes(edict_t *pAdmin);
extern void AdmStartPath(edict_t *pAdmin, const char *Trackname);
extern void DrawPath(edict_t *pAdmin);
extern void AdmEndPath(edict_t *pAdmin, int UserEnded);
extern void MakePathTrain(edict_t *pAdmin, const char *TrackColor);
extern void DeleteTrack(edict_t *pAdmin, const char *TrackColor);
extern void DrawTrackLines(edict_t *pAdmin, const char *TrackColor);
extern int CheckTrack(const char *TrackColor);
extern int TrackExist(const char *TrackColor);

//ADM Player Control commands
extern void AdmForceBlue(edict_t *pAdmin, const char *Player);
extern void AdmForceRed(edict_t *pAdmin, const char *Player);
extern void AdmForceYellow(edict_t *pAdmin, const char *Player);
extern void AdmForceGreen(edict_t *pAdmin, const char *Player);
extern void AdmClientExecute(edict_t *pAdmin, const char *Player, const char *Command);
extern void AdmVoxToEveryone(edict_t *pAdmin, const char *Text);
extern void AdmExecuteToAll(edict_t *pAdmin, const char *Command);
extern void AdmMoveHere(edict_t *pAdmin, const char *Player);
extern void AdmMoveThere(edict_t *pAdmin, const char *Player);
extern void AdmChangeClass(edict_t *pAdmin, const char *Player, const char *Class);
extern void AdmTakeGuns(edict_t *pAdmin, const char *Player);
extern void AdmSpray(edict_t *pAdmin, const char *Player);
extern void AdmAllowVote(edict_t *pAdmin, const char *Player);
extern void AdmWarn(edict_t *pAdmin, const char *Player);
extern void AdmKillCustomSpawn(edict_t *pAdmin, const char *Player);
extern void AdmSetCustomSpawn(edict_t *pAdmin, const char *Player);
extern void AdmSetTime(edict_t *pAdmin, const char *Player, int Hours);
extern void AdmAllowSpec(edict_t *pAdmin, const char *Player);
extern void AdmMsgBox(edict_t *pAdmin, const char *Text);

//ADM Teleporter commands
extern void AdmCreateTeleStart(edict_t *pAdmin, const char *szName);
extern void AdmCreateTeleEnd(edict_t *pAdmin, const char *szName);
extern void AdmKillTeleStructure(edict_t *pAdmin, const char *szName);

#endif // COMMANDS_H