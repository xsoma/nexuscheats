/*#include "walk_bot.h"
#include <iostream>
#include <Windows.h>
#include <ShlObj.h>
#include <fstream>

#define fs namespace std::filesystem;

void caucolos(Vector src, Vector dst, Vector &angles)
{

	double delta[3] = { (src[0] - dst[0]), (src[1] - dst[1]), (src[2] - dst[2]) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles[0] = (float)(asinf(delta[2] / hyp) * 57.295779513082f);
	angles[1] = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles[2] = 0.0f;

	if (delta[0] >= 0.0)
		angles[1] += 180.0f;
}

std::string get_folder_paths()
{
	std::string folder;

	auto get_dir = [&folder]() -> void
	{
		static TCHAR path[MAX_PATH];

		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path)))
			folder = std::string(path) + ("\\Nexus\\");

		CreateDirectory(folder.c_str(), NULL);
	};

	get_dir();

	return folder;
}

const std::vector<Vector> vSamplingSpaces = { Vector(279.11f,2175.06f,-128.18f),
	Vector(1380.f, 2131.6f, -5.48f), Vector(1398.04f, 928.93f, -8.78f), Vector(841.17f, 950.18f, 0.14f),
	Vector(581.58f, 674.48f, 1.50f) , Vector(690.59f, 345.20f, 1.54f), Vector(446.82f, 70.25f, -3.26f),
	Vector(-5.52f, -829.16f, -3.58f), Vector(-503.56f, -739.92f, 114.16f), Vector(-1913.50f,-735.72f, 121.20f),
	Vector(-1676.94f, 726.70f, 31.00f), Vector(-1655.66f,1053.87f,31.57f), Vector(-1989.64f,1330.22f,28.93f),
	Vector(-1926.35f,2197.58f,-3.22f), Vector(-1331.98f,2231.99f,2.05f), Vector(-1286.65f,2164.06f,4.25f),
	Vector(-606.69f,2370.25f,-105.67f),Vector(-429.61f, 2196.21f, -123.86) };

void walk_bot::WalkBot_painttraverse() { //Insert this in painttraverse.

	if (!m_engine()->IsConnected() && !m_engine()->IsInGame())
		return;

	auto* local = g_ctx.local();

	if (!local)
		return;

	if (local->GetAbsOrigin().x == 0 || local->GetAbsOrigin().y == 0)
		return;

	for (int i = 0; i < vSamplingSpaces.size(); i++) {
		if (math::world_to_screen(vSamplingSpaces[i], vNewLocation[i])) {
			render::get().text(fonts[ESP], vNewLocation[i].x, vNewLocation[i].y, Color(255, 0, 0), 0, std::to_string(i).c_str());
			render::get().Draw3DFilledCircle(vSamplingSpaces[i], 10.f, 20.f, Color(255, 255, 255));
			//m_debugoverlay()->AddSphereOverlay(vSamplingSpaces[i], 2, 2, 2, 255, 255, 255, 255, 10000);
			//gDrawings->DrawString(vNewLocation[i].x, vNewLocation[i].y, Color::Red(), menufont, std::to_string(i).c_str());
		}
	}
}

bool walk_bot::MarksIsVisible(player_t* local, Vector& vTo) {
	Ray_t ray;
	trace_t trace;
	CTraceFilter filter;
	filter.pSkip = local;

	ray.Init(local->GetEyePos(), vTo);
	m_trace()->TraceRay(ray, 0x4600400B, &filter, &trace);
	return (trace.fraction > 0.99f);
}

void MakeVector(Vector pfin, Vector& vector)
{
	float pitch = (float)(pfin.x * M_PI / 180);
	float yaw = (float)(pfin.y * M_PI / 180);
	float tmp = (float)(cos(pitch));

	vector.x = (float)(-tmp * -cos(yaw));
	vector.y = (float)(sin(yaw) * tmp);
	vector.z = (float)(-sin(pitch));
}

float GetFov(Vector& vMyView, Vector& vMyPos, Vector& vHitbox)
{
	Vector ang, aim;

	float fov;
	ang = math::calculate_angle(vMyPos, vHitbox);
	MakeVector(vMyView, aim);
	MakeVector(ang, ang);
	float mag_s = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float mag_d = sqrt((aim[0] * aim[0]) + (aim[1] * aim[1]) + (aim[2] * aim[2]));
	float u_dot_v = aim[0] * ang[0] + aim[1] * ang[1] + aim[2] * ang[2];
	fov = acos(u_dot_v / (mag_s*mag_d)) * (180.0 / M_PI);
	return fov;
}

void walk_bot::MakeFebsMove(CUserCmd* cmd, player_t* local) { // Make him move.
	if (!m_engine()->IsConnected() && !m_engine()->IsInGame())
		return;

	if (!local)
		return;

	if (local->GetAbsOrigin().x == 0 || local->GetAbsOrigin().y == 0)
		return;

	if (!GetAsyncKeyState(0x4E))//N
		return;

	cmd->m_forwardmove = 450;

	for (int i = 0; i < vSamplingSpaces.size(); i++) {
		Vector vNewTo = vNewLocation[i];
		Vector vOldie = vSamplingSpaces[i];
		if(local->GetAbsAngles() == vNewTo)
			caucolos(local->GetEyePos(), vOldie, vNewTo);
		else if(vNewLocation[i] == 0 || vNewLocation[i] == NULL)
			caucolos(local->GetEyePos(), vOldie, vNewTo);

		//gDrawings->DrawString(vNewLocation[i].x, vNewLocation[i].y, Color::Red(), menufont, std::to_string(i).c_str());

		
		if (vNewLocation[i] != 0 || vNewLocation[i] != NULL)
		{
			if (GetFov(cmd->m_viewangles, local->GetEyePos(), vOldie) < 90) {

				cmd->m_viewangles = vNewTo;
				cmd->m_viewangles.x = 0;
				m_engine()->SetViewAngles(cmd->m_viewangles);
			}
		}
	}o kira deixando vazar a area de trabalho inteira dele kkkkkkkkk n tem nd ele ja escondeu
}*/