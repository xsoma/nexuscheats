#pragma once

#include "..\includes.hpp"
#include "singleton.h"
#include "../cheats/menu.h"

#define DEG2RADD(x) ((float)(x) * (float)((float)(M_PI) / 180.0f))

enum FontCenteringFlags 
{
	HFONT_CENTERED_NONE = (1 << 0),
	HFONT_CENTERED_X = (1 << 1),
	HFONT_CENTERED_Y = (1 << 2)
};

enum GradientType 
{
	GRADIENT_HORIZONTAL,
	GRADIENT_VERTICAL
};

enum FONT_INDEX 
{
	LOGS,
	ESP,
	Nexus,
	NAME,
	SUBTABWEAPONS,
	KNIFES,
	GRENADES,
	INDICATORFONT,
	DAMAGE_MARKER,
	TAHOMA_NORMAL,
	VELOCITY,
	WATERMARK,
	FONT_MAX,
};

extern vgui::HFont fonts[FONT_MAX];

template<typename T>
class singleton;

struct CUSTOMVERTEX2 {
	FLOAT x, y, z;
	FLOAT rhw;
	DWORD color;
};

class render : public singleton< render > {
	friend class singleton<render>;
private:
	//ImDrawList* draw_list;
	//ImDrawData draw_data;
	float alpha_factor = 1.0f;
	LPDIRECT3DDEVICE9 device; //-V122
	D3DVIEWPORT9      m_viewport;
public:
	bool initalized = false;

	

	float get_alphafactor();
	void set_alphafactor(float factor);
	void rect(int x, int y, int w, int h, Color color);
	void arc(float x, float y, float radius, float min_angle, float max_angle, Color col, float thickness);
	void rect_filled(int x, int y, int w, int h, Color color);
	void setup_states() const;
	void invalidate_objects();
	void filled_rect_world(Vector center, Vector2D size, Color color, int angle);
	void restore_objects(LPDIRECT3DDEVICE9 m_device);
	void gradient(int x, int y, int w, int h, Color first, Color second, GradientType type);
	void rounded_box(int x, int y, int w, int h, int points, int radius, Color color);
	void grid(int x, int y, int w, int h, Color first, Color second, Color third, GradientType type);
	void circle(int x, int y, int points, int radius, Color color);

	Vector2D GetMousePosition() // bolbi ware
	{
		POINT mousePosition;
		GetCursorPos(&mousePosition);
		ScreenToClient(FindWindow(0, "Counter-Strike: Global Offensive"), &mousePosition);
		return { static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y) };
	}

	inline bool MouseInRegion(int x, int y, int x2, int y2) {
		if (GetMousePosition().x > x && GetMousePosition().y > y && GetMousePosition().x < x2 + x && GetMousePosition().y < y2 + y)
			return true;
		return false;
	}

	void circle_filled(int x, int y, int points, int radius, Color color);
	void triangle(Vector2D point_one, Vector2D point_two, Vector2D point_three, Color color);

	template <class T>
	inline void ARC(T x, T y, float radius, float min_angle, float max_angle, Color col, float thickness)
	{
		//c_menu::get().draw_list->PathArcTo(ImVec2(x, y), radius, DEG2RADD(min_angle), DEG2RADD(max_angle), 16);
		//c_menu::get().draw_list->PathStroke(col.u32(), false, thickness);
	}

	void line(int x, int y, int x2, int y2, Color color);
	void text(vgui::HFont font, int x, int y, Color color, DWORD flags, const char* msg, ...);
	void wtext(vgui::HFont font, int x, int y, Color color, DWORD flags, wchar_t* msg);
	int text_width(vgui::HFont font, const char* msg, ...);
	int text_height(vgui::HFont font, const char* msg, ...);
	void DrawFilled3DBox(const Vector& origin, int width, int height, Color outline, Color filling);
	void Draw3DCircle(const Vector& origin, float radius, float points, Color color);
	void Draw3DFilledCircle(const Vector& origin, float radius, float points, Color color);
	void Draw3DRainbowCircle(const Vector& origin, float radius);
	RECT viewport();
	void ARC(float x, float y, float radius, float min_angle, float max_angle, Color col, float thickness);
	void arc_poly(const Vector & center, float radius, float a_min, float a_max, float num_segments, float thiccboi, Color color);
};