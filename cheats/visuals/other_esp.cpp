// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "other_esp.h"
#include "..\autowall\autowall.h"
#include "..\ragebot\antiaim.h"
#include "..\misc\logs.h"
#include "..\misc\misc.h"
#include "..\lagcompensation\local_animations.h"

bool can_penetrate(weapon_t* weapon)
{
	auto weapon_info = weapon->get_csweapon_info();

	if (!weapon_info)
		return false;

	Vector view_angles;
	m_engine()->GetViewAngles(view_angles);

	Vector direction;
	math::angle_vectors(view_angles, direction);

	CTraceFilter filter;
	filter.pSkip = g_ctx.local();

	trace_t trace;
	util::trace_line(g_ctx.globals.eye_pos, g_ctx.globals.eye_pos + direction * weapon_info->flRange, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &trace);

	if (trace.fraction == 1.0f) //-V550
		return false;

	auto eye_pos = g_ctx.globals.eye_pos;
	auto hits = 1;
	auto damage = (float)weapon_info->iDamage;
	auto penetration_power = weapon_info->flPenetration;

	static auto damageReductionBullets = m_cvar()->FindVar(crypt_str("ff_damage_reduction_bullets"));
	static auto damageBulletPenetration = m_cvar()->FindVar(crypt_str("ff_damage_bullet_penetration"));

	return autowall::get().handle_bullet_penetration(weapon_info, trace, eye_pos, direction, hits, damage, penetration_power, damageReductionBullets->GetFloat(), damageBulletPenetration->GetFloat());
}

void otheresp::molotov_shit(entity_t* ent)
{
	if (!g_cfg.esp.grenade_esp[3]) return;

	if (ent->GetClientClass()->m_ClassID != (int)ClassID::CInferno) return;

	auto origin = ent->m_vecOrigin();
	Vector screen;

	if (!math::world_to_screen(origin, screen)) return;

	//const auto ILspawnTime = *(float*)(uintptr_t(ent) + 0x20);
	//const auto ILfactor = ((ILspawnTime + 7.03125f) - g_GlobalVars->curtime) / 7.03125f;

	Vector min, max;

	ent->GetRenderBounds(min, max);

	Vector yes = max - min;

	const auto range = yes.Length2D() * 0.5f;

	Color col;

	player_t* owner = (player_t*)m_entitylist()->GetClientEntityFromHandle(ent->m_hOwnerEntity());

	if (owner == g_ctx.local())
		col = g_cfg.esp.molotov_shit_team;
	if (owner->m_iTeamNum() != g_ctx.local()->m_iTeamNum())
		col = g_cfg.esp.molotov_shit_enemy;
	if (owner->m_iTeamNum() == g_ctx.local()->m_iTeamNum() && owner != g_ctx.local())
		col = g_cfg.esp.molotov_shit_team;


	render::get().Draw3DFilledCircle(origin, range, 40.f, col);
	render::get().Draw3DCircle(origin, range, 40.f, Color(0, 0, 0, 255 / 2));
	render::get().Draw3DCircle(origin, range, 41.f, Color(0, 0, 0, 255 / 2));
	render::get().Draw3DCircle(origin, range, 39.f, Color(0, 0, 0, 255 / 2));

	const auto ILspawnTime = *(float*)(uintptr_t(ent) + 0x20);
	const auto ILfactor = ((ILspawnTime + 7.03125f) - m_globals()->m_curtime) / 7.03125f;

	//render::get().text(fonts[GRENADES], screen.x, screen.y - 21.0f, Color(255, 255, 255), NULL, "l");

	render::get().rect(screen.x - 75 * 0.5f, screen.y - 7 * 0.5f, 75, 7, Color(0, 0, 0));
	render::get().rect_filled(screen.x - 75 * 0.5f + 2.0f, screen.y - 7 * 0.5f + 2.0f, (75 - 4.0f) * ILfactor, 7.f - 4.0f, col);
}






