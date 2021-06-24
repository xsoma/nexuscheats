#include "ui.h"
#include <deque>
#include <algorithm>
#include <vector>
#include "sdk/structs.hpp"

const char* const KeyNames[] = {
	"None",
	"Mouse1",
	"Mouse2",
	"Esc",
	"Mouse3",
	"Mouse4",
	"Mouse5",
	"None",
	"Back",
	"Tab",
	"None",
	"None",
	"Backspace",
	"Enter",
	"None",
	"None",
	"Shift",
	"Ctrl",
	"Alt",
	"Pause Break",
	"CapsLock",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"Esc",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"Print Screen",
	"Ins",
	"Del",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"Left Windows",
	"Right Windows",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"Multiply",
	"+",
	"Separator",
	"Subtract",
	"-",
	"/",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Num Lock",
	"Scroll lock",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Left Shift",
	"Right Shift",
	"Left Ctrl",
	"Right Ctrl",
	"Left Alt",
	"Right Alt"
};

namespace pGui
{
	bool button(const char* label, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

		if (hovered || held)
			ImGui::SetMouseCursor(0);

		window->DrawList->AddRectFilled(ImVec2(bb.Min.x + 0, bb.Min.y + 0), ImVec2(bb.Max.x + 0, bb.Max.y + 0), ImColor(29, 28, 32, 244), 1, 15);

		if (!pressed)
		{
			window->DrawList->AddRect(ImVec2(bb.Min.x + 0, bb.Min.y + 0), ImVec2(bb.Max.x + 0, bb.Max.y + 0), ImColor(56, 54, 59, 0), 1, 15, 1.000000);// n sei
			window->DrawList->AddRect(ImVec2(bb.Min.x + -1, bb.Min.y + -1), ImVec2(bb.Max.x + 1, bb.Max.y + 1), ImColor(17, 17, 22, 0), 1, 15, 1.000000);// n sei
			window->DrawList->AddRect(ImVec2(bb.Min.x + 1, bb.Min.y + 1), ImVec2(bb.Max.x + -1, bb.Max.y + -1), ImColor(17, 17, 22, 0), 1, 15, 1.000000);// n sei
		}
		

		ImVec4 col = hovered ? ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f } : ImVec4(229 / 255.f, 229 / 255.f, 229 / 255.f, 255 / 255.f);

		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();

