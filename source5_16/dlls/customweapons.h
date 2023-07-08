/*****************************************************\
*	Auto-OP Shodan
*
*	File: customweapons.h
*	Description: AutoOP Custom GunsHeader File
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*
*	Use, distribution, and modification of this source
*	code is strictly prohibited without written
*	permission from the author.
*
\*****************************************************/

#ifndef CUSTOMWEAPONS_H
#define CUSTOMWEAPONS_H

#define MAX_CUST_GUNS 5

extern void CustomGunModelSwitch(edict_t *pEntity);

//Weapon ID numbers
typedef enum 
{
	ENERGYGUN_ID = 1,
	TAZAR_ID,
	ANTHRAX_BLASTER_ID,
	TRIPMINE_ID

} WEAPON_IDS;

//HW DualBar
extern void SwingDualBar(edict_t *pEntity);

//Energy Gun
extern int ENERGY_BALL_SPEED;
extern int ENERGY_AMMO;
#define ENERGY_VMODEL "models/autoop/v_tfc_energygun.mdl"
#define ENERGY_PMODEL "models/autoop/p_tfc_energygun.mdl"
extern int ENERGY_DELAY;
extern void FireEnergy(edict_t *pEntity);
extern void SwitchToEnergyGun(edict_t *pEntity);
extern bool EnergyBlastThink(edict_t *pEntity);
extern void KillEnergyBlast(edict_t *pEntity, int Explode);

//Tazar
extern int TAZAR_DELAY;
extern int TAZAR_DAMAGE;
#define TAZAR_VMODEL "models/autoop/v_tfc_tazar.mdl"
#define TAZAR_PMODEL "models/autoop/p_tfc_tazar.mdl"
extern void FireTazar(edict_t *pEntity);
extern void SwitchToTazar(edict_t *pEntity);

//Tripmine
extern float TRIPMINE_DELAY;
#define TRIPMINE_VMODEL "models/v_tripmine.mdl"
#define TRIPMINE_PMODEL "models/p_tripmine.mdl"

//Anthrax Blaster
extern int ANTHRAX_DELAY;
extern int ANTHRAX_AMMO;
extern int ANTHRAX_DAMAGE;
#define ANTHRAX_VMODEL "models/autoop/v_tfc_ablaster.mdl"
#define ANTHRAX_PMODEL "models/autoop/p_tfc_ablaster.mdl"
extern void SwitchToBlaster(edict_t *pEntity);
extern void FireBlaster(edict_t *pEntity);

//Team Fortress Weapon numbers (from tfdefs.h)
#define WEAPON_HOOK				1
#define WEAPON_BIOWEAPON		(WEAPON_HOOK+1)
#define WEAPON_MEDIKIT			(WEAPON_HOOK+2)
#define WEAPON_SPANNER			(WEAPON_HOOK+3)
#define WEAPON_AXE				(WEAPON_HOOK+4)
#define WEAPON_SNIPER_RIFLE		(WEAPON_HOOK+5)
#define WEAPON_AUTO_RIFLE		(WEAPON_HOOK+6)
#define WEAPON_TF_SHOTGUN		(WEAPON_HOOK+7)
#define WEAPON_SUPER_SHOTGUN	(WEAPON_HOOK+8)
#define WEAPON_NAILGUN			(WEAPON_HOOK+9)
#define WEAPON_SUPER_NAILGUN	(WEAPON_HOOK+10)
#define WEAPON_GRENADE_LAUNCHER	(WEAPON_HOOK+11)
#define WEAPON_FLAMETHROWER		(WEAPON_HOOK+12)
#define WEAPON_ROCKET_LAUNCHER	(WEAPON_HOOK+13)
#define WEAPON_INCENDIARY		(WEAPON_HOOK+14)
#define WEAPON_ASSAULT_CANNON	(WEAPON_HOOK+16)
#define WEAPON_LIGHTNING		(WEAPON_HOOK+17)
#define WEAPON_DETPACK			(WEAPON_HOOK+18)
#define WEAPON_TRANQ			(WEAPON_HOOK+19)
#define WEAPON_LASER			(WEAPON_HOOK+20)
#define WEAPON_PIPEBOMB_LAUNCHER (WEAPON_HOOK+21)
#define WEAPON_KNIFE			(WEAPON_HOOK+22)
#define WEAPON_BENCHMARK		(WEAPON_HOOK+23) //what is this?
#define WEAPON_AUTOOP_CUSTOM	(WEAPON_HOOK+24) //One I need

#endif //CUSTOMWEAPONS_H