void otheresp::penetration_reticle()
{
	if (!g_cfg.player.enable)
		return;

	if (!g_cfg.esp.penetration_reticle)
		return;

	if (!g_ctx.local()->is_alive())
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	CGameTrace enterTrace;
	CTraceFilter filter;
	Ray_t ray;

	Vector viewangles; m_engine()->GetViewAngles(viewangles);
	Vector direction; math::angle_vectors_deg_vec(viewangles, &direction);
	Vector start = g_ctx.local()->GetEyePos();
	auto m_flMaxRange = g_ctx.local()->m_hActiveWeapon().Get()->get_csweapon_info()->flRange * 2; //
	Vector end = start + (direction * m_flMaxRange);

	filter.pSkip = g_ctx.local();
	ray.Init(start, end);
	m_trace()->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &enterTrace);

	float anglez = math::dot_product(Vector(0, 0, 1), enterTrace.plane.normal);
	float invanglez = math::dot_product(Vector(0, 0, -1), enterTrace.plane.normal);
	float angley = math::dot_product(Vector(0, 1, 0), enterTrace.plane.normal);
	float invangley = math::dot_product(Vector(0, -1, 0), enterTrace.plane.normal);
	float anglex = math::dot_product(Vector(1, 0, 0), enterTrace.plane.normal);
	float invanglex = math::dot_product(Vector(-1, 0, 0), enterTrace.plane.normal);

	if (anglez > 0.5 || invanglez > 0.5)
		render::get().filled_rect_world(enterTrace.endpos, Vector2D(5, 5), //hidden blend xd
			can_penetrate(weapon) ? Color(0, 255, 0, 165) : Color(255, 0, 0, 165), 0);

	else if (angley > 0.5 || invangley > 0.5)
		render::get().filled_rect_world(enterTrace.endpos, Vector2D(5, 5), can_penetrate(weapon) ? Color(0, 255, 0, 165) : Color(255, 0, 0, 165), 1);

	else if (anglex > 0.5 || invanglex > 0.5)
		render::get().filled_rect_world(enterTrace.endpos, Vector2D(5, 5), can_penetrate(weapon) ? Color(0, 255, 0, 165) : Color(255, 0, 0, 165), 2);
}


/*void otheresp::indicators()
{
	if (!g_ctx.local()->is_alive()) //-V807
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (g_cfg.esp.indicators[INDICATOR_FAKE] && (antiaim::get().type == ANTIAIM_LEGIT || g_cfg.antiaim.type[antiaim::get().type].desync))
	{
		auto color = Color(130, 20, 20);
		auto animstate = g_ctx.local()->get_animation_state();

		if (animstate && local_animations::get().local_data.animstate)
		{
			auto delta = fabs(math::normalize_yaw(animstate->m_flGoalFeetYaw - local_animations::get().local_data.animstate->m_flGoalFeetYaw));
			auto desync_delta = max(g_ctx.local()->get_max_desync_delta(), 58.0f);

			color = Color(130, 20 + (int)(min(delta / desync_delta, 1.0f) * 150.0f), 20);
		}

		m_indicators.push_back(m_indicator("FAKE", color));
	}

	if (g_cfg.esp.indicators[INDICATOR_DESYNC_SIDE] && (antiaim::get().type == ANTIAIM_LEGIT && g_cfg.antiaim.desync == 1 || antiaim::get().type != ANTIAIM_LEGIT && g_cfg.antiaim.type[antiaim::get().type].desync == 1) && !antiaim::get().condition(g_ctx.get_command()))
	{
		auto side = antiaim::get().desync_angle > 0.0f ? "RIGHT" : "LEFT";

		if (antiaim::get().type == ANTIAIM_LEGIT)
			side = antiaim::get().desync_angle > 0.0f ? "LEFT" : "RIGHT";

		m_indicators.push_back(m_indicator(side, Color(130, 170, 20)));
	}

	auto choke_indicator = false;

	if (g_cfg.esp.indicators[INDICATOR_CHOKE] && !fakelag::get().condition && !misc::get().double_tap_enabled && !misc::get().hide_shots_enabled)
	{
		m_indicators.push_back(m_indicator(("CHOKE: " + std::to_string(fakelag::get().max_choke)), Color(130, 170, 20)));
		choke_indicator = true;
	}

	if (g_cfg.esp.indicators[INDICATOR_DAMAGE] && g_ctx.globals.current_weapon != -1 && key_binds::get().get_key_bind_state(4 + g_ctx.globals.current_weapon) && !weapon->is_non_aim())
	{
		if (g_cfg.ragebot.weapon[g_ctx.globals.current_weapon].minimum_override_damage > 100)
			m_indicators.push_back(m_indicator(("DAMAGE: HP + " + std::to_string(g_cfg.ragebot.weapon[g_ctx.globals.current_weapon].minimum_override_damage - 100)), Color(130, 170, 20)));
		else
			m_indicators.push_back(m_indicator(("DAMAGE: " + std::to_string(g_cfg.ragebot.weapon[g_ctx.globals.current_weapon].minimum_override_damage)), Color(130, 170, 20)));
	}

	if (g_cfg.esp.indicators[INDICATOR_SAFE_POINTS] && key_binds::get().get_key_bind_state(3) && !weapon->is_non_aim())
		m_indicators.push_back(m_indicator("SAFE POINTS", Color(130, 170, 20)));

	if (g_cfg.esp.indicators[INDICATOR_BODY_AIM] && key_binds::get().get_key_bind_state(22) && !weapon->is_non_aim())
		m_indicators.push_back(m_indicator("BODY AIM", Color(130, 170, 20)));

	if (choke_indicator)
		return;

	if (g_cfg.esp.indicators[INDICATOR_DT] && g_cfg.ragebot.double_tap && g_cfg.ragebot.double_tap_key.key > KEY_NONE && g_cfg.ragebot.double_tap_key.key < KEY_MAX && misc::get().double_tap_key)
		m_indicators.push_back(m_indicator("DT", !g_ctx.local()->m_bGunGameImmunity() && !(g_ctx.local()->m_fFlags() & FL_FROZEN) && !antiaim::get().freeze_check && misc::get().double_tap_enabled && !weapon->is_grenade() && weapon->m_iItemDefinitionIndex() != WEAPON_TASER && weapon->m_iItemDefinitionIndex() != WEAPON_REVOLVER && weapon->can_fire(false) ? Color(130, 170, 20) : Color(130, 20, 20)));

	if (g_cfg.esp.indicators[INDICATOR_HS] && g_cfg.antiaim.hide_shots && g_cfg.antiaim.hide_shots_key.key > KEY_NONE && g_cfg.antiaim.hide_shots_key.key < KEY_MAX && misc::get().hide_shots_key)
		m_indicators.push_back(m_indicator("HS", !g_ctx.local()->m_bGunGameImmunity() && !(g_ctx.local()->m_fFlags() & FL_FROZEN) && !antiaim::get().freeze_check && misc::get().hide_shots_enabled ? Color(130, 170, 20) : Color(130, 20, 20)));
}*/



