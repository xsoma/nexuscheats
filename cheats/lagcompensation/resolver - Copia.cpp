// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "animation_system.h"
#include "..\ragebot\aim.h"

void resolver::initialize(player_t* e, adjust_data* record, const float& goal_feet_yaw, const float& pitch)
{
	player = e;
	player_record = record;

	original_goal_feet_yaw = math::normalize_yaw(goal_feet_yaw);
	original_pitch = math::normalize_pitch(pitch);
}

void resolver::reset()
{
	player = nullptr;
	player_record = nullptr;

	side = false;
	fake = false;

	was_first_bruteforce = false;
	was_second_bruteforce = false;

	original_goal_feet_yaw = 0.0f;
	original_pitch = 0.0f;
}


float flAngleMod(float flAngle)
{
	return((360.0f / 65536.0f) * ((int32_t)(flAngle * (65536.0f / 360.0f)) & 65535));
}

float ApproachAngle(float flTarget, float flValue, float flSpeed)
{
	flTarget = flAngleMod(flTarget);
	flValue = flAngleMod(flValue);

	float delta = flTarget - flValue;

	if (flSpeed < 0)
		flSpeed = -flSpeed;

	if (delta < -180)
		delta += 360;
	else if (delta > 180)
		delta -= 360;

	if (delta > flSpeed)
		flValue += flSpeed;
	else if (delta < -flSpeed)
		flValue -= flSpeed;
	else
		flValue = flTarget;

	return flValue;
}

static auto GetSmoothedVelocity = [](float min_delta, Vector a, Vector b) {
	Vector delta = a - b;
	float delta_length = delta.Length();

	if (delta_length <= min_delta) {
		Vector result;
		if (-min_delta <= delta_length) {
			return a;
		}
		else {
			float iradius = 1.0f / (delta_length + FLT_EPSILON);
			return b - ((delta * iradius) * min_delta);
		}
	}
	else {
		float iradius = 1.0f / (delta_length + FLT_EPSILON);
		return b + ((delta * iradius) * min_delta);
	}
};

void resolver::resolve(player_t* player)
{
	if (!player || !player->get_animation_state())
		return;

	auto animState = player->get_animation_state();

	if (!animState)
		return;

	float lby = player->m_flLowerBodyYawTarget();
	float ey = player->m_angEyeAngles().y;
	float delta = abs(ey - lby);

	float p1 = ((delta > 0) ? -1.f : 1.f);
	float p2 = ((delta > 0) ? 1.f : -1.f);

	static float ang = 90.f;

	switch (g_ctx.globals.missed_shots[player->EntIndex()] % 2)
	{
	case 0:
		player->get_animation_state()->m_flGoalFeetYaw = ey + (ang * p1);
		break;
	case 1:
		player->get_animation_state()->m_flGoalFeetYaw = ey + (ang * p2);
		break;
	}
}

void resolver::resolve_yaw()
{
	player_info_t player_info;

	if (!m_engine()->GetPlayerInfo(player->EntIndex(), &player_info))
		return;

#if RELEASE
	if (player_info.fakeplayer || !g_ctx.local()->is_alive() || player->m_iTeamNum() == g_ctx.local()->m_iTeamNum()) //-V807
#else
	if (!g_ctx.local()->is_alive() || player->m_iTeamNum() == g_ctx.local()->m_iTeamNum())
#endif
	{
		player_record->side = RESOLVER_ORIGINAL;
		return;
	}

	auto animstate = player->get_animation_state();

	if (!animstate)
	{
		player_record->side = RESOLVER_ORIGINAL;
		return;
	}

	float lby = player->m_flLowerBodyYawTarget();
	float ey = player->m_angEyeAngles().y;
	float delta = abs(ey - lby);

	float p1 = ((delta > 0) ? -1.f : 1.f);
	float p2 = ((delta > 0) ? 1.f : -1.f);

	if (g_ctx.globals.missed_shots[player->EntIndex()] <= 0)
	{
		int extend = player->get_animlayer(3)->m_flCycle == 0.f && player->get_animlayer(3)->m_flWeight == 0.f;

		if (extend)
		{
			float fire_yaw = math::normalize_yaw(math::calculate_angle(player->GetBonePos(8), g_ctx.local()->GetBonePos(0)).y);
			//float fire_yaw = math::normalize_yaw(math::calculate_angle(player->m_BoneAccessor().m_pBones[8].GetOrigin(), g_ctx.local()->m_BoneAccessor().m_pBones[0].GetOrigin()).y);

			float fire_yaw_left = fabsf(math::normalize_yaw(fire_yaw - (player->m_angEyeAngles().y + 58.f)));
			float fire_yaw_right = fabsf(math::normalize_yaw(fire_yaw - (player->m_angEyeAngles().y - 58.f)));

			if (fire_yaw_left > fire_yaw_right)
			{
				player_record->side = LEGEND_1;
			}
			else
			{
				player_record->side = LEGEND_2;
			}
		}
		else
		{
			float fire_yaw = math::normalize_yaw(math::calculate_angle(player->GetBonePos(8), g_ctx.local()->GetBonePos(0)).y);

			float fire_yaw_left = fabsf(math::normalize_yaw(fire_yaw - (player->m_angEyeAngles().y + 30.f)));
			float fire_yaw_right = fabsf(math::normalize_yaw(fire_yaw - (player->m_angEyeAngles().y - 30.f)));

			if (fire_yaw_left > fire_yaw_right)
			{
				player_record->side = LEGEND_3;
			}
			else
			{
				player_record->side = LEGEND_4;
			}
		}
	}
	else
	{
		int extend = player->get_animlayer(3)->m_flCycle == 0.f && player->get_animlayer(3)->m_flWeight == 0.f;
		switch (g_ctx.globals.missed_shots[player->EntIndex()] % 2)
		{
		case 0:
			if (extend)
			{
				switch (last_side)
				{
				case RESOLVER_ORIGINAL:
					if (p1 == 1.f) player_record->side = LEGEND_1;
					else if (p1 == -1.f) player_record->side = LEGEND_2;
					break;
				case LEGEND_1:
					player_record->side = LEGEND_2;
					break;
				case LEGEND_2:
					player_record->side = LEGEND_1;
					break;
				}
			}
			else
			{
				switch (last_side)
				{
				case RESOLVER_ORIGINAL:
					if (p1 == 1.f) player_record->side = LEGEND_3;
					else if (p1 == -1.f) player_record->side = LEGEND_4;
					break;
				case LEGEND_3:
					player_record->side = LEGEND_4;
					break;
				case LEGEND_4:
					player_record->side = LEGEND_3;
					break;
				}
			}
			
			break;
		case 1:
			if (extend)
			{
				switch (last_side)
				{
				case RESOLVER_ORIGINAL:
					if (p2 == 1.f) player_record->side = LEGEND_2;
					else if (p2 == -1.f) player_record->side = LEGEND_1;
					break;
				case LEGEND_1:
					player_record->side = LEGEND_2;
					break;
				case LEGEND_2:
					player_record->side = LEGEND_1;
					break;
				}
			}
			else
			{
				switch (last_side)
				{
				case RESOLVER_ORIGINAL:
					if (p2 == 1.f) player_record->side = LEGEND_4;
					else if (p2 == -1.f) player_record->side = LEGEND_3;
					break;
				case LEGEND_3:
					player_record->side = LEGEND_4;
					break;
				case LEGEND_4:
					player_record->side = LEGEND_3;
					break;
				}
			}
			
			break;
		}
	}
}

float resolver::resolve_pitch()
{
	return original_pitch;
}