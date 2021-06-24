// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "world_esp.h"
#include "other_esp.h"

void worldesp::paint_traverse()
{
	skybox_changer();

	for (int i = 1; i <= m_entitylist()->GetHighestEntityIndex(); i++)  //-V807
	{
		auto e = static_cast<entity_t*>(m_entitylist()->GetClientEntity(i));

		if (!e)
			continue;

		if (e->is_player())
			continue;

		if (e->IsDormant())
			continue;

		auto client_class = e->GetClientClass();

		if (!client_class)
			continue;

		otheresp::get().molotov_shit(e);

		switch (client_class->m_ClassID)
		{
		case CEnvTonemapController:
			world_modulation(e);
			break;
		case CInferno:
			molotov_timer(e);
			break;
		case CSmokeGrenadeProjectile:
			smoke_timer(e);
			break;
		case CPlantedC4:
			bomb_timer(e);
			break;
		case CC4:
			if (g_cfg.player.type[ENEMY].flags[FLAGS_C4] || g_cfg.player.type[TEAM].flags[FLAGS_C4] || g_cfg.player.type[LOCAL].flags[FLAGS_C4] || g_cfg.esp.bomb_timer)
			{
				auto owner = (player_t*)m_entitylist()->GetClientEntityFromHandle(e->m_hOwnerEntity());

				if ((g_cfg.player.type[ENEMY].flags[FLAGS_C4] || g_cfg.player.type[TEAM].flags[FLAGS_C4] || g_cfg.player.type[LOCAL].flags[FLAGS_C4]) && owner->valid(false, false))
					g_ctx.globals.bomb_carrier = owner->EntIndex();
				else if (g_cfg.esp.bomb_timer && !owner->is_player())
				{
					auto screen = ZERO;

					if (math::world_to_screen(e->GetAbsOrigin(), screen))
						render::get().text(fonts[ESP], screen.x, screen.y, Color(215, 20, 20), HFONT_CENTERED_X | HFONT_CENTERED_Y, "BOMB");
				}
			}

			break;
		default:
			grenade_projectiles(e);

			if (client_class->m_ClassID == CAK47 || client_class->m_ClassID == CDEagle || client_class->m_ClassID >= CWeaponAug && client_class->m_ClassID <= CWeaponZoneRepulsor) //-V648
				dropped_weapons(e);

			break;
		}
	}
}



void worldesp::RenderAsus()
{
	if (!m_engine()->IsInGame() && !m_engine()->IsConnected())
		return;
	static bool bPerformed = false, bLastSetting;
	static float bLastSettingWorldfl;
	static float bLastSettingsPropfl;


	if (bLastSettingWorldfl != g_cfg.esp.world_transparency || bLastSettingsPropfl != g_cfg.esp.prop_transparency)
	{
		bLastSettingWorldfl = g_cfg.esp.world_transparency;
		bLastSettingsPropfl = g_cfg.esp.prop_transparency;
		bPerformed = false;
	}

	if (!bPerformed)
	{
		ConVar* r_drawspecificstaticprop = m_cvar()->FindVar("r_drawspecificstaticprop");
		*(int*)((DWORD)&r_drawspecificstaticprop->m_fnChangeCallbacks + 0xC) = 0; // ew
		r_drawspecificstaticprop->SetValue(1);


		for (auto i = m_materialsystem()->FirstMaterial(); i != m_materialsystem()->InvalidMaterial(); i = m_materialsystem()->NextMaterial(i))
		{
			IMaterial* pMaterial = m_materialsystem()->GetMaterial(i);

			if (!pMaterial || pMaterial->IsErrorMaterial())
				continue;


			if (strstr(pMaterial->GetTextureGroupName(), "World"))
			{
				pMaterial->AlphaModulate(g_cfg.esp.world_transparency / 100.f);
			}

			if (strstr(pMaterial->GetTextureGroupName(), "StaticProp"))
			{
				pMaterial->AlphaModulate(g_cfg.esp.prop_transparency / 100.f);
			}
		} bPerformed = true;
	}
}