void otheresp::draw_velocity()
{
	if (!g_cfg.esp.velocity_graph)
		return;

	if (!g_ctx.local())
		return;

	if (!m_engine()->IsInGame() || !g_ctx.local()->is_alive())
		return;

	static std::vector<float> velData(120, 0);

	Vector vecVelocity = g_ctx.local()->m_vecVelocity();
	float currentVelocity = sqrt(vecVelocity.x * vecVelocity.x + vecVelocity.y * vecVelocity.y);

	velData.erase(velData.begin());
	velData.push_back(currentVelocity);

	int vel = g_ctx.local()->m_vecVelocity().Length2D();

	static int width, height;
	m_engine()->GetScreenSize(width, height);
	render::get().text(fonts[VELOCITY], width / 2, height / 1.1, Color(255, 255, 255, 255), HFONT_CENTERED_X | HFONT_CENTERED_Y, "%i u/s", vel);


	for (auto i = 0; i < velData.size() - 1; i++)
	{
		int cur = velData.at(i);
		int next = velData.at(i + 1);

		render::get().line(
			width / 2 + (velData.size() * 3 / 2) - (i - 1) * 3.f,
			height / 1.15 - (std::clamp(cur, 0, 450) * .2f),
			width / 2 + (velData.size() * 3 / 2) - i * 3.f,
			height / 1.15 - (std::clamp(next, 0, 450) * .2f), Color(255, 255, 255, 200)
		);
	}
}
void otheresp::indicators()
{
	if (!g_ctx.local()->is_alive()) //-V807
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	//if (g_cfg.esp.indicators[INDICATOR_DUCK] && key_binds::get().get_key_bind_state(20))

	/*int miss = 0;
	int hitted = 0;
	int fired = 0;
	for (int i = 0; i < 65; i++)
	{
		hitted += g_ctx.globals.hitted_shots[i];
	}

	for (int i = 0; i < 65; i++)
	{
		fired += g_ctx.globals.fired_shots2[i];
	}

	for (int i = 0; i < 65; i++)
	{
		g_ctx.globals.missed_shots[i] = g_ctx.globals.fired_shots2[i] - g_ctx.globals.hitted_shots[i];
	}

	for (int i = 0; i < 65; i++)
	{
		miss += g_ctx.globals.missed_shots[i];
	}*/

	

	//m_indicators.push_back(m_indicator("Fired:", fired, Color(255, 255, 255)));
	//m_indicators.push_back(m_indicator("Missed:", miss, Color(255, 255, 255)));
	//m_indicators.push_back(m_indicator("Hitted:", hitted, Color(255, 255, 255)));
}
void otheresp::draw_indicators()
{
	if (!g_ctx.local()->is_alive()) //-V807
		return;

	static int width, height;
	m_engine()->GetScreenSize(width, height);

	auto h = height / 2 + 50;

	for (auto& indicator : m_indicators)
	{
		//ender::get().gradient(10,10,10,h Color(0,0,0,200), Color(0,0,0,0))
		render::get().gradient(5, h - 15, 85, 30, Color(0, 0, 0, 150), Color(0, 0, 0, 0), GRADIENT_HORIZONTAL);
		render::get().text(fonts[INDICATORFONT], 10, h, indicator.m_color, HFONT_CENTERED_Y, indicator.m_text.c_str());
		h -= 35;
	}

	m_indicators.clear();
}

