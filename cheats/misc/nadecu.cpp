#include "nadecu.h"
#include <cheats\visuals\GrenadePrediction.h>

c_nade_prediction nade_pred;

void c_nade_prediction::predict(CUserCmd* user_cmd) noexcept {
	if (!m_engine()->IsConnected() && !m_engine()->IsInGame())
		return;

	auto local_player = g_ctx.local();
	//    readability
	constexpr float restitution = 0.45f;
	constexpr float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	constexpr float velocity = 403.0f * 0.9f;

	float step, gravity, new_velocity, unk01;
	int index{}, grenade_act{ 1 };
	Vector pos, thrown_direction, start, eye_origin;
	QAngle angles, thrown;

	//    first time setup
	static auto sv_gravity = m_cvar()->FindVar("sv_gravity");

	//    calculate step and actual gravity value
	gravity = sv_gravity->GetFloat() / 8.0f;
	step = m_globals()->m_intervalpertick;

	//    get local view and eye origin
	eye_origin = local_player->GetEyePos();
	angles = QAngle(user_cmd->m_viewangles.x, user_cmd->m_viewangles.y, user_cmd->m_viewangles.z);

	//QAngle AA;
	//Vector BB;

//	AA = QAngle(BB.x, BB.y, BB.z);
	//BB = Vector(AA.pitch, AA.yaw, AA.roll);



	//    copy current angles and normalise pitch
	thrown = angles;

	if (thrown.pitch < 0) {
		thrown.pitch = -10 + thrown.pitch * ((90 - 10) / 90.0f);
	}
	else {
		thrown.pitch = -10 + thrown.pitch * ((90 + 10) / 90.0f);
	}

	//    find out how we're throwing the grenade
	auto primary_attack = user_cmd->m_buttons & IN_ATTACK;
	auto secondary_attack = user_cmd->m_buttons & IN_ATTACK2;

	if (primary_attack && secondary_attack) {
		grenade_act = ACT_LOB;
	}
	else if (secondary_attack) {
		grenade_act = ACT_DROP;
	}

	//    apply 'magic' and modulate by velocity
	unk01 = power[grenade_act];

	unk01 = unk01 * 0.7f;
	unk01 = unk01 + 0.3f;

	new_velocity = velocity * unk01;

	//    here's where the fun begins
	math::AngleVectors(thrown, thrown_direction);

	start = eye_origin + thrown_direction * 16.0f;
	thrown_direction = (thrown_direction * new_velocity) + local_player->m_vecVelocity();

	//    let's go ahead and predict
	for (auto time = 0.0f; index < 500; time += step) {

		pos = start + thrown_direction * step;
		trace_t trace;
		Ray_t ray;
		CTraceFilterSkipEntity filter(local_player);

		ray.Init(start, pos);
		m_trace()->TraceRay(ray, MASK_SOLID, &filter, &trace);

		//    modify path if we have hit something
		if (trace.fraction != 1.0f) {
			thrown_direction = trace.plane.normal * -2.0f * thrown_direction.Dot(trace.plane.normal) + thrown_direction;

			thrown_direction *= restitution;

			pos = start + thrown_direction * trace.fraction * step;

			time += (step * (1.0f - trace.fraction));
		}

		//    check for detonation
		auto detonate = detonated(local_player->m_hActiveWeapon().Get(), time, trace);

		//    emplace nade point
		_points.at(index++) = c_nadepoint(start, pos, trace.fraction != 1.0f, true, trace.plane.normal, detonate);
		start = pos;

		//    apply gravity modifier
		thrown_direction.z -= gravity * trace.fraction * step;

		if (detonate) {
			break;
		}
	}

	//    invalidate all empty points and finish prediction
	for (auto n = index; n < 500; ++n) {
		_points.at(n).m_valid = false;
	}

	_predicted = true;
}


bool c_nade_prediction::detonated(weapon_t* weapon, float time, trace_t& trace)noexcept {

	if (!weapon) {
		return true;
	}

	//    get weapon item index
	const auto index = weapon->m_iItemDefinitionIndex();

	switch (index) {
		//    flash and HE grenades only live up to 2.5s after thrown
	case WEAPON_HEGRENADE:
	case WEAPON_FLASHBANG:
		if (time > 2.5f) {
			return true;
		}
		break;

		//    fire grenades detonate on ground hit, or 3.5s after thrown
	case WEAPON_MOLOTOV:
	case WEAPON_INCGRENADE:
		if (trace.fraction != 1.0f && trace.plane.normal.z > 0.7f || time > 3.5f) {
			return true;
		}
		break;

		//    decoy and smoke grenades were buggy in prediction, so i used this ghetto work-around
	case WEAPON_DECOY:
	case WEAPON_SMOKEGRENADE:
		if (time > 5.0f) {
			return true;
		}
		break;
	}

	return false;
}


void c_nade_prediction::trace(CUserCmd* user_cmd) noexcept {
	if (!m_engine()->IsConnected() && !m_engine()->IsInGame())
		return;


	auto local_player = g_ctx.local();

	if (!g_cfg.misc.nadecu) {
		return;
	}

	if (!(user_cmd->m_buttons & IN_ATTACK) && !(user_cmd->m_buttons & IN_ATTACK2)) {
		_predicted = false;
		return;
	}

	const static std::vector< int > nades{
		WEAPON_FLASHBANG,
		WEAPON_SMOKEGRENADE,
		WEAPON_HEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE
	};

	//    grab local weapon
	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon) {
		return;
	}

	if (std::find(nades.begin(), nades.end(), weapon->m_iItemDefinitionIndex()) != nades.end()) {
		return predict(user_cmd);
	}

	_predicted = false;
}


void c_nade_prediction::draw() noexcept {
	if (!m_engine()->IsConnected() && !m_engine()->IsInGame())
		return;

	auto local_player = g_ctx.local();
	if (!local_player) {
		return;
	}
	if (!g_cfg.misc.nadecu) {
		return;
	}

	if (!m_engine()->IsInGame()) {
		return;
	}

	if (!local_player->is_alive()) {
		return;
	}

	auto draw_3d_dotted_circle = [](Vector position, float points, float radius) {
		float step = 3.141592654f * 2.0f / points;
		for (float a = 0; a < 3.141592654f * 2.0f; a += step) {
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);

			Vector start2d;
			if (math::world_to_screen(start, start2d))
				render::get().line(start2d.x, start2d.y, start2d.x + 1, start2d.y + 1, Color(255, 255, 255));
		}
	};

	Vector start, end;

	//    draw nade path
	if (_predicted) {
		for (auto& p : _points) {
			if (!p.m_valid) {
				break;
			}

			if (math::world_to_screen(p.m_start, start) && math::world_to_screen(p.m_end, end)) {

				render::get().line(start.x, start.y, end.x, end.y, Color(0, 0, 0));

				if (p.m_detonate || p.m_plane) {
					;
					render::get().circle(start.x - 2, start.y - 2, 5, 5, p.m_detonate ? Color(255, 0, 0) : Color(255, 255, 255));
				}
				if (p.m_detonate)
					draw_3d_dotted_circle(p.m_end, 100, 150);
			}
		}
	}
}