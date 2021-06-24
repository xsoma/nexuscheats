// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "configs.h"
#include "base64.h"
#include "..\includes.hpp"
#include "..\utils\util.hpp"
#include <shlobj.h>
#include "../cheats/menu.h"
#include <ostream>
#include "../default_text_config.h"
#include "../cheats/inventory changer/inventorychanger.h"
#include "../protobuffs/Protobuffs.h"

std::unordered_map <std::string, float[4]> colors;

C_ConfigManager* cfg_manager = new C_ConfigManager();
Config g_cfg;

item_setting* get_by_definition_index(const int definition_index)
{
	auto it = std::find_if(std::begin(g_cfg.skins.skinChanger), std::end(g_cfg.skins.skinChanger), [definition_index](const item_setting& e)
	{
		return e.itemId == definition_index;
	});

	return it == std::end(g_cfg.skins.skinChanger) ? nullptr : &*it;
}

void C_ConfigManager::setup()
{
	
	setup_item(&g_cfg.ragebot.enable, false, crypt_str("regitbot_enable"));
	setup_item(&g_cfg.ragebot.silent_aim, false, crypt_str("regitbot_silent_aim"));
	setup_item(&g_cfg.ragebot.field_of_view, 1, crypt_str("regitbot_fov"));
	setup_item(&g_cfg.ragebot.autowall, false, crypt_str("regitbot_autowall"));
	setup_item(&g_cfg.ragebot.zeus_bot, false, crypt_str("regitbot_zeus_bot"));
	setup_item(&g_cfg.ragebot.knife_bot, false, crypt_str("regitbot_knife_bot"));
	setup_item(&g_cfg.ragebot.slow_walk_velocity, 0, crypt_str("regitbot_slow_walk_velocity"));
	setup_item(&g_cfg.ragebot.autoshoot, false, crypt_str("regitbot_autoshoot"));
	setup_item(&g_cfg.ragebot.double_tap, false, crypt_str("regitbot_double_tap"));
	setup_item(&g_cfg.ragebot.slow_teleport, false, crypt_str("regitbot_slow_teleport"));
	setup_item(&g_cfg.ragebot.double_tap_key, key_bind(TOGGLE), crypt_str("regitbot_double_tap_key"));
	setup_item(&g_cfg.ragebot.autoscope, false, crypt_str("regitbot_autoscope"));
	setup_item(&g_cfg.ragebot.safe_point_key, key_bind(HOLD), crypt_str("regitbot_safe_point_key"));
	setup_item(&g_cfg.ragebot.body_aim_key, key_bind(HOLD), crypt_str("regitbot_body_aim_key"));
	setup_item(&g_cfg.ragebot.pitch_antiaim_correction, false, crypt_str("regitbot_pitch_antiaim_correction"));

	for (auto i = 0; i < 8; i++)
	{
		setup_item(&g_cfg.ragebot.weapon[i].selection_type, 0, std::to_string(i) + crypt_str("regitbot_selection_type"));
		setup_item(&g_cfg.ragebot.weapon[i].autostop, false, std::to_string(i) + crypt_str("Ragebot_automaticstop"));
		setup_item(&g_cfg.ragebot.weapon[i].autostop_modifiers, 8, std::to_string(i) + crypt_str("regitbot_autostop_conditions"));
		setup_item(&g_cfg.ragebot.weapon[i].double_tap_hitchance, false, std::to_string(i) + crypt_str("regitbot_double_tap_hitchance"));
		setup_item(&g_cfg.ragebot.weapon[i].double_tap_hitchance_amount, 1, std::to_string(i) + crypt_str("regitbot_double_tap_hitchance_amount"));
		setup_item(&g_cfg.ragebot.weapon[i].hitchance, false, std::to_string(i) + crypt_str("regitbot_hitchance"));
		setup_item(&g_cfg.ragebot.weapon[i].hitchance_amount, 1, std::to_string(i) + crypt_str("regitbot_hitchance_amount"));
		setup_item(&g_cfg.ragebot.weapon[i].minimum_visible_damage, 1, std::to_string(i) + crypt_str("regitbot_minimum_visible_damage"));
		setup_item(&g_cfg.ragebot.weapon[i].minimum_damage, 1, std::to_string(i) + crypt_str("regitbot_minimum_damage"));
		setup_item(&g_cfg.ragebot.weapon[i].damage_override_key, key_bind(HOLD), std::to_string(i) + crypt_str("regitbot_damage_override_key"));
		setup_item(&g_cfg.ragebot.weapon[i].minimum_override_damage, 1, std::to_string(i) + crypt_str("regitbot_minimum_override_damage"));
		setup_item(&g_cfg.ragebot.weapon[i].hitboxes, 11, std::to_string(i) + crypt_str("regitbot_hitboxes"));
		setup_item(&g_cfg.ragebot.weapon[i].static_point_scale, false, std::to_string(i) + crypt_str("regitbot_static_point_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].head_scale, 0.0f, std::to_string(i) + crypt_str("regitbot_head_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].body_scale, 0.0f, std::to_string(i) + crypt_str("regitbot_body_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].max_misses, false, std::to_string(i) + crypt_str("regitbot_max_misses"));
		setup_item(&g_cfg.ragebot.weapon[i].max_misses_amount, 0, std::to_string(i) + crypt_str("regitbot_max_misses_amount"));
		setup_item(&g_cfg.ragebot.weapon[i].prefer_safe_points, false, std::to_string(i) + crypt_str("regitbot_prefer_safe_points"));
		setup_item(&g_cfg.ragebot.weapon[i].prefer_body_aim, false, std::to_string(i) + crypt_str("regitbot_prefer_body_aim"));
		setup_item(&g_cfg.ragebot.weapon[i].static_point_scale, false, std::to_string(i) + crypt_str("Ragebot.static_point_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].head_scale, 0.0f, std::to_string(i) + crypt_str("Ragebot.head_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].body_scale, 0.0f, std::to_string(i) + crypt_str("Ragebot.body_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].multipoints, false, std::to_string(i) + crypt_str("Ragebot.multipoints"));
		setup_item(&g_cfg.ragebot.weapon[i].multipoints_hitboxes, 9, std::to_string(i) + crypt_str("Ragebot.multipoints_hitboxes"));
		setup_item(&g_cfg.ragebot.weapon[i].multipoints_scale, 0.0f, std::to_string(i) + crypt_str("Ragebot.multipoints_scale"));
		setup_item(&g_cfg.ragebot.weapon[i].stomach_scale, 0.0f, std::to_string(i) + crypt_str("Ragebot.stomach_scale"));
	}

	setup_item(&g_cfg.legitbot.enabled, false, crypt_str("Legitbot_enabled"));
	setup_item(&g_cfg.legitbot.friendly_fire, false, crypt_str("Legitbot_friendly_fire"));
	setup_item(&g_cfg.legitbot.autopistol, false, crypt_str("Legitbot_autopistol"));
	setup_item(&g_cfg.legitbot.autoscope, false, crypt_str("Legitbot_autoscope"));
	setup_item(&g_cfg.legitbot.unscope, false, crypt_str("Legitbot_unscope"));
	setup_item(&g_cfg.legitbot.sniper_in_zoom_only, false, crypt_str("Legitbot_sniper_in_zoom_only"));
	setup_item(&g_cfg.legitbot.do_if_local_flashed, false, crypt_str("Legitbot_do_if_local_flashed"));
	setup_item(&g_cfg.legitbot.do_if_local_in_air, false, crypt_str("Legitbot_do_if_local_in_air"));
	setup_item(&g_cfg.legitbot.do_if_enemy_in_smoke, false, crypt_str("Legitbot_do_if_enemy_in_smoke"));
	setup_item(&g_cfg.legitbot.backtrackl, false, crypt_str("Legitbot_backtrackl"));
	setup_item(&g_cfg.legitbot.legit_resolver, false, crypt_str("Legitbot_legit_resolver"));
	setup_item(&g_cfg.legitbot.autofire_delay, 0, crypt_str("Legitbot_autofire_delay"));
	setup_item(&g_cfg.legitbot.autofire_key, key_bind(HOLD), crypt_str("Legitbot_autofire_key"));
	setup_item(&g_cfg.legitbot.key, key_bind(HOLD), crypt_str("Legitbot_key"));

	for (auto i = 0; i < 5; i++)
	{
		setup_item(&g_cfg.legitbot.weapon[i].priority, 0, std::to_string(i) + crypt_str("Legitbot_rcs"));
		setup_item(&g_cfg.legitbot.weapon[i].auto_stop, false, std::to_string(i) + crypt_str("Legitbot_auto_stop"));
		setup_item(&g_cfg.legitbot.weapon[i].fov_type, 0, std::to_string(i) + crypt_str("Legitbot_fov_type"));
		setup_item(&g_cfg.legitbot.weapon[i].fov, 0.0f, std::to_string(i) + crypt_str("Legitbot_fov"));
		setup_item(&g_cfg.legitbot.weapon[i].smooth_type, 0, std::to_string(i) + crypt_str("Legitbot_smooth_type"));
		setup_item(&g_cfg.legitbot.weapon[i].smooth, 1.0f, std::to_string(i) + crypt_str("Legitbot_smooth"));
		setup_item(&g_cfg.legitbot.weapon[i].silent_fov, 0.0f, std::to_string(i) + crypt_str("Legitbot_silent_fov"));
		setup_item(&g_cfg.legitbot.weapon[i].rcs_type, 0, std::to_string(i) + crypt_str("Legitbot_rcs_type"));
		setup_item(&g_cfg.legitbot.weapon[i].rcs, 0.0f, std::to_string(i) + crypt_str("Legitbot_rcs"));
		setup_item(&g_cfg.legitbot.weapon[i].custom_rcs_smooth, 0.0f, std::to_string(i) + crypt_str("Legitbot_custom_rcs_smooth"));
		setup_item(&g_cfg.legitbot.weapon[i].rcs, 0.0f, std::to_string(i) + crypt_str("Legitbot_custom_rcs"));
		setup_item(&g_cfg.legitbot.weapon[i].custom_rcs_fov, 0.0f, std::to_string(i) + crypt_str("Legitbot_custom_rcs_fov"));
		setup_item(&g_cfg.legitbot.weapon[i].awall_dmg, 0, std::to_string(i) + crypt_str("Legitbot_awall_dmg"));
		setup_item(&g_cfg.legitbot.weapon[i].target_switch_delay, 0.0f, std::to_string(i) + crypt_str("Legitbot_target_switch_delay"));
		setup_item(&g_cfg.legitbot.weapon[i].backtrack_ticks, 0, std::to_string(i) + crypt_str("Legitbot_backtrack_ticks"));
		setup_item(&g_cfg.legitbot.weapon[i].autofire_hitchance, 0, std::to_string(i) + crypt_str("Legitbot_autofire_hitchance"));
	}

	setup_item(&g_cfg.antiaim.enable, false, crypt_str("aa_enable"));
	setup_item(&g_cfg.antiaim.antiaim_type, 0, crypt_str("aa_antiaim_type"));
	setup_item(&g_cfg.antiaim.lagsync, false, crypt_str("Antiaim.lagsync"));
	setup_item(&g_cfg.antiaim.hide_shots, false, crypt_str("regitbot_hide_shots"));
	setup_item(&g_cfg.antiaim.hide_shots_key, key_bind(TOGGLE), crypt_str("regitbot_hide_shots_key"));
	setup_item(&g_cfg.antiaim.desync, 0, crypt_str("aa_desync"));
	setup_item(&g_cfg.antiaim.legit_lby_type, 0, crypt_str("aa_legit_lby_type"));
	setup_item(&g_cfg.antiaim.lby_type, 0, crypt_str("aa_lby_type"));
	setup_item(&g_cfg.antiaim.manual_back, key_bind(TOGGLE), crypt_str("aa_manual_back"));
	setup_item(&g_cfg.antiaim.manual_left, key_bind(TOGGLE), crypt_str("aa_manual_left"));
	setup_item(&g_cfg.antiaim.manual_right, key_bind(TOGGLE), crypt_str("aa_manual_right"));
	setup_item(&g_cfg.antiaim.flip_desync, key_bind(TOGGLE), crypt_str("aa_flip_desync"));
	setup_item(&g_cfg.antiaim.flip_indicator, false, crypt_str("aa_flip_indicator"));
	setup_item(&g_cfg.antiaim.flip_indicator_color, Color(255, 255, 255), crypt_str("aa_flip_indicator_color"));
	setup_item(&g_cfg.esp.scopecolor, Color(117, 122, 202), crypt_str("Esp.scopecolor"));
	setup_item(&g_cfg.misc.newscope, false, crypt_str("miscnewscope"));
	setup_item(&g_cfg.antiaim.fakelag, false, crypt_str("aa_fake_lag"));
	setup_item(&g_cfg.antiaim.fakelag_type, 0, crypt_str("aa_fake_lag_type"));
	setup_item(&g_cfg.antiaim.fakelag_enablers, 14, crypt_str("aa_fake_lag_enablers"));
	setup_item(&g_cfg.antiaim.fakelag_amount, 1, crypt_str("aa_fake_lag_limit"));
	setup_item(&g_cfg.antiaim.triggers_fakelag_amount, 1, crypt_str("aa_triggers_fakelag_limit"));


	setup_item(&g_cfg.profilechanger.enable, false, ("profilechanger.enable"));
	setup_item(&g_cfg.profilechanger.profile_friend, 0, ("profilechanger.profile_friend"));
	setup_item(&g_cfg.profilechanger.profile_teach, 0, ("profilechanger.profile_teach"));
	setup_item(&g_cfg.profilechanger.profile_leader, 0, ("profilechanger.profile_leader"));
	setup_item(&g_cfg.profilechanger.profile_rank, 0, ("profilechanger.profile_rank"));
	setup_item(&g_cfg.profilechanger.profile_rank_wins, 0, ("profilechanger.profile_rank_wins"));
	setup_item(&g_cfg.profilechanger.profile_lvl, 0, ("profilechanger.profile_lvl"));
	setup_item(&g_cfg.profilechanger.profile_xp, 0, ("profilechanger.profile_xp"));
	setup_item(&g_cfg.profilechanger.r_rank, 0, ("profilechanger.r_rank"));
	setup_item(&g_cfg.profilechanger.r_wins, 0, ("profilechanger.r_wins"));
	setup_item(&g_cfg.profilechanger.t_rank, 0, ("profilechanger.t_rank"));
	setup_item(&g_cfg.profilechanger.t_wins, 0, ("profilechanger.t_wins"));

	for (auto i = 0; i < 4; i++)
	{
		setup_item(&g_cfg.antiaim.type[i].pitch, 0, std::to_string(i) + crypt_str("aa_pitch"));
		setup_item(&g_cfg.antiaim.type[i].base_angle, 0, std::to_string(i) + crypt_str("aa_base_angle"));
		setup_item(&g_cfg.antiaim.type[i].yaw, 0, std::to_string(i) + crypt_str("aa_yaw"));
		setup_item(&g_cfg.antiaim.type[i].range, 1, std::to_string(i) + crypt_str("aa_range"));
		setup_item(&g_cfg.antiaim.type[i].speed, 1, std::to_string(i) + crypt_str("aa_speed"));
		setup_item(&g_cfg.antiaim.type[i].desync, 0, std::to_string(i) + crypt_str("aa_desync"));
		setup_item(&g_cfg.antiaim.type[i].desync_range, 60, std::to_string(i) + crypt_str("aa_desync_range"));
		setup_item(&g_cfg.antiaim.type[i].inverted_desync_range, 60, std::to_string(i) + crypt_str("aa_inverted_desync_range"));
		setup_item(&g_cfg.antiaim.type[i].body_lean, 0, std::to_string(i) + crypt_str("aa_body_lean"));
		setup_item(&g_cfg.antiaim.type[i].inverted_body_lean, 0, std::to_string(i) + crypt_str("aa_inverted_body_lean"));
	}

	setup_item(&g_cfg.radar.render_local, false, crypt_str("play_render_local"));
	setup_item(&g_cfg.radar.render_enemy, false, crypt_str("play_render_enemy"));
	setup_item(&g_cfg.radar.render_team, false, crypt_str("play_render_team"));
	setup_item(&g_cfg.radar.render_planted_c4, false, crypt_str("play_render_planted_c4"));
	setup_item(&g_cfg.radar.render_dropped_c4, false, crypt_str("play_render_dropped_c4"));
	setup_item(&g_cfg.radar.render_health, false, crypt_str("play_render_health"));

	setup_item(&g_cfg.player.enable, false, crypt_str("play_enable"));
	setup_item(&g_cfg.player.arrows_color, Color(255, 255, 255), crypt_str("play_arrows_color"));
	setup_item(&g_cfg.player.arrows, false, crypt_str("play_arrows"));
	setup_item(&g_cfg.player.distance, 1, crypt_str("play_arrows_distance"));
	setup_item(&g_cfg.player.size, 1, crypt_str("play_arrows_size"));
	setup_item(&g_cfg.player.show_multi_points, false, crypt_str("play_show_multi_points"));
	setup_item(&g_cfg.player.show_multi_points_color, Color(255, 255, 255), crypt_str("play_show_multi_points_color"));
	setup_item(&g_cfg.player.lag_hitbox, false, crypt_str("play_lag_hitbox"));
	setup_item(&g_cfg.player.lag_hitbox_color, Color(255, 255, 255), crypt_str("play_lag_hitbox_color"));
	setup_item(&g_cfg.player.player_model_t, 0, crypt_str("wallhack_player_model_t"));
	setup_item(&g_cfg.player.player_model_ct, 0, crypt_str("wallhack_player_model_ct"));
	setup_item(&g_cfg.player.local_chams_type, 0, crypt_str("play_local_chams_type"));
	setup_item(&g_cfg.player.fake_chams_enable, false, crypt_str("play_fake_chams_enable"));
	setup_item(&g_cfg.player.visualize_lag, false, crypt_str("play_vizualize_lag"));
	setup_item(&g_cfg.player.layered, false, crypt_str("play_layered"));
	setup_item(&g_cfg.player.fake_chams_color, Color(255, 255, 255), crypt_str("play_fake_chams_color"));
	setup_item(&g_cfg.player.fake_chams_type, 0, crypt_str("play_fake_chams_type"));
	setup_item(&g_cfg.player.fake_double_material, false, crypt_str("play_fake_double_material"));
	setup_item(&g_cfg.player.fake_double_material_color, Color(255, 255, 255), crypt_str("play_fake_double_material_color"));
	setup_item(&g_cfg.player.fake_animated_material, false, crypt_str("play_fake_animated_material"));
	setup_item(&g_cfg.player.fake_animated_material_color, Color(255, 255, 255), crypt_str("play_fake_animated_material_color"));
	setup_item(&g_cfg.player.backtrack_chams, false, crypt_str("play_backtrack_chams"));
	setup_item(&g_cfg.player.backtrack_chams_material, 0, crypt_str("play_backtrack_chams_material"));
	setup_item(&g_cfg.player.backtrack_chams_color, Color(255, 255, 255), crypt_str("play_backtrack_chams_color"));
	setup_item(&g_cfg.player.transparency_in_scope, false, crypt_str("play_transparency_in_scope"));
	setup_item(&g_cfg.player.transparency_in_scope_amount, 1.0f, crypt_str("play_transparency_in_scope_amount"));

	for (auto i = 0; i < 3; i++)
	{
		setup_item(&g_cfg.player.type[i].box, false, std::to_string(i) + crypt_str("play_box"));
		setup_item(&g_cfg.player.type[i].box_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_box_color"));
		setup_item(&g_cfg.player.type[i].name, false, std::to_string(i) + crypt_str("play_name"));
		setup_item(&g_cfg.player.type[i].name_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_name_color"));
		setup_item(&g_cfg.player.type[i].health, false, std::to_string(i) + crypt_str("play_health"));
		setup_item(&g_cfg.player.type[i].custom_health_color, true, std::to_string(i) + crypt_str("play_custom_health_color"));
		setup_item(&g_cfg.player.type[i].health_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_health_color"));
		setup_item(&g_cfg.player.type[i].weapon, 15, std::to_string(i) + crypt_str("play_weapon"));
		setup_item(&g_cfg.player.type[i].weapon_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_weapon_bar_color"));
		setup_item(&g_cfg.player.type[i].skeleton, false, std::to_string(i) + crypt_str("play_skeleton"));
		setup_item(&g_cfg.player.type[i].skeleton_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_skeleton_color"));
		setup_item(&g_cfg.player.type[i].ammo, false, std::to_string(i) + crypt_str("play_ammo"));
		setup_item(&g_cfg.player.type[i].ammobar_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_ammobar_color"));
		setup_item(&g_cfg.player.type[i].snap_lines, false, std::to_string(i) + crypt_str("play_snap_lines"));
		setup_item(&g_cfg.player.type[i].snap_lines_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_snap_lines_color"));
		setup_item(&g_cfg.player.type[i].footsteps, false, std::to_string(i) + crypt_str("play_footsteps"));
		setup_item(&g_cfg.player.type[i].footsteps_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_footsteps_color"));
		setup_item(&g_cfg.player.type[i].thickness, 0, std::to_string(i) + crypt_str("play_thickness"));
		setup_item(&g_cfg.player.type[i].radius, 0, std::to_string(i) + crypt_str("play_radius"));
		setup_item(&g_cfg.player.type[i].glow, false, std::to_string(i) + crypt_str("play_glow"));
		setup_item(&g_cfg.player.type[i].glow_type, 0, std::to_string(i) + crypt_str("play_glow_type"));
		setup_item(&g_cfg.player.type[i].glow_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_glow_color"));
		setup_item(&g_cfg.player.type[i].chams, 5, std::to_string(i) + crypt_str("play_chams"));//parece q achei o problema em
		setup_item(&g_cfg.player.type[i].chams_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_chams_color"));
		setup_item(&g_cfg.player.type[i].xqz_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_xqz_color"));
		setup_item(&g_cfg.player.type[i].chams_type, 0, std::to_string(i) + crypt_str("play_chams_type"));
		setup_item(&g_cfg.player.type[i].double_material, false, std::to_string(i) + crypt_str("play_double_material"));
		setup_item(&g_cfg.player.type[i].double_material_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_double_material_color"));
		setup_item(&g_cfg.player.type[i].animated_material, false, std::to_string(i) + crypt_str("play_animated_material"));
		setup_item(&g_cfg.player.type[i].animated_material_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_animated_material_color"));
		setup_item(&g_cfg.player.type[i].ragdoll_chams, false, std::to_string(i) + crypt_str("play_ragdoll_chams"));
		setup_item(&g_cfg.player.type[i].ragdoll_chams_material, 0, std::to_string(i) + crypt_str("play_ragdoll_chams_material"));
		setup_item(&g_cfg.player.type[i].ragdoll_chams_color, Color(255, 255, 255), std::to_string(i) + crypt_str("play_ragdoll_chams_color"));
		setup_item(&g_cfg.player.type[i].flags, 15, std::to_string(i) + crypt_str("play_esp_flags"));
	}

	setup_item(&g_cfg.esp.indicators, 15, crypt_str("wallhack_rage_indicators"));

	setup_item(&g_cfg.esp.removals, 15, crypt_str("wallhack_world_removals"));
	setup_item(&g_cfg.esp.fix_zoom_sensivity, false, crypt_str("wallhack_fix_zoom_sensivity"));
	setup_item(&g_cfg.esp.velocity_graph, false, crypt_str("wallhack_velocity_graph"));

	setup_item(&g_cfg.misc.thirdperson_toggle, key_bind(TOGGLE), crypt_str("miscellaneous_thirdperson_toggle"));
	setup_item(&g_cfg.misc.thirdperson_when_spectating, false, crypt_str("miscellaneous_thirdperson_when_spectating"));
	setup_item(&g_cfg.misc.tp_grenade, false, crypt_str("miscellaneous_tp_grenade"));
	setup_item(&g_cfg.misc.nadecu, false, crypt_str("Grenade_Predict"));
	setup_item(&g_cfg.misc.forcecrosshair, false, crypt_str("forcecrosshair"));
	setup_item(&g_cfg.misc.thirdperson_distance, 100, crypt_str("miscellaneous_thirdperson_distance"));
	setup_item(&g_cfg.esp.grenade_prediction, false, crypt_str("wallhack_grenade_prediction"));
	setup_item(&g_cfg.esp.on_click , false, crypt_str("wallhack_on_click"));
	setup_item(&g_cfg.esp.grenade_prediction_color, Color(255, 255, 255), crypt_str("wallhack_grenade_prediction_color"));
	setup_item(&g_cfg.esp.grenade_prediction_tracer_color, Color(255, 255, 255), crypt_str("wallhack_grenade_prediction_tracer_color"));
	setup_item(&g_cfg.esp.projectiles, false, crypt_str("wallhack_projectiles"));
	setup_item(&g_cfg.esp.projectiles_color, Color(255, 255, 255), crypt_str("wallhack_projectiles_color"));
	setup_item(&g_cfg.esp.molotov_timer, false, crypt_str("wallhack_molotov_timer"));
	setup_item(&g_cfg.esp.molotov_timer_color, Color(255, 255, 255), crypt_str("wallhack_molotov_timer_color"));
	setup_item(&g_cfg.esp.smoke_timer, false, crypt_str("wallhack_smoke_timer"));
	setup_item(&g_cfg.esp.smoke_timer_color, Color(255, 255, 255), crypt_str("wallhack_smoke_timer_color"));
	setup_item(&g_cfg.esp.bomb_timer, false, crypt_str("wallhack_bomb_timer"));
	setup_item(&g_cfg.esp.bright, false, crypt_str("wallhack_bright"));
	setup_item(&g_cfg.esp.nightmode, false, crypt_str("wallhack_nightmode"));
	setup_item(&g_cfg.esp.world_color, Color(255, 255, 255), crypt_str("wallhack_world_color"));
	setup_item(&g_cfg.esp.world_color, Color(255, 255, 255), crypt_str("wallhack_world_color"));
	setup_item(&g_cfg.esp.props_color, Color(255, 255, 255), crypt_str("wallhack_props_color"));
	setup_item(&g_cfg.esp.skybox, 0, crypt_str("wallhack_skybox"));
	setup_item(&g_cfg.esp.custom_skybox, crypt_str(""), crypt_str("wallhack_custom_skybox"));
	setup_item(&g_cfg.esp.skybox_color, Color(255, 255, 255), crypt_str("wallhack_skybox_color"));
	setup_item(&g_cfg.esp.client_bullet_impacts, false, crypt_str("wallhack_client_bullet_impacts"));
	setup_item(&g_cfg.esp.client_bullet_impacts_color, Color(255, 255, 255), crypt_str("wallhack_client_bullet_impacts_color"));
	setup_item(&g_cfg.esp.server_bullet_impacts, false, crypt_str("wallhack_server_bullet_impacts"));
	setup_item(&g_cfg.esp.server_bullet_impacts_color, Color(255, 255, 255), crypt_str("wallhack_server_bullet_impacts_color"));
	setup_item(&g_cfg.esp.bullet_tracer, false, crypt_str("wallhack_bullet_tracer"));
	setup_item(&g_cfg.esp.rainbow_trails, false, crypt_str("rainbow_trails"));
	setup_item(&g_cfg.esp.bullet_tracer_color, Color(255, 255, 255), crypt_str("wallhack_bullet_tracer_color"));
	setup_item(&g_cfg.esp.enemy_bullet_tracer, false, crypt_str("wallhack_enemy_bullet_tracer"));
	setup_item(&g_cfg.esp.enemy_bullet_tracer_color, Color(255, 255, 255), crypt_str("wallhack_enemy_bullet_tracer_color"));
	setup_item(&g_cfg.esp.preserve_killfeed, false, crypt_str("wallhack_preserve_killfeed"));
	setup_item(&g_cfg.esp.hitmarker, 4, crypt_str("wallhack_hit_marker"));
	setup_item(&g_cfg.esp.hitsound, 0, crypt_str("wallhack_hitsound"));
	setup_item(&g_cfg.esp.killsound, false, crypt_str("wallhack_killsound"));
	setup_item(&g_cfg.esp.trails, false, crypt_str("Esp.trails"));
	setup_item(&g_cfg.esp.trails_color, Color(255, 255, 255), crypt_str("Esp.trails_color"));
	setup_item(&g_cfg.esp.damage_marker, false, crypt_str("wallhack_damage_marker"));
	setup_item(&g_cfg.esp.kill_effect, false, crypt_str("wallhack_kill_effect"));
	setup_item(&g_cfg.esp.kill_effect_duration, 3.0f, crypt_str("wallhack_kill_effect_duration"));
	setup_item(&g_cfg.esp.fov, 0, crypt_str("wallhack_fov"));
	setup_item(&g_cfg.esp.viewmodel_fov, 0, crypt_str("wallhack_viewmodel_fov"));
	setup_item(&g_cfg.esp.viewmodel_x, 0, crypt_str("wallhack_viewmodel_x"));
	setup_item(&g_cfg.esp.viewmodel_y, 0, crypt_str("wallhack_viewmodel_y"));
	setup_item(&g_cfg.esp.viewmodel_z, 0, crypt_str("wallhack_viewmodel_z"));
	setup_item(&g_cfg.esp.viewmodel_roll, 0, crypt_str("wallhack_viewmodel_roll"));
	setup_item(&g_cfg.esp.arms_chams, false, crypt_str("wallhack_arms_chams"));
	setup_item(&g_cfg.esp.arms_chams_type, 0, crypt_str("wallhack_arms_chams_type"));
	setup_item(&g_cfg.esp.arms_chams_color, Color(255, 255, 255), crypt_str("wallhack_arms_chams_color"));
	setup_item(&g_cfg.esp.arms_double_material, false, crypt_str("wallhack_arms_double_material"));
	setup_item(&g_cfg.esp.arms_double_material_color, Color(255, 255, 255), crypt_str("wallhack_arms_double_material_color"));
	setup_item(&g_cfg.esp.arms_animated_material, false, crypt_str("wallhack_arms_animated_material"));
	setup_item(&g_cfg.esp.arms_animated_material_color, Color(255, 255, 255), crypt_str("wallhack_arms_animated_material_color"));
	setup_item(&g_cfg.esp.weapon_chams, false, crypt_str("wallhack_weapon_chams"));
	setup_item(&g_cfg.esp.weapon_chams_type, 0, crypt_str("wallhack_weapon_chams_type"));
	setup_item(&g_cfg.esp.weapon_chams_color, Color(255, 255, 255), crypt_str("wallhack_weapon_chams_color"));
	setup_item(&g_cfg.esp.weapon_double_material, false, crypt_str("wallhack_weapon_double_material"));
	setup_item(&g_cfg.esp.weapon_double_material_color, Color(255, 255, 255), crypt_str("wallhack_weapon_double_material_color"));
	setup_item(&g_cfg.esp.weapon_animated_material, false, crypt_str("wallhack_weapon_animated_material"));
	setup_item(&g_cfg.esp.weapon_animated_material_color, Color(255, 255, 255), crypt_str("wallhack_weapon_animated_material_color"));
	setup_item(&g_cfg.esp.taser_range, false, crypt_str("wallhack_taser_range"));
	setup_item(&g_cfg.esp.show_spread, false, crypt_str("wallhack_show_spread"));
	setup_item(&g_cfg.esp.show_spread_color, Color(255, 255, 255), crypt_str("wallhack_show_spread_color"));
	setup_item(&g_cfg.esp.penetration_reticle, false, crypt_str("wallhack_penetration_reticle"));
	setup_item(&g_cfg.esp.world_modulation, false, crypt_str("wallhack_world_modulation"));
	setup_item(&g_cfg.esp.rain, false, crypt_str("wallhack_rain"));
	setup_item(&g_cfg.esp.bloom, 0.0f, crypt_str("wallhack_bloom"));

	setup_item(&g_cfg.esp.molotov_shit, false, crypt_str("molotov_shit"));
	setup_item(&g_cfg.esp.molotov_shit_enemy, Color::Red, crypt_str("molotov_shit_enemy"));
	setup_item(&g_cfg.esp.molotov_shit_team, Color::Green, crypt_str("molotov_shit_team"));
	

	setup_item(&g_cfg.esp.exposure, 0.0f, crypt_str("wallhack_exposure"));
	setup_item(&g_cfg.esp.ambient, 0.0f, crypt_str("wallhack_ambient"));
	setup_item(&g_cfg.esp.fog, false, crypt_str("wallhack_fog"));
	setup_item(&g_cfg.esp.fog_distance, 0, crypt_str("wallhack_fog_distance"));
	setup_item(&g_cfg.esp.fog_density, 0, crypt_str("wallhack_fog_density"));
	setup_item(&g_cfg.esp.fog_color, Color(255, 255, 255), crypt_str("wallhack_fog_color"));
	setup_item(&g_cfg.esp.weapon, 8, crypt_str("wallhack_dropped_weapon"));
	setup_item(&g_cfg.esp.box_color, Color(255, 255, 255), crypt_str("wallhack_dropped_weapon_box_color"));
	setup_item(&g_cfg.esp.weapon_color, Color(255, 255, 255), crypt_str("wallhack_dropped_weapon_color"));
	setup_item(&g_cfg.esp.prop_transparency, 100.f, crypt_str("wallhack_prop_transparency"));
	setup_item(&g_cfg.esp.world_transparency, 100.f, crypt_str("wallhack_world_transparency"));
	setup_item(&g_cfg.esp.weapon_glow_color, Color(255, 255, 255), crypt_str("wallhack_dropped_weapon_glow_color"));
	setup_item(&g_cfg.esp.weapon_ammo_color, Color(255, 255, 255), crypt_str("wallhack_dropped_weapon_ammo_color"));
	setup_item(&g_cfg.esp.grenade_esp, 6, crypt_str("wallhack_grenade_esp"));
	setup_item(&g_cfg.esp.grenade_glow_color, Color(255, 255, 255), crypt_str("wallhack_grenade_glow_color"));
	setup_item(&g_cfg.esp.grenade_box_color, Color(255, 255, 255), crypt_str("wallhack_grenade_box_color"));


	setup_item(&g_cfg.misc.spectators_list, false, crypt_str("miscellaneous_spectators_list"));
	setup_item(&g_cfg.misc.ingame_radar, false, crypt_str("miscellaneous_ingame_radar"));
	setup_item(&g_cfg.misc.ragdolls, false, crypt_str("miscellaneous_ragdolls"));
	setup_item(&g_cfg.misc.bunnyhop, false, crypt_str("miscellaneous_autohop"));
	setup_item(&g_cfg.misc.airstrafe, 0, crypt_str("miscellaneous_airstrafe"));
	setup_item(&g_cfg.ragebot.dt_mode, 0, crypt_str("g_cfg.ragebot.dt_mode"));
	//setup_item(&gay, false crypt_str("miscellaneous_airstrafegaydocrl"));
	setup_item(&g_cfg.misc.crouch_in_air, false, crypt_str("miscellaneous_crouch_in_air"));
	setup_item(&g_cfg.misc.automatic_peek, key_bind(HOLD), crypt_str("miscellaneous_automatic_peek"));
	setup_item(&g_cfg.misc.edge_jump, key_bind(HOLD), crypt_str("miscellaneous_edge_jump"));
	setup_item(&g_cfg.misc.edgebug, key_bind(HOLD), crypt_str("miscellaneous_edgebug"));
	setup_item(&g_cfg.misc.jumpbug, key_bind(HOLD), crypt_str("miscellaneous_jumpbug"));

	setup_item(&g_cfg.misc.noduck, false, crypt_str("miscellaneous_noduck"));
	setup_item(&g_cfg.misc.anti_obs, false, crypt_str("miscellaneous_obs"));
	setup_item(&g_cfg.misc.fakeduck_key, key_bind(HOLD), crypt_str("miscellaneous_fakeduck_key"));
	setup_item(&g_cfg.misc.fast_stop, false, crypt_str("miscellaneous_fast_stop"));
	setup_item(&g_cfg.misc.slidewalk, false, crypt_str("miscellaneous_slidewalk"));
	setup_item(&g_cfg.misc.slowwalk_key, key_bind(HOLD), crypt_str("miscellaneous_slowwalk_key"));
	setup_item(&g_cfg.misc.chat, false, crypt_str("miscellaneous_chat"));
	setup_item(&g_cfg.misc.log_output, 2, crypt_str("miscellaneous_log_output"));//rumando
	setup_item(&g_cfg.misc.events_to_log, 3, crypt_str("miscellaneous_events_to_log"));
	setup_item(&g_cfg.misc.show_default_log, false, crypt_str("miscellaneous_show_default_log"));
	setup_item(&g_cfg.misc.log_color, Color(255, 255, 255), crypt_str("miscellaneous_log_color"));
	setup_item(&g_cfg.misc.inventory_access, false, crypt_str("miscellaneous_inventory_access"));
	setup_item(&g_cfg.misc.rank_reveal, false, crypt_str("miscellaneous_rank_reveal"));
	setup_item(&g_cfg.misc.clantag_spammer, false, crypt_str("miscellaneous_clantag_spammer"));
	setup_item(&g_cfg.misc.aspect_ratio, false, crypt_str("miscellaneous_aspect_ratio"));
	setup_item(&g_cfg.misc.aspect_ratio_amount, 1.0f, crypt_str("miscellaneous_aspect_ratio_amount"));
	setup_item(&g_cfg.misc.anti_untrusted, true, crypt_str("miscellaneous_antiuntrusted"));//crash quando da load por causa do debug
	setup_item(&g_cfg.misc.buybot_enable, false, crypt_str("miscellaneous_buybot"));
	setup_item(&g_cfg.misc.buybot1, 0, crypt_str("play_buybot1"));
	setup_item(&g_cfg.misc.buybot2, 0, crypt_str("play_buybot2"));
	setup_item(&g_cfg.misc.buybot3, 4, crypt_str("miscellaneous_buybot3"));

	/*setup_item(&g_cfg.skins.rare_animations, false, crypt_str("texture_changer_rare_animations"));

	for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
	{
		setup_item(&g_cfg.skins.skinChanger.at(i).definition_override_index, 0, std::to_string(i) + crypt_str("texture_changer_definition_override_index")); //-V807
		setup_item(&g_cfg.skins.skinChanger.at(i).definition_override_vector_index, 0, std::to_string(i) + crypt_str("texture_changer_definition_override_vector_index"));
		setup_item(&g_cfg.skins.skinChanger.at(i).entity_quality_vector_index, 0, std::to_string(i) + crypt_str("texture_changer_entity_quality_vector_index"));
		setup_item(&g_cfg.skins.skinChanger.at(i).itemId, 0, std::to_string(i) + crypt_str("texture_changer_itemId"));
		setup_item(&g_cfg.skins.skinChanger.at(i).itemIdIndex, 0, std::to_string(i) + crypt_str("texture_changer_itemIdIndex"));
		setup_item(&g_cfg.skins.skinChanger.at(i).paintKit, 0, std::to_string(i) + crypt_str("texture_changer_paintKit"));
		setup_item(&g_cfg.skins.skinChanger.at(i).paint_kit_vector_index, 0, std::to_string(i) + crypt_str("texture_changer_paint_kit_vector_index"));
		setup_item(&g_cfg.skins.skinChanger.at(i).quality, 0, std::to_string(i) + crypt_str("texture_changer_quality"));
		setup_item(&g_cfg.skins.skinChanger.at(i).seed, 0, std::to_string(i) + crypt_str("texture_changer_seed"));
		setup_item(&g_cfg.skins.skinChanger.at(i).stat_trak, 0, std::to_string(i) + crypt_str("texture_changer_stat_trak"));
		setup_item(&g_cfg.skins.skinChanger.at(i).wear, 0.0f, std::to_string(i) + crypt_str("texture_changer_wear"));
		setup_item(&g_cfg.skins.custom_name_tag[i], crypt_str(""), std::to_string(i) + crypt_str("texture_changer_custom_name_tag"));
	}*/

	setup_item(&g_cfg.inventory.activate_inventory, false, "inv_enable");

	setup_item(&g_cfg.menu.menu_theme, Color(117, 66, 245), crypt_str("Menu_menu_color"));
	setup_item(&g_cfg.menu.watermark, false, crypt_str("Menu_watermark"));
	setup_item(&g_cfg.misc.keybinds, false, crypt_str("key_binds_indicators"));

	setup_item(&g_cfg.player.chams_on_hit, false, crypt_str("c_on_hit"));
	setup_item(&g_cfg.player.chams_hit_color, Color(255, 255, 255), crypt_str("c_on_hit_color"));
	setup_item(&g_cfg.player.chams_hit_material, 0, crypt_str("c_on_hit_mat"));

	setup_item(&g_cfg.misc.key_x, 0, crypt_str("key_binds_indicators_x"));
	setup_item(&g_cfg.misc.key_y, 0, crypt_str("key_binds_indicators_y"));

	setup_item(&kdopipi::g, false, "kdopenis_g");
	setup_item(&kdopipi::gg, false, "kdopenis_gg");
	setup_item(&kdopipi::event_log, false, "kdopenis_eventlog");

	setup_item(&g_cfg.scripts.scripts, crypt_str("Scripts_loaded"));
}

void C_ConfigManager::add_item(void * pointer, const char * name, const std::string& type) {
	items.push_back(new C_ConfigItem(std::string(name), pointer, type));
}

void C_ConfigManager::setup_item(int * pointer, int value, const std::string& name) 
{
	add_item(pointer, name.c_str(), crypt_str("int"));
	*pointer = value;
}

void C_ConfigManager::setup_item(bool * pointer, bool value, const std::string& name)
{
	add_item(pointer, name.c_str(), crypt_str("bool"));
	*pointer = value;
}

void C_ConfigManager::setup_item(float * pointer, float value, const std::string& name)
{
	add_item(pointer, name.c_str(), crypt_str("float"));
	*pointer = value;
}

void C_ConfigManager::setup_item(key_bind* pointer, key_bind value, const std::string& name) //-V813
{
	add_item(pointer, name.c_str(), crypt_str("KeyBind"));
	*pointer = value;
}

void C_ConfigManager::setup_item(Color* pointer, Color value, const std::string& name)
{
	colors[name][0] = (float)value.r() / 255.0f;
	colors[name][1] = (float)value.g() / 255.0f;
	colors[name][2] = (float)value.b() / 255.0f;
	colors[name][3] = (float)value.a() / 255.0f;

	add_item(pointer, name.c_str(), crypt_str("Color"));
	*pointer = value;
}

void C_ConfigManager::setup_item(std::vector< int > * pointer, int size, const std::string& name) 
{
	add_item(pointer, name.c_str(), crypt_str("vector<int>"));
	pointer->clear();

	for (int i = 0; i < size; i++)
		pointer->push_back(FALSE);
}

void C_ConfigManager::setup_item(std::vector< std::string >* pointer, const std::string& name)
{ 
	add_item(pointer, name.c_str(), crypt_str("vector<string>"));
}

void C_ConfigManager::setup_item(std::string * pointer, const std::string& value, const std::string& name)
{
	add_item(pointer, name.c_str(), crypt_str("string"));
	*pointer = value; //-V820
}

void C_ConfigManager::reset_config(std::string config)
{
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + crypt_str("\\Nexus\\");
			file = std::string(path) + crypt_str("\\Nexus\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();

	std::ofstream stream;
	stream.open(file);

	std::string all = one_p + two_p + tree_p + four_p + five_p + six_p + seven_p + eight_p + nine_p + teen_p + eleven_p + tweelve_p + thirdten_p + fourten_p + fiveten_p;

	stream << all;

	stream.close();

}

void C_ConfigManager::save(std::string config)
{
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + crypt_str("\\Nexus\\");
			file = std::string(path) + crypt_str("\\Nexus\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();
	json allJson;

	for (auto it : items)
	{
		json j;

		j[crypt_str("name")] = it->name;
		j[crypt_str("type")] = it->type;

		if (!it->type.compare(crypt_str("int"))) 
			j[crypt_str("value")] = (int)*(int*)it->pointer; //-V206
		else if (!it->type.compare(crypt_str("float"))) 
			j[crypt_str("value")] = (float)*(float*)it->pointer;
		else if (!it->type.compare(crypt_str("bool"))) 
			j[crypt_str("value")] = (bool)*(bool*)it->pointer;
		else if (!it->type.compare(crypt_str("KeyBind")))
		{
			auto k = *(key_bind*)(it->pointer);

			std::vector <int> a = { k.key, k.mode };
			json ja;

			for (auto& i : a)
				ja.push_back(i);

			j[crypt_str("value")] = ja.dump();
		}
		else if (!it->type.compare(crypt_str("Color")))
		{
			auto c = *(Color*)(it->pointer);

			std::vector<int> a = { c.r(), c.g(), c.b(), c.a() };
			json ja;

			for (auto& i : a) 
				ja.push_back(i);

			j[crypt_str("value")] = ja.dump();
		}
		else if (!it->type.compare(crypt_str("vector<int>")))
		{
			auto& ptr = *(std::vector<int>*)(it->pointer);
			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j[crypt_str("value")] = ja.dump();
		}
		else if (!it->type.compare(crypt_str("vector<string>")))
		{
			auto& ptr = *(std::vector<std::string>*)(it->pointer);
			json ja;

			for (auto& i : ptr)
				ja.push_back(i);

			j[crypt_str("value")] = ja.dump();
		}
		else if (!it->type.compare(crypt_str("string"))) 
			j[crypt_str("value")] = (std::string)*(std::string*)it->pointer;

		allJson.push_back(j);
	}

	auto get_type = [](menu_item_type type)
	{
		switch (type) //-V719
		{
		case CHECK_BOX:
			return "bool";
		case COMBO_BOX:
		case SLIDER_INT:
			return "int";
		case SLIDER_FLOAT:
			return "float";
		case COLOR_PICKER:
			return "Color";
		}
	};

	for (auto i = 0; i < c_lua::get().scripts.size(); ++i)
	{
		auto& script = c_lua::get().scripts.at(i); 

		for (auto& item : c_lua::get().items.at(i)) 
		{
			if (item.second.type == NEXT_LINE)
				continue;

			json j;
			auto type = (std::string)get_type(item.second.type);

			j[crypt_str("name")] = item.first;
			j[crypt_str("type")] = type;

			if (!type.compare(crypt_str("bool")))
				j[crypt_str("value")] = item.second.check_box_value;
			else if (!type.compare(crypt_str("int")))
				j[crypt_str("value")] = item.second.type == COMBO_BOX ? item.second.combo_box_value : item.second.slider_int_value;
			else if (!type.compare(crypt_str("float")))
				j[crypt_str("value")] = item.second.slider_float_value;
			else if (!type.compare(crypt_str("Color")))
			{
				std::vector <int> color = 
				{ 
					item.second.color_picker_value.r(), 
					item.second.color_picker_value.g(),
					item.second.color_picker_value.b(), 
					item.second.color_picker_value.a() 
				};

				json j_color;

				for (auto& i : color)
					j_color.push_back(i);

				j[crypt_str("value")] = j_color.dump();
			}

			allJson.push_back(j);
		}
	}

	std::string data;

	Base64 base64;
	base64.encode(allJson.dump(), &data);

	std::ofstream ofs;
	ofs.open(file + '\0', std::ios::out | std::ios::trunc);

	ofs << std::setw(4) << data << std::endl;
	ofs.close();

	static TCHAR path[MAX_PATH];
	std::string zfolder;
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path);
	zfolder = std::string(path) + ("\\Nexus\\");
	auto zfile = zfolder + "Inventory";

	WritePrivateProfileStringA("changer", "Count", std::to_string(_inv.inventory.itemCount).c_str(), zfile.c_str());

	int i = 0;
	for (auto weapon : g_InventorySkins)
	{
		if (!weapon.second.wId || !weapon.second.paintKit)
			continue;

		std::string itemid = "inv" + std::to_string(i) + "id";
		std::string paintkitstr = "inv" + std::to_string(i) + "kit";
		std::string wearkitstr = "inv" + std::to_string(i) + "wear";
		std::string seedkitstr = "inv" + std::to_string(i) + "seed";
		std::string inusetstr = "inv" + std::to_string(i) + "tr";
		std::string inusectstr = "inv" + std::to_string(i) + "ct";
		//std::string qual = "inventory" + std::to_string(i) + "_qual";
		//std::string name = "inventory" + std::to_string(i) + "_name";
		//std::string stattrak = "inventory" + std::to_string(i) + "_stattrak";


		std::string st1 = "inv" + std::to_string(i) + "1";
		std::string st2 = "inv" + std::to_string(i) + "2";
		std::string st3 = "inv" + std::to_string(i) + "3";
		std::string st4 = "inv" + std::to_string(i) + "4";




		WritePrivateProfileStringA("changer", itemid.c_str(), std::to_string(weapon.second.wId).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", paintkitstr.c_str(), std::to_string(weapon.second.paintKit).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", wearkitstr.c_str(), std::to_string(weapon.second.wear).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", seedkitstr.c_str(), std::to_string(weapon.second.seed).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", inusetstr.c_str(), weapon.second.in_use_t ? "true" : "false", zfile.c_str());
		WritePrivateProfileStringA("changer", inusectstr.c_str(), weapon.second.in_use_ct ? "true" : "false", zfile.c_str());
		//WritePrivateProfileStringA("InventoryChanger", qual.c_str(), std::to_string(weapon.second.quality).c_str(), zfile.c_str());
		//WritePrivateProfileStringA("InventoryChanger", name.c_str(), weapon.second.name.c_str(), zfile.c_str());
		//WritePrivateProfileStringA("InventoryChanger", stattrak.c_str(), std::to_string(weapon.second.stattrak).c_str(), zfile.c_str());

		WritePrivateProfileStringA("changer", st1.c_str(), std::to_string(weapon.second.sicker[0]).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", st2.c_str(), std::to_string(weapon.second.sicker[1]).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", st3.c_str(), std::to_string(weapon.second.sicker[2]).c_str(), zfile.c_str());
		WritePrivateProfileStringA("changer", st4.c_str(), std::to_string(weapon.second.sicker[3]).c_str(), zfile.c_str());




		i++;
	}


	WritePrivateProfileStringA("medal", "Count", std::to_string(g_MedalSkins.size()).c_str(), zfile.c_str());
	i = 0;
	for (auto weapon : g_MedalSkins)
	{
		if (!weapon.second.paintKit)
			continue;

		std::string paintkitstr = "medal" + std::to_string(i) + "kit";
		std::string equip = "medal" + std::to_string(i) + "equip";
		WritePrivateProfileStringA("medal", paintkitstr.c_str(), std::to_string(weapon.second.paintKit).c_str(), zfile.c_str());
		WritePrivateProfileStringA("medal", equip.c_str(), std::to_string(weapon.second.equip_state).c_str(), zfile.c_str());

		i++;
	}
}

void C_ConfigManager::load(std::string config, bool load_script_items)
{
	static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem* 
	{
		for (int i = 0; i < (int)items.size(); i++) //-V202
			if (!items[i]->name.compare(name))
				return items[i];

		return nullptr;
	};

	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() ->void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + crypt_str("\\Nexus\\");
			file = std::string(path) + crypt_str("\\Nexus\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();
	std::string data;

	std::ifstream ifs;
	ifs.open(file + '\0');

	ifs >> data;
	ifs.close();

	if (data.empty())
		return;

	Base64 base64;

	std::string decoded_data;
	base64.decode(data, &decoded_data);

	std::ofstream ofs;
	ofs.open(file + '\0', std::ios::out | std::ios::trunc);

	ofs << decoded_data;
	ofs.close();

	json allJson;

	std::ifstream ifs_final;
	ifs_final.open(file + '\0');

	ifs_final >> allJson;
	ifs_final.close();

	base64.encode(allJson.dump(), &data);

	std::ofstream final_ofs;
	final_ofs.open(file + '\0', std::ios::out | std::ios::trunc);

	final_ofs << data;
	final_ofs.close();

	for (auto it = allJson.begin(); it != allJson.end(); ++it)
	{
		json j = *it;

		std::string name = j[crypt_str("name")];
		std::string type = j[crypt_str("type")];

		auto script_item = std::count_if(name.begin(), name.end(),
			[](char& c)
			{
				return c == '.';
			}
		) >= 2;

		if (load_script_items && script_item)
		{
			std::string script_name;
			auto first_point = false;

			for (auto& c : name)
			{
				if (c == '.')
				{
					if (first_point)
						break;
					else
						first_point = true;
				}
	
				script_name.push_back(c);
			}

			auto script_id = c_lua::get().get_script_id(script_name);

			if (script_id == -1)
				continue;

			for (auto& current_item : c_lua::get().items.at(script_id))
			{
				if (current_item.first == name)
				{
					if (!type.compare(crypt_str("bool")))
					{
						current_item.second.type = CHECK_BOX;
						current_item.second.check_box_value = j[crypt_str("value")].get<bool>();
					}
					else if (!type.compare(crypt_str("int")))
					{
						if (current_item.second.type == COMBO_BOX)
							current_item.second.combo_box_value = j[crypt_str("value")].get<int>();
						else
							current_item.second.slider_int_value = j[crypt_str("value")].get<int>();
					}
					else if (!type.compare(crypt_str("float")))
						current_item.second.slider_float_value = j[crypt_str("value")].get<float>();
					else if (!type.compare(crypt_str("Color")))
					{
						std::vector<int> a;
						json ja = json::parse(j[crypt_str("value")].get<std::string>().c_str());

						for (json::iterator it = ja.begin(); it != ja.end(); ++it)
							a.push_back(*it);

						colors.erase(name);
						current_item.second.color_picker_value = Color(a[0], a[1], a[2], a[3]);
					}
				}
			}
		}
		else if (!load_script_items && !script_item)
		{
			auto item = find_item(items, name);

			if (item)
			{
				if (!type.compare(crypt_str("int")))
					*(int*)item->pointer = j[crypt_str("value")].get<int>(); //-V206
				else if (!type.compare(crypt_str("float")))
					*(float*)item->pointer = j[crypt_str("value")].get<float>();
				else if (!type.compare(crypt_str("bool")))
					*(bool*)item->pointer = j[crypt_str("value")].get<bool>();
				else if (!type.compare(crypt_str("KeyBind")))
				{
					std::vector<int> a;
					json ja = json::parse(j[crypt_str("value")].get<std::string>().c_str());

					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
						a.push_back(*it);

					*(key_bind*)item->pointer = key_bind((ButtonCode_t)a[0], (key_bind_mode)a[1]);
				}
				else if (!type.compare(crypt_str("Color")))
				{
					std::vector<int> a;
					json ja = json::parse(j[crypt_str("value")].get<std::string>().c_str());

					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
						a.push_back(*it);

					colors.erase(item->name);
					*(Color*)item->pointer = Color(a[0], a[1], a[2], a[3]);
				}
				else if (!type.compare(crypt_str("vector<int>")))
				{
					auto ptr = static_cast<std::vector <int>*> (item->pointer);
					ptr->clear();

					json ja = json::parse(j[crypt_str("value")].get<std::string>().c_str());

					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
						ptr->push_back(*it);
				}
				else if (!type.compare(crypt_str("vector<string>")))
				{
					auto ptr = static_cast<std::vector <std::string>*> (item->pointer);
					ptr->clear();

					json ja = json::parse(j[crypt_str("value")].get<std::string>().c_str());

					for (json::iterator it = ja.begin(); it != ja.end(); ++it)
						ptr->push_back(*it);
				}
				else if (!type.compare(crypt_str("string")))
					*(std::string*)item->pointer = j[crypt_str("value")].get<std::string>();
			}
		}
	}

	static TCHAR path[MAX_PATH];
	std::string zfolder;
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path);
	zfolder = std::string(path) + ("\\NEXUS\\");
	//zfolder += ("\\Inventory\\");
	auto zfile = zfolder + "Inventory";
	g_InventorySkins.clear();
	char value_l[32] = { '\0' };
	GetPrivateProfileStringA("changer", "Count", "", value_l, 32, zfile.c_str());
	_inv.inventory.itemCount = atoi(value_l);

	for (int i = 0; i < _inv.inventory.itemCount; i++)
	{
		std::string itemid = "inv" + std::to_string(i) + "id";
		std::string paintkitstr = "inv" + std::to_string(i) + "kit";
		std::string wearkitstr = "inv" + std::to_string(i) + "wear";
		std::string seedkitstr = "inv" + std::to_string(i) + "seed";
		std::string inusetstr = "inv" + std::to_string(i) + "tr";
		std::string inusectstr = "inv" + std::to_string(i) + "ct";
		//std::string qual = "inventory" + std::to_string(i) + "_qual";
		//std::string name = "inventory" + std::to_string(i) + "_name";
		//std::string stattrak = "inventory" + std::to_string(i) + "_stattrak";


		std::string st1 = "inv" + std::to_string(i) + "1";
		std::string st2 = "inv" + std::to_string(i) + "2";
		std::string st3 = "inv" + std::to_string(i) + "3";
		std::string st4 = "inv" + std::to_string(i) + "4";


		wskin skinInfo;

		GetPrivateProfileStringA("changer", itemid.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.wId = atoi(value_l);
		GetPrivateProfileStringA("changer", paintkitstr.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.paintKit = atoi(value_l);
		GetPrivateProfileStringA("changer", wearkitstr.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.wear = atof(value_l);
		GetPrivateProfileStringA("changer", seedkitstr.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.seed = atoi(value_l);
		GetPrivateProfileStringA("changer", inusetstr.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.in_use_t = !strcmp(value_l, "true");
		GetPrivateProfileStringA("changer", inusectstr.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.in_use_ct = !strcmp(value_l, "true");


		GetPrivateProfileStringA("changer", st1.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.sicker[0] = atoi(value_l);
		GetPrivateProfileStringA("changer", st2.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.sicker[1] = atoi(value_l);
		GetPrivateProfileStringA("changer", st3.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.sicker[2] = atoi(value_l);
		GetPrivateProfileStringA("changer", st4.c_str(), "", value_l, 32, zfile.c_str());
		skinInfo.sicker[3] = atoi(value_l);

		g_InventorySkins.insert({ g_csgo.fRandomInt(20000, 200000), skinInfo });
	}

	g_MedalSkins.clear();
	char zvalue_l[32] = { '\0' };
	GetPrivateProfileStringA("medal", "Count", "", zvalue_l, 32, zfile.c_str());
	//atoi(zvalue_l);
	for (int i = 0; i < atoi(zvalue_l); i++)
	{
		std::string paintkitstr = "medal" + std::to_string(i) + "kit";
		std::string equip = "medal" + std::to_string(i) + "equip";


		GetPrivateProfileStringA("medal", paintkitstr.c_str(), "", value_l, 32, zfile.c_str());
		int pk = atoi(value_l);
		GetPrivateProfileStringA("medal", equip.c_str(), "", value_l, 32, zfile.c_str());
		int eq = atoi(value_l);

		g_MedalSkins[g_csgo.fRandomInt(200001, 1000000)] = { pk , eq };
	}


	write.SendClientHello();
	write.SendMatchmakingClient2GCHello();
}

void C_ConfigManager::remove(std::string config)
{
	std::string folder, file;

	auto get_dir = [&folder, &file, &config]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
		{
			folder = std::string(path) + crypt_str("\\Nexus\\");
			file = std::string(path) + crypt_str("\\Nexus\\") + config;
		}

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();

	std::string path = file + '\0';
	std::remove(path.c_str());
}

void C_ConfigManager::config_files()
{
	std::string folder;

	auto get_dir = [&folder]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
			folder = std::string(path) + crypt_str("\\Nexus\\");

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();
	files.clear();

	std::string path = folder + crypt_str("/*.cfg");
	WIN32_FIND_DATA fd;

	HANDLE hFind = FindFirstFile(path.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				files.push_back(fd.cFileName);
		} 
		while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}
}
