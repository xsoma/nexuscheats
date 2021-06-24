// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "misc.h"
#include "fakelag.h"
#include "..\ragebot\aim.h"
#include "..\visuals\world_esp.h"
#include "prediction_system.h"
#include "logs.h"

#if RELEASE
#if BETA
#define VERSION crypt_str("Nexus | ")
#else
#define VERSION crypt_str("Nexus | ")
#endif
#else
#define VERSION crypt_str("Nexus | ")
#endif
int pre_flags = 0;

void misc::edgebug(CUserCmd* pCmd) {

	if (g_ctx.local()->get_move_type() == MOVETYPE_LADDER || g_ctx.local()->get_move_type() == MOVETYPE_NOCLIP) return;

	if (engineprediction::get().backup_data.flags & FL_ONGROUND && !(g_ctx.local()->m_fFlags() & FL_ONGROUND))
		pCmd->m_buttons |= IN_DUCK;

}
bool jumpbugged = false;

void misc::jumpbug(CUserCmd* pCmd) {

	auto poop = g_ctx.local()->m_fFlags();
	bool unduck = false;
	static bool niggacheck = false;

	if ((g_ctx.local()->m_fFlags() & (1 << 0)) && !(engineprediction::get().backup_data.flags & (1 << 0))) {

		if (g_cfg.misc.bunnyhop) {
			g_cfg.misc.bunnyhop = false;
			niggacheck = true;
		}

		if (unduck) {
			pCmd->m_buttons &= ~IN_DUCK;
			pCmd->m_buttons |= IN_JUMP;
			unduck = false;
			jumpbugged = true;
		}

		pCmd->m_buttons |= IN_DUCK;
		pCmd->m_buttons &= ~IN_JUMP;
		unduck = true;
		jumpbugged = false;
	}
	else if (niggacheck) {
		g_cfg.misc.bunnyhop = true;
		niggacheck = false;
	}

}

const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}

float frameRate = 0.5f;

std::string GetTimeString()
{
	time_t current_time;
	struct tm* time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}


void misc::break_prediction(CUserCmd* cmd) {

	if (g_cfg.misc.break_prediction) {

		if (cmd->m_forwardmove > 0.0f)
		{
			g_ctx.send_packet = 4.0f;
			cmd->m_buttons |= IN_BACK;
			cmd->m_buttons &= ~IN_FORWARD;
		}
		else if (cmd->m_forwardmove < 0.0f)
		{
			g_ctx.send_packet = 4.0f;
			cmd->m_buttons |= IN_FORWARD;
			cmd->m_buttons &= ~IN_BACK;
		}

		if (cmd->m_sidemove > 0.0f)
		{
			g_ctx.send_packet = 4.0f;
			cmd->m_buttons |= IN_MOVELEFT;
			cmd->m_buttons &= ~IN_MOVERIGHT;
		}
		else if (cmd->m_sidemove < 0.0f)
		{
			g_ctx.send_packet = 4.0f;
			cmd->m_buttons |= IN_MOVERIGHT;
			cmd->m_buttons &= ~IN_MOVELEFT;
		}
	}
}


void misc::watermark()
{
	if (!g_cfg.menu.watermark)
		return;

	int w, h;
	m_engine()->GetScreenSize(w, h); //-V807

	frameRate = 0.9f * frameRate + (1.0 - 0.9f) * m_globals()->m_absoluteframetime;
	std::stringstream ss;

	//static int fps, old_tick_count;

	//if ((m_globals()->m_tickcount - old_tick_count) > 59) {
	//	fps = static_cast<int>(1.f / m_globals()->m_frametime);
		//old_tick_count = m_globals()->m_tickcount;
	//}

	auto local_player = g_ctx.local();
	std::string outgoing = local_player ? std::to_string((int)(m_engine()->GetNetChannelInfo()->GetLatency(FLOW_OUTGOING) * 1000)) : "0";

	if (m_engine()->IsConnected())
		ss << "Nexus" << " | " << static_cast<int>(1.f / frameRate) << " | " << outgoing.c_str() << "ms" << " | " << GetTimeString().c_str() << "";
	else
		ss << "Nexus" << " | " << static_cast<int>(1.f / frameRate) << " | " << GetTimeString().c_str() << "";
	//vtnc rapa\
	//std::string text = "nexuS | v1.3";
	auto size = render::get().text_width(fonts[WATERMARK], ss.str().c_str());
	//me esqueci a regra de 3 q tem q fazer '-'
	static float rgb = 0;
	rgb += 0.0005f;
	if (rgb > 1.f) rgb = 0.f;

	render::get().rect_filled(w - (size + 20), 15, size + 10, 20, Color(29, 28, 32, 255));
	render::get().rect(w - (size + 20), 15, size + 10, 20, Color(56, 54, 59, 0));
	render::get().rect(w - (size + 20) + 1, 15 + 1, size + 10 - 2, 20 - 2, Color(17, 17, 22, 0));
	render::get().rect(w - (size + 20) - 1, 15 - 1, size + 10 + 2, 20 + 2, Color(17, 17, 22, 0));

	//quando tu ativar manda print pra mim pq no any n consigo ver bem

	//render::get().rect_filled(w - (size + 20), 13, size + 10, 2, Color::FromHSB(rgb, 1.f, 1.f));
	render::get().text(fonts[WATERMARK], w - (size + 15), 25, Color(255, 255, 255), HFONT_CENTERED_Y, ss.str().c_str());

	/*if (g_ctx.local()->is_alive())
	{
		render::get().text(fonts[WATERMARK], w / 2, 50, Color(255, 255, 255), 0, std::to_string(strenght).c_str());
	}*/
}