void worldesp::noeffects()
{
	static auto noeffects = m_cvar()->FindVar("r_drawparticles");
	noeffects->SetValue(!g_cfg.esp.removals[REMOVALS_EFFECTS]);
}

void worldesp::NoShadows()
{
	/*cl_csm_static_prop_shadows
		cl_csm_shadows
		cl_csm_world_shadows
		cl_foot_contact_shadows
		cl_csm_viewmodel_shadows
		cl_csm_rope_shadows
		cl_csm_sprite_shadows*/

	static auto cl_csm_static_prop_shadows = m_cvar()->FindVar("cl_csm_static_prop_shadows");
	*(int*)((DWORD)&cl_csm_static_prop_shadows->m_fnChangeCallbacks + 0xC) = 0;

	static auto cl_csm_shadows = m_cvar()->FindVar("cl_csm_shadows");
	*(int*)((DWORD)&cl_csm_shadows->m_fnChangeCallbacks + 0xC) = 0;

	static auto cl_csm_world_shadows = m_cvar()->FindVar("cl_csm_world_shadows");
	*(int*)((DWORD)&cl_csm_world_shadows->m_fnChangeCallbacks + 0xC) = 0;

	static auto cl_foot_contact_shadows = m_cvar()->FindVar("cl_foot_contact_shadows");
	*(int*)((DWORD)&cl_foot_contact_shadows->m_fnChangeCallbacks + 0xC) = 0;

	static auto cl_csm_viewmodel_shadows = m_cvar()->FindVar("cl_csm_viewmodel_shadows");
	*(int*)((DWORD)&cl_csm_viewmodel_shadows->m_fnChangeCallbacks + 0xC) = 0;

	static auto cl_csm_rope_shadows = m_cvar()->FindVar("cl_csm_rope_shadows");
	*(int*)((DWORD)&cl_csm_rope_shadows->m_fnChangeCallbacks + 0xC) = 0;

	static auto cl_csm_sprite_shadows = m_cvar()->FindVar("cl_csm_sprite_shadows");
	*(int*)((DWORD)&cl_csm_sprite_shadows->m_fnChangeCallbacks + 0xC) = 0;

	if (g_cfg.esp.removals[REMOVALS_SHADOWS])
	{
		cl_csm_static_prop_shadows->SetValue(0);
		cl_csm_shadows->SetValue(0);
		cl_csm_world_shadows->SetValue(0);
		cl_foot_contact_shadows->SetValue(0);
		cl_csm_viewmodel_shadows->SetValue(0);
		cl_csm_rope_shadows->SetValue(0);
		cl_csm_sprite_shadows->SetValue(0);
	}
	else
	{
		cl_csm_static_prop_shadows->SetValue(1);
		cl_csm_shadows->SetValue(1);
		cl_csm_world_shadows->SetValue(1);
		cl_foot_contact_shadows->SetValue(1);
		cl_csm_viewmodel_shadows->SetValue(1);
		cl_csm_rope_shadows->SetValue(1);
		cl_csm_sprite_shadows->SetValue(1);
	}

}


void worldesp::no3dcu()
{
	//static auto nod3 = m_cvar()->FindVar("r_3dsky");
	//*(int*)((DWORD)&nod3->m_fnChangeCallbacks + 0xC) = 0;
	//nod3->SetValue(!g_cfg.esp.removals[REMOVALS_SKY]);


}