		return pressed;
	}

	bool tab(const char* icon, const char* label, bool selected, ImVec2 size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(icon);

		ImGui::PushFont(c_menu::get().menuTab);
		const ImVec2 label_size = ImGui::CalcTextSize(icon, NULL, true);
		ImGui::PopFont();

		ImGui::PushFont(c_menu::get().tabLabel);
		const ImVec2 label_size2 = ImGui::CalcTextSize(label, NULL, true);
		ImGui::PopFont();

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
		ImVec2 size2 = ImGui::CalcItemSize(size_arg, label_size2.x + style.FramePadding.x * 2.0f, label_size2.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		float t = selected ? 1.0f : 0.0f;
		float ANIM_SPEED = 0.50f; // Bigger = Slower
		if (g.LastActiveId == g.CurrentWindow->GetID(icon)) {
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = selected ? (t_anim) : (1.0f - t_anim);
		}

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

		if (hovered || held)
			ImGui::SetMouseCursor(7);

		ImVec4 col = ImLerp(ImVec4{ 140 / 255.f, 140 / 255.f, 140 / 255.f, 1.f }, ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f }, t);
		ImVec4 col2 = ImLerp(ImVec4{ 140 / 255.f, 140 / 255.f, 140 / 255.f, 0 / 255.f }, ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f }, t);

		ImGui::PushFont(c_menu::get().menuTab);
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::RenderText(ImVec2(bb.Min.x + (size_arg.x / 2 - label_size.x / 2), bb.Min.y + (size_arg.y / 2 - label_size.y / 2)), icon);
		ImGui::PopStyleColor();

		ImGui::PopFont();

		ImGui::PushFont(c_menu::get().tabLabel);

		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::RenderText(ImVec2(bb.Min.x + (size_arg.x / 2 - label_size2.x / 2), bb.Min.y + (size_arg.y / 2 - label_size2.y / 2 + 25)), label);
		ImGui::PopStyleColor();

		ImGui::PopFont();

		// 

		 //window->DrawList->AddLine(bb.Min + ImVec2(70, size_arg.y - 15), bb.Max - ImVec2(70, 15), ImGui::GetColorU32(col2));

		return pressed;
	}




	static bool IsKeyPressedMap(ImGuiKey key, bool repeat)
	{
		const int key_index = GImGui->IO.KeyMap[key];
		return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
	}

	bool LabelClick(const char* label, bool* v, const char* unique_id)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		// The concatoff/on thingies were for my weapon config system so if we're going to make that, we still need this aids.
		char Buf[64];
		_snprintf(Buf, 62, crypt_str("%s"), label);

		char getid[128];
		sprintf_s(getid, 128, crypt_str("%s%s"), label, unique_id);


		ImGuiContext& g = *GImGui;
		ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(getid);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		
		const ImRect check_bb(window->DC.CursorPos, ImVec2(label_size.y + style.FramePadding.y * 2 + window->DC.CursorPos.x, window->DC.CursorPos.y + label_size.y + style.FramePadding.y * 2));
		ImGui::ItemSize(check_bb, style.FramePadding.y);

		ImRect total_bb = check_bb;

		if (label_size.x > 0)
		{
			ImGui::SameLine(0, style.ItemInnerSpacing.x);
			const ImRect text_bb(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), ImVec2(window->DC.CursorPos.x + label_size.x, window->DC.CursorPos.y + style.FramePadding.y + label_size.y));

			ImGui::ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
			total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
		}

		if (!ImGui::ItemAdd(total_bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed)
			*v = !(*v);

		//ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f));

		if (*v)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f));
		if (label_size.x > 0.0f)
			ImGui::RenderText(ImVec2(check_bb.GetTL().x + 12, check_bb.GetTL().y), Buf);
		if (*v)
			ImGui::PopStyleColor();
		//ImGui::PopStyleColor(1);

		return pressed;

	}

	void draw_keybind(const char* label, key_bind* key_bind, const char* unique_id)
	{
		// reset bind if we re pressing esc
		if (key_bind->key == KEY_ESCAPE)
			key_bind->key = KEY_NONE;

		auto clicked = false;
		auto text = (std::string)m_inputsys()->ButtonCodeToString(key_bind->key);

		if (key_bind->key <= KEY_NONE || key_bind->key >= KEY_MAX)
			text = crypt_str("None");

		// if we clicked on keybind
		if (hooks::input_shouldListen && hooks::input_receivedKeyval == &key_bind->key)
		{
			clicked = true;
			text = crypt_str("...");
		}

		auto textsize = ImGui::CalcTextSize(text.c_str()).x + 8 * c_menu::get().dpi_scale;
		auto labelsize = ImGui::CalcTextSize(label);
		ImGui::SetCursorPosX(4);
		ImGui::Text(label);
		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - (ImGui::GetWindowSize().x - ImGui::CalcItemWidth()) - max(50 * 1, textsize));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3 * c_menu::get().dpi_scale);

		if (ImGui::KeybindButton(text.c_str(), unique_id, ImVec2(max(50 * 1, textsize), 23 * 1), clicked))
			clicked = true;


		if (clicked)
		{
			hooks::input_shouldListen = true;
			hooks::input_receivedKeyval = &key_bind->key;
		}

		static auto hold = false, toggle = false;

		switch (key_bind->mode)
		{
		case HOLD:
			hold = true;
			toggle = false;
			break;
		case TOGGLE:
			toggle = true;
			hold = false;
			break;
		}

		if (ImGui::BeginPopup(unique_id))
		{
			if (LabelClick(crypt_str("Hold"), &hold, unique_id))
			{
				if (hold)
				{
					toggle = false;
					key_bind->mode = HOLD;
				}
				else if (toggle)
				{
					hold = false;
					key_bind->mode = TOGGLE;
				}
				else
				{
					toggle = false;
					key_bind->mode = HOLD;
				}

				ImGui::CloseCurrentPopup();
			}


			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9 * 1);

			if (LabelClick(crypt_str("Toggle"), &toggle, unique_id))
			{
				if (toggle)
				{
					hold = false;
					key_bind->mode = TOGGLE;
				}
				else if (hold)
				{
					toggle = false;
					key_bind->mode = HOLD;
				}
				else
				{
					hold = false;
					key_bind->mode = TOGGLE;
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	bool KeyBind(const char* label, int& k, const ImVec2& size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;



		ImGuiContext& g = *GImGui;
		ImGuiIO& io = g.IO;
		const ImGuiStyle& style = g.Style;

		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(KeyNames[k], NULL, true);
		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 3.0f, label_size.y + style.FramePadding.y * 3.0f);

		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
		const ImRect total_bb(window->DC.CursorPos, frame_bb.Max);

		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id))
			return false;

		const bool focus_requested = ImGui::FocusableItemRegister(window, g.ActiveId == id);
		//const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
		//const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

		const bool hovered = ImGui::ItemHoverable(frame_bb, id);

		if (hovered) {
			ImGui::SetHoveredID(id);
			g.MouseCursor = ImGuiMouseCursor_TextInput;
		}

		const bool user_clicked = hovered && io.MouseClicked[0];

		if (focus_requested || user_clicked) {
			if (g.ActiveId != id) {
				// Start edition
				memset(io.MouseDown, 0, sizeof(io.MouseDown));
				memset(io.KeysDown, 0, sizeof(io.KeysDown));
				k = 0;
			}
			ImGui::SetActiveID(id, window);
			ImGui::FocusWindow(window);
		}
		else if (io.MouseClicked[0]) {
			// Release focus when we click outside
			if (g.ActiveId == id)
				ImGui::ClearActiveID();
		}

		bool value_changed = false;
		int key = k;

		if (g.ActiveId == id) {
			for (auto i = 0; i < 5; i++) {
				if (io.MouseDown[i]) {
					switch (i) {
					case 0:
						key = VK_LBUTTON;
						break;
					case 1:
						key = VK_RBUTTON;
						break;
					case 2:
						key = VK_MBUTTON;
						break;
					case 3:
						key = VK_XBUTTON1;
						break;
					case 4:
						key = VK_XBUTTON2;
						break;
					}
					value_changed = true;
					ImGui::ClearActiveID();
				}
			}
			if (!value_changed) {
				for (auto i = VK_BACK; i <= VK_RMENU; i++) {
					if (io.KeysDown[i]) {
						key = i;
						value_changed = true;
						ImGui::ClearActiveID();
					}
				}
			}

			if (IsKeyPressedMap(ImGuiKey_Escape)) {
				k = 0;
				ImGui::ClearActiveID();
			}
			else {
				k = key;
			}
		}

		// Render
		// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is Set 'buf' might still be the old value. We Set buf to NULL to prevent accidental usage from now on.

		char buf_display[64] = "None";

		//ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_FrameBg]), true, style.FrameRounding);
		window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x + 0, frame_bb.Min.y + 0), ImVec2(frame_bb.Max.x + 0, frame_bb.Max.y + 0), ImColor(29, 28, 32, 244), 1, 15);
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + 0, frame_bb.Min.y + 0), ImVec2(frame_bb.Max.x + 0, frame_bb.Max.y + 0), ImColor(56, 54, 59, 0), 1, 15, 1.000000);// n sei
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + -1, frame_bb.Min.y + -1), ImVec2(frame_bb.Max.x + 1, frame_bb.Max.y + 1), ImColor(17, 17, 22, 0), 1, 15, 1.000000);// n sei
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + 1, frame_bb.Min.y + 1), ImVec2(frame_bb.Max.x + -1, frame_bb.Max.y + -1), ImColor(17, 17, 22, 0), 1, 15, 1.000000);// n sei

		if (k != 0 && g.ActiveId != id) {
			strcpy_s(buf_display, KeyNames[k]);
		}
		else if (g.ActiveId == id) {
			strcpy_s(buf_display, "<Press a key>");
		}

		const ImRect clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
		ImVec2 render_pos = frame_bb.Min + style.FramePadding;
		ImGui::RenderTextClipped(frame_bb.Min + style.FramePadding * 0.8, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, style.ButtonTextAlign, &clip_rect);
		//RenderTextClipped(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding, buf_display, NULL, NULL, GetColorU32(ImGuiCol_Text), style.ButtonTextAlign, &clip_rect);
		//draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, NULL, 0.0f, &clip_rect);

		if (label_size.x > 0)
			ImGui::RenderText(ImVec2(total_bb.Max.x + style.FramePadding.x, frame_bb.Min.y + style.FramePadding.y), label);

		return value_changed;
	}

	bool subtab(const char* label, bool selected, ImVec2 size_arg)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

		const ImRect bb(pos, pos + size);
		ImGui::ItemSize(size, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		float t = selected ? 1.0f : 0.0f;
		float ANIM_SPEED = 0.50f; // Bigger = Slower
		if (g.LastActiveId == g.CurrentWindow->GetID(label)) {
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = selected ? (t_anim) : (1.0f - t_anim);
		}

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, NULL);

		if (hovered || held)
			ImGui::SetMouseCursor(7);

		ImVec4 col = ImLerp(ImVec4{ 140 / 255.f, 140 / 255.f, 140 / 255.f, 1.f }, ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f }, t);
		ImVec4 col2 = ImLerp(ImVec4{ 140 / 255.f, 140 / 255.f, 140 / 255.f, 0 / 255.f }, ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f }, t);

		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::RenderText(ImVec2(bb.Min.x + (size_arg.x / 2 - label_size.x / 2), bb.Min.y + (size_arg.y / 2 - label_size.y / 2)), label);
		ImGui::PopStyleColor();

		//window->DrawList->AddLine(bb.Min + ImVec2(40, size_arg.y), bb.Max - ImVec2(40, 0), ImGui::GetColorU32(col2));

		return pressed;
	}

	static bool BeginChildExX(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags, bool WantText)
	{
		ImGuiContext& g = *GImGui;
		ImGuiWindow* parent_window = g.CurrentWindow;

		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
		flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

		// Size
		const ImVec2 content_avail = ImGui::GetContentRegionAvail();
		ImVec2 size = ImFloor(size_arg);
		const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
		if (size.x <= 0.0f)
			size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
		if (size.y <= 0.0f)
			size.y = ImMax(content_avail.y + size.y, 4.0f);
		ImGui::SetNextWindowSize(size);

		// Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
		char title[256];
		if (name)
			ImFormatString(title, IM_ARRAYSIZE(title), "%s/%s_%08X", parent_window->Name, name, id);
		else
			ImFormatString(title, IM_ARRAYSIZE(title), "%s/%08X", parent_window->Name, id);



		const float backup_border_size = g.Style.ChildBorderSize;
		if (!border)
			g.Style.ChildBorderSize = 0.0f;
		bool ret = ImGui::Begin(title, NULL, flags);
		g.Style.ChildBorderSize = backup_border_size;


		ImGuiWindow* child_window = g.CurrentWindow;
		child_window->ChildId = id;
		child_window->AutoFitChildAxises = auto_fit_axises;

		auto ss = ImGui::CalcTextSize(name);



		// Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
		// While this is not really documented/defined, it seems that the expected thing to do.
		if (child_window->BeginCount == 1)
			parent_window->DC.CursorPos = child_window->Pos;
		ImVec2 child_bb(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y);
		// Process navigation-in immediately so NavInit can run on first frame
		if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayerActiveMask != 0 || child_window->DC.NavHasScroll))
		{
			ImGui::FocusWindow(child_window);
			ImGui::NavInitWindow(child_window, false);
			ImGui::SetActiveID(id + 1, child_window); // Steal ActiveId with a dummy id so that key-press won't activate child item
			g.ActiveIdSource = ImGuiInputSource_Nav;
		}
		//if (WantText)
		  //  parent_window->DrawList->AddText(g_pMenu.fonteMenu, 13.f, child_bb + ImVec2(size.x / 2.5f, -15), ImColor(255, 255, 255, 255), name);

		parent_window->DrawList->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + size_arg, ImColor(12, 12, 12, int(1.f * 170)), 0);
		parent_window->DrawList->AddRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + size_arg, ImColor(59, 59, 59, int(1.f * 140)), 0);
		parent_window->DrawList->AddRect(ImGui::GetWindowPos() + ImVec2(3, 3), ImGui::GetWindowPos() + size_arg - ImVec2(3, 3), ImColor(2, 2, 2, int(1.f * 170)), 0);

		return ret;
	}

	bool BeginChildD(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags, bool WantText)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		return pGui::BeginChildExX(str_id, window->GetID(str_id), size_arg, border, extra_flags, WantText);
	}

	bool Checkbox(const char* label, bool* v, const char* toolTip)
	{

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

		const ImVec2 pos = window->DC.CursorPos;
		const ImRect total_bb(pos, pos + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, label_size.y));
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
		if (pressed)
		{
			*v = !(*v);
			ImGui::MarkItemEdited(id);
		}

		float t = *v ? 1.0f : 0.0f;
		float ANIM_SPEED = 0.30f;
		if (g.LastActiveId == g.CurrentWindow->GetID(label))
		{
			float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
			t = *v ? (t_anim) : (1.0f - t_anim);
		}

		ImColor col = ImLerp(ImVec4{ 140 / 255.f, 140 / 255.f, 140 / 255.f, 1.f }, ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f }, t);

		ImVec2 p = ImGui::GetWindowSize();



		if (*v)
		{
			window->DrawList->AddRectFilled(pos, pos + ImVec2(13, 13), col, 2.f);

			ImGui::RenderCheckMark(window->DrawList, ImVec2(pos.x + 1.3f, pos.y + 1.3f), ImColor(0.f, 0.f, 0.f, t), 10);
		}
		else
		{
			if (hovered)
			{
				ImVec4 g = ImVec4(g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, 1.f);
				window->DrawList->AddRectFilled(pos, pos + ImVec2(13, 13), ImGui::GetColorU32(g), 2.f);
				window->DrawList->AddRect(pos, pos + ImVec2(13, 13), ImGui::GetColorU32(g), 2.f);
			}
			else
			{
				window->DrawList->AddRectFilled(pos, pos + ImVec2(13, 13), ImColor(30 / 255.f, 32 / 255.f, 42 / 255.f, g.Style.Alpha), 2.f);
				window->DrawList->AddRect(pos, pos + ImVec2(13, 13), ImColor(54, 57, 67), 2.f);
				//RenderCheckMark(total_bb.Min + ImVec2(10, 10), ImColor(0,0,0,255), total_bb.GetWidth() - 10 * 2.0f);
			}
		}

		window->DrawList->AddText(pos + ImVec2(16, 0), ImColor(205 / 255.f, 205 / 255.f, 205 / 255.f, g.Style.Alpha), label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));

		return pressed;
	}

	static const char* PatchFormatStringFloatToInt(const char* fmt)
	{
		if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0)
			return "%d";

		const char* fmt_start = ImParseFormatFindStart(fmt);
		const char* fmt_end = ImParseFormatFindEnd(fmt_start);
		if (fmt_end > fmt_start && fmt_end[-1] == 'f')
		{
			if (fmt_start == fmt && fmt_end[0] == 0)
				return "%d";
			ImGuiContext& g = *GImGui;
			ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end);
			return g.TempBuffer;
		}
		return fmt;
	}

	bool SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power)
	{
		return ImGui::SliderScalar(label, data_type, p_data, p_min, p_max, format, power);
	}

	bool slider_int(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
	}

	bool slider_float(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return ImGui::SliderFloat(label, v, v_min, v_max, format);
	}

	static float CalcMaxPopupHeightFromItemCount(int items_count)
	{
		ImGuiContext& g = *GImGui;
		if (items_count <= 0)
			return FLT_MAX;
		return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
	}

	bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items);
	bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items);
	bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items);

	bool begincombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
	{
		IM_USE;
		ImGuiContext& g = *GImGui;
		bool has_window_size_constraint = (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint) != 0;
		g.NextWindowData.Flags &= ~ImGuiNextWindowDataFlags_HasSizeConstraint;

		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);


		const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ImGui::GetFrameHeight();
		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const float expected_w = CalcItemWidth();
		const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : expected_w;

		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w + 0, 0 + style.FramePadding.y * 2.0f + 43));
		const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + 0 : 0.0f, 0.0f));

		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
			return false;

		bool hovered, held;
		bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held);
		bool popup_open = IsPopupOpen(id, ImGuiPopupFlags_None);

		if (hovered || held)
			ImGui::SetMouseCursor(7);

		const float value_x2 = ImMax(frame_bb.Min.x, frame_bb.Max.x - arrow_size);

		window->DrawList->AddRectFilled(ImVec2(frame_bb.Min.x + 1, frame_bb.Min.y + 26), ImVec2(frame_bb.Max.x + 4, frame_bb.Max.y + -1), ImColor(0.115454f, 0.112457f, 0.127451f, 0.956863f), 1, 15);
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + 0, frame_bb.Min.y + 25), ImVec2(frame_bb.Max.x + 5, frame_bb.Max.y + 0), ImColor(0.219608f, 0.215686f, 0.235294f, 1.000000f), 1, 15, 1.000000);
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + -1, frame_bb.Min.y + 24), ImVec2(frame_bb.Max.x + 6, frame_bb.Max.y + 1), ImColor(0.070588f, 0.070588f, 0.090196f, 1.000000f), 1, 15, 1.000000);
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + 1, frame_bb.Min.y + 26), ImVec2(frame_bb.Max.x + 4, frame_bb.Max.y + -1), ImColor(0.070588f, 0.070588f, 0.090196f, 1.000000f), 1, 15, 1.000000);

		if (!(flags & ImGuiComboFlags_NoArrowButton))
		{
			int mod = 2;

			if (hovered)
				mod = 0;
			else if (!hovered && !popup_open)
				mod = 2;
			else if (popup_open)
				mod = 3;

			ImU32 text_col = ImGui::GetColorU32(ImGuiCol_Text);
			//if (value_x2 + arrow_size - style.FramePadding.x <= frame_bb.Max.x)
			  //  ImGui::RenderArrow(window->DrawList, ImVec2(value_x2 + style.FramePadding.y + -2, frame_bb.Min.y + style.FramePadding.y + 28), text_col, 3, 1);
		}

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255 / 255.f, 254 / 255.f, 254 / 255.f, 255 / 255.f));

		auto size = ImGui::CalcTextSize(preview_value);
		if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
		{
			if(size.x >= 140)
				ImGui::RenderText(frame_bb.Min + style.FramePadding + ImVec2(5, 27), "...");
			else
				ImGui::RenderText(frame_bb.Min + style.FramePadding + ImVec2(5, 27), preview_value);
		}
			

		// static std::string preview = std::to_string(preview_value);



		ImGui::PopStyleColor();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255 / 255.f, 254 / 255.f, 254 / 255.f, 255 / 255.f));
		if (label_size.x > 0)
			ImGui::RenderText(ImVec2(frame_bb.Min.x + style.ItemInnerSpacing.x + -4, frame_bb.Min.y + style.FramePadding.y + 3), label);

		ImGui::PopStyleColor();

		if ((pressed || g.NavActivateId == id) && !popup_open)
		{
			if (window->DC.NavLayerCurrent == 0)
				window->NavLastIds[0] = id;
			ImGui::OpenPopupEx(id);
			popup_open = true;


		}

		if (!popup_open)
			return false;

		if ((flags & ImGuiComboFlags_HeightMask_) == 0)
			flags |= ImGuiComboFlags_HeightRegular;
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_));
		int popup_max_height_in_items = -1;
		if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
		else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
		else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
		ImGui::SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

		char name[16];
		ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size);



		if (ImGuiWindow * popup_window = ImGui::FindWindowByName(name))
			if (popup_window->WasActive)
			{
				ImVec2 size_expected = ImGui::CalcWindowExpectedSize(popup_window);
				if (flags & ImGuiComboFlags_PopupAlignLeft)
					popup_window->AutoPosLastDirection = ImGuiDir_Left;
				ImRect r_outer = ImGui::GetWindowAllowedExtentRect(popup_window);
				ImVec2 pos = ImGui::FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
				ImGui::SetNextWindowPos(pos);


			}

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(style.FramePadding.x, style.WindowPadding.y));
		bool ret = ImGui::Begin(name, NULL, window_flags);
		ImGui::PopStyleVar();

		float t = popup_open ? 1.0f : 0.0f;
		float ANIM_SPEED = 0.3f; // Bigger = Slower

		float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
		t = popup_open ? (t_anim) : (1.f - t_anim);



		ImVec4 col = ImLerp(ImVec4{ 140 / 255.f, 140 / 255.f, 140 / 255.f, 1.f }, ImVec4{ g_cfg.menu.menu_theme.r() / 255.f, g_cfg.menu.menu_theme.g() / 255.f, g_cfg.menu.menu_theme.b() / 255.f, g_cfg.menu.menu_theme.a() / 255.f }, t);

		// if (popup_open)
		window->DrawList->AddRect(ImVec2(frame_bb.Min.x + 1, frame_bb.Min.y + 26), ImVec2(frame_bb.Max.x + 4, frame_bb.Max.y + -1), GetColorU32(col), 1, 15, 1.000000);




		if (!ret)
		{
			ImGui::EndPopup();
			return false;
		}
		return true;
	}



	static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
	{
		const char* const* items = (const char* const*)data;
		if (out_text)
			*out_text = items[idx];
		return true;
	}

	// Getter for the old Combo() API: "item1\0item2\0item3\0"
	static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
	{
		// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
		const char* items_separated_by_zeros = (const char*)data;
		int items_count = 0;
		const char* p = items_separated_by_zeros;
		while (*p)
		{
			if (idx == items_count)
				break;
			p += strlen(p) + 1;
			items_count++;
		}
		if (!*p)
			return false;
		if (out_text)
			*out_text = p;
		return true;
	}

	bool combo(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
	{
		IM_USE;
		ImGuiContext& g = *GImGui;
		const char* preview_value = NULL;
		if (*current_item >= 0 && *current_item < items_count)
			items_getter(data, *current_item, &preview_value);
		if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
			SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
		if (!begincombo(label, preview_value, ImGuiComboFlags_None))
			return false;
		bool value_changed = false;

		for (int i = 0; i < items_count; i++)
		{
			PushID((void*)(intptr_t)i);
			const bool item_selected = (i == *current_item);
			const char* item_text;

			if (!items_getter(data, i, &item_text))
				item_text = " * Unknown item * ";

			if (Selectable(item_text, item_selected))
			{
				value_changed = true;
				*current_item = i;
			}
			if (item_selected)
				SetItemDefaultFocus();
			PopID();
		}

		EndCombo();
		return value_changed;
	}

	bool combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
	{
		const bool value_changed = combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
		return value_changed;
	}

	bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
	{
		int items_count = 0;
		const char* p = items_separated_by_zeros;

		while (*p)
		{
			p += strlen(p) + 1;
			items_count++;
		}

		bool value_changed = combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
		return value_changed;
	}


	const char* const KeyNames[] = {
   "[-]",
	"[M1]",
	"[M2]",
	"[CN]",
	"[M3]",
	"[M4]",
	"[M5]",
	"[-]",
	"[BAC]",
	"[TAB]",
	"[-]",
	"[-]",
	"[CLR]",
	"[RET]",
	"[-]",
	"[-]",
	"[SHI]",
	"[CTL]",
	"[ALT]",
	"[PAU]",
	"[CAP]",
	"[KAN]",
	"[-]",
	"[JUN]",
	"[FIN]",
	"[KAN]",
	"[-]",
	"[ESC]",
	"[CON]",
	"[NCO]",
	"[ACC]",
	"[MAD]",
	"[SPA]",
	"[PGU]",
	"[PGD]",
	"[END]",
	"[HOM]",
	"[LEF]",
	"[UP]",
	"[RIG]",
	"[DOW]",
	"[SEL]",
	"[PRI]",
	"[EXE]",
	"[PRI]",
	"[INS]",
	"[DEL]",
	"[HEL]",
	"[0]",
	"[1]",
	"[2]",
	"[3]",
	"[4]",
	"[5]",
	"[6]",
	"[7]",
	"[8]",
	"[9]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[A]",
	"[B]",
	"[C]",
	"[D]",
	"[E]",
	"[F]",
	"[G]",
	"[H]",
	"[I]",
	"[J]",
	"[K]",
	"[L]",
	"[M]",
	"[N]",
	"[O]",
	"[P]",
	"[Q]",
	"[R]",
	"[S]",
	"[T]",
	"[U]",
	"[V]",
	"[W]",
	"[X]",
	"[Y]",
	"[Z]",
	"[WIN]",
	"[WIN]",
	"[APP]",
	"[-]",
	"[SLE]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[NUM]",
	"[MUL]",
	"[ADD]",
	"[SEP]",
	"[MIN]",
	"[DEC]",
	"[DIV]",
	"[F1]",
	"[F2]",
	"[F3]",
	"[F4]",
	"[F5]",
	"[F6]",
	"[F7]",
	"[F8]",
	"[F9]",
	"[F10]",
	"[F11]",
	"[F12]",
	"[F13]",
	"[F14]",
	"[F15]",
	"[F16]",
	"[F17]",
	"[F18]",
	"[F19]",
	"[F20]",
	"[F21]",
	"[F22]",
	"[F23]",
	"[F24]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[NUM]",
	"[SCR]",
	"[EQU]",
	"[MAS]",
	"[TOY]",
	"[OYA]",
	"[OYA]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[-]",
	"[SHI]",
	"[SHI]",
	"[CTR]",
	"[CTR]",
	"[ALT]",
	"[ALT]"
	};

	bool Hotkey(const char* label, int* k, const ImVec2& size_arg)
	{
		IM_USE;
		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems)
			return false;

		SameLine(window->Size.x - 28);

		ImGuiContext& g = *GImGui;

		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		ImGuiIO* io = &GetIO();

		const ImVec2 label_size = CalcTextSize(KeyNames[*k]);
		const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + label_size);
		const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(window->Pos.x + window->Size.x - window->DC.CursorPos.x, label_size.y));
		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, id, &frame_bb))
			return false;

		const bool hovered = IsItemHovered();
		const bool edit_requested = hovered && io->MouseClicked[0];
		const bool style_requested = hovered && io->MouseClicked[1];

		if (edit_requested) {
			if (g.ActiveId != id) {
				memset(io->MouseDown, 0, sizeof(io->MouseDown));
				memset(io->KeysDown, 0, sizeof(io->KeysDown));
				*k = 0;
			}

			SetActiveID(id, window);
			FocusWindow(window);
		}
		else if (!hovered && io->MouseClicked[0] && g.ActiveId == id)
			ClearActiveID();

		bool value_changed = false;
		int key = *k;

		if (g.ActiveId == id) {
			for (auto i = 0; i < 5; i++) {
				if (io->MouseDown[i]) {
					switch (i) {
					case 0:
						key = VK_LBUTTON;
						break;
					case 1:
						key = VK_RBUTTON;
						break;
					case 2:
						key = VK_MBUTTON;
						break;
					case 3:
						key = VK_XBUTTON1;
						break;
					case 4:
						key = VK_XBUTTON2;
					}
					value_changed = true;
					ClearActiveID();
				}
			}

			if (!value_changed) {
				for (auto i = VK_BACK; i <= VK_RMENU; i++) {
					if (io->KeysDown[i]) {
						key = i;
						value_changed = true;
						ClearActiveID();
					}
				}
			}

			if (IsKeyPressedMap(ImGuiKey_Escape)) {
				*k = 0;
				ClearActiveID();
			}
			else
				*k = key;
		}


		char buf_display[64] = "[-]";

		if (*k != 0 && g.ActiveId != id)
			strcpy_s(buf_display, KeyNames[*k]);
		else if (g.ActiveId == id)
			strcpy_s(buf_display, "[-]");

		//PushFont(_shon->tabff);
		window->DrawList->AddText(frame_bb.Min, g.ActiveId == id ? ImColor(255 / 255.f, 16 / 255.f, 16 / 255.f, g.Style.Alpha) : ImColor(90 / 255.f, 90 / 255.f, 90 / 255.f, g.Style.Alpha), buf_display);
		//PopFont();

		return value_changed;
	}






}