const std::string key_type(key_bind* key)
{
	switch (key->mode)
	{
	case HOLD:
		return "[HOLD]";
		break;
	case TOGGLE:
		return "[TOGGLE]";
		break;
	}
}


bool is_key_on(key_bind* key, int id)
{
	if (key->mode == HOLD)
	{
		if (util::is_button_down(key->key))
			return true;
	}
	else if (key->mode == TOGGLE)
	{
		if (key_binds::get().is_enabled(id)) return true;
	}

	return false;
}

void misc::keybinds()
{
    #define DEFX 300
    #define DEFY 300
	//if (!g_options.visuals_keybinds)
		//return;
	if (!g_cfg.misc.keybinds) return;

	static int x = 0;
	static int y = 0;
	static int _drag_x = 0;
	static int _drag_y = 0;
	static int w = 230;
	static int h = 28;

	static bool _dragging = false;
	bool _click = false;

	static int iScreenWidth, iScreenHeight;
	m_engine()->GetScreenSize(iScreenWidth, iScreenHeight);
	Vector2D MousePos = render::get().GetMousePosition();
	if (GetAsyncKeyState(VK_LBUTTON)) _click = true;

	if (_dragging && !_click)
	{
		_dragging = false;
	}

	if (_dragging && _click)
	{
		x = MousePos.x - _drag_x;
		y = MousePos.y - _drag_y;
	}

	if (render::get().MouseInRegion(x, y, w, 20))
	{
		_dragging = true;
		_drag_x = MousePos.x - x;
		_drag_y = MousePos.y - y;
	}

	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if ((x + w) > iScreenWidth)
		x = iScreenWidth - w;
	if ((y + h) > iScreenHeight)
		y = iScreenHeight - h;

	static bool autopeek = false;

	using namespace std;
	//	static Vector2D toggledSZ = g_render.get_size(g_fonts.other, "toggled");
		//static Vector2D holdingSZ = g_render.get_size(g_fonts.other, "holding ON");
	vector<pair<string, bool>> inds;

	int to_add = 0;
	// header
	render::get().rect_filled(x, y, w, to_add + h, Color(25, 25, 25, 155));
	render::get().rect(x, y, w, to_add + h, Color(56, 54, 59, 0));
	render::get().rect(x + 1, y + 1, w - 2, to_add + h - 2, Color(17, 17, 22, 0));
	render::get().rect(x - 1, y - 1, w + 2, to_add + h + 2, Color(17, 17, 22, 0));

	/*render::get().rect(w - (size + 20), 15, size + 10, 20, Color(56, 54, 59));
	render::get().rect(w - (size + 20) + 1, 15 + 1, size + 10 - 2, 20 - 2, Color(17, 17, 22));
	render::get().rect(w - (size + 20) - 1, 15 - 1, size + 10 + 2, 20 + 2, Color(17, 17, 22));*/

	//vo ter q sair ja volto 30 minuto
	//render::get().text(g_fonts.other, "Keybinds", x + 6, y + 4, WHITED_COL);

	if (g_cfg.misc.clicked_save)
	{
		g_cfg.misc.key_x = x; g_cfg.misc.key_y = y;
		g_cfg.misc.clicked_save = false;
	}

	if (g_cfg.misc.clicked_load)
	{
		x = g_cfg.misc.key_x; y = g_cfg.misc.key_y;
		g_cfg.misc.clicked_load = false;
	}

	auto ss = render::get().text_width(fonts[WATERMARK], "Keybinds");

	render::get().text(fonts[WATERMARK], x + ((w / 2) - (ss / 2)), y + 8, Color::White, NULL, "Keybinds");
	// other
	if (m_engine()->IsInGame())
	{
		if (is_key_on(&g_cfg.ragebot.double_tap_key, 2))
			inds.push_back(pair<string, bool>(string("Doubletap " + key_type(&g_cfg.ragebot.double_tap_key)), 0));

		if (is_key_on(&g_cfg.antiaim.hide_shots_key, 12))
			inds.push_back(pair<string, bool>(string("Hideshots " + key_type(&g_cfg.antiaim.hide_shots_key)), 0));

		if (is_key_on(&g_cfg.antiaim.manual_back, 13))
			inds.push_back(pair<string, bool>(string("MANUAL: Back " + key_type(&g_cfg.antiaim.manual_back)), 0));

		if (is_key_on(&g_cfg.antiaim.manual_left, 14))
			inds.push_back(pair<string, bool>(string("MANUAL: Left " + key_type(&g_cfg.antiaim.manual_left)), 0));

		if (is_key_on(&g_cfg.antiaim.manual_right, 15))
			inds.push_back(pair<string, bool>(string("MANUAL: Right " + key_type(&g_cfg.antiaim.manual_right)), 0));

		if (is_key_on(&g_cfg.legitbot.key, 1))
			inds.push_back(pair<string, bool>(string("Legit Aim " + key_type(&g_cfg.legitbot.key)), 0));

		if (is_key_on(&g_cfg.legitbot.autofire_key, 0))
			inds.push_back(pair<string, bool>(string("Trigger " + key_type(&g_cfg.legitbot.autofire_key)), 0));

		if (is_key_on(&g_cfg.antiaim.flip_desync, 16))
			inds.push_back(pair<string, bool>(string("Inverter " + key_type(&g_cfg.antiaim.flip_desync)), 0));

		if (is_key_on(&g_cfg.misc.thirdperson_toggle, 17))
			inds.push_back(pair<string, bool>(string("Thirdperson " + key_type(&g_cfg.misc.thirdperson_toggle)), 0));

		if (is_key_on(&g_cfg.misc.automatic_peek, 18))
			inds.push_back(pair<string, bool>(string("Auto Peek " + key_type(&g_cfg.misc.automatic_peek)), 0));

		if (is_key_on(&g_cfg.misc.fakeduck_key, 20))
			inds.push_back(pair<string, bool>(string("Fake Duck " + key_type(&g_cfg.misc.fakeduck_key)), 0));

		if (is_key_on(&g_cfg.misc.slowwalk_key, 21))
			inds.push_back(pair<string, bool>(string("Slow Walk " + key_type(&g_cfg.misc.slowwalk_key)), 0));

		if (is_key_on(&g_cfg.ragebot.body_aim_key, 22))
			inds.push_back(pair<string, bool>(string("Baim " + key_type(&g_cfg.ragebot.body_aim_key)), 0));

		for (int i = 0; i < 8; i++)
		{
			if (is_key_on(&g_cfg.ragebot.weapon[i].damage_override_key, 4 + i))
				inds.push_back(pair<string, bool>(string("Damage override " + key_type(&g_cfg.ragebot.weapon[i].damage_override_key)), 0));
		}
		


		//int to_add = 0;
		for (auto i : inds) {
			//render::get().rect_filled(x, y + 28 + to_add, w, h - 4, Color(0, 0, 0, 200));
			render::get().text(fonts[WATERMARK], x + 6, y + 28 + to_add + 6, Color::White, NULL, i.first.c_str());
			//g += 24;
			//g_render.text(g_fonts.other, i.second ? "holding ON" : "toggled", x + w - (i.second ? holdingSZ.x : toggledSZ.x) - 4, y + 28 + to_add + 4, WHITE_COL);
			to_add += 24;
		}
		//Render::Get().DrawFilleddRect(x, y + 28 + to_add, w, 14, LIGHT_COL);
	}
	//g_Options.KeyBindY = y;
}

