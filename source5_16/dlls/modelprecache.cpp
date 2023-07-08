/*****************************************************\
*	Auto-OP Shodan
*
*	File: modelprecache.cpp
*	Description: Precaching of mod resources.
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*	This program is free software; you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation; either version 2 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
************************************/

#include "extdll.h"
#include "enginecallback.h"
#include "util.h"
#include "cbase.h"
#include "shake.h"
#include "autoop.h"
#include "stripper.h"
#include "skins.h"
#include <direct.h>
#include <stdio.h>

extern AOPlayerInfo_s PlayerInfo[33];
extern TFXFeatureEnabled_s Features;
extern FILE *fp;

//Model Storage
int m_Chain = 0;
short m_Fire = 0;
short g_sModelIndexBloodSpray = 0;
short g_sModelIndexBloodDrop = 0;

void PrecacheResources()
{
	// do level initialization stuff here...
	//Cache the sprites for the new icons
	PRECACHE_MODEL( "sprites/autoop/exclamation.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/exclamation.spr");

	PRECACHE_MODEL( "sprites/autoop/ammo.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/ammo.spr");

	PRECACHE_MODEL( "sprites/autoop/armour.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/armour.spr");

	PRECACHE_MODEL( "sprites/autoop/sad.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/sad.spr");

	PRECACHE_MODEL( "sprites/autoop/smiley.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/smiley.spr");

	PRECACHE_MODEL( "sprites/autoop/fixit.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/fixit.spr");

	PRECACHE_MODEL( "sprites/autoop/bulb.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/bulb.spr");

	PRECACHE_MODEL( "sprites/autoop/coke.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/coke.spr");

	PRECACHE_MODEL( "sprites/autoop/disabled.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/disabled.spr");

	PRECACHE_MODEL( "sprites/autoop/germany.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/germany.spr");

	PRECACHE_MODEL( "sprites/autoop/infected.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/infected.spr");
	
	PRECACHE_MODEL( "sprites/autoop/uk.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/uk.spr");

	PRECACHE_MODEL( "sprites/autoop/usa.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/usa.spr");

	PRECACHE_MODEL( "sprites/autoop/canada.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/canada.spr");

	//Spy Targeter sprites
	PRECACHE_MODEL( "sprites/autoop/predaim_red.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/predaim_red.spr");

	PRECACHE_MODEL( "sprites/autoop/predaim_yel.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/predaim_yel.spr");

	//Misc. Model/Sound precaching
	PRECACHE_MODEL( "models/aflock.mdl");
	PRECACHE_MODEL( "models/presentlg.mdl");
	PRECACHE_MODEL( "models/presentsm.mdl");
	PRECACHE_MODEL( "models/agrunt.mdl");
	PRECACHE_MODEL( "models/bigrat.mdl");
	PRECACHE_MODEL( "models/chumtoad.mdl");
	PRECACHE_MODEL( "models/gman.mdl");
	PRECACHE_MODEL( "models/uplant3.mdl");
	PRECACHE_MODEL( "models/ball.mdl");
	PRECACHE_MODEL( "sprites/laserbeam.spr");
	PRECACHE_MODEL( "models/mechgibs.mdl");
	
	//xen_tree
	PRECACHE_MODEL( "models/tree.mdl");
	PRECACHE_SOUND( "zombie/claw_strike1.wav");
	PRECACHE_SOUND( "zombie/claw_strike2.wav");
	PRECACHE_SOUND( "zombie/claw_strike3.wav");
	PRECACHE_SOUND( "zombie/claw_miss1.wav");
	PRECACHE_SOUND( "zombie/claw_miss2.wav");

	//xen_light
	PRECACHE_MODEL( "models/light.mdl" );
	PRECACHE_MODEL( "sprites/flare3.spr" );

	//xen_hair
	PRECACHE_MODEL( "models/hair.mdl" );

	//xen_spore_large/_small/_medium
	PRECACHE_MODEL( "models/fungus(small).mdl" );
	PRECACHE_MODEL( "models/fungus.mdl" );
	PRECACHE_MODEL( "models/fungus(large).mdl" );

	//The 3 Quake\TF powerups precaches.
	PRECACHE_MODEL( "models/quad.mdl" );
	PRECACHE_MODEL( "models/invis.mdl" );
	PRECACHE_MODEL( "models/pent.mdl" );
	PRECACHE_SOUND( "items/damage.wav");
	PRECACHE_SOUND( "items/damage2.wav");
	PRECACHE_SOUND( "items/damage3.wav");
	PRECACHE_SOUND( "items/inv1.wav");
	PRECACHE_SOUND( "items/inv2.wav");
	PRECACHE_SOUND( "items/inv3.wav");
	PRECACHE_SOUND( "items/protect.wav");
	PRECACHE_SOUND( "items/protect2.wav");
	PRECACHE_SOUND( "items/protect3.wav");

	//monster_turret Precaches
	PRECACHE_SOUND ("turret/tu_fire1.wav");
	PRECACHE_SOUND ("turret/tu_ping.wav");
	PRECACHE_SOUND ("turret/tu_active2.wav");
	PRECACHE_SOUND ("turret/tu_die.wav");
	PRECACHE_SOUND ("turret/tu_die2.wav");
	PRECACHE_SOUND ("turret/tu_die3.wav");
	PRECACHE_SOUND ("turret/tu_deploy.wav");
	PRECACHE_SOUND ("turret/tu_spinup.wav");
	PRECACHE_SOUND ("turret/tu_spindown.wav");
	PRECACHE_SOUND ("turret/tu_search.wav");
	PRECACHE_SOUND ("turret/tu_alert.wav");
	PRECACHE_MODEL ("sprites/flare3.spr");
	PRECACHE_MODEL( "models/turret.mdl" );
	PRECACHE_MODEL ("models/miniturret.mdl");	
	PRECACHE_SOUND("weapons/hks1.wav");
	PRECACHE_SOUND("weapons/hks2.wav");
	PRECACHE_SOUND("weapons/hks3.wav");
	PRECACHE_MODEL ("models/sentry.mdl");

	//Func_mortar precaches
	PRECACHE_SOUND ("weapons/mortar.wav");
	PRECACHE_SOUND ("weapons/mortarhit.wav");
	PRECACHE_MODEL( "sprites/lgtning.spr" );

	//Snark stuff
	PRECACHE_MODEL("models/w_squeak.mdl");
	PRECACHE_SOUND("squeek/sqk_blast1.wav");
	PRECACHE_SOUND("common/bodysplat.wav");
	PRECACHE_SOUND("squeek/sqk_die1.wav");
	PRECACHE_SOUND("squeek/sqk_hunt1.wav");
	PRECACHE_SOUND("squeek/sqk_hunt2.wav");
	PRECACHE_SOUND("squeek/sqk_hunt3.wav");
	PRECACHE_SOUND("squeek/sqk_deploy1.wav");

	//Hgrunt stuff
	PRECACHE_MODEL( "models/hgrunt.mdl" );
	PRECACHE_SOUND("hgrunt/gr_mgun1.wav");
	PRECACHE_SOUND("hgrunt/gr_mgun2.wav");
	PRECACHE_SOUND( "hgrunt/gr_pain1.wav" );
	PRECACHE_SOUND( "hgrunt/gr_pain2.wav" );
	PRECACHE_SOUND( "hgrunt/gr_pain3.wav" );
	PRECACHE_SOUND( "hgrunt/gr_pain4.wav" );
	PRECACHE_SOUND( "hgrunt/gr_pain5.wav" );
	PRECACHE_SOUND( "hgrunt/gr_die1.wav" );

	//Gargantua Stuff
	PRECACHE_MODEL( "models/garg.mdl" );
	PRECACHE_MODEL( "sprites/gargeye1.spr" );
	PRECACHE_SOUND( "garg/gar_stomp1.wav" );
	PRECACHE_SOUND( "garg/gar_pain3.wav" );

	//Ammo
	PRECACHE_MODEL( "models/w_rpgammo.mdl" );
	PRECACHE_MODEL( "models/w_shotbox.mdl" );
	PRECACHE_MODEL( "models/w_crossbow_clip.mdl" );
	PRECACHE_MODEL( "models/chainammo.mdl");

	//AutoOP Specifics
	PRECACHE_MODEL("models/autoop/flashbang.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/flashbang.mdl");

	PRECACHE_MODEL("models/autoop/sensorpost.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/sensorpost.mdl");

	PRECACHE_MODEL("models/autoop/proxmine.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/proxmine.mdl");

	PRECACHE_MODEL("models/autoop/airstrike.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/airstrike.mdl");

	PRECACHE_MODEL("models/autoop/marker.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/marker.mdl");

	PRECACHE_MODEL("models/autoop/ao_dispenser.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/ao_dispenser.mdl");

	PRECACHE_MODEL("sprites/autoop/grapple1.spr"); //For the Grapple
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/grapple1.spr");

	PRECACHE_MODEL("models/autoop/aotripmine.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/aotripmine.mdl");

	PRECACHE_MODEL("models/autoop/mediflag.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/mediflag.mdl");

	PRECACHE_MODEL("models/autoop/mailflag.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/mailflag.mdl");

	PRECACHE_MODEL("models/autoop/packbomb.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/packbomb.mdl");

	PRECACHE_MODEL("models/autoop/aoc4.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/aoc4.mdl");

	PRECACHE_SOUND("weapons/mine_activate.wav");
	PRECACHE_SOUND("weapons/xbow_hit1.wav");
	PRECACHE_SOUND("weapons/xbow_hit2.wav");
	PRECACHE_SOUND("weapons/xbow_fire1.wav");
	PRECACHE_SOUND("weapons/electro6.wav"); //For the sensor array.
	PRECACHE_SOUND("buttons/blip2.wav"); //Airstrike marker.sound
	PRECACHE_SOUND("ambience/particle_suck1.wav");
	PRECACHE_SOUND("ambience/flameburst1.wav");
	PRECACHE_SOUND("player/pl_fallpain1.wav");
	PRECACHE_SOUND("items/medshot4.wav");
	PRECACHE_SOUND("player/geiger6.wav");
	PRECACHE_MODEL("sprites/enter1.spr");
	PRECACHE_MODEL("sprites/exit1.spr");
	PRECACHE_SOUND("buttons/button9.wav");
	PRECACHE_MODEL("sprites/fthrow.spr"); //Superflame
	PRECACHE_MODEL("sprites/laserbeam.spr"); //Tripmine beam
	PRECACHE_SOUND("weapons/mine_deploy.wav");
	PRECACHE_SOUND("weapons/mine_charge.wav");

	//Weapon dropping/picking up
	PRECACHE_SOUND("items/gunpickup2.wav");
	PRECACHE_SOUND("items/weapondrop1.wav");

	//Skins
	PRECACHE_MODEL(SKIN_MDL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL);

	PRECACHE_MODEL(SKIN_MDL2);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL2);

	PRECACHE_MODEL(SKIN_MDL3);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL3);

	PRECACHE_MODEL(SKIN_MDL4);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL4);

	PRECACHE_MODEL(SKIN_MDL5);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL5);

	PRECACHE_MODEL(SKIN_MDL6);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL6);
	
	PRECACHE_MODEL(SKIN_MDL7);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),SKIN_MDL7);

	PRECACHE_MODEL("models/autoop/tree1.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/tree1.mdl");
	PRECACHE_MODEL("models/autoop/tree2.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/tree2.mdl");
	PRECACHE_MODEL("models/autoop/tree3.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/tree3.mdl");
	PRECACHE_MODEL("models/autoop/chick.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/chick.mdl");
	PRECACHE_MODEL("models/autoop/origbackpack.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/origbackpack.mdl");
	PRECACHE_MODEL("models/autoop/packbomb.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/packbomb.mdl");
	PRECACHE_MODEL("sprites/autoop/player_warning.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/autoop/player_warning.spr");

	//TFC gun world models
	PRECACHE_MODEL("models/autoop/w_guns.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/w_guns.mdl");

	//Sword
	PRECACHE_MODEL("models/autoop/v_tfc_dualbar.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/v_tfc_dualbar.mdl");

	PRECACHE_MODEL("models/autoop/p_tfc_dualbar.mdl");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"models/autoop/p_tfc_dualbar.mdl");
	
	PRECACHE_SOUND("weapons/xbow_hit1.wav");

	/*
	//Tripmine
	PRECACHE_MODEL(TRIPMINE_VMODEL);
	PRECACHE_MODEL(TRIPMINE_PMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),TRIPMINE_PMODEL);
	*/

	//Energy Gun
	PRECACHE_MODEL("sprites/flare1.spr");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"sprites/flare1.spr");

	PRECACHE_MODEL(ENERGY_VMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),ENERGY_VMODEL);

	PRECACHE_MODEL(ENERGY_PMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),ENERGY_PMODEL);

	PRECACHE_SOUND("buttons/spark6.wav");
	PRECACHE_SOUND("weapons/electro5.wav");

	//Tazar
	PRECACHE_MODEL(TAZAR_VMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),TAZAR_VMODEL);

	PRECACHE_MODEL(TAZAR_PMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),TAZAR_PMODEL);

	PRECACHE_SOUND("weapons/electro4.wav");

	//Anthrax Blaster
	PRECACHE_MODEL(ANTHRAX_VMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),ANTHRAX_VMODEL);

	PRECACHE_MODEL(ANTHRAX_PMODEL);
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),ANTHRAX_PMODEL);

	//START SOUNDS ----------
	PRECACHE_SOUND("autoop/endmap.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/endmap.wav");
	PRECACHE_SOUND("autoop/roborob.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/roborob.wav");
	PRECACHE_SOUND("autoop/girlpower.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/girlpower.wav");
	PRECACHE_SOUND("autoop/gp2.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/gp2.wav");
	PRECACHE_SOUND("autoop/llama.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/llama.wav");
	PRECACHE_SOUND("autoop/rob.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/rob.wav");
	PRECACHE_SOUND("autoop/w00t.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/w00t.wav");
	PRECACHE_SOUND("autoop/loop1.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop1.wav");
	PRECACHE_SOUND("autoop/loop2.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop2.wav");
	PRECACHE_SOUND("autoop/loop3.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop3.wav");
	PRECACHE_SOUND("autoop/loop4.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop4.wav");
	PRECACHE_SOUND("autoop/loop5.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop5.wav");
	PRECACHE_SOUND("autoop/loop6.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop6.wav");
	PRECACHE_SOUND("autoop/loop7.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop7.wav");
	PRECACHE_SOUND("autoop/loop8.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop8.wav");
	PRECACHE_SOUND("autoop/loop9.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop9.wav");
	PRECACHE_SOUND("autoop/loop10.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop10.wav");
	PRECACHE_SOUND("autoop/loop11.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop11.wav");
	PRECACHE_SOUND("autoop/loop12.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop12.wav");
	PRECACHE_SOUND("autoop/loop13.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop13.wav");
	PRECACHE_SOUND("autoop/loop14.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop14.wav");
	PRECACHE_SOUND("autoop/loop15.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop15.wav");
	PRECACHE_SOUND("autoop/loop16.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop16.wav");
	PRECACHE_SOUND("autoop/loop17.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop17.wav");
	PRECACHE_SOUND("autoop/loop18.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop18.wav");
	PRECACHE_SOUND("autoop/loop19.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop19.wav");
	PRECACHE_SOUND("autoop/loop20.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop20.wav");
	PRECACHE_SOUND("autoop/loop21.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop21.wav");
	PRECACHE_SOUND("autoop/loop22.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop22.wav");
	PRECACHE_SOUND("autoop/loop23.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop23.wav");
	PRECACHE_SOUND("autoop/loop24.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop24.wav");
	PRECACHE_SOUND("autoop/loop25.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop25.wav");
	PRECACHE_SOUND("autoop/loop26.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop26.wav");
	PRECACHE_SOUND("autoop/loop27.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop27.wav");
	PRECACHE_SOUND("autoop/loop28.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop28.wav");
	PRECACHE_SOUND("autoop/loop29.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop29.wav");
	PRECACHE_SOUND("autoop/loop30.wav");
	ENGINE_FORCE_UNMODIFIED(force_exactfile, Vector ( 0, 0, 0 ), Vector ( 0, 0, 0 ),"autoop/loop30.wav");
	//END SOUNDS ----------

	PRECACHE_SOUND("weapons/gauss2.wav");

	//Models to be stored.
	m_Chain = PRECACHE_MODEL( "sprites/autoop/grapple1.spr" ); 
	m_Fire = PRECACHE_MODEL( "sprites/fthrow.spr" ); 
	g_sModelIndexBloodSpray = PRECACHE_MODEL ("sprites/bloodspray.spr");
	g_sModelIndexBloodDrop = PRECACHE_MODEL ("sprites/blood.spr");

	if(Features.Debug >= 1)
	{
		fp=fopen("sdebug.log","a");
		fprintf(fp, "Precached resources.\n");
		fclose(fp);
	}

}