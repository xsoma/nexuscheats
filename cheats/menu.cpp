#include "menu.h"
#include <ShlObj_core.h>
#include <unordered_map>
#include "inventory changer/items.h"
#include "inventory changer/inventorychanger.h"
#include "../protobuffs/Protobuffs.h"
#include <libloaderapi.h>
#include <WinInet.h>
#include <utils/util.hpp>
#include "../hooks/hooks.hpp"
#include "../model_3d.h"
std::vector <std::string> files;
std::vector <std::string> scripts;
auto selected_script = 0;
IDirect3DTexture9* all_skins[36];

std::string get_wep(int id, int custom_index = -1, bool knife = true)
{
	if (custom_index > -1)
	{
		if (knife)
		{
			switch (custom_index)
			{
			case 0: return crypt_str("weapon_knife");
			case 1: return crypt_str("weapon_bayonet");
			case 2: return crypt_str("weapon_knife_css");
			case 3: return crypt_str("weapon_knife_skeleton");
			case 4: return crypt_str("weapon_knife_outdoor");
			case 5: return crypt_str("weapon_knife_cord");
			case 6: return crypt_str("weapon_knife_canis");
			case 7: return crypt_str("weapon_knife_flip");
			case 8: return crypt_str("weapon_knife_gut");
			case 9: return crypt_str("weapon_knife_karambit");
			case 10: return crypt_str("weapon_knife_m9_bayonet");
			case 11: return crypt_str("weapon_knife_tactical");
			case 12: return crypt_str("weapon_knife_falchion");
			case 13: return crypt_str("weapon_knife_survival_bowie");
			case 14: return crypt_str("weapon_knife_butterfly");
			case 15: return crypt_str("weapon_knife_push");
			case 16: return crypt_str("weapon_knife_ursus");
			case 17: return crypt_str("weapon_knife_gypsy_jackknife");
			case 18: return crypt_str("weapon_knife_stiletto");
			case 19: return crypt_str("weapon_knife_widowmaker");
			}
		}
		else
		{
			switch (custom_index)
			{
			case 0: return crypt_str("ct_gloves"); //-V1037
			case 1: return crypt_str("studded_bloodhound_gloves");
			case 2: return crypt_str("t_gloves");
			case 3: return crypt_str("ct_gloves");
			case 4: return crypt_str("sporty_gloves");
			case 5: return crypt_str("slick_gloves");
			case 6: return crypt_str("leather_handwraps");
			case 7: return crypt_str("motorcycle_gloves");
			case 8: return crypt_str("specialist_gloves");
			case 9: return crypt_str("studded_hydra_gloves");
			}
		}
	}
	else
	{
		switch (id)
		{
		case 0: return crypt_str("knife");
		case 1: return crypt_str("gloves");
		case 2: return crypt_str("weapon_ak47");
		case 3: return crypt_str("weapon_aug");
		case 4: return crypt_str("weapon_awp");
		case 5: return crypt_str("weapon_cz75a");
		case 6: return crypt_str("weapon_deagle");
		case 7: return crypt_str("weapon_elite");
		case 8: return crypt_str("weapon_famas");
		case 9: return crypt_str("weapon_fiveseven");
		case 10: return crypt_str("weapon_g3sg1");
		case 11: return crypt_str("weapon_galilar");
		case 12: return crypt_str("weapon_glock");
		case 13: return crypt_str("weapon_m249");
		case 14: return crypt_str("weapon_m4a1_silencer");
		case 15: return crypt_str("weapon_m4a1");
		case 16: return crypt_str("weapon_mac10");
		case 17: return crypt_str("weapon_mag7");
		case 18: return crypt_str("weapon_mp5sd");
		case 19: return crypt_str("weapon_mp7");
		case 20: return crypt_str("weapon_mp9");
		case 21: return crypt_str("weapon_negev");
		case 22: return crypt_str("weapon_nova");
		case 23: return crypt_str("weapon_hkp2000");
		case 24: return crypt_str("weapon_p250");
		case 25: return crypt_str("weapon_p90");
		case 26: return crypt_str("weapon_bizon");
		case 27: return crypt_str("weapon_revolver");
		case 28: return crypt_str("weapon_sawedoff");
		case 29: return crypt_str("weapon_scar20");
		case 30: return crypt_str("weapon_ssg08");
		case 31: return crypt_str("weapon_sg556");
		case 32: return crypt_str("weapon_tec9");
		case 33: return crypt_str("weapon_ump45");
		case 34: return crypt_str("weapon_usp_silencer");
		case 35: return crypt_str("weapon_xm1014");
		default: return crypt_str("unknown");
		}
	}
}

std::string wep_c(int id, int custom_index = -1, bool knife = true)
{
	if (custom_index > -1)
	{
		if (knife)
		{
			switch (custom_index)
			{
			case 0: return crypt_str("Default knife");
			case 1: return crypt_str("Bayonet");
			case 2: return crypt_str("Classic Knife");
			case 3: return crypt_str("Skeleton");
			case 4: return crypt_str("Hustman");
			case 5: return crypt_str("Paracord");
			case 6: return crypt_str("Survival");
			case 7: return crypt_str("Flip");
			case 8: return crypt_str("Gut");
			case 9: return crypt_str("Karambit");
			case 10: return crypt_str("M9");
			case 11: return crypt_str("Tactical");
			case 12: return crypt_str("Falchion");
			case 13: return crypt_str("Bowie");
			case 14: return crypt_str("Butterfly");
			case 15: return crypt_str("Shadow Daggers");
			case 16: return crypt_str("Ursus");
			case 17: return crypt_str("Navaja");
			case 18: return crypt_str("Stiletto");
			case 19: return crypt_str("Talon");
			}
		}
		else
		{
			switch (custom_index)
			{
			case 0: return crypt_str("Gloves"); //-V1037
			case 1: return crypt_str("BloodHound Gloves");
			case 2: return crypt_str("T Gloves");
			case 3: return crypt_str("CT Glovers");
			case 4: return crypt_str("Sporty Gloves");
			case 5: return crypt_str("Slick Gloves");
			case 6: return crypt_str("Handwarps");
			case 7: return crypt_str("Motorcycle");
			case 8: return crypt_str("Specialist");
			case 9: return crypt_str("Hydra");
			}
		}
	}
	else
	{
		switch (id)
		{
		case 0: return crypt_str("Knife");
		case 1: return crypt_str("Gloves");
		case 2: return crypt_str("AK K7");
		case 3: return crypt_str("AUG");
		case 4: return crypt_str("AWP");
		case 5: return crypt_str("CZ");
		case 6: return crypt_str("Deagle");
		case 7: return crypt_str("Elite");
		case 8: return crypt_str("Famas");
		case 9: return crypt_str("Five-Seven");
		case 10: return crypt_str("G3SG1");
		case 11: return crypt_str("Galil");
		case 12: return crypt_str("Glock");
		case 13: return crypt_str("M249");
		case 14: return crypt_str("M4A1-S");
		case 15: return crypt_str("M4A4");
		case 16: return crypt_str("MAC 10");
		case 17: return crypt_str("MAG 7");
		case 18: return crypt_str("MP5");
		case 19: return crypt_str("MP7");
		case 20: return crypt_str("MP9");
		case 21: return crypt_str("Negev");
		case 22: return crypt_str("Nova");
		case 23: return crypt_str("P2000");
		case 24: return crypt_str("P250");
		case 25: return crypt_str("P90");
		case 26: return crypt_str("Bizon");
		case 27: return crypt_str("R8");
		case 28: return crypt_str("Sawedoff");
		case 29: return crypt_str("SCAR-20");
		case 30: return crypt_str("SSG08");
		case 31: return crypt_str("SG556");
		case 32: return crypt_str("Tec9");
		case 33: return crypt_str("UMP-45");
		case 34: return crypt_str("USP");
		case 35: return crypt_str("XM1014");
		default: return crypt_str("unknown");
		}
	}
}

namespace kdopipi
{
	bool g;
	bool gg;
	bool event_log;
}