void misc::NoDuck(CUserCmd* cmd)
{
	if (!g_cfg.misc.noduck)
		return;

	if (m_gamerules()->m_bIsValveDS())
		return;

	cmd->m_buttons |= IN_BULLRUSH;
}

void misc::ChangeRegion()
{
	switch (g_cfg.misc.region_changer) {
	case 0:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster waw");
		break;
	case 1:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster atl");
		break;
	case 2:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster bom");
		break;
	case 3:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster can");
		break;
	case 4:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster canm");
		break;
	case 5:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster cant");
		break;
	case 6:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster canu");
		break;
	case 7:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster dxb");
		break;
	case 8:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster eat");
		break;
	case 9:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster fra");
		break;
	case 10:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster gru");
		break;
	case 11:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster hkg");
		break;
	case 12:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster iad");
		break;
	case 13:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster jnb");
		break;
	case 14:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster lax");
		break;
	case 15:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster lhr");
		break;
	case 16:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster lim");
		break;
	case 17:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster lux");
		break;
	case 18:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster maa");
		break;
	case 19:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster mad");
		break;
	case 20:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster man");
		break;
	case 21:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster okc");
		break;
	case 22:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster ord");
		break;
	case 23:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster par");
		break;
	case 24:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster pwg");
		break;
	case 25:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster pwj");
		break;
	case 26:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster pwu");
		break;
	case 27:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster pww");
		break;
	case 28:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster pwz");
		break;
	case 29:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster scl");
		break;
	case 30:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster sea");
		break;
	case 31:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster sgp");
		break;
	case 32:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster sha");
		break;
	case 33:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster sham");
		break;
	case 34:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster shat");
		break;
	case 35:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster shau");
		break;
	case 36:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster shb");
		break;
	case 37:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster sto");
		break;
	case 38:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster sto2");
		break;
	case 39:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster syd");
		break;
	case 40:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster tsn");
		break;
	case 41:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster tsnm");
		break;
	case 42:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster tsnt");
		break;
	case 43:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster tsnu");
		break;
	case 44:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster tyo");
		break;
	case 45:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster tyo1");
		break;
	case 46:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster vie");
		break;
	case 47:
		m_engine()->ExecuteClientCmd("sdr SDRClient_ForceRelayCluster ams");
		break;
	}
}

void misc::ChatSpamer()
{
	if (!g_cfg.misc.chat)
		return;

	static std::string chatspam[] = 
	{ 
		crypt_str("Nexus.online - stay with us or lose the game."),
		crypt_str("Get good. Get Nexus"),
		crypt_str("Nexus.online - just the best."),
		crypt_str("Go to another level with Nexus.online."),
	};

	static auto lastspammed = 0;

	if (GetTickCount() - lastspammed > 800)
	{
		lastspammed = GetTickCount();

		srand(m_globals()->m_tickcount);
		std::string msg = crypt_str("say ") + chatspam[rand() % 4];

		m_engine()->ExecuteClientCmd(msg.c_str());
	}
}