void worldesp::skybox_changer()
{
	static auto load_skybox = reinterpret_cast<void(__fastcall*)(const char*)>(util::FindSignature(crypt_str("engine.dll"), crypt_str("55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45")));
	auto skybox_name = backup_skybox;

	switch (g_cfg.esp.skybox)
	{
	case 1:
		skybox_name = "cs_tibet";
		break;
	case 2:
		skybox_name = "cs_baggage_skybox_";
		break;
	case 3:
		skybox_name = "italy";
		break;
	case 4:
		skybox_name = "jungle";
		break;
	case 5:
		skybox_name = "office";
		break;
	case 6:
		skybox_name = "sky_cs15_daylight01_hdr";
		break;
	case 7:
		skybox_name = "sky_cs15_daylight02_hdr";
		break;
	case 8:
		skybox_name = "vertigoblue_hdr";
		break;
	case 9:
		skybox_name = "vertigo";
		break;
	case 10:
		skybox_name = "sky_day02_05_hdr";
		break;
	case 11:
		skybox_name = "nukeblank";
		break;
	case 12:
		skybox_name = "sky_venice";
		break;
	case 13:
		skybox_name = "sky_cs15_daylight03_hdr";
		break;
	case 14:
		skybox_name = "sky_cs15_daylight04_hdr";
		break;
	case 15:
		skybox_name = "sky_csgo_cloudy01";
		break;
	case 16:
		skybox_name = "sky_csgo_night02";
		break;
	case 17:
		skybox_name = "sky_csgo_night02b";
		break;
	case 18:
		skybox_name = "sky_csgo_night_flat";
		break;
	case 19:
		skybox_name = "sky_dust";
		break;
	case 20:
		skybox_name = "vietnam";
		break;
	case 21:
		skybox_name = "sky_lunacy";
		break;
	}

	static auto skybox_number = 0;
	static auto old_skybox_name = skybox_name;

	static auto color_r = (unsigned char)255;
	static auto color_g = (unsigned char)255;
	static auto color_b = (unsigned char)255;

	if (skybox_number != g_cfg.esp.skybox)
	{
		changed = true;
		skybox_number = g_cfg.esp.skybox;
	}
	else if (old_skybox_name != skybox_name)
	{
		changed = true;
		old_skybox_name = skybox_name;
	}
	else if (color_r != g_cfg.esp.skybox_color[0])
	{
		changed = true;
		color_r = g_cfg.esp.skybox_color[0];
	}
	else if (color_g != g_cfg.esp.skybox_color[1])
	{
		changed = true;
		color_g = g_cfg.esp.skybox_color[1];
	}
	else if (color_b != g_cfg.esp.skybox_color[2])
	{
		changed = true;
		color_b = g_cfg.esp.skybox_color[2];
	}

	if (changed)
	{
		changed = false;
		load_skybox(skybox_name.c_str());

		auto materialsystem = m_materialsystem();

		for (auto i = materialsystem->FirstMaterial(); i != materialsystem->InvalidMaterial(); i = materialsystem->NextMaterial(i))
		{
			auto material = materialsystem->GetMaterial(i);

			if (!material)
				continue;

			if (strstr(material->GetTextureGroupName(), crypt_str("SkyBox")))
				material->ColorModulate(g_cfg.esp.skybox_color[0] / 255.0f, g_cfg.esp.skybox_color[1] / 255.0f, g_cfg.esp.skybox_color[2] / 255.0f);
		}
	}
}



void worldesp::DrawBeam(Vector src, Vector end, Color color)
{
	BeamInfo_t beamInfo;
	beamInfo.m_nType = 0; //TE_BEAMPOINTS
	beamInfo.m_vecStart = src;
	beamInfo.m_vecEnd = end;
	beamInfo.m_pszModelName = "sprites/purplelaser1.vmt";
	beamInfo.m_pszHaloName = "sprites/purplelaser1.vmt";
	beamInfo.m_flHaloScale = 3.5; //3
	beamInfo.m_flWidth = 4.5f; //45
	beamInfo.m_flEndWidth = 4.5f; //45
	beamInfo.m_flFadeLength = 1.f;
	beamInfo.m_flAmplitude = 0;
	beamInfo.m_flBrightness = 255.f;
	beamInfo.m_flSpeed = 0.f;
	beamInfo.m_nStartFrame = 0.0;
	beamInfo.m_flFrameRate = 0.0;
	beamInfo.m_flRed = color.r();
	beamInfo.m_flGreen = color.g();
	beamInfo.m_flBlue = color.b();
	beamInfo.m_nSegments = 1;
	beamInfo.m_bRenderable = true;
	beamInfo.m_flLife = 1; //1
	beamInfo.m_nFlags = 16 | 256 | 65536;
	Beam_t* myBeam = m_viewrenderbeams()->CreateBeamPoints(beamInfo);
	if (myBeam)
		m_viewrenderbeams()->DrawBeam(myBeam);

}

