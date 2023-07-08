/*****************************************************\
*	Auto-OP Shodan
*
*	File: autoop.h
*	Description: AutoOP Header File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

//Predefined sizes
#define BUF_SIZE 100
#define MAX_TEXT_LENGTH 200
#define MAX_NAME_LENGTH 50
#define MAX_NUMBER_LENGTH 20

#ifndef AUTOOP_H
#define AUTOOP_H

#include "leveleditor.h"
#include "customweapons.h"

//Version info
#define MOD_VERSION "5.17"
#define MOD_REVISION "A"
#define CLIENTPACK_VERSION "5.31"
#define AEF_VERSION 1
#define USERFILE_VERSION 3
#define USERFILE_V2 2
#define PLUGIN_INTERFACE_VER 1.00

extern int MAX_USERMSG;

//Access Levels
typedef enum 
{
	LEVEL_NONE = 0,
	LEVEL_LOW,
	LEVEL_OP,
	LEVEL_POWEROP,
	LEVEL_SUPEROP,
	LEVEL_HYPEROP

} ADMIN_LEVELS;

//Misc Functions
extern int TFXDispatchThink(edict_t *pEntity);
extern bool TFXPlayerCommand(edict_t *pEntity, const char *pcmd, const char *pcmd1, const char *pcmd2, const char *pcmd3, const char *pcmd4, const char *pcmd5);
extern void LogoText( );
extern void AdminGreenNames( );
extern void FakeScoreColor(edict_t *pPlayer, int iTeamNumber);
extern void UserLogger(edict_t *pEntity, const char *pszName, const char *pszAddress);
extern char *UserLogEncryption(const char *szString, const char *szKey);
extern void AddAdmin(edict_t *pAdmin, const char *szWONID, const char *szLevel);
extern void DelAdmin(edict_t *pAdmin, const char *szWONID);
extern void AdminAuth(edict_t *pEntity, int iConnecting);
extern void AdminDeAuth(edict_t *pEntity);
extern void CheckClientPack(edict_t *pEntity);
extern void HandleServerCommands();
extern void KickHighPing();
extern void GiveCredits(edict_t *pGiver, const char *Player, int iAmount);

//Dropped guns
extern int PlayerCurrentWeapon(edict_t *pEntity, const char *pcmd);
extern bool DroppedGunThink(edict_t *pEntity);
extern void DropGun(edict_t *pEntity);
extern void UserPickedMeUp(edict_t *pEntity, CBaseEntity *pPlayer);
extern int StartingWeapon(edict_t *pEntity);
extern int GUN_RESPAWN_DELAY;

//Text spam control
extern int FreqCheck(edict_t *pPlayer);
#define	FREQ_NUM	0
#define	FREQ_SEC	1

//Gibbing
extern void FakeGib(edict_t *pEntity);
extern bool GibThink(edict_t *pEntity);

//Anti Spawn camper stuff
extern void AntiCamp(edict_t *pEntity);
extern void AntiCampOver(edict_t *pEntity);

//Player Stuff
extern int TFXPlayerPreThink(edict_t *pEntity);

//NightGame Functions
extern void MakePlayersShine( );
extern void MakePlayersDull( );
extern void NAOChecks( );
extern void BLDChecks( );

//Class Numbers
#define CLASS_SCOUT 1
#define CLASS_SNIPER 2
#define CLASS_SOLDIER 3
#define CLASS_DEMOMAN 4
#define CLASS_MEDIC 5
#define CLASS_HWGUY 6
#define CLASS_PYRO 7
#define CLASS_SPY 8
#define CLASS_ENGINEER 9
#define CLASS_CIVILIAN 11

//Mail flag
extern void DropMailFlag(edict_t *pEntity, const char *Message);
extern bool MailFlagThink(edict_t *pEntity);
extern void DestroyMailFlag(edict_t *pEntity);

//Grapple Functions
extern void MoveToDest( float OrigX, float OrigY, float OrigZ, float flSpeed, edict_t *pEntity );
extern void PlayerMovedtoDest( edict_t *pEntity );
extern void PlayerDrops( edict_t *pEntity, int TakeBack );
extern void TossGrapple( edict_t *pPlayer );
extern void BeamRedraw( edict_t *pPlayer );

//New Grapple Prototypes
extern void ThrowGrapple(edict_t *pEntity);
extern void GrappleTouch(edict_t *pGrappleEnt, edict_t *pOtherEnt);
extern bool GrappleHookThink(edict_t *pGrappleEnt);
extern void KillGrapple(edict_t *pPlayerEnt);
extern int GRAPPLE_HEALTH;
extern int GRAPPLE_DROPTIME;

//Help System Functions
extern void ShowHelp(edict_t *pEntity, const char *szCommand);

//Messaging Stuff
extern char BOT_NAME[];
extern char BOT_NAME2[]; //Used as a shorter alternative to communicate with the bot
extern void BotTalkAll(char *Speech);
extern void BotTalkOne(edict_t *pEntity, char *Speech);
extern void BotTalkAllArgs( char *format, ... );
extern void BotTalkOneArgs( edict_t *pEntity, char *format, ... );
extern void BotCom(edict_t *pEntity, const char *FullSpeech, int iName);
extern void PrivateMessage(edict_t *pSender, const char *Player, const char *Message);

//Icon Functions
extern void ApplyIcon(edict_t *pEntity, const char *szIcon);
extern int ICON_DELAY;

//GAMEPLAY ADDONS --------------------------

#define CAREA_NOBUILD 2
#define CAREA_BLOCKED 1
#define CAREA_CLEAR 0

//Laser Fence Post Functions
extern void BuildLaserFence(edict_t *pEntity);
extern bool LaserFenceThink(edict_t *pEntity);
extern void DetonateFencePost(edict_t *pEntity);
extern void RepairSensor(edict_t *pEntity);
extern void SensorDisconnect(edict_t *pEntity);
extern int FENCEPOST_HEALTH;
extern int FENCEPOST_FIX_REQ;

//Big Sentry Stuff
extern void BuildGun(edict_t *pEntity);
extern void SentryShoot(edict_t *pEntity);
extern bool BigGunThink(edict_t *pEntity);
extern void UseGun(edict_t *pEntity);
extern void DestroyPlayersGun(edict_t *pEntity);
extern void DestroyGun(edict_t *pEntity);
extern void FixSentry(edict_t *pEntity);
extern int BIGGUN_REQUIRED;
extern int BIGGUN_FIX_REQ;
extern int BIGGUN_HEALTH;

//Proximity Mine stuff
extern void DropMine(edict_t *pEntity);
extern void DetonateMines(edict_t *pEntity, int iDetonate);
extern bool ProxMineThink(edict_t *pEntity);
extern int PROX_MINE_AMMO;

//Airstrike stuff
extern void PlantMarker(edict_t *pEntity);
extern void CancelLaunch(edict_t *pEntity);
extern void SendStrike(edict_t *pEntity);
extern bool StartStrikeThink(edict_t *pEntity);
extern void StrikeLocation(Vector vArea, int iBomb);
extern void DoTheStrike(Vector vArea);
extern bool AirStrikeCounterThink(edict_t *pEntity);
extern int AIRSTRIKE_RANDOMNESS;
extern int AIRSTRIKE_CHARGE_TIME;
extern int AIRSTRIKE_AMOUNT;

//SuperFlame stuff
extern void ChargeFlame(edict_t *pEntity);
extern bool StartFlameThink(edict_t *pEntity);
extern void CancelFlame(edict_t *pEntity);
extern void DoTheFlame(Vector vArea, edict_t *pOwner);
extern bool SuperFlameThink(edict_t *pEntity);

//Tripmine stuff
extern void SwitchToTripMine(edict_t *pEntity);
extern void PlaceTripMine(edict_t *pEntity);
extern bool TripMineThink(edict_t *pEntity);
extern void DestroyTripMine(edict_t *pEntity);
extern void DestroyPlayerTripMines(edict_t *pEntity, int iDetonate);
extern int TRIPMINE_HEALTH;
extern int TRIPMINE_DAMAGE;
extern int TRIPMINE_AMMO;

//C4
extern void PlaceC4(edict_t *pEntity, int iTimer);
extern int HasAlreadyPlaced(edict_t *pEntity);
extern bool C4Think(edict_t *pEntity);
extern void DetonateC4(edict_t *pEntity, int iDetonate);
extern bool C4BombCounterThink(edict_t *pEntity);
extern int C4_HEALTH;
extern float C4_DETDELAY;
extern int C4_AMMO;

//MediFlag
extern void DropFlag(edict_t *pEntity);
extern bool MediFlagThink(edict_t *pEntity);
extern void DestroyFlag(edict_t *pEntity, int iDetonate);
extern int MEDI_FLAG_HEALTH;
extern int MEDI_FLAG_RADIUS;

//Flashbang
extern void TossFlashBang(edict_t *pEntity);
extern bool FBThink(edict_t *pEntity);

//Spy Stealth
extern void ActivateStealth(edict_t *pEntity);
extern bool StealthEntThink(edict_t *pEntity);
extern void DeactivateStealth(edict_t *pEntity);

//Medic revival
extern void MediRevive(edict_t *pEntity);
extern void RevivePlayer(edict_t *pEntity);
extern void ReSupplyPlayer(edict_t *pEntity);

//Snark
extern void ThrowSnark(edict_t *pEntity);
extern void SnarkTouch(edict_t *pEntity, edict_t *pVictim);
extern bool SnarkThink(edict_t *pEntity);
extern edict_t *FindVictim(edict_t *pEntity, int iDistance);
extern int EnemyIsVisible(edict_t *pSnark, edict_t *pTarget);
extern void KillSnark(edict_t *pEntity);
extern int SNARK_HEALTH;
extern int SNARK_AMMO;

//Stealth camo
extern int STEALTH_DRAIN;

//Vampirism
extern void SuckBlood(edict_t *pEntity);

//Holoduke
extern void KillHoloDuke(edict_t *pEntity);
extern bool HoloDukeThink(edict_t *pEntity);
extern void DeployHoloDuke(edict_t *pEntity);
extern void DeactivateHoloDuke(edict_t *pEntity);

//HWRadar
extern void DoRadar(edict_t *pEntity);

//END OF GAMEPLAY ADDONS ----------------------------

typedef struct TFXFeatureEnabled_s
{
	int		Airstrikes;
	int		Biggun;
	int		C4;
	int		Grapple;
	int		Icons;
	int		LaserFence;
	int		MediFlag;
	int		ProxMines;
	int		Snarks;
	int		TripMines;
	int		Skins;
	int		SkinsShowTeam;
	int		Kickvote;
	int		Debug;
	int		SGCloak;
	int		SuperFlame;
	int		MapVote;
	int		AdminSpect;
	int		FlashBangs;
	int		Credits;
	int		SkyHook;
	int		StealthCamo;
	int		HWSword;
	int		AntiSpawnCamp;
	int		LogStuff;
	int		Revival;
	int		VoteRatio;
	int		EditorProtection;
	int		CustomWeapons;
	float	ReservedSlotTime;
	int		BotTalk;
	int		TeamGrapple;
	int		SnipeCheck;
	int		LockDown;
	int		PrivMessages;
	int		AdminSeePriv;
	int		AllowGunDrop;
	int		Vampirism;
	int		Thirdperson;
	int		WeaponsOnSkins;
	int		AdminTeam;
	int		HWRadar;
	int		CustDispenser;
	int		C4OnPlayers;
	int		C4AutoDet;
	int		ForceTeamBalance;
	int		Buying;
	int		ClassicGrapple;
	int		GrappleSpeed;
	int		ICQNotify;
	int		Messageflags;

} TFXFeatureEnabled_t;

typedef struct TFXUserFileHeader_s
{
	int Version; //Version of userfile
	char OrigName[MAX_NAME_LENGTH]; //Original name

} TFXUserFileHeader_t;

typedef struct TFXPlayerRank_s
{
	long int Requirement; //Credits needed to get rank
	char RankName[MAX_TEXT_LENGTH]; //Name of Rank

} TFXPlayerRank_t;

typedef struct TFXUserDataFile_s
{
	int CanSpray; //Can he use impulse 201? (spraycan)
	int CanTalk; //Can player talk?
	int CanVote; //Can player vote in a mapvote?
	int Warnings; //Number of warnings player has
	char Name[MAX_NAME_LENGTH]; //Last used name
	
	long int Credits; //Current Number of credits
	long int TimeLen; //Current number of seconds on server

	int flag1; //1 = Do not authenticate as op when joining
	int flag2; //Stored Admin level
	int flag3; //For expansion
	int flag4; //For expansion

} TFXUserDataFile_t;

typedef struct TFXUserDataFileV3_s
{
	int CanSpray; //Can he use impulse 201? (spraycan)
	int CanTalk; //Can player talk?
	int CanVote; //Can player vote in a mapvote?
	int Warnings; //Number of warnings player has
	char Name[MAX_NAME_LENGTH]; //Last used name
	
	long int Credits; //Current Number of credits
	long int TimeLen; //Current number of seconds on server

	int flag1; //1 = Do not authenticate as op when joining
	int flag2; //Stored Admin level
	int flag3; //For expansion
	int flag4; //1 = Cant run cheat commands
	long int flag5; //Rank Credits
	long int flag6; //For expansion
	long int flag7; //For expansion
	long int flag8; //For expansion
	long int flag9; //For expansion
	long int flag10; //For expansion
	char Char1[MAX_NAME_LENGTH]; //For expansion
	char Char2[MAX_NAME_LENGTH]; //For expansion
	char Char3[MAX_NAME_LENGTH]; //For expansion
	char Char4[MAX_NAME_LENGTH]; //For expansion

} TFXUserDataFileV3_t;

//extern int LoadFile(int WONID, TFXUserFileHeader_s *Header, TFXUserDataFile_s *Data);
//extern int SaveFile(int WONID, TFXUserFileHeader_s Header, TFXUserDataFile_s Data);

typedef struct AOPlayerInfo_s
{
	//Flashbang stuff
	float	FBTime;
	int		FBAmmo;

	//Icon stuff
    float	IconTime;

	//Grapple Stuff
    int		GrapStat;
	Vector	GrapOrig;
	float	GrapTime;
	float	LineTime;
	float	HangTime;

	//Player Info Stuff
	long int NeedFrags;
	long int GotFrags;
	long int Deaths;
	long int LastKnownFrags;

	//Building stuff
	int		HasFencePost;
	int		HasBigGun;

	//Stealth
	int		StealthOn;

	//Custom Weaponry Stuff
	int		ProxMineAmmo;
	int		TripMineAmmo;
	int		C4Ammo;
	int		SnarkAmmo;
	float	SnarkLastThrow;
	float	DualBarLastSwing;
	int		AlreadyHealed;
	int		IsUsingTurret;
	float	LastBuilt;
	float	CantChangeTeam;
	int		SwitchingToCustomGun;
	int		RemoveDisguise;
	
	//Vampirism
	float	LastSuckTime;
	int		IsSucking;

	float	LastUsedEnergy[MAX_CUST_GUNS];

	//Airstrike stuff
	float	PlacedPoint;

	int		FlashLightOff;

	//Administrative Stuff
	int		IsAdmin; //Stores a players admin level
	int		NoGreen;
	int		AdminChat; //If 1 next speech command goes to admins only.
	int		FakeCreds; //This is the value used for the fake credits.
	long	FakeTime;	

	//KickVote Stuff
	int		KickVote;
	int		StartedAVote;

	//Selected Entity stuff
    struct Selected
    {
		int	LockX;
		int LockY;
		int LockZ;
		int	SelectedEnts[MAX_ENTS];

		//'Pull' Style moving
		int IsPulling;
		Vector PullOrig;
		float PullTime;
    };
    Selected Selected;
	int		MovingEnt;

	//Mediflag
	float	LastMovedFlag;

	//Map voted for
	int		VotedMapIndex;

    struct PathBuild
	{
		int			Drawingpath;
		int			NodeCount;
		float		NextDraw;
		char		TrackName[MAX_TEXT_LENGTH];
	};
	PathBuild PathBuild;

	//Spawn stuff
	int		HasDied;
	int		IsInvuln;
	float	InvulnEnd;
	Vector	CustomSpawnPoint;

	TFXUserDataFileV3_s LoadedInfo; //Information loaded from file
	char	OrigName[MAX_NAME_LENGTH];
	
	int		CurrentWeapon;

	int		SpecOverride;

	int		VisualEffect; //This sets a visual effect on player.
	float	EffectNextThink; //Event time store
	float	VisualThinkDelay; //Visuals fire every x seconds.
	float	VisualCounter; //Can be used for anything.

	int		IsSteamer;
	char	IPAddress[50];

	int		PMBlock;
	char	LastStatusText[MAX_TEXT_LENGTH];
	float	StatusReset;

	int		MenuStatus; //Which menu am I in?
	int		AbilityOverride; //Bitmap of ability overrides

	//Store the _gp2clpckver variable here
	char	ClientPackVer[MAX_NAME_LENGTH];

	//When user is gagged their name must = this
	char	GaggedName[MAX_NAME_LENGTH];
	int		TimerActive; //Users timer is active?
	int		PackBombAmmo;

	//Mailflag
	float	LastMovedMessage;

} AOPlayerInfo_t;

#define	ABILITY_SCOUTSPEED			(1<<0)
#define	ABILITY_LOWGRAV				(1<<1)
#define	ABILITY_LOWGRAV2			(1<<2)
#define	ABILITY_5GREN1				(1<<3)
#define	ABILITY_5GREN2				(1<<4)
#define	ABILITY_SPAWNRPG			(1<<5)
#define	ABILITY_SPAWNCHAINGUN		(1<<6)
#define	ABILITY_SPAWNSNIPERRIFLE	(1<<7)
#define	ABILITY_SPAWNSHOTGUN		(1<<8)
#define	ABILITY_ADMINSKIN			(1<<9)

//Light entity
extern bool LightThink(edict_t *pEntity);

extern int KEEP_CLEAR; //Must be at least this amount of entities free to build

#endif // AUTOOP_H