void rage()
{
	ImVec2 size = ImGui::GetWindowSize();

	static int tab = 0;

	pDraw::addchild("sub", ImVec2(size.x - 18, size.y - 405), true, ImGuiWindowFlags_NoScrollbar);
	{
		ImVec2 ss = ImGui::GetWindowSize();

		if (pDraw::addsubtab("RAGE", tab == 0, ImVec2((ss.x - 15) / 2, ss.y - 15))) tab = 0;
		ImGui::SameLine();
		if (pDraw::addsubtab("ANTI-AIM", tab == 1, ImVec2((ss.x - 15) / 2, ss.y - 15))) tab = 1;
	}
	ImGui::EndChild();

	if (tab == 0)
	{
		pDraw::addchild("Rage_1", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			pDraw::addcheck(crypt_str("Enabled"), &g_cfg.ragebot.enable);


			const char* autostop_modifiers[] =
			{
				"Between shots",
				"On lethal",
				"Visible",
				"Center",
				"Force accuracy",
				"Predictive"
			};
			const char* DTSpeed[] =
			{
				"Defensive",
				"Offensive",
				"Normal"
			};


			ImGui::SliderInt(crypt_str("FOV"), &g_cfg.ragebot.field_of_view, 1, 180, false, crypt_str("%d°"));
			pDraw::addcheck(crypt_str("Silent"), &g_cfg.ragebot.silent_aim);
			pDraw::addcheck(crypt_str("Auto Wall"), &g_cfg.ragebot.autowall);
			pDraw::addcheck(crypt_str("Auto knife"), &g_cfg.ragebot.knife_bot);
			pDraw::addcheck(crypt_str("Auto zeus"), &g_cfg.ragebot.zeus_bot);
			pDraw::addcheck(crypt_str("Auto fire"), &g_cfg.ragebot.autoshoot);
			pDraw::addcheck(crypt_str("Auto scope"), &g_cfg.ragebot.autoscope);
			ImGui::Checkbox(crypt_str("Auto stop"), &g_cfg.ragebot.weapon[hooks::rage_weapon].autostop);

			g_cfg.ragebot.double_tap = true;


			if (g_cfg.ragebot.double_tap)
			{
				pGui::draw_keybind(crypt_str("Double tap"), &g_cfg.ragebot.double_tap_key, crypt_str("##HOTKEY_DT"));
				static auto myp2e = 0;
				//ImGui::SliderInt(crypt_str("Shift value##DT"), &g_cfg.ragebot.double_tap_shift_value, 1, 15);
				//ImGui::SliderInt(crypt_str("Tolerence value##DT"), &g_cfg.ragebot.double_tap_tolerence, 1, 15);
				pDraw::singleselect(crypt_str("Double tap mode"), &g_cfg.ragebot.dt_mode, DTSpeed, ARRAYSIZE(DTSpeed));

				if (g_cfg.ragebot.dt_mode == 0)
				{
					g_cfg.ragebot.dt_shift = 13;
					g_cfg.ragebot.dt_tolerance = 0;
					g_cfg.ragebot.slow_teleport = false;
				}

				if (g_cfg.ragebot.dt_mode == 1)
				{
					g_cfg.ragebot.dt_shift = 14;
					g_cfg.ragebot.dt_tolerance = 0;
					g_cfg.ragebot.slow_teleport = true;
				}

				if (g_cfg.ragebot.dt_mode == 2)
				{
					g_cfg.ragebot.dt_shift = 15;
					g_cfg.ragebot.dt_tolerance = 0;
					g_cfg.ragebot.slow_teleport = true;
				}

				if (g_cfg.ragebot.resikmode == 0)
				{
					g_cfg.ragebot.res_side = 30.0f;
					g_cfg.ragebot.res_side_third = 30.0f;
					g_cfg.ragebot.res_side_second = -30.0f;
				}

				if (g_cfg.ragebot.resikmode == 1)
				{
					g_cfg.ragebot.res_side = 19.0f;
					g_cfg.ragebot.res_side_third = 19.0f;
					g_cfg.ragebot.res_side_second = -19.0f;
				}

				if (g_cfg.ragebot.resikmode == 2)
				{
					g_cfg.ragebot.res_side = 60.0f;
					g_cfg.ragebot.res_side_third = 60.0f;
					g_cfg.ragebot.res_side_second = -60.0f;
				}

				//ImGui::Checkbox(crypt_str("Slow teleport"), &g_cfg.ragebot.slow_teleport);
			}

			


			g_cfg.antiaim.hide_shots = true;
			g_cfg.ragebot.double_tap = true;
			//ImGui::Spacing();
			//ImGui::Checkbox(crypt_str("Double tap"), &g_cfg.ragebot.double_tap);

			//ImGui::SameLine(30);
			//pGui::draw_keybind(crypt_str("Double tap"), &g_cfg.ragebot.double_tap_key, crypt_str("##HOTKEY_DT"));
			//pGui::draw_keybind(crypt_str("Hide Shots"), &g_cfg.antiaim.hide_shots_key, crypt_str("##HOTKEY_HIDESHOTS"));
			


			//pDraw::addcheck(crypt_str("Hide shots"), &g_cfg.antiaim.hide_shots);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		//Config


		pDraw::addchild("Rage_2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* rage_weapons[8] = { ("Heavy Pistol"), ("Pistols"), ("SMG"), ("Rifles"), ("Auto"), ("Scout"), ("AWP"), ("Heavy") };

			const char* selection[] =
			{
				"Cycle",
				"Near crosshair",
				"Lowest distance",
				"Lowest health",
				"Highest damage"
			};

			const char* hitboxes[] =
			{
				"Head",
				"Upper chest",
				"Chest",
				"Lower chest",
				"Stomach",
				"Pelvis",
				"Arms",
				"Legs",
				"Feet"
			};


			g_cfg.ragebot.weapon[hooks::rage_weapon].selection_type = 0;

			pDraw::singleselect(crypt_str("Weapon"), &hooks::rage_weapon, rage_weapons, ARRAYSIZE(rage_weapons));

			//pDraw::singleselect(crypt_str("Target selection"), &, selection, ARRAYSIZE(selection));

			pDraw::multiselect2(crypt_str("Hitboxes"), g_cfg.ragebot.weapon[hooks::rage_weapon].hitboxes, hitboxes, ARRAYSIZE(hitboxes), hitboxes);

		    ImGui::Checkbox(crypt_str("Static point scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].static_point_scale);


			if (g_cfg.ragebot.weapon[hooks::rage_weapon].static_point_scale)
			{
				ImGui::SliderFloat(crypt_str("Head scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].head_scale, 0.0f, 1.0f, g_cfg.ragebot.weapon[hooks::rage_weapon].head_scale ? crypt_str("%.2f") : crypt_str("None"));
				ImGui::SliderFloat(crypt_str("Body scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].body_scale, 0.0f, 1.0f, g_cfg.ragebot.weapon[hooks::rage_weapon].body_scale ? crypt_str("%.2f") : crypt_str("None"));
			}

			ImGui::Checkbox(crypt_str("Multipoint"), &g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints);

			if (g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints)
			{
				pDraw::multiselect2(crypt_str("Multipoint hitboxes"), g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints_hitboxes, hitboxes, ARRAYSIZE(hitboxes), hitboxes);
				ImGui::SliderFloat(crypt_str("Multipoint scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints_scale, 0.0f, 1.0f, g_cfg.ragebot.weapon[hooks::rage_weapon].multipoints_scale ? crypt_str("%.2f") : crypt_str("None"));
				ImGui::SliderFloat(crypt_str("Stomach scale"), &g_cfg.ragebot.weapon[hooks::rage_weapon].stomach_scale, 0.0f, 1.0f, g_cfg.ragebot.weapon[hooks::rage_weapon].stomach_scale ? crypt_str("%.2f") : crypt_str("None"));
			}

			g_cfg.ragebot.weapon[hooks::rage_weapon].hitchance = true;
			//ImGui::Checkbox(crypt_str("Hit chance"), &);

			//if (g_cfg.ragebot.weapon[hooks::rage_weapon].hitchance)
			ImGui::SliderInt(crypt_str("Hitchance"), &g_cfg.ragebot.weapon[hooks::rage_weapon].hitchance_amount, 1, 100);

			ImGui::SliderInt(crypt_str("Damage"), &g_cfg.ragebot.weapon[hooks::rage_weapon].minimum_visible_damage, 1, 120, true);

			if (g_cfg.ragebot.autowall)
				ImGui::SliderInt(crypt_str("Wall damage"), &g_cfg.ragebot.weapon[hooks::rage_weapon].minimum_damage, 1, 120, true);

			//vdd
			//ImGui::Checkbox(crypt_str("Teleport"), &g_cfg.ragebot.slow_teleport);
			pDraw::addcheck(crypt_str("Resolver"), &g_cfg.ragebot.pitch_antiaim_correction);

			ImGui::Checkbox(crypt_str("Max misses"), &g_cfg.ragebot.weapon[hooks::rage_weapon].max_misses);
			ImGui::Spacing();
			

			if (g_cfg.ragebot.weapon[hooks::rage_weapon].max_misses)
				ImGui::SliderInt(crypt_str("##max_misses"), &g_cfg.ragebot.weapon[hooks::rage_weapon].max_misses_amount, 0, 6);

			ImGui::Checkbox(crypt_str("Prefer safe points"), &g_cfg.ragebot.weapon[hooks::rage_weapon].prefer_safe_points);
			ImGui::Checkbox(crypt_str("Prefer baim"), &g_cfg.ragebot.weapon[hooks::rage_weapon].prefer_body_aim);

			
			//pGui::draw_keybind(crypt_str("Force baim"), &g_cfg.ragebot.body_aim_key, crypt_str("##HOKEY_FORCE_BODY_AIM"));

			g_cfg.ragebot.weapon[hooks::rage_weapon].double_tap_hitchance = true;
			g_cfg.ragebot.weapon[hooks::rage_weapon].double_tap_hitchance_amount = 60;
		} 




		ImGui::EndChild();
	}
	else
	{
		//Anti-Aim
		pDraw::addchild("Rage_3", ImVec2((size.x - 35) / 3, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			static auto type = 0;
			//static bool g = false;
			pDraw::addcheck(crypt_str("Enabled"), &kdopipi::g);
			//draw_combo(crypt_str("Anti-aim type"), g_cfg.antiaim.antiaim_type, antiaim_type, ARRAYSIZE(antiaim_type));

			const char* movement_type[] =
			{
				"Standing",
				"Slow walking",
				"Moving",
				"In Air"
			};

			const char* pitch[] =
			{
				"None",
				"Down",
				"Up",
			};

			const char* yaw[] =
			{
				"Backwards",
				"Jitter"
			};

			const char* baseangle[] =
			{
				"Local View",
				"At targets"
			};

			const char* desync[] =
			{
				"None",
				"Static",
				"Jitter"
			};

			const char* lby_type[] =
			{
				"Low Delta",
				"Opposite",
				"Sway"
			};


			{
				pDraw::singleselect(crypt_str("Move Type"), &type, movement_type, ARRAYSIZE(movement_type));

				pDraw::singleselect(crypt_str("Pitch"), &g_cfg.antiaim.type[type].pitch, pitch, ARRAYSIZE(pitch));
				//g_cfg.antiaim.type[type].pitch = 1;
				//g_cfg.antiaim.type[type].yaw = 0;

				pDraw::singleselect(crypt_str("Yaw"), &g_cfg.antiaim.type[type].yaw, yaw, ARRAYSIZE(yaw));

				pDraw::singleselect(crypt_str("Based On"), &g_cfg.antiaim.type[type].base_angle, baseangle, ARRAYSIZE(baseangle));

				if (g_cfg.antiaim.type[type].yaw)
				{
					ImGui::SliderInt(g_cfg.antiaim.type[type].yaw == 1 ? crypt_str("Jitter") : crypt_str("Spin range"), &g_cfg.antiaim.type[type].range, 1, 180);

					//if (g_cfg.antiaim.type[type].yaw == 2)
						//ImGui::SliderInt(crypt_str("Spin speed"), &g_cfg.antiaim.type[type].speed, 1, 15);
				}
				//g_cfg.antiaim.type[type].desync = 1;
				pDraw::singleselect(crypt_str("Desync"), &g_cfg.antiaim.type[type].desync, desync, ARRAYSIZE(desync));

				if (g_cfg.antiaim.type[type].desync)
				{
					//if (type == ANTIAIM_STAND)
					{
						pDraw::singleselect(crypt_str("LBY type"), &g_cfg.antiaim.lby_type, lby_type, ARRAYSIZE(lby_type));
					}

					if (!g_cfg.antiaim.lby_type)
					{
						ImGui::SliderInt(crypt_str("Desync ammount"), &g_cfg.antiaim.type[type].desync_range, 1, 60);
					}

					ImGui::Spacing();


				}








				if (g_cfg.antiaim.manual_back.key > KEY_NONE && g_cfg.antiaim.manual_back.key < KEY_MAX || g_cfg.antiaim.manual_left.key > KEY_NONE && g_cfg.antiaim.manual_left.key < KEY_MAX || g_cfg.antiaim.manual_right.key > KEY_NONE && g_cfg.antiaim.manual_right.key < KEY_MAX)
				{
					ImGui::Checkbox(crypt_str("Manual indicator"), &g_cfg.antiaim.flip_indicator);
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##invc"), &g_cfg.antiaim.flip_indicator_color);
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();
#include "../model_3d.h"
		pDraw::addchild("Rage_4", ImVec2((size.x - 35) / 3, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* fakelags[] =
			{
				"Static",
				"Switch",
				"Dynamic",
				"Factor",
				"Nuclear"
			};

			const char* lagstrigger[] =
			{
				"Slow walk",
				"Move",
				"Air",
				"Peek"
			};


			ImGui::Checkbox(crypt_str("Fake-lag"), &g_cfg.antiaim.fakelag);
			if (g_cfg.antiaim.fakelag)
			{
				pDraw::singleselect(crypt_str("Type##fucklag"), &g_cfg.antiaim.fakelag_type, fakelags, ARRAYSIZE(fakelags));
				ImGui::SliderInt(crypt_str("Value"), &g_cfg.antiaim.fakelag_amount, 1, 16);
				ImGui::Spacing();
				//ImGui::Checkbox(crypt_str("Break Enemy Prediction"), &g_cfg.misc.break_prediction);

				//pDraw::multiselect2(crypt_str("Triggers"), g_cfg.antiaim.fakelag_enablers, lagstrigger, ARRAYSIZE(lagstrigger));

				//auto enabled_fakelag_triggers = true;
//
				//for (auto i = 0; i < ARRAYSIZE(lagstrigger); i++)
				//	if (g_cfg.antiaim.fakelag_enablers[i])
				//		enabled_fakelag_triggers = true;

				//	ImGui::SliderInt(crypt_str("Triggers limit"), &g_cfg.antiaim.triggers_fakelag_amount, 1, 16);
					
					


			}

			//ImGui::Checkbox(crypt_str("Lag Sync"), &g_cfg.antiaim.lagsync);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("Rage_5", ImVec2((size.x - 35) / 3, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{


			pGui::draw_keybind(crypt_str("Invert"), &g_cfg.antiaim.flip_desync, crypt_str("##HOTKEY_INVERT_DESYNC"));
			ImGui::Spacing();
			pGui::draw_keybind(crypt_str("Peek"), &g_cfg.misc.automatic_peek, crypt_str("##HOTKEY_PEEk_DESYNC"));
			ImGui::Text(crypt_str("peek color"));
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##AUTOPEEKCOLOR"), &g_cfg.esp.autopeek_color);
			ImGui::Spacing();
			pGui::draw_keybind(crypt_str("Slow walk"), &g_cfg.misc.slowwalk_key, crypt_str("##SLOWWALK__HOTKEY"));
			if (g_cfg.misc.slowwalk_key.key > KEY_NONE && g_cfg.misc.slowwalk_key.key <= KEY_MAX)
			{
				ImGui::Spacing();
				pDraw::addslider("Velocity", &g_cfg.ragebot.slow_walk_velocity, 0, 100);
				ImGui::Spacing();
			}
			else
			{
				ImGui::Spacing();
			}
			pGui::draw_keybind(crypt_str("Fake duck"), &g_cfg.misc.fakeduck_key, crypt_str("##FAKEDUCK__HOTKEY"));
			ImGui::Spacing();
		    pGui::draw_keybind(crypt_str("Force baim"), &g_cfg.ragebot.body_aim_key, crypt_str("##HOKEY_FORCE_BODY_AIM"));
			ImGui::Spacing();
			pGui::draw_keybind(crypt_str("Override"), &g_cfg.ragebot.weapon[hooks::rage_weapon].damage_override_key, crypt_str("##HOTKEY__DAMAGE_OVERRIDE"));
			ImGui::Spacing();
			if (g_cfg.ragebot.weapon[hooks::rage_weapon].damage_override_key.key > KEY_NONE && g_cfg.ragebot.weapon[hooks::rage_weapon].damage_override_key.key < KEY_MAX)
				ImGui::SliderInt(crypt_str("Damage Value"), &g_cfg.ragebot.weapon[hooks::rage_weapon].minimum_override_damage, 1, 120, true);
			ImGui::Spacing();
			pGui::draw_keybind(crypt_str("Safe Points"), &g_cfg.ragebot.safe_point_key, crypt_str("##HOKEY_FORCE_SAFE_POINTS"));
			ImGui::Spacing();
			pGui::draw_keybind(crypt_str("Hide Shots"), &g_cfg.antiaim.hide_shots_key, crypt_str("##HOTKEY_HIDESHOTS"));

		}

		ImGui::EndChild();
	}
	//ImGui::SameLine();
}

void legit()
{
	ImVec2 size = ImGui::GetWindowSize();
	static int tab = 0;

	pDraw::addchild("sub", ImVec2(size.x - 18, size.y - 405), true, ImGuiWindowFlags_NoScrollbar);
	{
		ImVec2 ss = ImGui::GetWindowSize();

		if (pDraw::addsubtab("MAIN", tab == 0, ImVec2((ss.x - 15) / 2, ss.y - 15))) tab = 0;
		ImGui::SameLine();
		if (pDraw::addsubtab("OTHERS", tab == 1, ImVec2((ss.x - 15) / 2, ss.y - 15))) tab = 1;
	}
	ImGui::EndChild();

	if (tab == 0)
	{
		const char* legit_weapons[6] = { ("Deagle"), ("Pistols"), ("Rifles"), ("SMG"), ("Snipers"), ("Heavy") };
		const char* hitbox_legit[3] = { ("Nearest"), ("Head"), ("Body") };

		pDraw::addchild("legit", ImVec2((size.x - 30) / 3, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{

			ImGui::Checkbox(crypt_str("Enabled"), &g_cfg.legitbot.enabled);
			ImGui::SameLine();
			pGui::draw_keybind(crypt_str(""), &g_cfg.legitbot.key, crypt_str("##HOTKEY_LGBT_KEY")); //lgbt

			if (g_cfg.legitbot.enabled)
				g_cfg.ragebot.enable = false;

			ImGui::Checkbox(crypt_str("Friendly fire"), &g_cfg.legitbot.friendly_fire);
			ImGui::Checkbox(crypt_str("Auto pistols"), &g_cfg.legitbot.autopistol);

			static bool a[4] = { g_cfg.legitbot.do_if_local_in_air, g_cfg.legitbot.do_if_local_flashed, g_cfg.legitbot.do_if_enemy_in_smoke, g_cfg.legitbot.sniper_in_zoom_only };
			/*a[0] =  a[1] = g_cfg.legitbot.do_if_local_flashed;
			a[2] = g_cfg.legitbot.do_if_enemy_in_smoke;*/
			const char* y[] = { "Air", "Flashed", "Smoke", "Zoom" };

			pDraw::multiselect("Aim checks", y, a, IM_ARRAYSIZE(y));
			ImGui::Spacing();
			//ImGui::Checkbox(crypt_str("Aim if in air"), &g_cfg.legitbot.do_if_local_in_air);
			//ImGui::Checkbox(crypt_str("Aim if flashed"), &g_cfg.legitbot.do_if_local_flashed);//vou sair .=. manda a sourc
			//ImGui::Checkbox(crypt_str("Aim trough smoke"), &g_cfg.legitbot.do_if_enemy_in_smoke);

			ImGui::Checkbox(crypt_str("Backtrack"), &g_cfg.legitbot.backtrackl);
			//pDraw::addcheck(crypt_str("Enabled AA"), &g_cfg.antiaim.enable);//esse cu nao funfa abre o notepad macaco
			static bool g = false;//vc n compilou macaco comp sim burro


			//ImGui::Checkbox(crypt_str("Legit Resolver"), &g_cfg.legitbot.legit_resolver);
			//draw_keybind(crypt_str("Automatic fire key"), &g_cfg.legitbot.autofire_key, crypt_str("##HOTKEY_AUTOFIRE_LGBT_KEY"));
			//ImGui::SliderInt(crypt_str("Automatic fire delay"), &g_cfg.legitbot.autofire_delay, 0, 12, false, (!g_cfg.legitbot.autofire_delay ? crypt_str("None") : (g_cfg.legitbot.autofire_delay == 1 ? crypt_str("%d tick") : crypt_str("%d ticks"))));
		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("legit2", ImVec2((size.x - 30) / 3, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* LegitFov[] =
			{
				"Static",
				"Dynamic"
			};

			pDraw::singleselect(crypt_str("Weapon"), &hooks::legit_weapon, legit_weapons, ARRAYSIZE(legit_weapons));
			//ImGui::Spacing();
			pDraw::singleselect(crypt_str("Hitbox"), &g_cfg.legitbot.weapon[hooks::legit_weapon].priority, hitbox_legit, ARRAYSIZE(hitbox_legit));

			//ImGui::Checkbox(crypt_str("Auto stop"), &g_cfg.legitbot.weapon[hooks::legit_weapon].auto_stop);
			//ImGui::Checkbox(crypt_str("Fast stop"), &g_cfg.misc.fast_stop);-

			pDraw::singleselect(crypt_str("FOV mode"), &g_cfg.legitbot.weapon[hooks::legit_weapon].fov_type, LegitFov, ARRAYSIZE(LegitFov));
			ImGui::SliderFloat(crypt_str("FOV value"), &g_cfg.legitbot.weapon[hooks::legit_weapon].fov, 0.f, 30.f);

			//ImGui::Spacing();

			ImGui::SliderFloat(crypt_str("Silent FOV"), &g_cfg.legitbot.weapon[hooks::legit_weapon].silent_fov, 0.f, 30.f, (!g_cfg.legitbot.weapon[hooks::legit_weapon].silent_fov ? crypt_str("None") : crypt_str("%.2f"))); //-V550

			//ImGui::Spacing();

			const char* LegitSmooth[] =
			{
				"Static",
				"Humanized"
			};

			pDraw::singleselect(crypt_str("Smooth type"), &g_cfg.legitbot.weapon[hooks::legit_weapon].smooth_type, LegitSmooth, ARRAYSIZE(LegitSmooth));
			ImGui::SliderFloat(crypt_str("Smooth value"), &g_cfg.legitbot.weapon[hooks::legit_weapon].smooth, 1.f, 20.f);

			//ImGui::Spacing();


			ImGui::SliderFloat(crypt_str("Switch delay"), &g_cfg.legitbot.weapon[hooks::legit_weapon].target_switch_delay, 0.f, 1.f);
			ImGui::SliderFloat(crypt_str("Backtrack ticks"), &g_cfg.legitbot.weapon[hooks::legit_weapon].backtrack_ticks, 0, 18);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("rcs", ImVec2((size.x - 30) / 3, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* legit_weapons[6] = { ("Deagle"), ("Pistols"),("Rifles"), ("SMGs"), ("Snipers"), ("Heavy") };
			const char* hitbox_legit[3] = { ("Nearest"), ("Head"), ("Body") };

			const char* RCSType[] =
			{
				"Standalone",
				"Aim"
			};


			pDraw::singleselect(crypt_str("RCS Weapon"), &hooks::legit_weapon, legit_weapons, ARRAYSIZE(legit_weapons));

			pDraw::singleselect(crypt_str("RCS Type"), &g_cfg.legitbot.weapon[hooks::legit_weapon].rcs_type, RCSType, ARRAYSIZE(RCSType));
			ImGui::SliderFloat(crypt_str("RCS Value"), &g_cfg.legitbot.weapon[hooks::legit_weapon].rcs, 0.f, 100.f, crypt_str("%.0f%%"), 1.f);


			ImGui::SliderFloat(crypt_str("RCS FOV"), &g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_fov, 0.f, 30.f, (!g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_fov ? crypt_str("None") : crypt_str("%.2f"))); //-V550
			ImGui::SliderFloat(crypt_str("RCS Smooth"), &g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_smooth, 0.f, 20.f, (!g_cfg.legitbot.weapon[hooks::legit_weapon].custom_rcs_smooth ? crypt_str("None") : crypt_str("%.1f"))); //-V550

		}
		ImGui::EndChild();
	}
	else
	{
		pDraw::addchild("blatant_1", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* desync[] =
			{
				"None",
				"Static",
			};

			const char* lby_type[] =
			{
				"Normal",
				"Opposite",
			};

			pDraw::addcheck(crypt_str("Legit Anti-Aim"), &kdopipi::gg);

			//pDraw::singleselect(crypt_str("Legit AA"), &g_cfg.antiaim.antiaim_type, antiaim_type, ARRAYSIZE(antiaim_type));
			if (kdopipi::gg)
			{
				//pDraw::singleselect(crypt_str("Desync"), &g_cfg.antiaim.desync, desync, ARRAYSIZE(desync));
				g_cfg.antiaim.desync = 1;

				pDraw::singleselect(crypt_str("LBY type"), &g_cfg.antiaim.legit_lby_type, lby_type, ARRAYSIZE(lby_type));

				pGui::draw_keybind(crypt_str("Invert"), &g_cfg.antiaim.flip_desync, crypt_str("##HOTKEY_INVERT_DESYNC"));

				//pGui::draw_keybind(crypt_str("Trigger"), &g_cfg.legitbot.autofire_key, crypt_str("##TRIGGER_KEY"));

			}

			//ImGui::Checkbox(crypt_str("Unscope after shot"), &g_cfg.legitbot.unscope);

			//ImGui::Checkbox(crypt_str("Zoom only"), &g_cfg.legitbot.sniper_in_zoom_only);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("blatant_2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			pGui::draw_keybind(crypt_str("Trigger"), &g_cfg.legitbot.autofire_key, crypt_str("##TRIGGER_KEY"));
			if (g_cfg.legitbot.autofire_key.key > KEY_NONE && g_cfg.legitbot.autofire_key.key <= KEY_MAX)
			{
				ImGui::SliderInt(crypt_str("Auto Fire delay"), &g_cfg.legitbot.autofire_delay, 0, 12, false, "%d");
				ImGui::SliderInt(crypt_str("Auto Fire Minimum Damage"), &g_cfg.legitbot.weapon[hooks::legit_weapon].awall_dmg, 0, 100, false, "%d");
				ImGui::SliderInt(crypt_str("Auto Fire Hitchance"), &g_cfg.legitbot.weapon[hooks::legit_weapon].autofire_hitchance, 0, 100, false, "%d");
			}
			ImGui::Checkbox(crypt_str("Auto scope"), &g_cfg.legitbot.autoscope);
			ImGui::Checkbox(crypt_str("Auto stop"), &g_cfg.legitbot.weapon[hooks::legit_weapon].auto_stop);


		}
		ImGui::EndChild();
	}
}

void visuals()
{
	auto player = g_cfg.player.teams;
	ImVec2 size = ImGui::GetWindowSize();
	static int tab = 0;

	pDraw::addchild("sub", ImVec2(size.x - 18, size.y - 405), true, ImGuiWindowFlags_NoScrollbar);
	{
		ImVec2 ss = ImGui::GetWindowSize();

		if (pDraw::addsubtab("ESP", tab == 0, ImVec2((ss.x - 15) / 3, ss.y - 15))) tab = 0;
		ImGui::SameLine();
		if (pDraw::addsubtab("CHAMS", tab == 1, ImVec2((ss.x - 15) / 3, ss.y - 15))) tab = 1;
		ImGui::SameLine();
		if (pDraw::addsubtab("OTHER", tab == 2, ImVec2((ss.x - 15) / 3, ss.y - 15))) tab = 2;
	}
	ImGui::EndChild();

	const char* player_teams[] =
	{
		"Enemy",
		"Team",
		"Local"
	};

	if (tab == 0)
	{


		const char* flags[] =
		{
			"Money",
			"Armor",
			"Defuse",
			"Scoped",
			"Fake Duck",
			"Lethal",
			"Bomb"
		};

		const char* weaponplayer[] =
		{
			"Icon",
			"Text"
		};

		pDraw::addchild("visuals_1", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			ImGui::Checkbox(crypt_str("Enabled"), &g_cfg.player.enable);
			/*ImGui::Checkbox(crypt_str("Enable key"), &g_cfg.player.want_key);
			if (g_cfg.player.want_key)
			{
				ImGui::SameLine();
				pGui::draw_keybind("", &g_cfg.player.enable_key, "##ESP_KEY_MONKEY");
			}*/

			pDraw::singleselect(crypt_str("Set team"), &g_cfg.player.teams, player_teams, ARRAYSIZE(player_teams));
			/*if (player == 0 || ENEMY)
			{
				if (g_cfg.player.arrows)
				{
					ImGui::SliderInt(crypt_str("Arrows distance"), &g_cfg.player.distance, 1, 100);
					ImGui::SliderInt(crypt_str("Arrows size"), &g_cfg.player.size, 1, 100);
				}
			}*/

			ImGui::Checkbox(crypt_str("Box"), &g_cfg.player.type[player].box);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##Box Color"), &g_cfg.player.type[player].box_color);

			ImGui::Checkbox(crypt_str("Name"), &g_cfg.player.type[player].name);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##namecolor"), &g_cfg.player.type[player].name_color);

			ImGui::Checkbox(crypt_str("Health"), &g_cfg.player.type[player].health);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##healthcolor"), &g_cfg.player.type[player].health_color);
			g_cfg.player.type[player].custom_health_color = true;
			//ImGui::Checkbox(crypt_str(" health color"), &g_cfg.player.type[player].custom_health_color);
			//ImGui::ColorEdit(crypt_str("##healthcolor"), &g_cfg.player.type[player].health_color, ALPHA);

			pDraw::multiselect2(crypt_str("Flags"), g_cfg.player.type[player].flags, flags, IM_ARRAYSIZE(flags), flags);
			pDraw::multiselect2(crypt_str("Weapon"), g_cfg.player.type[player].weapon, weaponplayer, ARRAYSIZE(weaponplayer), weaponplayer);

			if (g_cfg.player.type[player].weapon[WEAPON_ICON] || g_cfg.player.type[player].weapon[WEAPON_TEXT])
			{
				ImGui::Text("Weapon Color");
				ImGui::SameLine();
				ImGui::ColorEdit(crypt_str("##Weapon Color"), &g_cfg.player.type[player].weapon_color);
			}

			ImGui::Checkbox(crypt_str("Skeleton"), &g_cfg.player.type[player].skeleton);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##skeletoncolor"), &g_cfg.player.type[player].skeleton_color);

			ImGui::Checkbox(crypt_str("Show points"), &g_cfg.player.show_multi_points);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##showmultipointscolor"), &g_cfg.player.show_multi_points_color);

			//ImGui::Checkbox(crypt_str("Ammo bar"), &g_cfg.player.type[player].ammo);
			//ImGui::SameLine();
			//ImGui::ColorEdit(crypt_str("##ammocolor"), &g_cfg.player.type[player].ammobar_color);
		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("visuals_2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{



			const char* weaponesp[] =
			{
				"Icon",
				"Name",
				"Box",
			};


			if (player == 0 || ENEMY || player == 1 || TEAM)
			{
				pDraw::multiselect2(crypt_str("Weapon ESP"), g_cfg.esp.weapon, weaponesp, ARRAYSIZE(weaponesp), weaponesp);

				if (g_cfg.esp.weapon[WEAPON_ICON] || g_cfg.esp.weapon[WEAPON_TEXT] || g_cfg.esp.weapon[WEAPON_AMMO])
				{
					ImGui::Text(crypt_str("Color "));
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##weaponcolor"), &g_cfg.esp.weapon_color);
				}

				if (g_cfg.esp.weapon[WEAPON_BOX])
				{
					ImGui::Text(crypt_str("Box color "));
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##weaponboxcolor"), &g_cfg.esp.box_color);
				}


				if (g_cfg.esp.weapon[WEAPON_AMMO])
				{
					ImGui::Text(crypt_str("Ammo bar color "));
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##weaponammocolor"), &g_cfg.esp.weapon_ammo_color);
				}
			}
			else
			{

			}
		}
		ImGui::EndChild();

	}
	else if (tab == 1)
	{
		const char* local_chams_type[] =
		{
			"Local",
			"Desync"
		};

		const char* chamsvisact[] =
		{
			"Visible",
			"Invisible"
		};

		const char* chamsvis[] =
		{
			"Visible"
		};

		const char* glowtype[] =
		{
			"Standard",
			"Pulse",
			"Inner"
		};

		const char* chamstype[] =
		{
			"Regular",
			"Mettalic",
			"Flat",
			"Pulse",
			"Glaussian",
			"Glass",
			"Outline",
			"Gold",
			"Glow",
			"Animated"
		};

		pDraw::addchild("chams_1", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{

			pDraw::singleselect(crypt_str("Set team"), &g_cfg.player.teams, player_teams, ARRAYSIZE(player_teams));
			//tab_start();
				//ImGui::Spacing();
			if (g_cfg.player.teams == 2)
				pDraw::singleselect(crypt_str("Chams Type"), &g_cfg.player.local_chams_type, local_chams_type, ARRAYSIZE(local_chams_type));

			if (player != 2 || LOCAL || !g_cfg.player.local_chams_type)
				pDraw::multiselect2(crypt_str("Chams"), g_cfg.player.type[player].chams, chamsvisact, ARRAYSIZE(chamsvisact), g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] ? chamsvisact : chamsvis);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("chams_2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			if (g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] || player == 2 || LOCAL && g_cfg.player.local_chams_type) //-V648
			{
				if (player == LOCAL && g_cfg.player.local_chams_type)
				{
					ImGui::Checkbox(crypt_str("Enable desync chams"), &g_cfg.player.fake_chams_enable);
					ImGui::Spacing();
					ImGui::Checkbox(crypt_str("Visualize Lag"), &g_cfg.player.visualize_lag);
					//ImGui::Checkbox(crypt_str("Desync Layered"), &g_cfg.player.layered);

					pDraw::singleselect(crypt_str("Local chams material"), &g_cfg.player.fake_chams_type, chamstype, ARRAYSIZE(chamstype));

					ImGui::Text(crypt_str("Desync Chams Color "));
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##Desync Chams Color"), &g_cfg.player.fake_chams_color);

					/*if (g_cfg.player.fake_chams_type != 6)
					{
						ImGui::Checkbox(crypt_str("Fake Double material "), &g_cfg.player.fake_double_material);
						ImGui::SameLine();
						ImGui::ColorEdit(crypt_str("##doublematerialcolor"), &g_cfg.player.fake_double_material_color);
					}

					//ImGui::Checkbox(crypt_str("Animated material"), &g_cfg.player.fake_animated_material);
					//ImGui::SameLine();
					//ImGui::ColorEdit(crypt_str("##animcolormat"), &g_cfg.player.fake_animated_material_color);*/

					/*ImGui::Checkbox(crypt_str("Arms chams"), &g_cfg.esp.arms_chams);
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##armscolor"), &g_cfg.esp.arms_chams_color);


					pDraw::singleselect(crypt_str("Arms chams material"), &g_cfg.esp.arms_chams_type, chamstype, ARRAYSIZE(chamstype));

					if (g_cfg.esp.arms_chams_type != 6)
					{
						ImGui::Checkbox(crypt_str("Arms double material "), &g_cfg.esp.arms_double_material);
						ImGui::SameLine();
						ImGui::ColorEdit(crypt_str("##armsdoublematerial"), &g_cfg.esp.arms_double_material_color);
					}

					ImGui::Checkbox(crypt_str("Arms animated material "), &g_cfg.esp.arms_animated_material);
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##armsanimatedmaterial"), &g_cfg.esp.arms_animated_material_color);

					ImGui::Spacing();

					ImGui::Checkbox(crypt_str("Weapon chams"), &g_cfg.esp.weapon_chams);
					ImGui::SameLine();
					ImGui::ColorEdit(crypt_str("##weaponchamscolors"), &g_cfg.esp.weapon_chams_color);

					pDraw::singleselect(crypt_str("Weapon chams material"), &g_cfg.esp.weapon_chams_type, chamstype, ARRAYSIZE(chamstype));
					*/



				}
				else
				{
					pDraw::singleselect(crypt_str("Chams material"), &g_cfg.player.type[player].chams_type, chamstype, ARRAYSIZE(chamstype));

					if (g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE])
					{
						ImGui::Text(crypt_str("Visible "));
						ImGui::SameLine();
						ImGui::ColorEdit(crypt_str("##chamsvisible"), &g_cfg.player.type[player].chams_color);
					}

					if (g_cfg.player.type[player].chams[PLAYER_CHAMS_VISIBLE] && g_cfg.player.type[player].chams[PLAYER_CHAMS_INVISIBLE])
					{
						ImGui::Text(crypt_str("Invisible "));
						ImGui::SameLine();
						ImGui::ColorEdit(crypt_str("##chamsinvisible"), &g_cfg.player.type[player].xqz_color);
					}

					if (player == 0 || ENEMY)

						ImGui::Checkbox(crypt_str("Backtrack chams"), &g_cfg.player.backtrack_chams);
					{
						if (g_cfg.player.backtrack_chams)

							pDraw::singleselect(crypt_str("Backtrack chams material"), &g_cfg.player.backtrack_chams_material, chamstype, ARRAYSIZE(chamstype));

						ImGui::Text(crypt_str("Color "));
						ImGui::SameLine();
						ImGui::ColorEdit(crypt_str("##backtrackcolor"), &g_cfg.player.backtrack_chams_color);
					}
					if (!g_cfg.player.local_chams_type)
					{
						ImGui::Checkbox(crypt_str("Scope Transparency"), &g_cfg.player.transparency_in_scope);

						if (g_cfg.player.transparency_in_scope)
							ImGui::SliderFloat(crypt_str("Alpha"), &g_cfg.player.transparency_in_scope_amount, 0.0f, 1.0f);
					}

					if (player == ENEMY)
					{
						ImGui::Checkbox("Draw chams on hit", &g_cfg.player.chams_on_hit);

						ImGui::Text("Hit color");
						ImGui::SameLine();
						ImGui::ColorEdit("##hit_color", &g_cfg.player.chams_hit_color);

						if (g_cfg.player.chams_on_hit)
						{
							pDraw::singleselect(crypt_str("Chams on hit material"), &g_cfg.player.chams_hit_material, chamstype, ARRAYSIZE(chamstype));
						}


						pDraw::addcheck(crypt_str("Ragdoll chams"), &g_cfg.player.type[player].ragdoll_chams);

						if (g_cfg.player.type[player].ragdoll_chams)
						{
							pDraw::singleselect(crypt_str("Ragdoll chams material"), &g_cfg.player.type[player].ragdoll_chams_material, chamstype, IM_ARRAYSIZE(chamstype));

							ImGui::ColorEdit(crypt_str("##ragdollcolor"), &g_cfg.player.type[player].ragdoll_chams_color);
						}

						ImGui::Checkbox(crypt_str("Glow"), &g_cfg.player.type[player].glow);



						if (g_cfg.player.type[player].glow)
						{
							pDraw::singleselect(crypt_str("Glow type"), &g_cfg.player.type[player].glow_type, glowtype, ARRAYSIZE(glowtype));
							ImGui::Text(crypt_str("Color "));
							ImGui::SameLine();
							ImGui::ColorEdit(crypt_str("##glowcolor"), &g_cfg.player.type[player].glow_color);
						}
					}
				}
			}
		}
		ImGui::EndChild();
	}
	else if (tab == 2)
	{
		pDraw::addchild("other", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			ImGui::Checkbox(crypt_str("Enabled"), &g_cfg.player.enable);

			//draw_multicombo(crypt_str("Indicators"), g_cfg.esp.indicators, indicators, ARRAYSIZE(indicators), preview);
			//padding(0, 3);

			const char* removals[] =
			{
				"Scope",
				"Zoom",
				"Smoke",
				"Flash",
				"Recoil",
				"Landing bob",
				"Postprocessing",
				"Fog"
			};

			pDraw::multiselect2(crypt_str("Removals"), g_cfg.esp.removals, removals, ARRAYSIZE(removals), removals);


			const char* hitmarkers[] =
			{
				"Local",
				"World"
			};

			const char* indicators[] =
			{
				"Fake",
				"Desync side",
				"Choke",
				"Damage override",
				"Safe points",
				"Body aim",
				"Double tap",
				"Hide shots"
			};

			const char* proj_combo[] =
			{
				"Icon",
			};


			ImGui::Checkbox(crypt_str("Damage Indicator"), &g_cfg.esp.damage_marker);

			ImGui::Checkbox(crypt_str("Grenade projectiles"), &g_cfg.esp.projectiles);

			if (g_cfg.esp.projectiles)
				pDraw::multiselect2(crypt_str("Grenade ESP"), g_cfg.esp.grenade_esp, proj_combo, ARRAYSIZE(proj_combo), proj_combo);

			if (g_cfg.esp.grenade_esp[GRENADE_ICON] || g_cfg.esp.grenade_esp[GRENADE_TEXT])
			{
				ImGui::Text(crypt_str("Color "));
				ImGui::SameLine();
				ImGui::ColorEdit(crypt_str("##projectcolor"), &g_cfg.esp.projectiles_color);
			}

			if (g_cfg.esp.grenade_esp[GRENADE_BOX])
			{
				ImGui::Text(crypt_str("Box color "));
				ImGui::SameLine();
				ImGui::ColorEdit(crypt_str("##grenade_box_color"), &g_cfg.esp.grenade_box_color);
			}

			if (g_cfg.esp.grenade_esp[3])
			{
				//ImGui::Checkbox()
				ImGui::Text("Radius Enemy Color"); ImGui::SameLine();
				ImGui::ColorEdit("##enemy_colOr_r", &g_cfg.esp.molotov_shit_enemy);

				ImGui::Text("Radius Team Color"); ImGui::SameLine();
				ImGui::ColorEdit("##team_color_r", &g_cfg.esp.molotov_shit_team);
			}

			//pDraw::multiselect2(crypt_str("Grenade ESP"), g_cfg.esp.grenade_esp, proj_combo, ARRAYSIZE(proj_combo), proj_combo);

			pDraw::multiselect2(crypt_str("Hit marker"), g_cfg.esp.hitmarker, hitmarkers, ARRAYSIZE(hitmarkers), hitmarkers);

			if (g_cfg.esp.kill_effect)
				ImGui::SliderFloat(crypt_str("Duration"), &g_cfg.esp.kill_effect_duration, 0.01f, 3.0f);
			ImGui::Checkbox(crypt_str("Fog modulation"), &g_cfg.esp.fog);

			if (g_cfg.esp.fog)
			{
				ImGui::SliderInt(crypt_str("Distance"), &g_cfg.esp.fog_distance, 0, 2500);
				ImGui::SliderInt(crypt_str("Density"), &g_cfg.esp.fog_density, 0, 100);
				ImGui::Text(crypt_str("Color "));
				ImGui::SameLine();
				ImGui::ColorEdit(crypt_str("##fogcolor"), &g_cfg.esp.fog_color);
			}

			ImGui::Checkbox(crypt_str("Bullet Impacts"), &g_cfg.esp.client_bullet_impacts);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##clientbulletimpacts"), &g_cfg.esp.client_bullet_impacts_color);

			//ImGui::Checkbox(crypt_str("Server bullet impacts"), &g_cfg.esp.server_bullet_impacts);
			//ImGui::SameLine();
			//ImGui::ColorEdit(crypt_str("##serverbulletimpacts"), &g_cfg.esp.server_bullet_impacts_color);

			ImGui::Checkbox(crypt_str("Local bullet tracers"), &g_cfg.esp.bullet_tracer);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##bulltracecolor"), &g_cfg.esp.bullet_tracer_color);

			ImGui::Checkbox(crypt_str("Enemy bullet tracers"), &g_cfg.esp.enemy_bullet_tracer);
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##enemybulltracecolor"), &g_cfg.esp.enemy_bullet_tracer_color);

			const char* bullet_traces[] =
			{
				"Glow",
				"Laser",
				"Flat",
				"Balls"
			};

			pDraw::singleselect("Tracer sprite", &g_cfg.esp.tracer_sel, bullet_traces, ARRAYSIZE(bullet_traces));

			ImGui::Checkbox(crypt_str("Grenade Prediction"), &g_cfg.esp.grenade_prediction);
			if (g_cfg.esp.grenade_prediction)
			{

				ImGui::ColorEdit(("##Grenade_Prediction"), &g_cfg.esp.grenade_prediction_color);

			}



			//ImGui::Checkbox(crypt_str("Damage marker"), &g_cfg.esp.damage_marker);

		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("other2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* skybox[] =
			{
				"None",
				"Tibet",
				"Baggage",
				"Italy",
				"Aztec",
				"Vertigo",
				"Daylight",
				"Daylight 2",
				"Clouds",
				"Clouds 2",
				"Gray",
				"Clear",
				"Canals",
				"Cobblestone",
				"Assault",
				"Clouds dark",
				"Night",
				"Night 2",
				"Night flat",
				"Dusty",
				"Rainy",
				"Moon"
			};


			

			pGui::draw_keybind(crypt_str("Third Person"), &g_cfg.misc.thirdperson_toggle, crypt_str("##TPKEY__HOTKEY"));
		
			if (g_cfg.misc.thirdperson_toggle.key > KEY_NONE && g_cfg.misc.thirdperson_toggle.key < KEY_MAX)
			{
			//	ImGui::Checkbox(crypt_str("Disable on grenade"), &g_cfg.misc.tp_grenade);
				ImGui::Checkbox(crypt_str("Spect Third Person"), &g_cfg.misc.thirdperson_when_spectating);

				ImGui::SliderInt(crypt_str("Third Person distance"), &g_cfg.misc.thirdperson_distance, 100, 300);
			}
				

			ImGui::SliderInt(crypt_str("Viewmodel"), &g_cfg.esp.fov, 0, 89);
			ImGui::Spacing();
			ImGui::SliderInt(crypt_str("Viewmodel X"), &g_cfg.esp.viewmodel_x, -50, 50);
			ImGui::Spacing();
			ImGui::SliderInt(crypt_str("Viewmodel Y"), &g_cfg.esp.viewmodel_y, -50, 50);
			ImGui::Spacing();
			//ImGui::SliderInt(crypt_str("Viewmodel Z"), &g_cfg.esp.viewmodel_z, -50, 50);
			//ImGui::Spacing();
			ImGui::SliderInt(crypt_str("Viewmodel Z"), &g_cfg.esp.viewmodel_z, -180, 180);
			ImGui::Spacing();
			//ImGui::Checkbox(crypt_str("Show spread"), &g_cfg.esp.show_spread);
			//ImGui::SameLine();
			pDraw::singleselect(crypt_str("Skybox"), &g_cfg.esp.skybox, skybox, ARRAYSIZE(skybox));

			ImGui::Text(crypt_str("Color "));
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##skyboxcolor"), &g_cfg.esp.skybox_color);

			/*if (g_cfg.esp.skybox == 21)
			{
				static char sky_custom[64] = "\0";

				if (!g_cfg.esp.custom_skybox.empty())
					strcpy_s(sky_custom, sizeof(sky_custom), g_cfg.esp.custom_skybox.c_str());

				ImGui::Text(crypt_str("Name"));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

				if (ImGui::InputText(crypt_str("##customsky"), sky_custom, sizeof(sky_custom)))
					g_cfg.esp.custom_skybox = sky_custom;
				ImGui::PopStyleVar();
			}*/



			ImGui::EndChild();
		}
	}
}

void add_config()
{
	auto empty = true;

	for (auto current : g_cfg.new_config_name)
	{
		if (current != ' ')
		{
			empty = false;
			break;
		}
	}

	if (empty)
		g_cfg.new_config_name = crypt_str("config");

	//eventlogs::get().add(crypt_str("Added ") + g_cfg.new_config_name + crypt_str(" config"), false);

	if (g_cfg.new_config_name.find(crypt_str(".cfg")) == std::string::npos)
		g_cfg.new_config_name += crypt_str(".cfg");

	cfg_manager->reset_config(g_cfg.new_config_name);
	cfg_manager->config_files();

	g_cfg.selected_config = cfg_manager->files.size() - 1; //-V103
	files = cfg_manager->files;

	for (auto& current : files)
		if (current.size() > 2)
			current.erase(current.size() - 3, 3);
}

void save_config()
{
	if (cfg_manager->files.empty())
		return;

	g_cfg.scripts.scripts.clear();

	for (auto i = 0; i < c_lua::get().scripts.size(); ++i)
	{
		auto script = c_lua::get().scripts.at(i);

		if (c_lua::get().loaded.at(i))
			g_cfg.scripts.scripts.emplace_back(script);
	}

	cfg_manager->save(cfg_manager->files.at(g_cfg.selected_config));
	cfg_manager->config_files();

	//eventlogs::get().add(crypt_str("Saved ") + files.at(g_cfg.selected_config) + crypt_str(" config"), false);
}

void load_config()
{
	if (cfg_manager->files.empty())
		return;

	cfg_manager->load(cfg_manager->files.at(g_cfg.selected_config), false);
	c_lua::get().unload_all_scripts();

	for (auto& script : g_cfg.scripts.scripts)
		c_lua::get().load_script(c_lua::get().get_script_id(script));

	scripts = c_lua::get().scripts;

	if (selected_script >= scripts.size())
		selected_script = scripts.size() - 1; //-V103

	for (auto& current : scripts)
	{
		if (current.size() >= 5 && current.at(current.size() - 1) == 'c')
			current.erase(current.size() - 5, 5);
		else if (current.size() >= 4)
			current.erase(current.size() - 4, 4);
	}

	for (auto i = 0; i < g_cfg.skins.skinChanger.size(); ++i)
		all_skins[i] = nullptr;

	g_cfg.scripts.scripts.clear();

	cfg_manager->load(cfg_manager->files.at(g_cfg.selected_config), true);
	cfg_manager->config_files();

	//eventlogs::get().add(crypt_str("Loaded ") + files.at(g_cfg.selected_config) + crypt_str(" config"), false);
}

void remove_config()
{
	if (cfg_manager->files.empty())
		return;

	//eventlogs::get().add(crypt_str("Removed ") + files.at(g_cfg.selected_config) + crypt_str(" config"), false);

	cfg_manager->remove(cfg_manager->files.at(g_cfg.selected_config));
	cfg_manager->config_files();

	files = cfg_manager->files;

	if (g_cfg.selected_config >= files.size())
		g_cfg.selected_config = files.size() - 1; //-V103

	for (auto& current : files)
		if (current.size() > 2)
			current.erase(current.size() - 3, 3);
}

void misc()
{
	ImVec2 size = ImGui::GetWindowSize();
	static int tab = 0;

	pDraw::addchild("sub", ImVec2(size.x - 18, size.y - 405), true, ImGuiWindowFlags_NoScrollbar);
	{
		ImVec2 ss = ImGui::GetWindowSize();

		if (pDraw::addsubtab("MISC", tab == 0, ImVec2((ss.x - 15) / 2, ss.y - 15))) tab = 0;
		ImGui::SameLine();
		if (pDraw::addsubtab("CONFIG", tab == 1, ImVec2((ss.x - 15) / 2, ss.y - 15))) tab = 1;
	}
	ImGui::EndChild();

	if (tab == 0)
	{
		pDraw::addchild("misc", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{

			const char* fakelags[] =
			{
				"Static",
				"Random",
				"Dynamic",
				"Fluctuate"
			};

			const char* lagstrigger[] =
			{
				"Slow walk",
				"Move",
				"Air",
				"Peek"
			};

			const char* sounds[] =
			{
				"None",
				"Mettalic",
				"Cod",
				"Bubble",
				//"Stapler",
				"Bell",
				"Flick",
				"Hit",
				"Laser",
				"Click"
			};

			const char* events_output[] =
			{
				"Console",
				"Chat"
			};

			const char* events[] =
			{
				"Player hits",
				"Items",
				"Bomb"
			};


			const char* mainwep[] =
			{
				"None",
				"Auto",
				"AWP",
				"SSG 08"
			};

			const char* secwep[] =
			{
				"None",
				"Dual Berettas",
				"Deagle/Revolver"
			};

			const char* grenades[] =
			{
				"Grenades",
				"Armor",
				"Taser",
				"Defuser"
			};


			const char* scope_style[] =
			{
				"None",
				"Off",
				"Style 1",
				"Style 2"
			};


			auto player = g_cfg.player.teams;
			ImGui::Checkbox(crypt_str("Unlock inventory"), &g_cfg.misc.inventory_access);
			ImGui::Checkbox(crypt_str("Gravity ragdolls"), &g_cfg.misc.ragdolls);
			//static bool log = 0;
			ImGui::Checkbox("Event Log", &kdopipi::event_log);

			if (kdopipi::event_log)
			{

			}

			ImGui::Checkbox(crypt_str("Preserve killfeed"), &g_cfg.esp.preserve_killfeed);
			ImGui::Checkbox(crypt_str("Rank reveal"), &g_cfg.misc.rank_reveal);
			ImGui::Checkbox(crypt_str("Taser range"), &g_cfg.esp.taser_range);
			ImGui::Checkbox(crypt_str("Aspect ratio"), &g_cfg.misc.aspect_ratio);

			if (g_cfg.misc.aspect_ratio)
			{
				ImGui::Spacing();
				ImGui::SliderFloat(crypt_str("Value"), &g_cfg.misc.aspect_ratio_amount, 1.0f, 2.0f);
				ImGui::Spacing();
			}

			ImGui::Checkbox(crypt_str("OOF"), &g_cfg.player.arrows);

			if (g_cfg.player.arrows)
			{
				ImGui::SameLine();
				ImGui::ColorEdit(crypt_str("##outfovcolor"), &g_cfg.esp.offcolors);


			}

			ImGui::Checkbox(crypt_str("Local trails"), &g_cfg.esp.trails);
			if (g_cfg.esp.trails)
			{

				ImGui::Checkbox("Rainbow trail", &g_cfg.esp.rainbow_trails);
				ImGui::ColorEdit(crypt_str("##Localtrailscolor"), &g_cfg.esp.trails_color);

			}
			ImGui::SameLine();
			ImGui::ColorEdit(crypt_str("##Localtrailscolor"), &g_cfg.esp.trails_color);
			ImGui::Spacing();
			ImGui::Checkbox("Scope", &g_cfg.misc.newscope);
			if (g_cfg.misc.newscope)
			{
				ImGui::SameLine();// do 0
				ImGui::ColorEdit(crypt_str("##scopecolor"), &g_cfg.esp.scopecolor);
			}


			

			ImGui::Checkbox(crypt_str("Clantag"), &g_cfg.misc.clantag_spammer);
			ImGui::Checkbox(crypt_str("Buybot"), &g_cfg.misc.buybot_enable);

			if (g_cfg.misc.buybot_enable)
			{
				pDraw::singleselect(crypt_str("Primary"), &g_cfg.misc.buybot1, mainwep, ARRAYSIZE(mainwep));
				pDraw::singleselect(crypt_str("Secondary"), &g_cfg.misc.buybot2, secwep, ARRAYSIZE(secwep));
				pDraw::multiselect2(crypt_str("Grenades"), g_cfg.misc.buybot3, grenades, ARRAYSIZE(grenades), grenades);
			}

			ImGui::Spacing();


			ImGui::Checkbox(crypt_str("Penetration crosshair"), &g_cfg.esp.penetration_reticle);
			ImGui::Checkbox(crypt_str("Force crosshair"), &g_cfg.misc.forcecrosshair);
			ImGui::Checkbox(crypt_str("Full bright"), &g_cfg.esp.bright);
			//ImGui::Checkbox(crypt_str("Rain"), &g_cfg.esp.rain);
			ImGui::Checkbox(crypt_str("Kill effect"), &g_cfg.esp.kill_effect);





			//ImGui::Checkbox(crypt_str("Spectators list"), &g_cfg.misc.spectators_list);
			//pDraw::singleselect(crypt_str("Hitsound"), g_cfg.esp.hitsound, sounds, ARRAYSIZE(sounds));

		}
		ImGui::EndChild();

		ImGui::SameLine();

		pDraw::addchild("misc2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			const char* strafes[] =
			{
				"None",
				"Legit",
				"Directional",
				"Rage"
			};

			const char* sounds[] =
			{
				"None",
				"Mettalic",
				"Cod",
				"Bubble",
				//"Stapler",
				"Bell",
				"Flick",
				"Hit",
				"Laser",
				"Click"
			};

			//static bool gay = false;
			ImGui::Checkbox(crypt_str("Bunny Hop"), &g_cfg.misc.bunnyhop);
			/*ImGui::Checkbox("Auto Strafe", &gay);
			if (gay)  g_cfg.misc.airstrafe = 2;
			else g_cfg.misc.airstrafe = 0;*/
			pDraw::singleselect(crypt_str("Auto Strafe"), &g_cfg.misc.airstrafe, strafes, ARRAYSIZE(strafes));
			ImGui::Spacing();
			pDraw::singleselect(crypt_str("Hitsound"), &g_cfg.esp.hitsound, sounds, ARRAYSIZE(sounds));

			ImGui::Checkbox(crypt_str("Velocity graph"), &g_cfg.esp.velocity_graph);
			//ImGui::Checkbox(crypt_str("Crouch in air"), &g_cfg.misc.crouch_in_air);
			ImGui::Checkbox(crypt_str("Fast stop"), &g_cfg.misc.fast_stop);
			//ImGui::Checkbox(crypt_str("OBS Spoofer"), &g_cfg.misc.anti_obs);
			//ImGui::Checkbox(crypt_str("Slide walk"), &g_cfg.misc.slidewalk);
			ImGui::Checkbox(crypt_str("Fast Duck"), &g_cfg.misc.noduck);

			ImGui::Spacing();

			ImGui::Checkbox(crypt_str("World Color"), &g_cfg.esp.nightmode);

			if (g_cfg.esp.nightmode)
			{
				ImGui::Text(crypt_str("Wolrd Color "));
				ImGui::SameLine();
				ImGui::ColorEdit(crypt_str("##nightmode"), &g_cfg.esp.world_color);
				ImGui::Spacing();
				ImGui::SliderInt(crypt_str("World Transparency"), &g_cfg.esp.world_transparency, 0, 100);
				ImGui::Spacing();
				ImGui::SliderInt(crypt_str("Prop Transparency"), &g_cfg.esp.prop_transparency, 0, 100);
			}
		}
		ImGui::EndChild();
	}
	else
	{
		pDraw::addchild("conf", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			//ImGui::PushItemWidth(220);

			static auto should_update = true;

			if (should_update)
			{
				should_update = false;

				cfg_manager->config_files();
				files = cfg_manager->files;

				for (auto& current : files)
					if (current.size() > 2)
						current.erase(current.size() - 3, 3);
			}

			if (pDraw::button(crypt_str("Open folder"), ImVec2(90, 30)))
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
				ShellExecute(NULL, crypt_str("open"), folder.c_str(), NULL, NULL, SW_SHOWNORMAL);
			}

			//ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			ImGui::ListBoxConfigArray(crypt_str("##CONFIGS"), &g_cfg.selected_config, files, 7);
			//ImGui::PopStyleVar();
			//SOLTA SA PORRA DE MOUSE KACETE
			if (pDraw::button(crypt_str("Refresh")))
			{
				cfg_manager->config_files();
				files = cfg_manager->files;

				for (auto& current : files)
					if (current.size() > 2)
						current.erase(current.size() - 3, 3);

				return;
			}

			static char config_name[64] = "\0";

			//ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			ImGui::InputText(crypt_str("##configname"), config_name, sizeof(config_name));
			//ImGui::PopStyleVar();

			if (pDraw::button(crypt_str("Create")))
			{
				g_cfg.new_config_name = config_name;
				add_config();
				return;
			}
			//legit ou skins agr?
			ImGui::SameLine();

			static auto next_delete = false;
			static auto prenext_delete = false;
			static auto clicked_sure_del = false;
			static auto delete_time = m_globals()->m_realtime;
			static auto delete_alpha = 1.0f;

			delete_alpha = math::clamp(delete_alpha + (4.f * ImGui::GetIO().DeltaTime * (!prenext_delete ? 1.f : -1.f)), 0.01f, 1.f);


			if (pDraw::button(crypt_str("Remove")))
			{
				remove_config();
			}



			static auto next_save = false;
			static auto prenext_save = false;
			static auto clicked_sure = false;
			static auto save_time = m_globals()->m_realtime;
			static auto save_alpha = 1.0f;

			save_alpha = math::clamp(save_alpha + (4.f * ImGui::GetIO().DeltaTime * (!prenext_save ? 1.f : -1.f)), 0.01f, 1.f);
			//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, save_alpha * public_alpha * (1.f - preview_alpha));

			if (pDraw::button(crypt_str("Save")))
			{
				g_cfg.misc.clicked_save = true;
				save_config();
			}

			ImGui::SameLine();

			//ImGui::PopStyleVar();

			if (pDraw::button(crypt_str("Load")))
			{
				load_config();
				g_cfg.misc.clicked_load = true;
				return;
			}



			//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, delete_alpha * public_alpha * (1.f - preview_alpha));



			//ImGui::PopStyleVar();
			//ImGui::PopItemWidth();
		}
		ImGui::EndChild();

		ImGui::SameLine(); // vou pegar a omida ja vorto





		pDraw::addchild("conf2", ImVec2((size.x - 25) / 2, size.y - 85), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			ImGui::Checkbox(crypt_str("Anti-untrusted"), &g_cfg.misc.anti_untrusted);
			ImGui::Checkbox(crypt_str("Watermark"), &g_cfg.menu.watermark);
			ImGui::Checkbox(crypt_str("ATOMIC STYLE (n ative)"), &g_cfg.misc.extended_backtracking);
			ImGui::Checkbox(crypt_str("Keybinds"), &g_cfg.misc.keybinds);
			ImGui::Text("Menu Color"); ImGui::SameLine();
			ImGui::ColorEdit("##Menu_Color", &g_cfg.menu.menu_theme);
		}
		ImGui::EndChild();
	}
}

IDirect3DTexture9* get_skin_preview(const char* weapon_name, const std::string& skin_name, IDirect3DDevice9* device)
{
	IDirect3DTexture9* skin_image = nullptr;
	std::string vpk_path;

	if (strcmp(weapon_name, crypt_str("unknown")) && strcmp(weapon_name, crypt_str("knife")) && strcmp(weapon_name, crypt_str("gloves"))) //-V526
	{
		if (skin_name.empty() || skin_name == crypt_str("default"))
			vpk_path = crypt_str("resource/flash/econ/weapons/base_weapons/") + std::string(weapon_name) + crypt_str(".png");
		else
			vpk_path = crypt_str("resource/flash/econ/default_generated/") + std::string(weapon_name) + crypt_str("_") + std::string(skin_name) + crypt_str("_light_large.png");
	}
	else
	{
		if (!strcmp(weapon_name, crypt_str("knife")))
			vpk_path = crypt_str("resource/flash/econ/weapons/base_weapons/weapon_knife.png");
		else if (!strcmp(weapon_name, crypt_str("gloves")))
			vpk_path = crypt_str("resource/flash/econ/weapons/base_weapons/ct_gloves.png");
		else if (!strcmp(weapon_name, crypt_str("unknown")))
			vpk_path = crypt_str("resource/flash/econ/weapons/base_weapons/weapon_snowball.png");

	}
	const auto handle = m_basefilesys()->Open(vpk_path.c_str(), crypt_str("r"), crypt_str("GAME"));
	if (handle)
	{
		int file_len = m_basefilesys()->Size(handle);
		char* image = new char[file_len]; //-V121

		m_basefilesys()->Read(image, file_len, handle);
		m_basefilesys()->Close(handle);

		D3DXCreateTextureFromFileInMemory(device, image, file_len, &skin_image);
		delete[] image;
	}

	if (!skin_image)
	{
		std::string vpk_path;

		if (strstr(weapon_name, crypt_str("bloodhound")) != NULL || strstr(weapon_name, crypt_str("hydra")) != NULL)
			vpk_path = crypt_str("resource/flash/econ/weapons/base_weapons/ct_gloves.png");
		else
			vpk_path = crypt_str("resource/flash/econ/weapons/base_weapons/") + std::string(weapon_name) + crypt_str(".png");

		const auto handle = m_basefilesys()->Open(vpk_path.c_str(), crypt_str("r"), crypt_str("GAME"));

		if (handle)
		{
			int file_len = m_basefilesys()->Size(handle);
			char* image = new char[file_len]; //-V121

			m_basefilesys()->Read(image, file_len, handle);
			m_basefilesys()->Close(handle);

			D3DXCreateTextureFromFileInMemory(device, image, file_len, &skin_image);
			delete[] image;
		}
	}

	return skin_image;
}

static const char* zweaponnames(const short id)
{
	switch (id)
	{
	case WEAPON_DEAGLE:
		return "deagle";
	case WEAPON_ELITE:
		return "elite";
	case WEAPON_FIVESEVEN:
		return "fiveseven";
	case WEAPON_GLOCK:
		return "glock";
	case WEAPON_AK47:
		return "ak47";
	case WEAPON_AUG:
		return "aug";
	case WEAPON_AWP:
		return "awp";
	case WEAPON_FAMAS:
		return "famas";
	case WEAPON_G3SG1:
		return "g3sg1";
	case WEAPON_GALILAR:
		return "galilar";
	case WEAPON_M249:
		return "m249";
	case WEAPON_M4A1_SILENCER:
		return "m4a1_silencer";
	case WEAPON_M4A1:
		return "m4a1";
	case WEAPON_MAC10:
		return "mac10";
	case WEAPON_P90:
		return "p90";
	case WEAPON_UMP45:
		return "ump45";
	case WEAPON_XM1014:
		return "xm1014";
	case WEAPON_BIZON:
		return "bizon";
	case WEAPON_MAG7:
		return "mag7";
	case WEAPON_NEGEV:
		return "negev";
	case WEAPON_SAWEDOFF:
		return "sawedoff";
	case WEAPON_TEC9:
		return "tec9";
	case WEAPON_HKP2000:
		return "hkp2000";
	case WEAPON_MP5SD:
		return "mp5sd";
	case WEAPON_MP7:
		return "mp7";
	case WEAPON_MP9:
		return "mp9";
	case WEAPON_NOVA:
		return "nova";
	case WEAPON_P250:
		return "p250";
	case WEAPON_SCAR20:
		return "scar20";
	case WEAPON_SG553:
		return "sg556";
	case WEAPON_SSG08:
		return "ssg08";
	case WEAPON_USP_SILENCER:
		return "usp_silencer";
	case WEAPON_CZ75A:
		return "cz75a";
	case WEAPON_REVOLVER:
		return "revolver";
	case WEAPON_KNIFE:
		return "knife";
	case WEAPON_KNIFE_T:
		return "knife_t";
	case WEAPON_KNIFE_M9_BAYONET:
		return "knife_m9_bayonet";
	case WEAPON_KNIFE_BAYONET:
		return "bayonet";
	case WEAPON_KNIFE_FLIP:
		return "knife_flip";
	case WEAPON_KNIFE_GUT:
		return "knife_gut";
	case WEAPON_KNIFE_KARAMBIT:
		return "knife_karambit";
	case WEAPON_KNIFE_TACTICAL:
		return "knife_tactical";
	case WEAPON_KNIFE_FALCHION:
		return "knife_falchion";
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		return "knife_survival_bowie";
	case WEAPON_KNIFE_BUTTERFLY:
		return "knife_butterfly";
	case WEAPON_KNIFE_PUSH:
		return "knife_push";
	case WEAPON_KNIFE_URSUS:
		return "knife_ursus";
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		return "knife_gypsy_jackknife";
	case WEAPON_KNIFE_STILETTO:
		return "knife_stiletto";
	case WEAPON_KNIFE_WIDOWMAKER:
		return "knife_widowmaker";
	case WEAPON_KNIFE_SKELETON:
		return "knife_skeleton";
	case WEAPON_KNIFE_OUTDOOR:
		return "knife_outdoor";
	case WEAPON_KNIFE_CANIS:
		return "knife_canis";
	case WEAPON_KNIFE_CORD:
		return "knife_cord";
	case WEAPON_KNIFE_CSS:
		return "knife_css";
	case GLOVE_STUDDED_BLOODHOUND:
		return "studded_bloodhound_gloves";
	case GLOVE_T_SIDE:
		return "t_gloves";
	case GLOVE_CT_SIDE:
		return "ct_gloves";
	case GLOVE_SPORTY:
		return "sporty_gloves";
	case GLOVE_SLICK:
		return "slick_gloves";
	case GLOVE_LEATHER_WRAP:
		return "leather_handwraps";
	case GLOVE_MOTORCYCLE:
		return "motorcycle_gloves";
	case GLOVE_SPECIALIST:
		return "specialist_gloves";
	case GLOVE_HYDRA:
		return "studded_hydra_gloves";
	default:
		return "";
	}
}

std::string FindURl(std::string paintName)
{
	std::ifstream inFile;
	inFile.open(".\\csgo\\scripts\\items\\items_game_cdn.txt");
	std::string line;
	//int founds = 0;
	unsigned int curLine = 0;
	std::string search = paintName.append("=");
	//string weapon = "aug";
	while (getline(inFile, line)) {
		curLine++;
		if (line.find(search, 0) != std::string::npos)
		{
			return line.substr(line.find(search)).erase(0, search.length());
		}

	}
	return "";
}
static IDirect3DTexture9* some_texture_test;
void DownloadBytes(const char* const szUrl)
{
	char sysdir[MAX_PATH] = { 0 };
	char Path[MAX_PATH] = { 0 };
	GetWindowsDirectory(sysdir, MAX_PATH);
	sprintf(Path, "C:\\test\\image.png", sysdir);
	URLDownloadToFile(NULL, szUrl, Path, 0, NULL);
}


std::string fDownloadBytes(const char* const szUrl)
{
	HINTERNET hOpen = NULL;
	HINTERNET hFile = NULL;
	char* lpBuffer = NULL;
	DWORD dwBytesRead = 0;
	//Pointer to dynamic buffer.
	char* data = 0;
	//Dynamic data size.
	DWORD dataSize = 0;

	hOpen = InternetOpenA("smef.pw INDIGO/4.3 paste", NULL, NULL, NULL, NULL);
	if (!hOpen) return (char*)"";

	hFile = InternetOpenUrlA(hOpen, szUrl, NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);

	if (!hFile) {
		InternetCloseHandle(hOpen);
		return (char*)"";
	}

	std::string output;
	do {
		char buffer[2000];
		InternetReadFile(hFile, (LPVOID)buffer, _countof(buffer), &dwBytesRead);
		output.append(buffer, dwBytesRead);
	} while (dwBytesRead);

	InternetCloseHandle(hFile);
	InternetCloseHandle(hOpen);

	return output;
}


static std::string old_link;
void UpdatePic(IDirect3DDevice9* thisptr, std::string link)
{
	if (old_link == link)
		return;
	std::string imData = fDownloadBytes(link.c_str());
	D3DXCreateTextureFromFileInMemoryEx(thisptr
		, imData.data(), imData.length(),
		300, 300, D3DUSAGE_DYNAMIC, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &some_texture_test);

	old_link = link;
}

void c_menu::inventory_tab()
{
	auto size = ImGui::GetWindowSize();

	static int index = 0;
	static wskin weaponSkin;

	pDraw::addchild("skins", ImVec2((size.x / 2 - 15), size.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	{
		ImGui::Checkbox("Enable", &g_cfg.inventory.activate_inventory);
		//ImGui::Checkbox("Enable autorarity", &g_cfg.inventory.autorarity);

		if (weaponSkin.wId == WEAPON_NONE)
			weaponSkin.wId = WEAPON_DEAGLE;

		if (pGui::begincombo("Weapon", k_inventory_names.at(weaponSkin.wId), ImGuiComboFlags_HeightLargest))
		{
			for (const auto& weapon : k_inventory_names)
			{
				if (ImGui::Selectable(weapon.second, weaponSkin.wId == weapon.first, 0, ImVec2()))
				{
					weaponSkin.wId = weapon.first;
					weaponSkin.paintKit = 0;
					//skinImage = nullptr;
				}
			}
			ImGui::EndCombo();
		}



		auto weaponName = zweaponnames(weaponSkin.wId);

		if (pGui::begincombo("##Paint Kit", weaponSkin.paintKit > 0 ? _inv.inventory.skinInfo[weaponSkin.paintKit].name.c_str() : "", ImGuiComboFlags_HeightLargest))
		{
			//adoro
			int lastID = ImGui::GetItemID();

			for (auto skin : _inv.inventory.skinInfo)
			{
				for (auto names : skin.second.weaponName)
				{
					if (weaponName != names)
						continue;

					ImGui::PushID(lastID++);//sim como soube? fui pra febem pq cortei
					//o pulso de um mlki
					if (ImGui::Selectable(skin.second.name.c_str(), skin.first == weaponSkin.paintKit, 0, ImVec2()))
						weaponSkin.paintKit = skin.first;

					ImGui::PopID();
				}
			}
			ImGui::EndCombo();
		}

		//kkkk
		pDraw::addsliderF("Wear", &weaponSkin.wear, 0.f, 1.f);
		pDraw::addslider("Seed", &weaponSkin.seed, 0, 100);
		//deixa o meu gabrielzinho em paz
		//if (!g_cfg.inventory.autorarity)
			//ui::SingleSelectSkeet("Rarity", &weaponSkin.quality, { "common","uncommon","rare","mythical","legendary","ancient","immortal" });

		//verde é a fiona e o shurek
		static char skinname[64] = "\0";

		//sim pq to com preguiça e isso mesmo
	//	ImGui::InputText(("##skinname"), skinname, sizeof(skinname));
	//    ImGui::InputInt("Stattrak", &weaponSkin.stattrak);

		static const char* preview = "Skins List";

		if(pGui::begincombo("List", preview, 0))
		{
			int i = 0;
			for (auto weapon : g_InventorySkins) {

				std::string weap = zweaponnames(weapon.second.wId);

				std::stringstream whatever;
				whatever << ++i;
				if (ImGui::Selectable(std::string(weap + " - " + (weapon.second.paintKit > 0 ? _inv.inventory.skinInfo[weapon.second.paintKit].name.c_str() : "")).c_str(), index == weapon.first)) {
					index = weapon.first;
					if (index == weapon.first)
					{
						preview = _inv.inventory.skinInfo[weapon.second.paintKit].name.c_str();
					}
				}
			}
			i = 0;
			ImGui::EndCombo();
		}

		if (g_InventorySkins.size() > 0)
		{
			if (pDraw::button("Delete"))
			{
				// if (g_InventorySkins[index] != NULL)
				g_InventorySkins.erase(index);
				_inv.inventory.itemCount = g_InventorySkins.size();
			}
		}

		//é obvio q n vai ser tão facil eu acho q n vai funfar pq n ta sendo chamado em porra nenhuma maIs ok
		if (pDraw::button("Add"))
		{
			//weaponSkin.sicker[0] = stickerkit[0];
			//weaponSkin.sicker[1] = stickerkit[1];
			//weaponSkin.sicker[2] = stickerkit[2];
			//weaponSkin.sicker[3] = stickerkit[3];

			std::string str(skinname);
			if (str.length() > 0)
				weaponSkin.name = str;
			g_InventorySkins.insert({ g_csgo.fRandomInt(20000, 200000), weaponSkin });
			_inv.inventory.itemCount = g_InventorySkins.size();
			//stickerkit[0] = 0;
			//stickerkit[1] = 0;
			//stickerkit[2] = 0;
			//stickerkit[3] = 0;
			//index = 0;
		}
		ImGui::SameLine();
		if (pDraw::button("Update"))
		{
			m_engine()->ExecuteClientCmd("econ_clear_inventory_images");
			write.SendClientHello();
			write.SendMatchmakingClient2GCHello();
		}

		if (weaponSkin.wId > 0 && weaponSkin.wId <= 100)
		{
			std::string name(zweaponnames(weaponSkin.wId));
			std::string smallfix = "weapon_" + name + "_" + _inv.inventory.skinInfo[weaponSkin.paintKit].cdnName;
			auto url = FindURl(smallfix);
			{
				UpdatePic(c_menu::get().device, url);
				if (some_texture_test)
				{
					//ui::SetCursorPos(ImVec2(ui::GetCursorPos().x + 5, ui::GetCursorPos().y - 20));
					ImGui::Image(some_texture_test, ImVec2(250, 200));
				}

			}
		}
		else
		{
			std::string name(zweaponnames(weaponSkin.wId));
			std::string smallfix = name + "_" + _inv.inventory.skinInfo[weaponSkin.paintKit].cdnName;
			auto url = FindURl(smallfix);
			{
				UpdatePic(c_menu::get().device, url);
				if (some_texture_test) {
					//ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 250);
					//ui::SetCursorPos(ImVec2(ui::GetCursorPos().x + 5, ui::GetCursorPos().y - 20));
					ImGui::Image(some_texture_test, ImVec2(250, 200));
				}
				
			}
		}

		/*if (g_InventorySkins.size() > 0)
		{//tomara
			if (ui::ButtonSkeet("Delete selected element", ImVec2(ui::GetWindowSize().x - 39, 26)))
			{
				// if (g_InventorySkins[index] != NULL)
				g_InventorySkins.erase(index);
				_inv.inventory.itemCount = g_InventorySkins.size();
			}
		}*/


	}
	ImGui::EndChild();

	ImGui::SameLine();

	pDraw::addchild("medals", ImVec2((size.x / 2 - 15), size.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	{
		static int tab = 0;
		const char* tt[] = { "Medals", "Profile" };
		pDraw::singleselect("##nothing", &tab, tt, IM_ARRAYSIZE(tt));
		if (tab == 0)
		{
			static int paintKit = 874;

			if (pGui::begincombo("Medal", fosso[paintKit].name.c_str(), ImGuiComboFlags_HeightLargest))
			{
				int lastID = ImGui::GetItemID();

				for (auto skin : fosso)
				{
					{
						ImGui::PushID(lastID++);
						if (ImGui::Selectable(skin.second.name.c_str(), skin.second.paintKit == paintKit, 0, ImVec2()))
							paintKit = skin.second.paintKit;
						ImGui::PopID();
					}
				}
				ImGui::EndCombo();
			}
			if (pDraw::button("Add"))
				g_MedalSkins[g_csgo.fRandomInt(200001, 1000000)] = { paintKit , 0 };

			ImGui::SameLine();

			if (pDraw::button("Update"))
			{
				paintKit;
				m_engine()->ExecuteClientCmd("econ_clear_inventory_images");
				write.SendClientHello();
				write.SendMatchmakingClient2GCHello();
			}
		}
		else
		{
			const char* Rank[] =
			{
				"Off",
		"Silver 1",
		"Silver 2",
		"Silver 3",
		"Silver 4",
		"Silver elite",
		"Silver elite master",
		"Gold nova 1",
		"Gold nova 2",
		"Gold nova 3",
		"Gold nova master",
		"Master guardian 1",
		"Master guardian 2",
		"Master guardian elite",
		"Distinguished master guardian",
		"Legendary eagle",
		"Legendary eagle master",
		"Supreme master first class",
		"The global elite"
			};

			const char* Lab[] =
			{
				"Off",
				"Lab rat 1",
				"Lab rat 2",
				"Sprinting Hare 1",
				"Sprinting Hare 2",
				"Wild Scout 1",
				"Wild Scout Elite",
				"Hunter Fox 1",
				"Hunter Fox 2",
				"Hunter Fox 3",
				"Hunter Fox Elite",
				"Timber Wolf",
				"Ember Wolf",
				"Wildfire Wolf",
				"The Howling Alpha"
			};

			//ImGui::Checkbox("Enable profile changer", &g_cfg.profilechanger.enable);

			ImGui::InputInt("Friend", &g_cfg.profilechanger.profile_friend);
			ImGui::InputInt("Leader", &g_cfg.profilechanger.profile_leader);
			ImGui::InputInt("Teach", &g_cfg.profilechanger.profile_teach);


			pDraw::singleselect("Rank", &g_cfg.profilechanger.profile_rank, Rank, ARRAYSIZE(Rank));
			ImGui::InputInt("Wins", &g_cfg.profilechanger.profile_rank_wins);
			pDraw::addslider("Level", &g_cfg.profilechanger.profile_lvl, 0, 40);
			pDraw::addslider("XP", &g_cfg.profilechanger.profile_xp, 0, 5000);

			pDraw::singleselect("Rank Wing", &g_cfg.profilechanger.r_rank, Rank, ARRAYSIZE(Rank));

			ImGui::InputInt("Wins Wing", &g_cfg.profilechanger.r_wins);

			pDraw::singleselect("Rank zone", &g_cfg.profilechanger.t_rank, Lab, ARRAYSIZE(Lab));

			ImGui::InputInt("Wins zone", &g_cfg.profilechanger.t_wins);

			if (pDraw::button("Update"))
			{
				write.SendClientHello();
				write.SendMatchmakingClient2GCHello();
			}

			const char* player_model_t[] =
			{
				"None",
				"Enforcer",
				"Soldier",
				"Ground Rebel",
				"Maximus",
				"Osiris",
				"Slingshot",
				"Dragomir",
				"Blackwolf",
				"Prof. Shahmat",
				"Rezan The Ready",
				"Doctor Romanov",
				"Mr. Muhlik",
				"Miami Darryl",
				"Silent Darryl",
				"Skullhead Darryl",
				"Darryl Royale",
				"Loudmouth Darryl",
				"Safecracker Voltzmann",
				"Little Kev",
				"Number K   (condom)",
				"Getaway Sally"
			};



			const char* player_model_ct[] =
			{
				"None",
				"Seal Team 6",
				"3rd Commando",
				"Operator FBI",
				"Squadron Officer",
				"Markus Delrow",
				"Buckshot",
				"McCoy",
				"Commander Ricksaw",
				"Agent Ava",
				"1st Lieutenant Farlow",
				"John 'Van Healen' Kask",
				"Bio-Haz Specialist",
				"Sergeant Bombson",
				"Chem-Haz Specialist",
				"Cmdr. Mae Jamison"
			};


			pDraw::singleselect(crypt_str("Model T"), &g_cfg.player.player_model_t, player_model_t, ARRAYSIZE(player_model_t));
			pDraw::singleselect(crypt_str("Model CT"), &g_cfg.player.player_model_ct, player_model_ct, ARRAYSIZE(player_model_ct));
		}

		//if (ImGui::Button("Apply", ImVec2(ImGui::GetWindowSize().x - 39, 26)))
			//write.SendClientGcRankUpdate2();
	}
	ImGui::EndChild();
}


void c_menu::skins()
{
	//foda negão
	//tabom senpai
	ImVec2 size = ImGui::GetWindowSize();
	// hey stewen, what r u doing there? he, hm heee, DRUGS
	static bool drugs = false;

	// some animation logic(switch)
	static bool active_animation = false;
	static bool preview_reverse = false;
	static float switch_alpha = 1.f;
	static int next_id = -1;

	if (active_animation)
	{
		if (preview_reverse)
		{
			if (switch_alpha == 1.f) //-V550
			{
				preview_reverse = false;
				active_animation = false;
			}

			switch_alpha = math::clamp(switch_alpha + (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
		}
		else
		{
			if (switch_alpha == 0.01f) //-V550
			{
				preview_reverse = true;
			}

			switch_alpha = math::clamp(switch_alpha - (4.f * ImGui::GetIO().DeltaTime), 0.01f, 1.f);
		}
	}
	else
		switch_alpha = math::clamp(switch_alpha + (4.f * ImGui::GetIO().DeltaTime), 0.0f, 1.f);

	pDraw::addchild("skins", ImVec2((size.x - 15), size.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	{
		//sou um macaquinho
		//negro e pobre
		//vc tira minha concentração
		//mano pd deixar o default e dps mod?

		auto same_line_counter = 0;

		//next_id = 0;

		for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
		{
			// do we need update our preview for some reasons?
			if (!all_skins[i])
			{
				g_cfg.skins.skinChanger.at(i).update();
				all_skins[i] = get_skin_preview(get_wep(i, (i == 0 || i == 1) ? g_cfg.skins.skinChanger.at(i).definition_override_vector_index : -1, i == 0).c_str(), g_cfg.skins.skinChanger.at(i).skin_name, device); //-V810
			}
			//young touch
		}

		/*ImGui::ListBoxHeader(("pau"), ImVec2(0, 277));
		{
			for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
			{
				if (ImGui::Selectable(get_wep(i, (i == 0 || i == 1) ? g_cfg.skins.skinChanger.at(i).definition_override_vector_index : -1, i == 0).c_str(), g_cfg.skins.skinChanger.at(i).definition_override_vector_index == i))
				{
					g_cfg.skins.skinChanger.at(i).definition_override_vector_index = i;
				}
			}

		}*/
		//pq tu fechou o menu do lw seu lixo pobre nojento seboso
		//seu cu seco na minha boca

		static bool upd = false;
		static int g = -50;
		static int id_new = -1;

		if (g == -50)
			g = next_id;
		else
		{
			upd = false;

			if (g != next_id)
			{
				upd = true;
				g = next_id;
			}

		}
		//vo ter q sair agr tmnc ir la na buceta do mercado e feira si fude
		//flw
		if (current_profile == -1)
		{
			if (pGui::begincombo("Weapon selector", "", 0))
			{
				for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
				{
					if (ImGui::Selectable(wep_c(i, (i == 0 || i == 1) ? g_cfg.skins.skinChanger.at(i).definition_override_vector_index : -1, i == 0).c_str(), next_id == i))
					{
						next_id = i;
						id_new = i;
						active_animation = true;
					}
				}
				ImGui::EndCombo();
			}
			//
			if (active_animation && preview_reverse)
			{
				ImGui::SetScrollY(0);
				current_profile = next_id;
			}

			if (same_line_counter < 4) { // continue push same-line
				ImGui::SameLine();
				same_line_counter++;//abre o notepad
			}
			else { // we have maximum elements in 1 line
				same_line_counter = 0;
			}
		}
		else//crasho?
		{
			if (pGui::begincombo("Weapon selector", "", 0))
			{
				for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
				{
					if (ImGui::Selectable(wep_c(i, (i == 0 || i == 1) ? g_cfg.skins.skinChanger.at(i).definition_override_vector_index : -1, i == 0).c_str(), next_id == i))
					{
						next_id = i;
						id_new = i;
						active_animation = true;
					}
				}
				ImGui::EndCombo();
			}


			// update skin preview bool
			static bool need_update[36];

			// we pressed crypt_str("Save & Close") button
			static bool leave;

			// update if we have nullptr texture or if we push force update
			if (!all_skins[current_profile] || need_update[current_profile])
			{
				all_skins[current_profile] = get_skin_preview(get_wep(current_profile, (current_profile == 0 || current_profile == 1) ? g_cfg.skins.skinChanger.at(current_profile).definition_override_vector_index : -1, current_profile == 0).c_str(), g_cfg.skins.skinChanger.at(current_profile).skin_name, device); //-V810
				need_update[current_profile] = false;
			}

			// get settings for selected weapon
			auto& selected_entry = g_cfg.skins.skinChanger[current_profile];
			selected_entry.itemIdIndex = current_profile;

			ImGui::BeginGroup();
			ImGui::PushItemWidth(260 * dpi_scale);

			// search input later
			static char search_skins[64] = "\0";
			static auto item_index = selected_entry.paint_kit_vector_index;

			if (!current_profile)
			{
				ImGui::Text(crypt_str("Knife"));
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
				if (pGui::combo(crypt_str("##Knife_combo"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
				{
					*out_text = game_data::knife_names[idx].name;
					return true;
				}, nullptr, IM_ARRAYSIZE(game_data::knife_names)))
					need_update[current_profile] = true; // push force update
			}
			else if (current_profile == 1)
			{
				ImGui::Text(crypt_str("Gloves"));
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
				if (pGui::combo(crypt_str("##Glove_combo"), &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
				{
					*out_text = game_data::glove_names[idx].name;
					return true;
				}, nullptr, IM_ARRAYSIZE(game_data::glove_names)))
				{
					item_index = 0; // set new generated paintkits element to 0;
					need_update[current_profile] = true; // push force update
				}
			}
			else
				selected_entry.definition_override_vector_index = 0;

			if (current_profile != 1)
			{
				ImGui::Text(crypt_str("Search"));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

				if (ImGui::InputText(crypt_str("##search"), search_skins, sizeof(search_skins)))
					item_index = -1;

				ImGui::PopStyleVar();
			}

			auto main_kits = current_profile == 1 ? SkinChanger::gloveKits : SkinChanger::skinKits;
			auto display_index = 0;

			SkinChanger::displayKits = main_kits;

			// we dont need custom gloves
			if (current_profile == 1)
			{
				for (auto i = 0; i < main_kits.size(); i++)
				{
					auto main_name = main_kits.at(i).name;

					for (auto i = 0; i < main_name.size(); i++)
						if (iswalpha((main_name.at(i))))
							main_name.at(i) = towlower(main_name.at(i));

					char search_name[64];

					if (!strcmp(game_data::glove_names[selected_entry.definition_override_vector_index].name, crypt_str("Hydra")))
						strcpy_s(search_name, sizeof(search_name), crypt_str("Bloodhound"));
					else
						strcpy_s(search_name, sizeof(search_name), game_data::glove_names[selected_entry.definition_override_vector_index].name);

					for (auto i = 0; i < sizeof(search_name); i++)
						if (iswalpha(search_name[i]))
							search_name[i] = towlower(search_name[i]);

					if (main_name.find(search_name) != std::string::npos)
					{
						SkinChanger::displayKits.at(display_index) = main_kits.at(i);
						display_index++;
					}
				}

				SkinChanger::displayKits.erase(SkinChanger::displayKits.begin() + display_index, SkinChanger::displayKits.end());
			}
			else
			{
				if (strcmp(search_skins, crypt_str(""))) //-V526
				{
					for (auto i = 0; i < main_kits.size(); i++)
					{
						auto main_name = main_kits.at(i).name;

						for (auto i = 0; i < main_name.size(); i++)
							if (iswalpha(main_name.at(i)))
								main_name.at(i) = towlower(main_name.at(i));

						char search_name[64];
						strcpy_s(search_name, sizeof(search_name), search_skins);

						for (auto i = 0; i < sizeof(search_name); i++)
							if (iswalpha(search_name[i]))
								search_name[i] = towlower(search_name[i]);

						if (main_name.find(search_name) != std::string::npos)
						{
							SkinChanger::displayKits.at(display_index) = main_kits.at(i);
							display_index++;
						}
					}

					SkinChanger::displayKits.erase(SkinChanger::displayKits.begin() + display_index, SkinChanger::displayKits.end());
				}
				else
					item_index = selected_entry.paint_kit_vector_index;
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
			if (!SkinChanger::displayKits.empty())
			{
				if (ImGui::ListBox(crypt_str("##PAINTKITS"), &item_index, [](void* data, int idx, const char** out_text) //-V107
				{
					while (SkinChanger::displayKits.at(idx).name.find(crypt_str("С‘")) != std::string::npos) //-V807
						SkinChanger::displayKits.at(idx).name.replace(SkinChanger::displayKits.at(idx).name.find(crypt_str("С‘")), 2, crypt_str("Рµ"));

					*out_text = SkinChanger::displayKits.at(idx).name.c_str();
					return true;
				}, nullptr, SkinChanger::displayKits.size(), SkinChanger::displayKits.size() > 9 ? 9 : SkinChanger::displayKits.size()) || !all_skins[current_profile])
				{
					//SkinChanger::scheduleHudUpdate();
					need_update[current_profile] = true;

					auto i = 0;

					while (i < main_kits.size())
					{
						if (main_kits.at(i).id == SkinChanger::displayKits.at(item_index).id)
						{
							selected_entry.paint_kit_vector_index = i;
							break;
						}

						i++;
					}

				}
			}
			ImGui::PopStyleVar();

			//if (ImGui::InputInt(crypt_str("Seed"), &selected_entry.seed, 1, 100))
				//SkinChanger::scheduleHudUpdate();

			ImGui::InputInt(crypt_str("StatTrak"), &selected_entry.stat_trak, 1, 15);

			//if (ImGui::InputInt(crypt_str("StatTrak"), &selected_entry.stat_trak, 1, 15))
				//SkinChanger::scheduleHudUpdate();

			/*if (ImGui::SliderFloat(crypt_str("Wear"), &selected_entry.wear, 0.0f, 1.0f))
				drugs = true;
			else if (drugs)
			{
				SkinChanger::scheduleHudUpdate();
				drugs = false;
			}*/

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6 * c_menu::get().dpi_scale);
			//ImGui::Text(crypt_str("Quality"));
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5 * c_menu::get().dpi_scale);
			/*if (pGui::combo(crypt_str("Quality"), &selected_entry.entity_quality_vector_index, [](void* data, int idx, const char** out_text)
			{
				*out_text = game_data::quality_names[idx].name;
				return true;
			}, nullptr, IM_ARRAYSIZE(game_data::quality_names)))
				SkinChanger::scheduleHudUpdate();*/

			if (current_profile != 1)
			{
				if (!g_cfg.skins.custom_name_tag[current_profile].empty())
					strcpy_s(selected_entry.custom_name, sizeof(selected_entry.custom_name), g_cfg.skins.custom_name_tag[current_profile].c_str());

				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
				/*//ImGui::Text(crypt_str("Name Tag"));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);

				if (ImGui::InputText(crypt_str("##nametag"), selected_entry.custom_name, sizeof(selected_entry.custom_name)))
				{
					g_cfg.skins.custom_name_tag[current_profile] = selected_entry.custom_name;
					SkinChanger::scheduleHudUpdate();
				}*/

				ImGui::PopStyleVar();
			}

			ImGui::PopItemWidth();

			ImGui::EndGroup();

			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);

			ImGui::BeginGroup();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
			if (ImGui::ImageButton(all_skins[current_profile], ImVec2(190, 155)))
			{
				// maybe i will do smth later where, who knows :/
			}
			ImGui::PopStyleColor(3);

			/*if (ImGui::CustomButton(crypt_str("Close"), crypt_str("##CLOSE__SKING"), ImVec2(198 * dpi_scale, 26 * dpi_scale)))
			{
				// start animation
				active_animation = true;
				next_id = -1;
				leave = true;
			}*/

			if (upd)
			{
				/*active_animation = true;
				next_id = -1;
				leave = true;*/
				next_id = -1;
				next_id = id_new;
				leave = true;
				upd = false;
			}

			if (pDraw::button("Update"))
			{
				SkinChanger::scheduleHudUpdate();
			}

			ImGui::EndGroup();

			// update element
			selected_entry.update();

			// we need to reset profile in the end to prevent render images with massive's index == -1
			if (leave && (preview_reverse || !active_animation))
			{
				ImGui::SetScrollY(0);
				current_profile = next_id;
				leave = false;
			}
		}
	}
	ImGui::EndChild();
}

void c_menu::render2(bool is_open)
{
	ImGui::GetIO().MouseDrawCursor = is_open;

	//draw_list = ImGui::GetWindowDrawList();

	if (kdopipi::gg)
	{
		kdopipi::g = false;
	}
	else if (kdopipi::g)
	{
		kdopipi::gg = false;
	}

	if (kdopipi::gg)
	{
		//kdopipi::gg = false;
		g_cfg.antiaim.enable = kdopipi::gg;
		g_cfg.antiaim.enable = true;
		g_cfg.antiaim.antiaim_type = 1;
	}
	else if (kdopipi::g)
	{
		//kdopipi::gg = false;
		g_cfg.antiaim.enable = kdopipi::g;
		g_cfg.antiaim.enable = true;
		g_cfg.antiaim.antiaim_type = 0;
	}
	else
		g_cfg.antiaim.enable = false;

	if (kdopipi::event_log)
	{
		g_cfg.misc.log_output[0] = true;
		g_cfg.misc.events_to_log[0] = true;
		g_cfg.misc.events_to_log[1] = true;
		g_cfg.misc.events_to_log[2] = true;
	}
	else
	{

		g_cfg.misc.log_output[0] = false;
		g_cfg.misc.events_to_log[0] = false;
		g_cfg.misc.events_to_log[1] = false;
		g_cfg.misc.events_to_log[2] = false;
	}

	if (!is_open)
		return;

	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 0.5f);
	ImGui::GetStyle().Colors[ImGuiCol_PopupBg] = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::GetStyle().Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.f, 0.f, 0.f, 0.f);
	ImGui::GetStyle().WindowRounding = 5.f;

	for (auto i = 0; i < g_cfg.skins.skinChanger.size(); i++)
		if (!all_skins[i])
			all_skins[i] = get_skin_preview(get_wep(i, (i == 0 || i == 1) ? g_cfg.skins.skinChanger.at(i).definition_override_vector_index : -1, i == 0).c_str(), g_cfg.skins.skinChanger.at(i).skin_name, device); //-V810


	ImGui::SetNextWindowSize(ImVec2(700, 500));
	ImGui::Begin("Cool", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
	{
		//draw_list = ImGui::GetWindowDrawList();
		ImVec2 size = ImGui::GetWindowSize();
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 1.f, 1.f, 0.f));
		pDraw::addchild("MAIN", ImVec2(size.x - 15, size.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		{
			static int ctab = 0;
			ImVec2 sizem = ImGui::GetWindowSize();
			pDraw::addchild("tabs", ImVec2(100, sizem.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
			{
				ImVec2 sizec = ImGui::GetWindowSize();
				if (pDraw::addtab("A", "", ctab == 0, ImVec2(sizec.x - 15, sizec.y / 5.7))) ctab = 0;
				if (pDraw::addtab("B", "", ctab == 1, ImVec2(sizec.x - 15, sizec.y / 5.7))) ctab = 1;
				if (pDraw::addtab("C", "", ctab == 2, ImVec2(sizec.x - 15, sizec.y / 5.7))) ctab = 2;
				if (pDraw::addtab("D", "", ctab == 3, ImVec2(sizec.x - 15, sizec.y / 5.7))) ctab = 3;
				if (pDraw::addtab("E", "", ctab == 4, ImVec2(sizec.x - 15, sizec.y / 5.7))) ctab = 4;
			}
			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::PushFont(menu);
			pDraw::addchild("contents", ImVec2(sizem.x - 125, sizem.y - 15), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
			{
				switch (ctab)
				{
				case 0: rage(); break;
				case 1: legit(); break;
				case 2: visuals(); break;
				case 3: misc(); break;
				case 4: inventory_tab(); break;
				}
			}
			ImGui::EndChild();
			ImGui::PopFont();//

		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
	ImGui::End();
}