void misc::AutoCrouch(CUserCmd* cmd)
{
	if (fakelag::get().condition)
	{
		g_ctx.globals.fakeducking = false;
		return;
	}

	if (!(g_ctx.local()->m_fFlags() & FL_ONGROUND && engineprediction::get().backup_data.flags & FL_ONGROUND))
	{
		g_ctx.globals.fakeducking = false;
		return;
	}

	if (m_gamerules()->m_bIsValveDS())
	{
		g_ctx.globals.fakeducking = false;
		return;
	}

	if (!key_binds::get().get_key_bind_state(20))
	{
		g_ctx.globals.fakeducking = false;
		return;
	}

	if (!g_ctx.globals.fakeducking && m_clientstate()->iChokedCommands != 7)
		return;

	if (m_clientstate()->iChokedCommands >= 7)
		cmd->m_buttons |= IN_DUCK;
	else
		cmd->m_buttons &= ~IN_DUCK;

	g_ctx.globals.fakeducking = true;
}

void misc::SlideWalk(CUserCmd* cmd)
{
	if (!g_ctx.local()->is_alive()) //-V807
		return;

	if (g_ctx.local()->get_move_type() == MOVETYPE_LADDER)
		return;

	if (!(g_ctx.local()->m_fFlags() & FL_ONGROUND && engineprediction::get().backup_data.flags & FL_ONGROUND))
		return;

	if (antiaim::get().condition(cmd, true) && g_cfg.misc.slidewalk)
	{
		if (cmd->m_forwardmove > 0.0f)
		{
			cmd->m_buttons |= IN_BACK;
			cmd->m_buttons &= ~IN_FORWARD;
		}
		else if (cmd->m_forwardmove < 0.0f)
		{
			cmd->m_buttons |= IN_FORWARD;
			cmd->m_buttons &= ~IN_BACK;
		}

		if (cmd->m_sidemove > 0.0f)
		{
			cmd->m_buttons |= IN_MOVELEFT;
			cmd->m_buttons &= ~IN_MOVERIGHT;
		}
		else if (cmd->m_sidemove < 0.0f)
		{
			cmd->m_buttons |= IN_MOVERIGHT;
			cmd->m_buttons &= ~IN_MOVELEFT;
		}
	}
	else
	{
		auto buttons = cmd->m_buttons & ~(IN_MOVERIGHT | IN_MOVELEFT | IN_BACK | IN_FORWARD);

		if (g_cfg.misc.slidewalk)
		{
			if (cmd->m_forwardmove <= 0.0f)
				buttons |= IN_BACK;
			else
				buttons |= IN_FORWARD;

			if (cmd->m_sidemove > 0.0f)
				goto LABEL_15;
			else if (cmd->m_sidemove >= 0.0f)
				goto LABEL_18;

			goto LABEL_17;
		}
		else
			goto LABEL_18;

		if (cmd->m_forwardmove <= 0.0f) //-V779
			buttons |= IN_FORWARD;
		else
			buttons |= IN_BACK;

		if (cmd->m_sidemove > 0.0f)
		{
		LABEL_17:
			buttons |= IN_MOVELEFT;
			goto LABEL_18;
		}

		if (cmd->m_sidemove < 0.0f)
			LABEL_15:

		buttons |= IN_MOVERIGHT;

	LABEL_18:
		cmd->m_buttons = buttons;
	}
}