namespace pDraw
{
	bool multiselect(const char* name, const char** displayName, bool* data, int dataSize)
	{
		ImGui::PushID(name);

		char previewText[1024] = { 0 };
		char buf[1024] = { 0 };
		char buf2[1024] = { 0 };
		int currentPreviewTextLen = 0;
		float multicomboWidth = 90.f;

		for (int i = 0; i < dataSize; i++) {

			if (data[i] == true) {

				if (currentPreviewTextLen == 0)
					sprintf(buf, "%s", displayName[i]);
				else
					sprintf(buf, ", %s", displayName[i]);

				strcpy(buf2, previewText);
				sprintf(buf2 + currentPreviewTextLen, buf);
				ImVec2 textSize = ImGui::CalcTextSize(buf2);

				if (textSize.x > multicomboWidth) {

					sprintf(previewText + currentPreviewTextLen, ", ...");
					currentPreviewTextLen += (int)strlen(", ...");
					break;
				}

				sprintf(previewText + currentPreviewTextLen, buf);
				currentPreviewTextLen += (int)strlen(buf);
			}
		}

		if (currentPreviewTextLen > 0)
			previewText[currentPreviewTextLen] = NULL;
		else
			sprintf(previewText, "");

		bool isDataChanged = false;

		if (pGui::begincombo(name, previewText, 0)) {

			for (int i = 0; i < dataSize; i++) {

				sprintf(buf, displayName[i]);

				auto textSize = ImGui::CalcTextSize(buf);



				if (ImGui::Selectable(buf, data[i], ImGuiSelectableFlags_DontClosePopups)) {

					data[i] = !data[i];
					isDataChanged = true;
				}
			}

			ImGui::EndCombo();
		}

		ImGui::PopID();
		return isDataChanged;
	}