void worldesp::PlayerTrails()
{
	if (g_ctx.local()->is_alive() && m_engine()->IsInGame() && m_engine()->IsConnected())
	{
		static float rainbow;
		rainbow += 0.001f;
		if (rainbow > 1.f)
			rainbow = 0.f;

		auto rainbow_col = Color::FromHSB(rainbow, 1, 1);

		auto local_pos = g_ctx.local()->m_vecOrigin();
		DrawBeam(local_pos, Vector(local_pos.x, local_pos.y + 10, local_pos.z), g_cfg.esp.rainbow_trails ? rainbow_col : g_cfg.esp.trails_color);
	}
}








void worldesp::fog_changer()
{
	static auto fog_override = m_cvar()->FindVar(crypt_str("fog_override")); //-V807

	if (!g_cfg.esp.fog)
	{
		if (fog_override->GetBool())
			fog_override->SetValue(FALSE);

		return;
	}

	if (!fog_override->GetBool())
		fog_override->SetValue(TRUE);

	static auto fog_start = m_cvar()->FindVar(crypt_str("fog_start"));

	if (fog_start->GetInt())
		fog_start->SetValue(0);

	static auto fog_end = m_cvar()->FindVar(crypt_str("fog_end"));

	if (fog_end->GetInt() != g_cfg.esp.fog_distance)
		fog_end->SetValue(g_cfg.esp.fog_distance);

	static auto fog_maxdensity = m_cvar()->FindVar(crypt_str("fog_maxdensity"));

	if (fog_maxdensity->GetFloat() != (float)g_cfg.esp.fog_density * 0.01f) //-V550
		fog_maxdensity->SetValue((float)g_cfg.esp.fog_density * 0.01f);

	char buffer_color[12];
	sprintf_s(buffer_color, 12, "%i %i %i", g_cfg.esp.fog_color.r(), g_cfg.esp.fog_color.g(), g_cfg.esp.fog_color.b());

	static auto fog_color = m_cvar()->FindVar(crypt_str("fog_color"));

	if (strcmp(fog_color->GetString(), buffer_color)) //-V526
		fog_color->SetValue(buffer_color);
}

void worldesp::world_modulation(entity_t* entity)
{
	if (!g_cfg.esp.world_modulation)
		return;

	entity->set_m_bUseCustomBloomScale(TRUE);
	entity->set_m_flCustomBloomScale(g_cfg.esp.bloom * 0.01f);

	entity->set_m_bUseCustomAutoExposureMin(TRUE);
	entity->set_m_flCustomAutoExposureMin(g_cfg.esp.exposure * 0.001f);

	entity->set_m_bUseCustomAutoExposureMax(TRUE);
	entity->set_m_flCustomAutoExposureMax(g_cfg.esp.exposure * 0.001f);
}

void worldesp::molotov_timer(entity_t* entity)
{
	if (!g_cfg.esp.molotov_timer)
		return;

	auto inferno = reinterpret_cast<inferno_t*>(entity);
	auto origin = inferno->GetAbsOrigin();

	Vector screen_origin;

	if (!math::world_to_screen(origin, screen_origin))
		return;

	auto spawn_time = inferno->get_spawn_time();
	auto factor = (spawn_time + inferno_t::get_expiry_time() - m_globals()->m_curtime) / inferno_t::get_expiry_time();

	static auto size = Vector2D(35.0f, 5.0f);
	render::get().circle_filled(screen_origin.x, screen_origin.y - size.y * 0.5f, 60, 20, Color(15, 15, 15, 187));

	render::get().rect_filled(screen_origin.x - size.x * 0.5f, screen_origin.y - size.y * 0.5f - 1.0f, size.x, size.y, Color(37, 37, 37, g_cfg.esp.molotov_timer_color.a()));
	render::get().rect_filled(screen_origin.x - size.x * 0.5f + 2.0f, screen_origin.y - size.y * 0.5f, (size.x - 4.0f) * factor, size.y - 2.0f, g_cfg.esp.molotov_timer_color);

	render::get().rect(screen_origin.x - size.x * 0.5f, screen_origin.y - size.y * 0.5f, size.x, size.y, Color(7, 7, 7, g_cfg.esp.molotov_timer_color.a()));
	render::get().text(fonts[ESP], screen_origin.x, screen_origin.y - size.y * 0.5f + 12.0f, g_cfg.esp.molotov_timer_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, "FIRE");
	render::get().text(fonts[SUBTABWEAPONS], screen_origin.x + 1.0f, screen_origin.y - size.y * 0.5f - 9.0f, g_cfg.esp.molotov_timer_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, "l");
}