void misc::automatic_peek(CUserCmd* cmd, float wish_yaw)
{
	if (!g_ctx.globals.weapon->is_non_aim() && key_binds::get().get_key_bind_state(18))
	{
		if (g_ctx.globals.start_position.IsZero())
		{
			g_ctx.globals.start_position = g_ctx.local()->GetAbsOrigin();

			if (!(engineprediction::get().backup_data.flags & FL_ONGROUND))
			{
				Ray_t ray;
				CTraceFilterWorldAndPropsOnly filter;
				CGameTrace trace;

				ray.Init(g_ctx.globals.start_position, g_ctx.globals.start_position - Vector(0.0f, 0.0f, 1000.0f));
				m_trace()->TraceRay(ray, MASK_SOLID, &filter, &trace);

				if (trace.fraction < 1.0f)
					g_ctx.globals.start_position = trace.endpos + Vector(0.0f, 0.0f, 2.0f);
			}
		}
		else
		{
			auto revolver_shoot = g_ctx.globals.weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER && !g_ctx.globals.revolver_working && (cmd->m_buttons & IN_ATTACK || cmd->m_buttons & IN_ATTACK2);

			if (cmd->m_buttons & IN_ATTACK && g_ctx.globals.weapon->m_iItemDefinitionIndex() != WEAPON_REVOLVER || revolver_shoot)
				g_ctx.globals.fired_shot = true;

			if (g_ctx.globals.fired_shot)
			{
				auto current_position = g_ctx.local()->GetAbsOrigin();
				auto difference = current_position - g_ctx.globals.start_position;

				if (difference.Length2D() > 5.0f)
				{
					auto velocity = Vector(difference.x * cos(wish_yaw / 180.0f * M_PI) + difference.y * sin(wish_yaw / 180.0f * M_PI), difference.y * cos(wish_yaw / 180.0f * M_PI) - difference.x * sin(wish_yaw / 180.0f * M_PI), difference.z);

					cmd->m_forwardmove = -velocity.x * 20.0f;
					cmd->m_sidemove = velocity.y * 20.0f;
				}
				else
				{
					g_ctx.globals.fired_shot = false;
					g_ctx.globals.start_position.Zero();
				}
			}
		}
	}
	else
	{
		g_ctx.globals.fired_shot = false;
		g_ctx.globals.start_position.Zero();
	}
}
void misc::ViewModel()
{
	if (g_cfg.esp.viewmodel_fov)
	{
		auto viewFOV = (float)g_cfg.esp.viewmodel_fov + 68.0f;
		static auto viewFOVcvar = m_cvar()->FindVar(crypt_str("viewmodel_fov"));

		if (viewFOVcvar->GetFloat() != viewFOV) //-V550
		{
			*(float*)((DWORD)&viewFOVcvar->m_fnChangeCallbacks + 0xC) = 0.0f;
			viewFOVcvar->SetValue(viewFOV);
		}
	}
	
	if (g_cfg.esp.viewmodel_x)
	{
		auto viewX = (float)g_cfg.esp.viewmodel_x / 2.0f;
		static auto viewXcvar = m_cvar()->FindVar(crypt_str("viewmodel_offset_x")); //-V807

		if (viewXcvar->GetFloat() != viewX) //-V550
		{
			*(float*)((DWORD)&viewXcvar->m_fnChangeCallbacks + 0xC) = 0.0f;
			viewXcvar->SetValue(viewX);
		}
	}

	if (g_cfg.esp.viewmodel_y)
	{
		auto viewY = (float)g_cfg.esp.viewmodel_y / 2.0f;
		static auto viewYcvar = m_cvar()->FindVar(crypt_str("viewmodel_offset_y"));

		if (viewYcvar->GetFloat() != viewY) //-V550
		{
			*(float*)((DWORD)&viewYcvar->m_fnChangeCallbacks + 0xC) = 0.0f;
			viewYcvar->SetValue(viewY);
		}
	}

	if (g_cfg.esp.viewmodel_z)
	{
		auto viewZ = (float)g_cfg.esp.viewmodel_z / 2.0f;
		static auto viewZcvar = m_cvar()->FindVar(crypt_str("viewmodel_offset_z"));

		if (viewZcvar->GetFloat() != viewZ) //-V550
		{
			*(float*)((DWORD)&viewZcvar->m_fnChangeCallbacks + 0xC) = 0.0f;
			viewZcvar->SetValue(viewZ);
		}
	}
}

void misc::FullBright()
{		
	if (!g_cfg.player.enable)
		return;

	static auto mat_fullbright = m_cvar()->FindVar(crypt_str("mat_fullbright"));

	if (mat_fullbright->GetBool() != g_cfg.esp.bright)
		mat_fullbright->SetValue(g_cfg.esp.bright);
}

void misc::PovArrows(player_t* e, Color color)
{
	auto isOnScreen = [](Vector origin, Vector& screen) -> bool
	{
		if (!math::world_to_screen(origin, screen))
			return false;

		static int iScreenWidth, iScreenHeight;
		m_engine()->GetScreenSize(iScreenWidth, iScreenHeight);

		auto xOk = iScreenWidth > screen.x;
		auto yOk = iScreenHeight > screen.y;

		return xOk && yOk;
	};

	Vector screenPos;

	if (isOnScreen(e->GetAbsOrigin(), screenPos))
		return;

	Vector viewangles;
	m_engine()->GetViewAngles(viewangles);

	static int wid, heigth;
	m_engine()->GetScreenSize(wid, heigth);

	auto angleYawRad = viewangles.y - math::calculate_angle(g_ctx.globals.eye_pos, e->GetAbsOrigin()).y - 90;

	float width = 10.f;

	render::get().arc(wid / 2, heigth / 2, /*250*/ 106, angleYawRad - width, angleYawRad + width, Color(255, 1, 1), 4.f);
	render::get().arc(wid / 2, heigth / 2, /*256*/ 100, angleYawRad - width, angleYawRad + width, Color(255, 1, 1), 1.5f);
}
 


void misc::zeus_range()
{
	if (!g_cfg.player.enable)
		return;

	if (!g_cfg.esp.taser_range)
		return;

	if (!g_ctx.local()->is_alive())
		return;

	auto weapon = g_ctx.local()->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (weapon->m_iItemDefinitionIndex() != WEAPON_TASER)
		return;

	if (!m_input()->m_fCameraInThirdPerson)
		return;

	render::get().Draw3DRainbowCircle(g_ctx.globals.eye_pos, 90.0f);
}

void misc::NightmodeFix()
{
	static auto in_game = false;

	if (m_engine()->IsInGame() && !in_game)
	{
		in_game = true;

		g_ctx.globals.change_materials = true;
		worldesp::get().changed = true;

		static auto skybox = m_cvar()->FindVar(crypt_str("sv_skyname"));
		worldesp::get().backup_skybox = skybox->GetString();
		return;
	}
	else if (!m_engine()->IsInGame() && in_game)
		in_game = false;

	static auto player_enable = g_cfg.player.enable;

	if (player_enable != g_cfg.player.enable)
	{
		player_enable = g_cfg.player.enable;
		g_ctx.globals.change_materials = true;
		return;
	}

	static auto setting = g_cfg.esp.nightmode;

	if (setting != g_cfg.esp.nightmode)
	{
		setting = g_cfg.esp.nightmode;
		g_ctx.globals.change_materials = true;
		return;
	}

	static auto setting_world = g_cfg.esp.world_color;

	if (setting_world != g_cfg.esp.world_color)
	{
		setting_world = g_cfg.esp.world_color;
		g_ctx.globals.change_materials = true;
		return;
	}

	static auto setting_props = g_cfg.esp.props_color;

	if (setting_props != g_cfg.esp.props_color)
	{
		setting_props = g_cfg.esp.props_color;
		g_ctx.globals.change_materials = true;
	}
}

