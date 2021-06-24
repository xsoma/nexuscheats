#pragma once
#include "../includes.hpp"
#include "../ui.h"

namespace kdopipi
{
	extern bool g;
	extern bool gg;
	extern bool event_log;
}

class c_menu : public singleton<c_menu> {
public:

	LPDIRECT3DTEXTURE9 CTmd1 = nullptr;
	LPDIRECT3DTEXTURE9 head = nullptr;
	LPDIRECT3DTEXTURE9 head1 = nullptr;

	LPDIRECT3DTEXTURE9 neck = nullptr;
	LPDIRECT3DTEXTURE9 neck1 = nullptr;
	LPDIRECT3DTEXTURE9 upchest = nullptr;
	LPDIRECT3DTEXTURE9 upchest1 = nullptr;
	LPDIRECT3DTEXTURE9 chest = nullptr;
	LPDIRECT3DTEXTURE9 chest1 = nullptr;
	LPDIRECT3DTEXTURE9 lowchest = nullptr;
	LPDIRECT3DTEXTURE9 lowchest1 = nullptr;
	LPDIRECT3DTEXTURE9 pel = nullptr;
	LPDIRECT3DTEXTURE9 pel1 = nullptr;

	LPDIRECT3DTEXTURE9 arms = nullptr;
	LPDIRECT3DTEXTURE9 arms1 = nullptr;
	LPDIRECT3DTEXTURE9 arms2 = nullptr;
	LPDIRECT3DTEXTURE9 arms3 = nullptr;

	LPDIRECT3DTEXTURE9 legs = nullptr;
	LPDIRECT3DTEXTURE9 legs1 = nullptr;
	LPDIRECT3DTEXTURE9 legs2 = nullptr;
	LPDIRECT3DTEXTURE9 legs3 = nullptr;

	LPDIRECT3DTEXTURE9 feet = nullptr;
	LPDIRECT3DTEXTURE9 feet1 = nullptr;
	LPDIRECT3DTEXTURE9 feet2 = nullptr;
	LPDIRECT3DTEXTURE9 feet3 = nullptr;
	//void draw( bool is_open );
	//void menu_setup(ImGuiStyle &style);
	void render2(bool is_open);
	ImDrawList* draw_list;
	float dpi_scale = 1.f;

	ImFont* futura;
	ImFont* futura_large;
	ImFont* icons;
	ImFont* bigxd;
	ImFont* futura_small;

	ImFont* gotham;

	ImFont* ico_menu;
	ImFont* ico_bottom;

	ImFont* menu;
	ImFont* tabLabel;
	ImFont* menuTab;

	float public_alpha;
	IDirect3DDevice9* device;
	float color_buffer[4] = { 1.f, 1.f, 1.f, 1.f };
private:
	struct {
		ImVec2 WindowPadding;
		float  WindowRounding;
		ImVec2 WindowMinSize;
		float  ChildRounding;
		float  PopupRounding;
		ImVec2 FramePadding;
		float  FrameRounding;
		ImVec2 ItemSpacing;
		ImVec2 ItemInnerSpacing;
		ImVec2 TouchExtraPadding;
		float  IndentSpacing;
		float  ColumnsMinSpacing;
		float  ScrollbarSize;
		float  ScrollbarRounding;
		float  GrabMinSize;
		float  GrabRounding;
		float  TabRounding;
		float  TabMinWidthForUnselectedCloseButton;
		ImVec2 DisplayWindowPadding;
		ImVec2 DisplaySafeAreaPadding;
		float  MouseCursorScale;
	} styles;//?

	void skins();
	void inventory_tab();

	int current_profile = 1;
	//void dpi_resize(float scale_factor, ImGuiStyle &style);

	std::string preview = crypt_str("None");
};
