/*****************************************************\
*	Auto-OP Shodan
*
*	File: linkfunc.cpp
*	Description: Core file.
*
*	(C)Copyright 2001 Rob Harwood All Rights Reserved
*   Based on work by Botman (http://www.planethalflife.com/botman)
*
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

extern HINSTANCE h_Library;

typedef void (FAR *LINK_ENTITY_FUNC)(entvars_t *);


#define LINK_ENTITY_TO_FUNC(mapClassName, mapClassString) \
 extern "C" _declspec( dllexport ) void mapClassName( entvars_t *pev ); \
 void mapClassName( entvars_t *pev ) { \
      static LINK_ENTITY_FUNC otherClassName = NULL; \
      static int skip_this = 0; \
      if (skip_this) return; \
      if (otherClassName == NULL) \
         otherClassName = (LINK_ENTITY_FUNC)GetProcAddress(h_Library, mapClassString); \
      if (otherClassName == NULL) { \
         skip_this = 1; return; \
      } \
      (*otherClassName)(pev); }



// entities for Valve's hl.dll and Standard SDK...
LINK_ENTITY_TO_FUNC(aiscripted_sequence, "aiscripted_sequence");
LINK_ENTITY_TO_FUNC(ambient_generic, "ambient_generic");
LINK_ENTITY_TO_FUNC(ammo_357, "ammo_357");
LINK_ENTITY_TO_FUNC(ammo_9mmAR, "ammo_9mmAR");
LINK_ENTITY_TO_FUNC(ammo_9mmbox, "ammo_9mmbox");
LINK_ENTITY_TO_FUNC(ammo_9mmclip, "ammo_9mmclip");
LINK_ENTITY_TO_FUNC(ammo_ARgrenades, "ammo_ARgrenades");
LINK_ENTITY_TO_FUNC(ammo_buckshot, "ammo_buckshot");
LINK_ENTITY_TO_FUNC(ammo_crossbow, "ammo_crossbow");
LINK_ENTITY_TO_FUNC(ammo_egonclip, "ammo_egonclip");
LINK_ENTITY_TO_FUNC(ammo_gaussclip, "ammo_gaussclip");
LINK_ENTITY_TO_FUNC(ammo_glockclip, "ammo_glockclip");
LINK_ENTITY_TO_FUNC(ammo_mp5clip, "ammo_mp5clip");
LINK_ENTITY_TO_FUNC(ammo_mp5grenades, "ammo_mp5grenades");
LINK_ENTITY_TO_FUNC(ammo_rpgclip, "ammo_rpgclip");
LINK_ENTITY_TO_FUNC(beam, "beam");
LINK_ENTITY_TO_FUNC(bmortar, "bmortar");
LINK_ENTITY_TO_FUNC(bodyque, "bodyque");
LINK_ENTITY_TO_FUNC(button_target, "button_target");
LINK_ENTITY_TO_FUNC(cine_blood, "cine_blood");
LINK_ENTITY_TO_FUNC(controller_energy_ball, "controller_energy_ball");
LINK_ENTITY_TO_FUNC(controller_head_ball, "controller_head_ball");
LINK_ENTITY_TO_FUNC(crossbow_bolt, "crossbow_bolt");
LINK_ENTITY_TO_FUNC(cycler, "cycler");
LINK_ENTITY_TO_FUNC(cycler_prdroid, "cycler_prdroid");
LINK_ENTITY_TO_FUNC(cycler_sprite, "cycler_sprite");
LINK_ENTITY_TO_FUNC(cycler_weapon, "cycler_weapon");
LINK_ENTITY_TO_FUNC(cycler_wreckage, "cycler_wreckage");
LINK_ENTITY_TO_FUNC(DelayedUse, "DelayedUse");
LINK_ENTITY_TO_FUNC(env_beam, "env_beam");
LINK_ENTITY_TO_FUNC(env_beverage, "env_beverage");
LINK_ENTITY_TO_FUNC(env_blood, "env_blood");
LINK_ENTITY_TO_FUNC(env_bubbles, "env_bubbles");
LINK_ENTITY_TO_FUNC(env_debris, "env_debris");
LINK_ENTITY_TO_FUNC(env_explosion, "env_explosion");
LINK_ENTITY_TO_FUNC(env_fade, "env_fade");
LINK_ENTITY_TO_FUNC(env_funnel, "env_funnel");
LINK_ENTITY_TO_FUNC(env_global, "env_global");
LINK_ENTITY_TO_FUNC(env_glow, "env_glow");
LINK_ENTITY_TO_FUNC(env_laser, "env_laser");
LINK_ENTITY_TO_FUNC(env_lightning, "env_lightning");
LINK_ENTITY_TO_FUNC(env_message, "env_message");
LINK_ENTITY_TO_FUNC(env_render, "env_render");
LINK_ENTITY_TO_FUNC(env_shake, "env_shake");
LINK_ENTITY_TO_FUNC(env_shooter, "env_shooter");
LINK_ENTITY_TO_FUNC(env_smoker, "env_smoker");
LINK_ENTITY_TO_FUNC(env_sound, "env_sound");
LINK_ENTITY_TO_FUNC(env_spark, "env_spark");
LINK_ENTITY_TO_FUNC(env_sprite, "env_sprite");
LINK_ENTITY_TO_FUNC(fireanddie, "fireanddie");
LINK_ENTITY_TO_FUNC(func_breakable, "func_breakable");
LINK_ENTITY_TO_FUNC(func_button, "func_button");
LINK_ENTITY_TO_FUNC(func_conveyor, "func_conveyor");
LINK_ENTITY_TO_FUNC(func_door, "func_door");
LINK_ENTITY_TO_FUNC(func_door_rotating, "func_door_rotating");
LINK_ENTITY_TO_FUNC(func_friction, "func_friction");
LINK_ENTITY_TO_FUNC(func_guntarget, "func_guntarget");
LINK_ENTITY_TO_FUNC(func_healthcharger, "func_healthcharger");
LINK_ENTITY_TO_FUNC(func_illusionary, "func_illusionary");
LINK_ENTITY_TO_FUNC(func_ladder, "func_ladder");
LINK_ENTITY_TO_FUNC(func_monsterclip, "func_monsterclip");
LINK_ENTITY_TO_FUNC(func_mortar_field, "func_mortar_field");
LINK_ENTITY_TO_FUNC(func_pendulum, "func_pendulum");
LINK_ENTITY_TO_FUNC(func_plat, "func_plat");
LINK_ENTITY_TO_FUNC(func_platrot, "func_platrot");
LINK_ENTITY_TO_FUNC(func_pushable, "func_pushable");
LINK_ENTITY_TO_FUNC(func_recharge, "func_recharge");
LINK_ENTITY_TO_FUNC(func_rotating, "func_rotating");
LINK_ENTITY_TO_FUNC(func_rot_button, "func_rot_button");
LINK_ENTITY_TO_FUNC(func_tank, "func_tank");
LINK_ENTITY_TO_FUNC(func_tankcontrols, "func_tankcontrols");
LINK_ENTITY_TO_FUNC(func_tanklaser, "func_tanklaser");
LINK_ENTITY_TO_FUNC(func_tankmortar, "func_tankmortar");
LINK_ENTITY_TO_FUNC(func_tankrocket, "func_tankrocket");
LINK_ENTITY_TO_FUNC(func_trackautochange, "func_trackautochange");
LINK_ENTITY_TO_FUNC(func_trackchange, "func_trackchange");
LINK_ENTITY_TO_FUNC(func_tracktrain, "func_tracktrain");
LINK_ENTITY_TO_FUNC(func_train, "func_train");
LINK_ENTITY_TO_FUNC(func_traincontrols, "func_traincontrols");
LINK_ENTITY_TO_FUNC(func_wall, "func_wall");
LINK_ENTITY_TO_FUNC(func_wall_toggle, "func_wall_toggle");
LINK_ENTITY_TO_FUNC(func_water, "func_water");
LINK_ENTITY_TO_FUNC(game_counter, "game_counter");
LINK_ENTITY_TO_FUNC(game_counter_set, "game_counter_set");
LINK_ENTITY_TO_FUNC(game_end, "game_end");
LINK_ENTITY_TO_FUNC(game_player_equip, "game_player_equip");
LINK_ENTITY_TO_FUNC(game_player_hurt, "game_player_hurt");
LINK_ENTITY_TO_FUNC(game_player_team, "game_player_team");
LINK_ENTITY_TO_FUNC(game_score, "game_score");
LINK_ENTITY_TO_FUNC(game_team_master, "game_team_master");
LINK_ENTITY_TO_FUNC(game_team_set, "game_team_set");
LINK_ENTITY_TO_FUNC(game_text, "game_text");
LINK_ENTITY_TO_FUNC(game_zone_player, "game_zone_player");
LINK_ENTITY_TO_FUNC(garg_stomp, "garg_stomp");
LINK_ENTITY_TO_FUNC(gibshooter, "gibshooter");
LINK_ENTITY_TO_FUNC(grenade, "grenade");
LINK_ENTITY_TO_FUNC(hornet, "hornet");
LINK_ENTITY_TO_FUNC(hvr_rocket, "hvr_rocket");
LINK_ENTITY_TO_FUNC(info_bigmomma, "info_bigmomma");
LINK_ENTITY_TO_FUNC(info_intermission, "info_intermission");
LINK_ENTITY_TO_FUNC(info_landmark, "info_landmark");
LINK_ENTITY_TO_FUNC(info_node, "info_node");
LINK_ENTITY_TO_FUNC(info_node_air, "info_node_air");
LINK_ENTITY_TO_FUNC(info_null, "info_null");
LINK_ENTITY_TO_FUNC(info_player_deathmatch, "info_player_deathmatch");
LINK_ENTITY_TO_FUNC(info_player_start, "info_player_start");
LINK_ENTITY_TO_FUNC(info_target, "info_target");
LINK_ENTITY_TO_FUNC(info_teleport_destination, "info_teleport_destination");
LINK_ENTITY_TO_FUNC(infodecal, "infodecal");
LINK_ENTITY_TO_FUNC(item_airtank, "item_airtank");
LINK_ENTITY_TO_FUNC(item_antidote, "item_antidote");
LINK_ENTITY_TO_FUNC(item_battery, "item_battery");
LINK_ENTITY_TO_FUNC(item_healthkit, "item_healthkit");
LINK_ENTITY_TO_FUNC(item_longjump, "item_longjump");
LINK_ENTITY_TO_FUNC(item_security, "item_security");
LINK_ENTITY_TO_FUNC(item_sodacan, "item_sodacan");
LINK_ENTITY_TO_FUNC(item_suit, "item_suit");
LINK_ENTITY_TO_FUNC(laser_spot, "laser_spot");
LINK_ENTITY_TO_FUNC(light, "light");
LINK_ENTITY_TO_FUNC(light_environment, "light_environment");
LINK_ENTITY_TO_FUNC(light_spot, "light_spot");
LINK_ENTITY_TO_FUNC(momentary_door, "momentary_door");
LINK_ENTITY_TO_FUNC(momentary_rot_button, "momentary_rot_button");
LINK_ENTITY_TO_FUNC(monstermaker, "monstermaker");
LINK_ENTITY_TO_FUNC(monster_alien_controller, "monster_alien_controller");
LINK_ENTITY_TO_FUNC(monster_alien_grunt, "monster_alien_grunt");
LINK_ENTITY_TO_FUNC(monster_alien_slave, "monster_alien_slave");
LINK_ENTITY_TO_FUNC(monster_apache, "monster_apache");
LINK_ENTITY_TO_FUNC(monster_babycrab, "monster_babycrab");
LINK_ENTITY_TO_FUNC(monster_barnacle, "monster_barnacle");
LINK_ENTITY_TO_FUNC(monster_barney, "monster_barney");
LINK_ENTITY_TO_FUNC(monster_barney_dead, "monster_barney_dead");
LINK_ENTITY_TO_FUNC(monster_bigmomma, "monster_bigmomma");
LINK_ENTITY_TO_FUNC(monster_bloater, "monster_bloater");
LINK_ENTITY_TO_FUNC(monster_bullchicken, "monster_bullchicken");
LINK_ENTITY_TO_FUNC(monster_cine2_hvyweapons, "monster_cine2_hvyweapons");
LINK_ENTITY_TO_FUNC(monster_cine2_scientist, "monster_cine2_scientist");
LINK_ENTITY_TO_FUNC(monster_cine2_slave, "monster_cine2_slave");
LINK_ENTITY_TO_FUNC(monster_cine3_barney, "monster_cine3_barney");
LINK_ENTITY_TO_FUNC(monster_cine3_scientist, "monster_cine3_scientist");
LINK_ENTITY_TO_FUNC(monster_cine_barney, "monster_cine_barney");
LINK_ENTITY_TO_FUNC(monster_cine_panther, "monster_cine_panther");
LINK_ENTITY_TO_FUNC(monster_cine_scientist, "monster_cine_scientist");
LINK_ENTITY_TO_FUNC(monster_cockroach, "monster_cockroach");
LINK_ENTITY_TO_FUNC(monster_flyer, "monster_flyer");
LINK_ENTITY_TO_FUNC(monster_flyer_flock, "monster_flyer_flock");
LINK_ENTITY_TO_FUNC(monster_furniture, "monster_furniture");
LINK_ENTITY_TO_FUNC(monster_gargantua, "monster_gargantua");
LINK_ENTITY_TO_FUNC(monster_generic, "monster_generic");
LINK_ENTITY_TO_FUNC(monster_gman, "monster_gman");
LINK_ENTITY_TO_FUNC(monster_grunt_repel, "monster_grunt_repel");
LINK_ENTITY_TO_FUNC(monster_headcrab, "monster_headcrab");
LINK_ENTITY_TO_FUNC(monster_hevsuit_dead, "monster_hevsuit_dead");
LINK_ENTITY_TO_FUNC(monster_hgrunt_dead, "monster_hgrunt_dead");
LINK_ENTITY_TO_FUNC(monster_houndeye, "monster_houndeye");
LINK_ENTITY_TO_FUNC(monster_human_assassin, "monster_human_assassin");
LINK_ENTITY_TO_FUNC(monster_human_grunt, "monster_human_grunt");
LINK_ENTITY_TO_FUNC(monster_ichthyosaur, "monster_ichthyosaur");
LINK_ENTITY_TO_FUNC(monster_leech, "monster_leech");
LINK_ENTITY_TO_FUNC(monster_miniturret, "monster_miniturret");
LINK_ENTITY_TO_FUNC(monster_mortar, "monster_mortar");
LINK_ENTITY_TO_FUNC(monster_nihilanth, "monster_nihilanth");
LINK_ENTITY_TO_FUNC(monster_osprey, "monster_osprey");
LINK_ENTITY_TO_FUNC(monster_rat, "monster_rat");
LINK_ENTITY_TO_FUNC(monster_satchel, "monster_satchel");
LINK_ENTITY_TO_FUNC(monster_scientist, "monster_scientist");
LINK_ENTITY_TO_FUNC(monster_scientist_dead, "monster_scientist_dead");
LINK_ENTITY_TO_FUNC(monster_sentry, "monster_sentry");
LINK_ENTITY_TO_FUNC(monster_sitting_scientist, "monster_sitting_scientist");
LINK_ENTITY_TO_FUNC(monster_snark, "monster_snark");
LINK_ENTITY_TO_FUNC(monster_tentacle, "monster_tentacle");
LINK_ENTITY_TO_FUNC(monster_tentaclemaw, "monster_tentaclemaw");
LINK_ENTITY_TO_FUNC(monster_tripmine, "monster_tripmine");
LINK_ENTITY_TO_FUNC(monster_turret, "monster_turret");
LINK_ENTITY_TO_FUNC(monster_vortigaunt, "monster_vortigaunt");
LINK_ENTITY_TO_FUNC(monster_zombie, "monster_zombie");
LINK_ENTITY_TO_FUNC(multisource, "multisource");
LINK_ENTITY_TO_FUNC(multi_manager, "multi_manager");
LINK_ENTITY_TO_FUNC(nihilanth_energy_ball, "nihilanth_energy_ball");
LINK_ENTITY_TO_FUNC(node_viewer, "node_viewer");
LINK_ENTITY_TO_FUNC(node_viewer_fly, "node_viewer_fly");
LINK_ENTITY_TO_FUNC(node_viewer_human, "node_viewer_human");
LINK_ENTITY_TO_FUNC(node_viewer_large, "node_viewer_large");
LINK_ENTITY_TO_FUNC(path_corner, "path_corner");
LINK_ENTITY_TO_FUNC(path_track, "path_track");
LINK_ENTITY_TO_FUNC(player, "player");
LINK_ENTITY_TO_FUNC(player_loadsaved, "player_loadsaved");
LINK_ENTITY_TO_FUNC(player_weaponstrip, "player_weaponstrip");
LINK_ENTITY_TO_FUNC(rpg_rocket, "rpg_rocket");
LINK_ENTITY_TO_FUNC(scripted_sentence, "scripted_sentence");
LINK_ENTITY_TO_FUNC(scripted_sequence, "scripted_sequence");
LINK_ENTITY_TO_FUNC(soundent, "soundent");
LINK_ENTITY_TO_FUNC(spark_shower, "spark_shower");
LINK_ENTITY_TO_FUNC(speaker, "speaker");
LINK_ENTITY_TO_FUNC(squidspit, "squidspit");
LINK_ENTITY_TO_FUNC(streak_spiral, "streak_spiral");
LINK_ENTITY_TO_FUNC(target_cdaudio, "target_cdaudio");
LINK_ENTITY_TO_FUNC(test_effect, "test_effect");
LINK_ENTITY_TO_FUNC(testhull, "testhull");
LINK_ENTITY_TO_FUNC(trigger, "trigger");
LINK_ENTITY_TO_FUNC(trigger_auto, "trigger_auto");
LINK_ENTITY_TO_FUNC(trigger_autosave, "trigger_autosave");
LINK_ENTITY_TO_FUNC(trigger_camera, "trigger_camera");
LINK_ENTITY_TO_FUNC(trigger_cdaudio, "trigger_cdaudio");
LINK_ENTITY_TO_FUNC(trigger_changelevel, "trigger_changelevel");
LINK_ENTITY_TO_FUNC(trigger_changetarget, "trigger_changetarget");
LINK_ENTITY_TO_FUNC(trigger_counter, "trigger_counter");
LINK_ENTITY_TO_FUNC(trigger_endsection, "trigger_endsection");
LINK_ENTITY_TO_FUNC(trigger_gravity, "trigger_gravity");
LINK_ENTITY_TO_FUNC(trigger_hurt, "trigger_hurt");
LINK_ENTITY_TO_FUNC(trigger_monsterjump, "trigger_monsterjump");
LINK_ENTITY_TO_FUNC(trigger_multiple, "trigger_multiple");
LINK_ENTITY_TO_FUNC(trigger_once, "trigger_once");
LINK_ENTITY_TO_FUNC(trigger_push, "trigger_push");
LINK_ENTITY_TO_FUNC(trigger_relay, "trigger_relay");
LINK_ENTITY_TO_FUNC(trigger_teleport, "trigger_teleport");
LINK_ENTITY_TO_FUNC(trigger_transition, "trigger_transition");
LINK_ENTITY_TO_FUNC(weapon_357, "weapon_357");
LINK_ENTITY_TO_FUNC(weapon_9mmAR, "weapon_9mmAR");
LINK_ENTITY_TO_FUNC(weapon_9mmhandgun, "weapon_9mmhandgun");
LINK_ENTITY_TO_FUNC(weapon_crossbow, "weapon_crossbow");
LINK_ENTITY_TO_FUNC(weapon_crowbar, "weapon_crowbar");
LINK_ENTITY_TO_FUNC(weapon_egon, "weapon_egon");
LINK_ENTITY_TO_FUNC(weapon_gauss, "weapon_gauss");
LINK_ENTITY_TO_FUNC(weapon_glock, "weapon_glock");
LINK_ENTITY_TO_FUNC(weapon_handgrenade, "weapon_handgrenade");
LINK_ENTITY_TO_FUNC(weapon_hornetgun, "weapon_hornetgun");
LINK_ENTITY_TO_FUNC(weapon_mp5, "weapon_mp5");
LINK_ENTITY_TO_FUNC(weapon_python, "weapon_python");
LINK_ENTITY_TO_FUNC(weapon_rpg, "weapon_rpg");
LINK_ENTITY_TO_FUNC(weapon_satchel, "weapon_satchel");
LINK_ENTITY_TO_FUNC(weapon_shotgun, "weapon_shotgun");
LINK_ENTITY_TO_FUNC(weapon_snark, "weapon_snark");
LINK_ENTITY_TO_FUNC(weapon_tripmine, "weapon_tripmine");
LINK_ENTITY_TO_FUNC(weaponbox, "weaponbox");
LINK_ENTITY_TO_FUNC(worldspawn, "worldspawn");
LINK_ENTITY_TO_FUNC(world_items, "world_items");
LINK_ENTITY_TO_FUNC(xen_hair, "xen_hair");
LINK_ENTITY_TO_FUNC(xen_hull, "xen_hull");
LINK_ENTITY_TO_FUNC(xen_plantlight, "xen_plantlight");
LINK_ENTITY_TO_FUNC(xen_spore_large, "xen_spore_large");
LINK_ENTITY_TO_FUNC(xen_spore_medium, "xen_spore_medium");
LINK_ENTITY_TO_FUNC(xen_spore_small, "xen_spore_small");
LINK_ENTITY_TO_FUNC(xen_tree, "xen_tree");
LINK_ENTITY_TO_FUNC(xen_ttrigger, "xen_ttrigger");

// entities for botman's bots
// duplicate LINK_ENTITY_TO_FUNC(bot, "bot");
LINK_ENTITY_TO_FUNC(entity_botcam, "entity_botcam");

// entities for TFC (1.1.0.6)
LINK_ENTITY_TO_FUNC(CTF_Map,"CTF_Map"); 
LINK_ENTITY_TO_FUNC(info_areadef,"info_areadef");
LINK_ENTITY_TO_FUNC(info_tf_teamcheck,"info_tf_teamcheck");
LINK_ENTITY_TO_FUNC(info_tf_teamset,"info_tf_teamset"); 
LINK_ENTITY_TO_FUNC(func_nogrenades,"func_nogrenades");
LINK_ENTITY_TO_FUNC(building_dispenser, "building_dispenser");
LINK_ENTITY_TO_FUNC(building_sentrygun, "building_sentrygun");
LINK_ENTITY_TO_FUNC(building_sentrygun_base, "building_sentrygun_base");
LINK_ENTITY_TO_FUNC(detpack, "detpack");
LINK_ENTITY_TO_FUNC(func_nobuild, "func_nobuild");
LINK_ENTITY_TO_FUNC(ghost, "ghost");
LINK_ENTITY_TO_FUNC(info_player_teamspawn, "info_player_teamspawn");
LINK_ENTITY_TO_FUNC(info_tfdetect, "info_tfdetect");
LINK_ENTITY_TO_FUNC(info_tfgoal, "info_tfgoal");
LINK_ENTITY_TO_FUNC(info_tfgoal_timer, "info_tfgoal_timer");
LINK_ENTITY_TO_FUNC(item_armor1, "item_armor1");
LINK_ENTITY_TO_FUNC(item_armor2, "item_armor2");
LINK_ENTITY_TO_FUNC(item_armor3, "item_armor3");
LINK_ENTITY_TO_FUNC(item_artifact_envirosuit, "item_artifact_envirosuit");
LINK_ENTITY_TO_FUNC(item_artifact_invisibility, "item_artifact_invisibility");
LINK_ENTITY_TO_FUNC(item_artifact_invulnerability, "item_artifact_invulnerability");
LINK_ENTITY_TO_FUNC(item_artifact_super_damage, "item_artifact_super_damage");
LINK_ENTITY_TO_FUNC(item_cells, "item_cells");
LINK_ENTITY_TO_FUNC(item_health, "item_health");
LINK_ENTITY_TO_FUNC(item_rockets, "item_rockets");
LINK_ENTITY_TO_FUNC(item_shells, "item_shells");
LINK_ENTITY_TO_FUNC(item_spikes, "item_spikes");
LINK_ENTITY_TO_FUNC(item_tfgoal, "item_tfgoal");
LINK_ENTITY_TO_FUNC(i_p_t, "i_p_t");
LINK_ENTITY_TO_FUNC(i_t_g, "i_t_g");
LINK_ENTITY_TO_FUNC(i_t_t, "i_t_t");
LINK_ENTITY_TO_FUNC(teledeath, "teledeath");
LINK_ENTITY_TO_FUNC(tf_ammo_rpgclip, "tf_ammo_rpgclip");
LINK_ENTITY_TO_FUNC(tf_flame, "tf_flame");
LINK_ENTITY_TO_FUNC(tf_flamethrower_burst, "tf_flamethrower_burst");
LINK_ENTITY_TO_FUNC(tf_gl_grenade, "tf_gl_grenade");
LINK_ENTITY_TO_FUNC(tf_ic_rocket, "tf_ic_rocket");
LINK_ENTITY_TO_FUNC(tf_nailgun_nail, "tf_nailgun_nail");
LINK_ENTITY_TO_FUNC(tf_rpg_rocket, "tf_rpg_rocket");
LINK_ENTITY_TO_FUNC(tf_weapon_ac, "tf_weapon_ac");
LINK_ENTITY_TO_FUNC(tf_weapon_autorifle, "tf_weapon_autorifle");
LINK_ENTITY_TO_FUNC(tf_weapon_axe, "tf_weapon_axe");
LINK_ENTITY_TO_FUNC(tf_weapon_caltrop, "tf_weapon_caltrop");
LINK_ENTITY_TO_FUNC(tf_weapon_caltropgrenade, "tf_weapon_caltropgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_concussiongrenade, "tf_weapon_concussiongrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_empgrenade, "tf_weapon_empgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_flamethrower, "tf_weapon_flamethrower");
LINK_ENTITY_TO_FUNC(tf_weapon_gasgrenade, "tf_weapon_gasgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_genericprimedgrenade, "tf_weapon_genericprimedgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_gl, "tf_weapon_gl");
LINK_ENTITY_TO_FUNC(tf_weapon_ic, "tf_weapon_ic");
LINK_ENTITY_TO_FUNC(tf_weapon_knife, "tf_weapon_knife");
LINK_ENTITY_TO_FUNC(tf_weapon_medikit, "tf_weapon_medikit");
LINK_ENTITY_TO_FUNC(tf_weapon_mirvbomblet, "tf_weapon_mirvbomblet");
LINK_ENTITY_TO_FUNC(tf_weapon_mirvgrenade, "tf_weapon_mirvgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_nailgrenade, "tf_weapon_nailgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_napalmgrenade, "tf_weapon_napalmgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_ng, "tf_weapon_ng");
LINK_ENTITY_TO_FUNC(tf_weapon_normalgrenade, "tf_weapon_normalgrenade");
LINK_ENTITY_TO_FUNC(tf_weapon_pl, "tf_weapon_pl");
LINK_ENTITY_TO_FUNC(tf_weapon_railgun, "tf_weapon_railgun");
LINK_ENTITY_TO_FUNC(tf_weapon_rpg, "tf_weapon_rpg");
LINK_ENTITY_TO_FUNC(tf_weapon_shotgun, "tf_weapon_shotgun");
LINK_ENTITY_TO_FUNC(tf_weapon_sniperrifle, "tf_weapon_sniperrifle");
LINK_ENTITY_TO_FUNC(tf_weapon_spanner, "tf_weapon_spanner");
LINK_ENTITY_TO_FUNC(tf_weapon_superng, "tf_weapon_superng");
LINK_ENTITY_TO_FUNC(tf_weapon_supershotgun, "tf_weapon_supershotgun");
LINK_ENTITY_TO_FUNC(tf_weapon_tranq, "tf_weapon_tranq");
LINK_ENTITY_TO_FUNC(timer, "timer");