void misc::desync_arrows()
{
	if (!g_ctx.local()->is_alive())
		return;

	if (!g_cfg.ragebot.enable)
		return;

	if (!g_cfg.antiaim.enable)
		return;

	if ((g_cfg.antiaim.manual_back.key <= KEY_NONE || g_cfg.antiaim.manual_back.key >= KEY_MAX) && (g_cfg.antiaim.manual_left.key <= KEY_NONE || g_cfg.antiaim.manual_left.key >= KEY_MAX) && (g_cfg.antiaim.manual_right.key <= KEY_NONE || g_cfg.antiaim.manual_right.key >= KEY_MAX))
		antiaim::get().manual_side = SIDE_NONE;

	if (!g_cfg.antiaim.flip_indicator)
		return;

	static int width, height;
	m_engine()->GetScreenSize(width, height);

	static auto alpha = 1.0f;
	static auto switch_alpha = false;

	if (alpha <= 0.0f || alpha >= 1.0f)
		switch_alpha = !switch_alpha;

	alpha += switch_alpha ? 2.0f * m_globals()->m_frametime : -2.0f * m_globals()->m_frametime;
	alpha = math::clamp(alpha, 0.0f, 1.0f);

	auto color = g_cfg.antiaim.flip_indicator_color;
	color.SetAlpha((int)(min(255.0f * alpha, color.a())));

	if (antiaim::get().manual_side == SIDE_BACK)
		render::get().triangle(Vector2D(width / 2, height / 2 + 80), Vector2D(width / 2 - 10, height / 2 + 60), Vector2D(width / 2 + 10, height / 2 + 60), color);
	else if (antiaim::get().manual_side == SIDE_LEFT)
		render::get().triangle(Vector2D(width / 2 - 55, height / 2 + 10), Vector2D(width / 2 - 75, height / 2), Vector2D(width / 2 - 55, height / 2 - 10), color);
	else if (antiaim::get().manual_side == SIDE_RIGHT)
		render::get().triangle(Vector2D(width / 2 + 55, height / 2 - 10), Vector2D(width / 2 + 75, height / 2), Vector2D(width / 2 + 55, height / 2 + 10), color);
}

void misc::aimbot_hitboxes()
{
	if (!g_cfg.player.enable)
		return;

	if (!g_cfg.player.lag_hitbox)
		return;

	auto player = (player_t*)m_entitylist()->GetClientEntity(aim::get().last_target_index);

	if (!player)
		return;

	auto model = player->GetModel(); //-V807

	if (!model)
		return;

	auto studio_model = m_modelinfo()->GetStudioModel(model);

	if (!studio_model)
		return;
	
	auto hitbox_set = studio_model->pHitboxSet(player->m_nHitboxSet());

	if (!hitbox_set)
		return;

	for (auto i = 0; i < hitbox_set->numhitboxes; i++)
	{
		auto hitbox = hitbox_set->pHitbox(i);

		if (!hitbox)
			continue;

		if (hitbox->radius == -1.0f) //-V550
			continue;

		auto min = ZERO;
		auto max = ZERO;

		math::vector_transform(hitbox->bbmin, aim::get().last_target[aim::get().last_target_index].record.matrixes_data.main[hitbox->bone], min);
		math::vector_transform(hitbox->bbmax, aim::get().last_target[aim::get().last_target_index].record.matrixes_data.main[hitbox->bone], max);

		m_debugoverlay()->AddCapsuleOverlay(min, max, hitbox->radius, g_cfg.player.lag_hitbox_color.r(), g_cfg.player.lag_hitbox_color.g(), g_cfg.player.lag_hitbox_color.b(), g_cfg.player.lag_hitbox_color.a(), 4.0f, 0, 1);
	}
}

void misc::ragdolls()
{
	if (!g_cfg.misc.ragdolls)
		return;

	for (auto i = 1; i <= m_entitylist()->GetHighestEntityIndex(); ++i)
	{
		auto e = static_cast<entity_t*>(m_entitylist()->GetClientEntity(i));

		if (!e)
			continue;

		if (e->IsDormant())
			continue;

		auto client_class = e->GetClientClass();

		if (!client_class)
			continue;

		if (client_class->m_ClassID != CCSRagdoll)
			continue;

		auto ragdoll = (ragdoll_t*)e;
		ragdoll->m_vecForce().z = 800000.0f;
	}
}

void misc::rank_reveal()
{
	if (!g_cfg.misc.rank_reveal)
		return;

	using RankReveal_t = bool(__cdecl*)(int*);
	static auto Fn = (RankReveal_t)(util::FindSignature(crypt_str("client.dll"), crypt_str("55 8B EC 51 A1 ? ? ? ? 85 C0 75 37")));

	int array[3] = 
	{
		0,
		0,
		0
	};

	Fn(array);
}