	void multiselect2(std::string name, std::vector<int>& variable, const char* labels[], int count, const char** displayName)
	{
		char previewText[1024] = { 0 };
		char buf[1024] = { 0 };
		char buf2[1024] = { 0 };
		int currentPreviewTextLen = 0;
		float multicomboWidth = 90.f;

		for (int i = 0; i < count; i++) {

			if (variable[i]) {

				if (currentPreviewTextLen == 0)
					sprintf(buf, "%s", displayName[i]);
				else
					sprintf(buf, ", %s", displayName[i]);

				strcpy(buf2, previewText);
				sprintf(buf2 + currentPreviewTextLen, buf);
				ImVec2 textSize = ImGui::CalcTextSize(buf2);

				if (textSize.x > multicomboWidth) {

					sprintf(previewText + currentPreviewTextLen, ", ...");
					currentPreviewTextLen += (int)strlen(", ...");
					break;
				}

				sprintf(previewText + currentPreviewTextLen, buf);
				currentPreviewTextLen += (int)strlen(buf);
			}
		}

		if (currentPreviewTextLen > 0)
			previewText[currentPreviewTextLen] = NULL;
		else
			sprintf(previewText, "");

		if (pGui::begincombo(name.c_str(), previewText, 0)) // draw start
		{
			for (auto i = 0; i < count; i++)
			{
				sprintf(buf, displayName[i]);

				auto textSize = ImGui::CalcTextSize(buf);
			
				ImGui::Selectable(labels[i], (bool*)&variable[i], ImGuiSelectableFlags_DontClosePopups);
			}
			ImGui::EndCombo();
		}

		//preview = crypt_str("None"); // reset preview to use later
	}

