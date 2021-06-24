// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "../ragebot/aim.h"
#include "fakelag.h"
#include "misc.h"
#include "prediction_system.h"
#include "logs.h"

void fakelag::Fakelag(CUserCmd* m_pcmd)
{
	if (g_cfg.antiaim.fakelag && !g_ctx.globals.exploits)
	{
		static auto force_choke = false;

		if (force_choke)
		{
			force_choke = false;
			g_ctx.send_packet = false;
			return;
		}

		if (g_ctx.local()->m_fFlags() & FL_ONGROUND && !(engineprediction::get().backup_data.flags & FL_ONGROUND))
		{
			force_choke = true;
			g_ctx.send_packet = false;
			return;
		}
	}

	static auto fluctuate_ticks = 0;
	static auto switch_ticks = false;
	static auto random_factor = min(rand() % 16 + 1, g_cfg.antiaim.triggers_fakelag_amount);

	auto choked = m_clientstate()->iChokedCommands; //-V807
	auto flags = engineprediction::get().backup_data.flags; //-V807
	auto velocity = engineprediction::get().backup_data.velocity.Length(); //-V807
	auto velocity2d = engineprediction::get().backup_data.velocity.Length2D();

	auto max_speed = 260.0f;
	auto weapon_info = g_ctx.globals.weapon->get_csweapon_info();

	if (weapon_info)
		max_speed = g_ctx.globals.scoped ? weapon_info->flMaxPlayerSpeedAlt : weapon_info->flMaxPlayerSpeed;

	switch (g_cfg.antiaim.fakelag_type)
	{
	case 0:
		max_choke = g_cfg.antiaim.triggers_fakelag_amount;
		break;
	case 1:
		if (g_cfg.antiaim.fakelag_varience == 0)
			g_cfg.antiaim.fakelag_varience = 15;
		else if (g_cfg.antiaim.fakelag_varience > 100)
			g_cfg.antiaim.fakelag_varience = 100;

		if (m_pcmd->m_command_number % g_cfg.antiaim.fakelag_varience < g_cfg.antiaim.fakelag_amount)
			max_choke = min(g_cfg.antiaim.fakelag_amount, 2);
		else
			max_choke = 1;
		break;
	case 2:
		if (velocity2d >= 5.0f)
		{
			auto dynamic_factor = std::ceilf(64.0f / (velocity2d * m_globals()->m_intervalpertick));

			if (dynamic_factor > 16)
				dynamic_factor = g_cfg.antiaim.triggers_fakelag_amount;

			max_choke = dynamic_factor;
		}
		else
			max_choke = g_cfg.antiaim.triggers_fakelag_amount;
		break;
	case 3:
		max_choke = fluctuate_ticks;
		break;
	case 4:
		auto backup_fakelagenablers = g_cfg.antiaim.fakelag_enablers.size();

		if (g_cfg.antiaim.fakelag_enablers.size() > 0)
			g_cfg.antiaim.fakelag_enablers.resize(0);
		int wish_ticks = 0;
		int AdaptiveTicks = 2;
		int velocity_local_pertick = g_ctx.local()->m_vecVelocity().Length2D() * m_globals()->m_intervalpertick;

		if (((AdaptiveTicks - 1) * velocity_local_pertick) > 68.0)
		{
			++wish_ticks;
			if ((wish_ticks * velocity_local_pertick) > 68.0)
				max_choke = wish_ticks;
		}
		else if ((AdaptiveTicks * velocity_local_pertick) > 68.0)
		{
			int v20 = wish_ticks + 2;
			max_choke = v20;
			max_choke = m_clientstate()->iChokedCommands >= v20;
		}
		else if (((AdaptiveTicks + 1) * velocity_local_pertick) > 68.0)
		{
			int v19 = wish_ticks + 3;
			max_choke = v19;
			max_choke = m_clientstate()->iChokedCommands >= v19;
		}
		else
		{
			if (((AdaptiveTicks + 2) * velocity_local_pertick) <= 68.0)
			{
				AdaptiveTicks += 5;
				wish_ticks += 5;
				if (AdaptiveTicks > 16)
				{
					max_choke = wish_ticks;
					break;
				}
			}
			int v18 = wish_ticks + 4;
			max_choke = m_clientstate()->iChokedCommands >= v18;
		}
		if (g_cfg.antiaim.fakelag_enablers.size() > 0)
			g_cfg.antiaim.fakelag_enablers.resize(backup_fakelagenablers);
		break;
	}

	auto tick_count = g_ctx.globals.current_tickcount + 8 - g_ctx.local()->EntIndex() % 32;

	auto nBaseTick = 100 * (tick_count / 100);
	if (nBaseTick <= 100 * ((tick_count - 1) / 100))
	{
		auto v26 = (m_pcmd->m_tickcount) - g_ctx.local()->EntIndex() % 32;
		if (100 * ((v26 + 1) / 100) <= 100 * (v26 / 100))
		{
			if (100
				* (((g_ctx.globals.current_tickcount + 8)
					- m_clientstate()->iChokedCommands
					- g_ctx.local()->EntIndex() % 32
					+ 4)
					/ 100) <= nBaseTick)
			{
				//
			}
			else
				g_ctx.send_packet = 0;
		}
		else
			g_ctx.send_packet = 1;
	}

	if (m_gamerules()->m_bIsValveDS()) //-V807
		max_choke = m_engine()->IsVoiceRecording() ? 1 : min(max_choke, 6);

	if (misc::get().recharging_double_tap)
		max_choke = g_ctx.globals.weapon->get_max_tickbase_shift();

	if (g_ctx.local()->m_fFlags() & FL_ONGROUND && engineprediction::get().backup_data.flags & FL_ONGROUND && !m_gamerules()->m_bIsValveDS() && key_binds::get().get_key_bind_state(20)) //-V807
	{
		max_choke = 14;

		if (choked < max_choke)
			g_ctx.send_packet = false;
		else
			g_ctx.send_packet = true;
	}
	else
	{
		if (g_cfg.ragebot.enable && g_ctx.globals.current_weapon != -1 && !g_ctx.globals.exploits && g_cfg.antiaim.fakelag && g_cfg.antiaim.fakelag_enablers[FAKELAG_PEEK] && g_cfg.antiaim.triggers_fakelag_amount > 6 && !started_peeking && velocity >= 5.0f)
		{
			auto predicted_eye_pos = g_ctx.globals.eye_pos + engineprediction::get().backup_data.velocity * m_globals()->m_intervalpertick * (float)g_cfg.antiaim.triggers_fakelag_amount * 0.5f;

			for (auto i = 1; i < m_globals()->m_maxclients; i++)
			{
				auto e = static_cast<player_t*>(m_entitylist()->GetClientEntity(i));

				if (!e->valid(true))
					continue;

				auto records = &player_records[i]; //-V826

				if (records->empty())
					continue;

				auto record = &records->front();

				if (!record->valid())
					continue;

				scan_data predicted_data;
				aim::get().scan(record, predicted_data, predicted_eye_pos);

				if (predicted_data.valid())
				{
					scan_data data;
					aim::get().scan(record, data, g_ctx.globals.eye_pos);

					if (!data.valid())
					{
						random_factor = min(rand() % 16 + 1, g_cfg.antiaim.triggers_fakelag_amount);
						switch_ticks = !switch_ticks;
						fluctuate_ticks = switch_ticks ? g_cfg.antiaim.triggers_fakelag_amount : max(g_cfg.antiaim.triggers_fakelag_amount - 2, 1);

						g_ctx.send_packet = true;
						started_peeking = true;

						return;
					}
				}
			}
		}

		if (!g_ctx.globals.exploits && g_cfg.antiaim.fakelag && g_cfg.antiaim.fakelag_enablers[FAKELAG_PEEK] && started_peeking)
		{
			if (choked < max_choke)
				g_ctx.send_packet = false;
			else
			{
				started_peeking = false;

				random_factor = min(rand() % 16 + 1, g_cfg.antiaim.triggers_fakelag_amount);
				switch_ticks = !switch_ticks;
				fluctuate_ticks = switch_ticks ? g_cfg.antiaim.triggers_fakelag_amount : max(g_cfg.antiaim.triggers_fakelag_amount - 2, 1);

				g_ctx.send_packet = true;
			}
		}
		else if (!g_ctx.globals.exploits && g_cfg.antiaim.fakelag && velocity >= 5.0f && g_ctx.globals.slowwalking && g_cfg.antiaim.fakelag_enablers[FAKELAG_SLOW_WALK])
		{
			if (choked < max_choke)
				g_ctx.send_packet = false;
			else
			{
				started_peeking = false;

				random_factor = min(rand() % 16 + 1, g_cfg.antiaim.triggers_fakelag_amount);
				switch_ticks = !switch_ticks;
				fluctuate_ticks = switch_ticks ? g_cfg.antiaim.triggers_fakelag_amount : max(g_cfg.antiaim.triggers_fakelag_amount - 2, 1);

				g_ctx.send_packet = true;
			}
		}
		else if (!g_ctx.globals.exploits && g_cfg.antiaim.fakelag && velocity >= 5.0f && !g_ctx.globals.slowwalking && g_ctx.local()->m_fFlags() & FL_ONGROUND && engineprediction::get().backup_data.flags & FL_ONGROUND && g_cfg.antiaim.fakelag_enablers[FAKELAG_MOVE])
		{
			if (choked < max_choke)
				g_ctx.send_packet = false;
			else
			{
				started_peeking = false;

				random_factor = min(rand() % 16 + 1, g_cfg.antiaim.triggers_fakelag_amount);
				switch_ticks = !switch_ticks;
				fluctuate_ticks = switch_ticks ? g_cfg.antiaim.triggers_fakelag_amount : max(g_cfg.antiaim.triggers_fakelag_amount - 2, 1);

				g_ctx.send_packet = true;
			}
		}
		else if (!g_ctx.globals.exploits && g_cfg.antiaim.fakelag && !g_ctx.globals.slowwalking && !(g_ctx.local()->m_fFlags() & FL_ONGROUND && engineprediction::get().backup_data.flags & FL_ONGROUND) && g_cfg.antiaim.fakelag_enablers[FAKELAG_AIR])
		{
			if (choked < max_choke)
				g_ctx.send_packet = false;
			else
			{
				started_peeking = false;

				random_factor = min(rand() % 16 + 1, g_cfg.antiaim.triggers_fakelag_amount);
				switch_ticks = !switch_ticks;
				fluctuate_ticks = switch_ticks ? g_cfg.antiaim.triggers_fakelag_amount : max(g_cfg.antiaim.triggers_fakelag_amount - 2, 1);

				g_ctx.send_packet = true;
			}
		}
		else if (!g_ctx.globals.exploits && g_cfg.antiaim.fakelag)
		{
			max_choke = g_cfg.antiaim.fakelag_amount;

			if (m_gamerules()->m_bIsValveDS())
				max_choke = min(max_choke, 6);

			if (choked < max_choke)
				g_ctx.send_packet = false;
			else
			{
				started_peeking = false;

				random_factor = min(rand() % 16 + 1, g_cfg.antiaim.fakelag_amount);
				switch_ticks = !switch_ticks;
				fluctuate_ticks = switch_ticks ? g_cfg.antiaim.fakelag_amount : max(g_cfg.antiaim.fakelag_amount - 2, 1);

				g_ctx.send_packet = true;
			}
		}
		else if (g_ctx.globals.exploits || !antiaim::get().condition(m_pcmd, false) && (antiaim::get().type == ANTIAIM_LEGIT || g_cfg.antiaim.desync)) //-V648
		{
			condition = true;
			started_peeking = false;

			if (choked < 1)
				g_ctx.send_packet = false;
			else
				g_ctx.send_packet = true;
		}
		else
			condition = true;
	}
}

void fakelag::Createmove()
{
	if (FakelagCondition(g_ctx.get_command()))
		return;

	Fakelag(g_ctx.get_command());

	if (!m_gamerules()->m_bIsValveDS() && m_clientstate()->iChokedCommands <= 16)
	{
		static auto Fn = util::FindSignature(crypt_str("engine.dll"), crypt_str("B8 ? ? ? ? 3B F0 0F 4F F0 89 5D FC")) + 0x1;
		DWORD old = 0;

		VirtualProtect((void*)Fn, sizeof(uint32_t), PAGE_EXECUTE_READWRITE, &old);
		*(uint32_t*)Fn = 17;
		VirtualProtect((void*)Fn, sizeof(uint32_t), old, &old);
	}
}

bool fakelag::FakelagCondition(CUserCmd* m_pcmd)
{
	condition = false;

	if (g_ctx.local()->m_bGunGameImmunity() || g_ctx.local()->m_fFlags() & FL_FROZEN)
		condition = true;

	if (antiaim::get().freeze_check && !misc::get().double_tap_enabled && !misc::get().hide_shots_enabled)
		condition = true;

	return condition;
}