void misc::fast_stop(CUserCmd* m_pcmd)
{
	if (!g_cfg.misc.fast_stop)
		return;

	if (!(g_ctx.local()->m_fFlags() & FL_ONGROUND && engineprediction::get().backup_data.flags & FL_ONGROUND))
		return;

	auto pressed_move_key = m_pcmd->m_buttons & IN_FORWARD || m_pcmd->m_buttons & IN_MOVELEFT || m_pcmd->m_buttons & IN_BACK || m_pcmd->m_buttons & IN_MOVERIGHT || m_pcmd->m_buttons & IN_JUMP;

	if (pressed_move_key)
		return;

	if (!((antiaim::get().type == ANTIAIM_LEGIT ? g_cfg.antiaim.desync : g_cfg.antiaim.type[antiaim::get().type].desync) && (antiaim::get().type == ANTIAIM_LEGIT ? !g_cfg.antiaim.legit_lby_type : !g_cfg.antiaim.lby_type) && (!g_ctx.globals.weapon->is_grenade() || g_cfg.esp.on_click & !(m_pcmd->m_buttons & IN_ATTACK) && !(m_pcmd->m_buttons & IN_ATTACK2))) || antiaim::get().condition(m_pcmd)) //-V648
	{
		auto velocity = g_ctx.local()->m_vecVelocity();

		if (velocity.Length2D() > 20.0f)
		{
			Vector direction;
			Vector real_view;

			math::vector_angles(velocity, direction);
			m_engine()->GetViewAngles(real_view);

			direction.y = real_view.y - direction.y;

			Vector forward;
			math::angle_vectors(direction, forward);

			static auto cl_forwardspeed = m_cvar()->FindVar(crypt_str("cl_forwardspeed"));
			static auto cl_sidespeed = m_cvar()->FindVar(crypt_str("cl_sidespeed"));

			auto negative_forward_speed = -cl_forwardspeed->GetFloat();
			auto negative_side_speed = -cl_sidespeed->GetFloat();

			auto negative_forward_direction = forward * negative_forward_speed;
			auto negative_side_direction = forward * negative_side_speed;

			m_pcmd->m_forwardmove = negative_forward_direction.x;
			m_pcmd->m_sidemove = negative_side_direction.y;
		}
	}
	else
	{
		auto velocity = g_ctx.local()->m_vecVelocity();

		if (velocity.Length2D() > 20.0f)
		{
			Vector direction;
			Vector real_view;

			math::vector_angles(velocity, direction);
			m_engine()->GetViewAngles(real_view);

			direction.y = real_view.y - direction.y;

			Vector forward;
			math::angle_vectors(direction, forward);

			static auto cl_forwardspeed = m_cvar()->FindVar(crypt_str("cl_forwardspeed"));
			static auto cl_sidespeed = m_cvar()->FindVar(crypt_str("cl_sidespeed"));

			auto negative_forward_speed = -cl_forwardspeed->GetFloat();
			auto negative_side_speed = -cl_sidespeed->GetFloat();

			auto negative_forward_direction = forward * negative_forward_speed;
			auto negative_side_direction = forward * negative_side_speed;

			m_pcmd->m_forwardmove = negative_forward_direction.x;
			m_pcmd->m_sidemove = negative_side_direction.y;
		}
		else
		{
			auto speed = 1.01f;

			if (m_pcmd->m_buttons & IN_DUCK || g_ctx.globals.fakeducking)
				speed *= 2.94117647f;

			static auto switch_move = false;

			if (switch_move)
				m_pcmd->m_sidemove += speed;
			else
				m_pcmd->m_sidemove -= speed;

			switch_move = !switch_move;
		}
	}
}

void misc::spectators_list()
{
	if (!g_cfg.misc.spectators_list)
		return;

	if (!g_ctx.local()->is_alive())
		return;

	std::vector <std::string> spectators;

	for (int i = 1; i < m_globals()->m_maxclients; i++)
	{
		auto e = static_cast<player_t *>(m_entitylist()->GetClientEntity(i));

		if (!e)
			continue;

		if (e->is_alive())
			continue;

		if (e->IsDormant())
			continue;

		if (e->m_hObserverTarget().Get() != g_ctx.local())
			continue;

		player_info_t player_info;
		m_engine()->GetPlayerInfo(i, &player_info);

		spectators.push_back(player_info.szName);
	}

	for (auto i = 0; i < spectators.size(); i++)
	{
		int width, heigth;
		m_engine()->GetScreenSize(width, heigth);

		auto x = render::get().text_width(fonts[LOGS], spectators.at(i).c_str()) + 6; //-V106
		auto y = i * 16;

		render::get().text(fonts[LOGS], width - x, g_cfg.menu.watermark ? y + 30 : y + 6, Color::White, HFONT_CENTERED_NONE, spectators.at(i).c_str()); //-V106
	}
}