void worldesp::smoke_timer(entity_t* entity)
{
	if (!g_cfg.esp.smoke_timer)
		return;

	auto smoke = reinterpret_cast<smoke_t*>(entity);

	if (!smoke->m_nSmokeEffectTickBegin() || !smoke->m_bDidSmokeEffect())
		return;

	auto origin = smoke->GetAbsOrigin();

	Vector screen_origin;

	if (!math::world_to_screen(origin, screen_origin))
		return;

	auto spawn_time = TICKS_TO_TIME(smoke->m_nSmokeEffectTickBegin());
	auto factor = (spawn_time + smoke_t::get_expiry_time() - m_globals()->m_curtime) / smoke_t::get_expiry_time();

	static auto size = Vector2D(35.0f, 5.0f);
	render::get().circle_filled(screen_origin.x, screen_origin.y - size.y * 0.5f, 60, 20, Color(15, 15, 15, 187));

	render::get().rect_filled(screen_origin.x - size.x * 0.5f, screen_origin.y - size.y * 0.5f - 1.0f, size.x, size.y, Color(37, 37, 37, g_cfg.esp.smoke_timer_color.a()));
	render::get().rect_filled(screen_origin.x - size.x * 0.5f + 2.0f, screen_origin.y - size.y * 0.5f, (size.x - 4.0f) * factor, size.y - 2.0f, g_cfg.esp.smoke_timer_color);

	render::get().rect(screen_origin.x - size.x * 0.5f, screen_origin.y - size.y * 0.5f, size.x, size.y, Color(7, 7, 7, g_cfg.esp.smoke_timer_color.a()));
	render::get().text(fonts[ESP], screen_origin.x, screen_origin.y - size.y * 0.5f + 12.0f, g_cfg.esp.smoke_timer_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, "SMOKE");
	render::get().text(fonts[SUBTABWEAPONS], screen_origin.x + 1.0f, screen_origin.y - size.y * 0.5f - 9.0f, g_cfg.esp.smoke_timer_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, "k");
}

void worldesp::Wireframe() {



	//static auto linegoesthrusmoke = Utils::PatternScan(GetModuleHandleA("client.dll"), "A3 ? ? ? ? 57 8B CB");
	static bool set = true;

	std::vector<const char*> vistasmoke_nodraw =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		"particle/vistasmokev1/vistasmokev1_smokegrenade"
	};

	//Iaterial* materialfoda = g_MatSystem->FindMaterial(vistasmoke_nodraw, TEXTURE_GROUP_OTHER);

	if (!g_cfg.esp.wireframe)
	{
		if (set)
		{
			for (auto material_name : vistasmoke_nodraw)
			{
				IMaterial* mat = m_materialsystem()->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}
			set = false;
		}
		return;
	}

	set = true;

	if (set)
	{
		for (auto mat_s : vistasmoke_nodraw)
		{
			IMaterial* mat = m_materialsystem()->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		}
	}


	//static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x1);
	//*(int*)(smokecout) = 0;

	static auto shit = *(DWORD*)(util::FindSignature("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0") + 0x8);
	if (shit)
		if (g_cfg.esp.wireframe)
			*(int*)(shit) = 0;

}




