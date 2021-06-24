#pragma once
#pragma once
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#define IM_USE using namespace ImGui;
//ja volto ovo pega lancher
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
//#define IMGUI_DEFINE_PLACEMENT_NEW
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
//#include "imgui/directx9/imgui_impl_dx9.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <vector>
#include "cheats/menu.h"

namespace pGui
{
	bool button(const char* label, const ImVec2& size_arg);

	bool tab(const char* icon, const char* label, bool selected, ImVec2 size_arg = ImVec2(0, 0));

	static bool IsKeyPressedMap(ImGuiKey key, bool repeat = true);

	bool KeyBind(const char* label, int& k, const ImVec2& size_arg = ImVec2(0, 0));

	bool subtab(const char* label, bool selected, ImVec2 size_arg = ImVec2(0, 0));

	static bool BeginChildExX(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags, bool WantText);

	bool BeginChildD(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags, bool WantText);

	bool Checkbox(const char* label, bool* v, const char* toolTip = "");

	static const char* PatchFormatStringFloatToInt(const char* fmt);

	bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power);

	bool slider_int(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags);

	bool slider_float(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);

	static float CalcMaxPopupHeightFromItemCount(int items_count);

	bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
	bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);

	bool begincombo(const char* label, const char* preview_value, ImGuiComboFlags flags);
	void draw_keybind(const char* label, key_bind* key_bind, const char* unique_id);

	static bool Items_ArrayGetter(void* data, int idx, const char** out_text);

	// Getter for the old Combo() API: "item1\0item2\0item3\0"
	static bool Items_SingleStringGetter(void* data, int idx, const char** out_text);

	bool combo(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items);

	bool combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items);

	bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items);

	bool Hotkey(const char* label, int* k, const ImVec2& size_arg = ImVec2(0, 0));
}

namespace pDraw
{
	bool multiselect(const char* name, const char** displayName, bool* data, int dataSize);

	void addkeybind(const char* label, const char* desc, int* keybind);

	bool addcheck(const char* label, bool* v, const char* toolTip = "");

	bool singleselect(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);

	bool addslider(const char* label, int* v, int v_min, int v_max, const char* format = NULL, ImGuiSliderFlags flags = 0);


	bool addsliderF(const char* label, float* v, float v_min, float v_max, const char* format = "%0.1f", ImGuiSliderFlags flags = 0);


	bool addchild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags, bool WantText = false);

	bool addtab(const char* label, const char* nn, bool selected, ImVec2 size_arg = ImVec2(0, 0));

	bool addsubtab(const char* label, bool selected, ImVec2 size_arg = ImVec2(0, 0));

	bool button(const char* label, const ImVec2& size_arg = ImVec2(70, 30));

	void multiselect2(std::string name, std::vector<int>& variable, const char* labels[], int count, const char** displayName);
	//bool addkey(const char* label, int& k, const ImVec2& size_arg = ImVec2(0, 0));
}