	void addkeybind(const char* label, const char* desc, int* keybind)
	{
		//ImGui::PushFont(roboto_medium);
		IM_USE;


		//ImGui::SameLine(15);
		//ImGui::TextColored(ImColor(255, 255, 255, 255), label);

		//ImGui::Spacing();
		//ImGui::SameLine(15);
		ImGui::TextColored(ImColor(255, 255, 255, 255), label);

		const bool hovered = IsItemHovered();

		if (hovered)
			if (desc != "")
				SetTooltip(desc);
		//ImGui::Spacing();
		//ImGui::PopFont();
		pGui::Hotkey(label, keybind);
		ImGui::Spacing();
	}
	
	bool addcheck(const char* label, bool* v, const char* toolTip)
	{
		return pGui::Checkbox(label, v, toolTip);
	}

	bool singleselect(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
	{
		return pGui::combo(label, current_item, items, items_count);

	}

	bool addslider(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
	{
		return pGui::slider_int(label, v, v_min, v_max, format, flags);
	}

	bool addsliderF(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
	{
		return pGui::slider_float(label, v, v_min, v_max, format, flags);
	}

	bool addchild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags, bool WantText)
	{
		return pGui::BeginChildD(str_id, size_arg, border, extra_flags, WantText);
	}

	bool addtab(const char* label, const char* nn, bool selected, ImVec2 size_arg)
	{
		return pGui::tab(label, nn, selected, size_arg);
	}

	bool addsubtab(const char* label, bool selected, ImVec2 size_arg)
	{
		return pGui::subtab(label, selected, size_arg);
	}

	bool button(const char* label, const ImVec2& size_arg)
	{
		return pGui::button(label, size_arg);
	}
}