void worldesp::grenade_projectiles(entity_t* entity)
{
	if (!g_cfg.esp.projectiles)
		return;

	Vector pos, pos3D;
	pos3D = entity->m_vecOrigin();
	if (!math::world_to_screen(pos3D, pos))
		return;

	auto local_player = g_ctx.local();
	if (!local_player) return;

	auto cClass = entity->GetClientClass();

	std::string model_name = m_modelinfo()->GetModelName(entity->GetModel());

	if (!model_name.empty())
	{
		if (cClass->m_ClassID == (int)ClassID::CBaseCSGrenadeProjectile) {

			if (model_name.find("fraggrenade") != std::string::npos) {
				//Drawing::DrawFilledCircle(pos.x, pos.y, 10, 100, color_t(255, 255, 255, 150));
				render::get().text(fonts[GRENADES], pos.x, pos.y - 5.0f, Color(255, 255, 255), NULL, "j");
			}
			else if (model_name.find("bang") != std::string::npos) {
				//Drawing::DrawFilledCircle(pos.x, pos.y, 10, 100, color_t(255, 255, 255, 150));
				render::get().text(fonts[GRENADES], pos.x, pos.y - 5.0f, Color(255, 255, 255), NULL, "i");

			}

		}
		if (cClass->m_ClassID == (int)ClassID::CDecoyProjectile) {

			if (model_name.find("decoy") != std::string::npos) {
				//Drawing::DrawFilledCircle(pos.x, pos.y, 10, 100, color_t(255, 255, 255, 150));
				render::get().text(fonts[GRENADES], pos.x, pos.y - 5.0f, Color(255, 255, 255), NULL, "m");
			}
		}
		if (cClass->m_ClassID == (int)ClassID::CMolotovProjectile) {

			if (model_name.find("molotov") != std::string::npos) {
				//    Drawing::DrawFilledCircle(pos.x, pos.y, 10, 100, color_t(255, 255, 255, 150));
				render::get().text(fonts[GRENADES], pos.x, pos.y - 5.0f, Color(255, 255, 255), NULL, "l");
			}
			else if (model_name.find("incendiary") != std::string::npos) {
				//Drawing::DrawFilledCircle(pos.x, pos.y, 10, 100, color_t(255, 255, 255, 150));
				render::get().text(fonts[GRENADES], pos.x, pos.y - 5.0f, Color(255, 255, 255), NULL, "n");
			}
		}
	}
}

void worldesp::bomb_timer(entity_t* entity)
{
	if (!g_cfg.esp.bomb_timer)
		return;

	if (!g_ctx.globals.bomb_timer_enable)
		return;

	static auto mp_c4timer = m_cvar()->FindVar(crypt_str("mp_c4timer"));
	auto bomb = (CCSBomb*)entity;

	auto c4timer = mp_c4timer->GetFloat();
	auto bomb_timer = bomb->m_flC4Blow() - m_globals()->m_curtime;

	if (bomb_timer < 0.0f)
		return;

	static int width, height;
	m_engine()->GetScreenSize(width, height);

	auto factor = bomb_timer / c4timer * height;

	auto red_factor = (int)(255.0f - bomb_timer / c4timer * 255.0f);
	auto green_factor = (int)(bomb_timer / c4timer * 255.0f);

	render::get().rect_filled(0, height - factor, 26, factor, Color(red_factor, green_factor, 0, 100));

	auto text_position = height - factor + 11;

	if (text_position > height - 9)
		text_position = height - 9;

	render::get().text(fonts[ESP], 13, text_position, Color::White, HFONT_CENTERED_X | HFONT_CENTERED_Y, "%0.1f", bomb_timer);

	Vector screen;

	if (math::world_to_screen(entity->GetAbsOrigin(), screen))
		render::get().text(fonts[ESP], screen.x, screen.y, Color(red_factor, green_factor, 0), HFONT_CENTERED_X | HFONT_CENTERED_Y, "BOMB");
}