bool misc::double_tap(CUserCmd* m_pcmd)
{
	double_tap_enabled = true;

	static auto recharge_double_tap = false;
	static auto last_double_tap = 0;

	if (recharge_double_tap)
	{
		recharge_double_tap = false;
		recharging_double_tap = true;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}
	
	if (recharging_double_tap)
	{
		auto recharge_time = g_ctx.globals.weapon->can_double_tap() ? TIME_TO_TICKS(0.30f) : /*TIME_TO_TICKS(1.5f)*/ TIME_TO_TICKS(1.0f);

		if (!aim::get().should_stop && fabs(g_ctx.globals.fixed_tickbase - last_double_tap) > recharge_time)
		{
			last_double_tap = 0;

			recharging_double_tap = false;
			double_tap_key = true;
		}
		else if (m_pcmd->m_buttons & IN_ATTACK)
			last_double_tap = g_ctx.globals.fixed_tickbase;
	}

	if (!g_cfg.ragebot.enable)
	{
		double_tap_enabled = false;
		double_tap_key = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (!g_cfg.ragebot.double_tap)
	{
		double_tap_enabled = false;
		double_tap_key = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (g_cfg.ragebot.double_tap_key.key <= KEY_NONE || g_cfg.ragebot.double_tap_key.key >= KEY_MAX)
	{
		double_tap_enabled = false;
		double_tap_key = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (double_tap_key && g_cfg.ragebot.double_tap_key.key != g_cfg.antiaim.hide_shots_key.key)
		hide_shots_key = false;

	if (!double_tap_key)
	{
		double_tap_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (g_ctx.local()->m_bGunGameImmunity() || g_ctx.local()->m_fFlags() & FL_FROZEN) //-V807
	{
		double_tap_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (m_gamerules()->m_bIsValveDS())
	{
		double_tap_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (g_ctx.globals.fakeducking)
	{
		double_tap_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return false;
	}

	if (antiaim::get().freeze_check)
		return true;

	auto max_tickbase_shift = g_ctx.globals.weapon->get_max_tickbase_shift();

	if (!g_ctx.globals.weapon->is_grenade() && g_ctx.globals.weapon->m_iItemDefinitionIndex() != WEAPON_TASER && g_ctx.globals.weapon->m_iItemDefinitionIndex() != WEAPON_REVOLVER && g_ctx.send_packet && (m_pcmd->m_buttons & IN_ATTACK || m_pcmd->m_buttons & IN_ATTACK2 && g_ctx.globals.weapon->is_knife())) //-V648
	{
		auto next_command_number = m_pcmd->m_command_number + 1;
		auto user_cmd = m_input()->GetUserCmd(next_command_number);

		memcpy(user_cmd, m_pcmd, sizeof(CUserCmd)); //-V598
		user_cmd->m_command_number = next_command_number;

		util::copy_command(user_cmd, max_tickbase_shift);

		if (g_ctx.globals.aimbot_working)
		{
			g_ctx.globals.double_tap_aim = true;
			g_ctx.globals.double_tap_aim_check = true;
		}

		recharge_double_tap = true;
		double_tap_enabled = false;
		double_tap_key = false;

		last_double_tap = g_ctx.globals.fixed_tickbase;
	}
	else if (!g_ctx.globals.weapon->is_grenade() && g_ctx.globals.weapon->m_iItemDefinitionIndex() != WEAPON_TASER && g_ctx.globals.weapon->m_iItemDefinitionIndex() != WEAPON_REVOLVER)
		g_ctx.globals.tickbase_shift = max_tickbase_shift;

	return true;
}

void misc::hide_shots(CUserCmd* m_pcmd, bool should_work)
{
	hide_shots_enabled = true;

	if (!g_cfg.ragebot.enable)
	{
		hide_shots_enabled = false;
		hide_shots_key = false;

		if (should_work)
		{
			g_ctx.globals.ticks_allowed = 0;
			g_ctx.globals.next_tickbase_shift = 0;
		}

		return;
	}

	if (!g_cfg.antiaim.hide_shots)
	{
		hide_shots_enabled = false;
		hide_shots_key = false;

		if (should_work)
		{
			g_ctx.globals.ticks_allowed = 0;
			g_ctx.globals.next_tickbase_shift = 0;
		}

		return;
	}

	if (g_cfg.antiaim.hide_shots_key.key <= KEY_NONE || g_cfg.antiaim.hide_shots_key.key >= KEY_MAX)
	{
		hide_shots_enabled = false;
		hide_shots_key = false;

		if (should_work)
		{
			g_ctx.globals.ticks_allowed = 0;
			g_ctx.globals.next_tickbase_shift = 0;
		}

		return;
	}

	if (!should_work && double_tap_key)
	{
		hide_shots_enabled = false;
		hide_shots_key = false;
		return;
	}

	if (!hide_shots_key)
	{
		hide_shots_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return;
	}

	double_tap_key = false;

	if (g_ctx.local()->m_bGunGameImmunity() || g_ctx.local()->m_fFlags() & FL_FROZEN)
	{
		hide_shots_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return;
	}

	if (g_ctx.globals.fakeducking)
	{
		hide_shots_enabled = false;
		g_ctx.globals.ticks_allowed = 0;
		g_ctx.globals.next_tickbase_shift = 0;
		return;
	}

	if (antiaim::get().freeze_check)
		return;

	g_ctx.globals.next_tickbase_shift = m_gamerules()->m_bIsValveDS() ? 6 : 9;

	auto revolver_shoot = g_ctx.globals.weapon->m_iItemDefinitionIndex() == WEAPON_REVOLVER && !g_ctx.globals.revolver_working && (m_pcmd->m_buttons & IN_ATTACK || m_pcmd->m_buttons & IN_ATTACK2);
	auto weapon_shoot = m_pcmd->m_buttons & IN_ATTACK && g_ctx.globals.weapon->m_iItemDefinitionIndex() != WEAPON_REVOLVER || m_pcmd->m_buttons & IN_ATTACK2 && g_ctx.globals.weapon->is_knife() || revolver_shoot;

	if (g_ctx.send_packet && !g_ctx.globals.weapon->is_grenade() && weapon_shoot)
		g_ctx.globals.tickbase_shift = g_ctx.globals.next_tickbase_shift;
}