void otheresp::hitmarker_paint()
{
	if (!g_cfg.esp.hitmarker[0] && !g_cfg.esp.hitmarker[1])
	{
		hitmarker.hurt_time = FLT_MIN;
		hitmarker.point = ZERO;
		return;
	}

	if (!g_ctx.local()->is_alive())
	{
		hitmarker.hurt_time = FLT_MIN;
		hitmarker.point = ZERO;
		return;
	}

	if (hitmarker.hurt_time + 0.7f > m_globals()->m_curtime)
	{
		if (g_cfg.esp.hitmarker[0])
		{
			static int width, height;
			m_engine()->GetScreenSize(width, height);

			auto alpha = (int)((hitmarker.hurt_time + 0.7f - m_globals()->m_curtime) * 255.0f);
			hitmarker.hurt_color.SetAlpha(alpha);

			auto offset = 9.0f - (float)alpha / 255.0f * 7.0f;
			int screenCenterY, screenCenterX;
			auto lineSize = 12;

	
			screenCenterX = width / 2;
			screenCenterY = height / 2;

			render::get().line(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 2), screenCenterY - (lineSize / 2), hitmarker.hurt_color);
			render::get().line(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 2), screenCenterY + (lineSize / 2), hitmarker.hurt_color);
			render::get().line(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 2), screenCenterY + (lineSize / 2), hitmarker.hurt_color);
			render::get().line(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 2), screenCenterY - (lineSize / 2), hitmarker.hurt_color);
		}

		if (g_cfg.esp.hitmarker[1])
		{
			Vector world;

			if (math::world_to_screen(hitmarker.point, world))
			{
				auto alpha = (int)((hitmarker.hurt_time + 0.7f - m_globals()->m_curtime) * 255.0f);
				hitmarker.hurt_color.SetAlpha(alpha);

				auto offset = 9.0f - (float)alpha / 255.0f * 7.0f;

				auto linesize = 12;


				render::get().line(world.x - linesize, world.y - linesize, world.x - (linesize / 2), world.y - (linesize / 2),hitmarker.hurt_color);
				render::get().line(world.x - linesize, world.y + linesize, world.x - (linesize / 2), world.y + (linesize / 2), hitmarker.hurt_color);
				render::get().line(world.x + linesize, world.y + linesize, world.x + (linesize / 2), world.y + (linesize / 2), hitmarker.hurt_color);
				render::get().line(world.x + linesize, world.y - linesize, world.x + (linesize / 2), world.y - (linesize / 2), hitmarker.hurt_color);
			}
		}
	}
}

void otheresp::damage_marker_paint()
{
	for (auto i = 1; i < m_globals()->m_maxclients; i++) //-V807
	{
		if (damage_marker[i].hurt_time + 2.0f > m_globals()->m_curtime)
		{
			Vector screen;

			if (!math::world_to_screen(damage_marker[i].position, screen))
				continue;

			auto alpha = (int)((damage_marker[i].hurt_time + 2.0f - m_globals()->m_curtime) * 127.5f);
			damage_marker[i].hurt_color.SetAlpha(alpha);

			damage_marker[i].anim += 0.5f;

			render::get().text(fonts[NAME], screen.x, screen.y - damage_marker[i].anim, damage_marker[i].hurt_color, HFONT_CENTERED_X | HFONT_CENTERED_Y, "%i", damage_marker[i].damage);
		}
	}
}