void worldesp::dropped_weapons(entity_t* entity)
{
	auto weapon = (weapon_t*)entity; //-V1027
	auto owner = (player_t*)m_entitylist()->GetClientEntityFromHandle(weapon->m_hOwnerEntity());

	if (owner->is_player())
		return;

	Box box;

	if (util::get_bbox(weapon, box, false))
	{
		auto offset = 0;

		if (g_cfg.esp.weapon[WEAPON_BOX])
		{
			render::get().rect(box.x, box.y, box.w, box.h, g_cfg.esp.box_color);

			if (g_cfg.esp.weapon[WEAPON_ICON])
			{
				render::get().text(fonts[SUBTABWEAPONS], box.x + box.w / 2, box.y - 14, g_cfg.esp.weapon_color, HFONT_CENTERED_X, weapon->get_icon());
				offset = 14;
			}

			if (g_cfg.esp.weapon[WEAPON_TEXT])
				render::get().text(fonts[ESP], box.x + box.w / 2, box.y + box.h + 2, g_cfg.esp.weapon_color, HFONT_CENTERED_X, weapon->get_name().c_str());

			if (g_cfg.esp.weapon[WEAPON_AMMO] && entity->GetClientClass()->m_ClassID != CBaseCSGrenadeProjectile && entity->GetClientClass()->m_ClassID != CSmokeGrenadeProjectile && entity->GetClientClass()->m_ClassID != CSensorGrenadeProjectile && entity->GetClientClass()->m_ClassID != CMolotovProjectile && entity->GetClientClass()->m_ClassID != CDecoyProjectile)
			{
				auto inner_back_color = Color::Black;
				inner_back_color.SetAlpha(153);

				render::get().rect_filled(box.x - 1, box.y + box.h + 14, box.w + 2, 4, inner_back_color);
				render::get().rect_filled(box.x, box.y + box.h + 15, weapon->m_iClip1() * box.w / weapon->get_csweapon_info()->iMaxClip1, 2, g_cfg.esp.weapon_ammo_color);
			}


		}
		else
		{
			if (g_cfg.esp.weapon[WEAPON_ICON])
				render::get().text(fonts[SUBTABWEAPONS], box.x + box.w / 2, box.y + box.h / 2 - 7, g_cfg.esp.weapon_color, HFONT_CENTERED_X, weapon->get_icon());

			if (g_cfg.esp.weapon[WEAPON_TEXT])
				render::get().text(fonts[ESP], box.x + box.w / 2, box.y + box.h / 2 + 6, g_cfg.esp.weapon_color, HFONT_CENTERED_X, weapon->get_name().c_str());

			if (g_cfg.esp.weapon[WEAPON_AMMO] && entity->GetClientClass()->m_ClassID != CBaseCSGrenadeProjectile && entity->GetClientClass()->m_ClassID != CSmokeGrenadeProjectile && entity->GetClientClass()->m_ClassID != CSensorGrenadeProjectile && entity->GetClientClass()->m_ClassID != CMolotovProjectile && entity->GetClientClass()->m_ClassID != CDecoyProjectile)
			{
				static auto pos = 0;

				if (g_cfg.esp.weapon[WEAPON_ICON] && g_cfg.esp.weapon[WEAPON_TEXT])
					pos = 19;
				else if (g_cfg.esp.weapon[WEAPON_ICON])
					pos = 8;
				else if (g_cfg.esp.weapon[WEAPON_TEXT])
					pos = 19;

				auto inner_back_color = Color::Black;
				inner_back_color.SetAlpha(153);

				render::get().rect_filled(box.x - 1, box.y + box.h / 2 + pos - 1, box.w + 2, 4, inner_back_color);
				render::get().rect_filled(box.x, box.y + box.h / 2 + pos, weapon->m_iClip1() * box.w / weapon->get_csweapon_info()->iMaxClip1, 2, g_cfg.esp.weapon_ammo_color);
			}

			if (g_cfg.esp.weapon[WEAPON_DISTANCE])
			{
				auto distance = g_ctx.local()->GetAbsOrigin().DistTo(weapon->GetAbsOrigin()) / 12.0f;

				if (g_cfg.esp.weapon[WEAPON_ICON] && g_cfg.esp.weapon[WEAPON_TEXT])
					offset = 21;
				else if (g_cfg.esp.weapon[WEAPON_ICON])
					offset = 21;
				else if (g_cfg.esp.weapon[WEAPON_TEXT])
					offset = 8;

				render::get().text(fonts[ESP], box.x + box.w / 2, box.y + box.h / 2 - offset, g_cfg.esp.weapon_color, HFONT_CENTERED_X, "%i FT", (int)distance);
			}
		}
	}
}