void draw_circe(float x, float y, float radius, int resolution, DWORD color, DWORD color2, LPDIRECT3DDEVICE9 device);

void otheresp::spread_crosshair(LPDIRECT3DDEVICE9 device)
{
	if (!g_cfg.player.enable)
		return;

	if (!g_cfg.esp.show_spread)
		return;

	if (!g_ctx.local()->is_alive())
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (weapon->is_non_aim())
		return;

	int w, h;
	m_engine()->GetScreenSize(w, h);

	draw_circe((float)w * 0.5f, (float)h * 0.5f, g_ctx.globals.inaccuracy * 500.0f, 50, D3DCOLOR_RGBA(g_cfg.esp.show_spread_color.r(), g_cfg.esp.show_spread_color.g(), g_cfg.esp.show_spread_color.b(), g_cfg.esp.show_spread_color.a()), D3DCOLOR_RGBA(0, 0, 0, 0), device);
}

void draw_circe(float x, float y, float radius, int resolution, DWORD color, DWORD color2, LPDIRECT3DDEVICE9 device)
{
	LPDIRECT3DVERTEXBUFFER9 g_pVB2 = nullptr;
	std::vector <CUSTOMVERTEX2> circle(resolution + 2);

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0.0f;

	circle[0].rhw = 1.0f;
	circle[0].color = color2;

	for (auto i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - radius * cos(D3DX_PI * ((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - radius * sin(D3DX_PI * ((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0.0f;

		circle[i].rhw = 1.0f;
		circle[i].color = color;
	}

	device->CreateVertexBuffer((resolution + 2) * sizeof(CUSTOMVERTEX2), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, nullptr); //-V107

	if (!g_pVB2)
		return;

	void* pVertices;

	g_pVB2->Lock(0, (resolution + 2) * sizeof(CUSTOMVERTEX2), (void**)&pVertices, 0); //-V107
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(CUSTOMVERTEX2));
	g_pVB2->Unlock();

	device->SetTexture(0, nullptr);
	device->SetPixelShader(nullptr);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX2));
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);

	g_pVB2->Release();
}

void otheresp::automatic_peek_indicator()
{
	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	static auto position = ZERO;

	if (!g_ctx.globals.start_position.IsZero())
		position = g_ctx.globals.start_position;

	if (position.IsZero())
		return;

	static auto alpha = 0.0f;

	if (!weapon->is_non_aim() && key_binds::get().get_key_bind_state(18) || alpha)
	{
		if (!weapon->is_non_aim() && key_binds::get().get_key_bind_state(18))
			alpha += 3.0f * m_globals()->m_frametime; //-V807
		else
			alpha -= 3.0f * m_globals()->m_frametime;

		alpha = math::clamp(alpha, 0.0f, 1.0f);
		render::get().Draw3DFilledCircle(position, 15.0f, 40.f, Color(255, 255, 255, 200));
		render::get().Draw3DCircle(position, 15.0f, 40.f, Color(255, 255, 255));

		Vector screen;

		if (math::world_to_screen(position, screen))
		{
			static auto offset = 30.0f;

			if (!g_ctx.globals.fired_shot)
			{
				static auto switch_offset = false;

				if (offset <= 30.0f || offset >= 55.0f)
					switch_offset = !switch_offset;

				offset += switch_offset ? 22.0f * m_globals()->m_frametime : -22.0f * m_globals()->m_frametime;
				offset = math::clamp(offset, 30.0f, 55.0f);

				//render::get().rect_filled(screen.x - 10.0f, screen.y - offset - 59.0f, 20.0f, 45.0f, Color(240, 30, 30, (int)(alpha * 168.0f)));
				//render::get().triangle(Vector2D(screen.x, screen.y - offset), Vector2D(screen.x - 10.5f, screen.y - offset - 14.5f), Vector2D(screen.x + 9.5f, screen.y - offset - 14.5f), Color(240, 30, 30, (int)(alpha * 168.0f)));
			}
		}
	}
}