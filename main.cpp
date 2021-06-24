// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <ShlObj.h>
#include <ShlObj_core.h>
#include "includes.hpp"
#include "utils\ctx.hpp"
#include "utils\recv.h"
#include "utils\imports.h"
#include "minihook/minhook.h"
//#include "utils\anti_debug.h"
#include "nSkinz\SkinChanger.h"
#include "cheats/inventory changer/kitparser.h"
#include "skinchanger/skins.h"
#include "License.h"
#include "Functions.h"
#include "base_64.h"
CLicense License;
//clb eu decido oq precisa ou nao78 é p78 k78ir78a
namespace fs = std::experimental::filesystem;

//#include "utils\sha-256.h"
//#include "utils\protect.h"

//using namespace jwt::params;

class zInterfaceReg
{
private:
	using InstantiateInterfaceFn = void* (*)();
public:
	InstantiateInterfaceFn m_CreateFn;
	const char* m_pName;
	zInterfaceReg* m_pNext;
};
template<typename T>
static T* zget_interface(const char* mod_name, const char* interface_name, bool exact = false) {
	T* iface = nullptr;
	zInterfaceReg* register_list;
	int part_match_len = strlen(interface_name); //-V103

	DWORD interface_fn = reinterpret_cast<DWORD>(GetProcAddress(GetModuleHandleA(mod_name), ("CreateInterface")));

	if (!interface_fn) {
		return nullptr;
	}

	unsigned int jump_start = (unsigned int)(interface_fn)+4;
	unsigned int jump_target = jump_start + *(unsigned int*)(jump_start + 1) + 5;

	register_list = **reinterpret_cast<zInterfaceReg***>(jump_target + 6);

	for (zInterfaceReg* cur = register_list; cur; cur = cur->m_pNext) {
		if (exact == true) {
			if (strcmp(cur->m_pName, interface_name) == 0)
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
		else {
			if (!strncmp(cur->m_pName, interface_name, part_match_len) && std::atoi(cur->m_pName + part_match_len) > 0) //-V106
				iface = reinterpret_cast<T*>(cur->m_CreateFn());
		}
	}
	return iface;
}

class CHLClient
{
	template < typename t = Address >
	t get_method(Address this_ptr, size_t index) {
		return (t)this_ptr.to< t* >()[index];
	}
public:
	ClientClass* GetAllClasses() {
		return get_method< ClientClass * (__thiscall*)(decltype(this))>(this, 8)(this);
	}
};
inline CHLClient* fm_client;

class Netvars_merge {
private:
	struct NetvarData {
		bool        m_datamap_var; // we can't do proxies on stuff from datamaps :).
		RecvProp* m_prop_ptr;
		size_t      m_offset;

		__forceinline NetvarData() : m_datamap_var{}, m_prop_ptr{}, m_offset{} { }
	};

	std::unordered_map< hash32_t, int > m_client_ids;

	// netvar container.
	std::unordered_map< hash32_t,		// hash key + tables
		std::unordered_map< hash32_t,	// hash key + props
		NetvarData						// prop offset / prop ptr
		> > m_offsets;

public:
	// ctor.
	Netvars_merge() : m_offsets{} {}

	void init() {
		ClientClass* list;



		// sanity check on client.
		if (!fm_client)
			return;

		// grab linked list.
		list = fm_client->GetAllClasses();
		if (!list)
			return;

		// iterate list of netvars.
		for (; list != nullptr; list = list->m_pNext)
			StoreDataTable(list->m_pRecvTable->netTableName, list->m_pRecvTable);

		// find all datamaps
		FindAndStoreDataMaps();
	}

	// dtor.
	~Netvars_merge() {
		m_offsets.clear();
	}

	// gather all classids dynamically on runtime.
	void SetupClassData() {
		ClientClass* list;

		// clear old shit.
		m_client_ids.clear();

		// sanity check on client.
		if (!m_client())
			return;

		// grab linked list.
		list = m_client()->GetAllClasses();
		if (!list)
			return;

		// iterate list.
		for (; list != nullptr; list = list->m_pNext)
			m_client_ids[FNV1a::get(list->m_pNetworkName)] = list->m_ClassID;
	}

	void StoreDataTable(const std::string& name, RecvTable* table, size_t offset = 0) {
		hash32_t	var, base{ FNV1a::get(name) };
		RecvProp* prop;
		RecvTable* child;

		// iterate props
		for (int i{}; i < table->propCount; ++i) {
			prop = &table->props[i];
			child = prop->dataTable;

			// we have a child table, that contains props.
			if (child && child->propCount > 0)
				StoreDataTable(name, child, prop->offset + offset);

			// hash var name.
			var = FNV1a::get(prop->name);

			// insert if not present.
			if (!m_offsets[base][var].m_offset) {
				m_offsets[base][var].m_datamap_var = false;
				m_offsets[base][var].m_prop_ptr = prop;
				m_offsets[base][var].m_offset = (size_t)(prop->offset + offset);
			}
		}
	}

	// iterate client module and find all datamaps.
	void FindAndStoreDataMaps() {
		pattern::patterns_t matches{};

		matches = pattern::FindAll(PE::GetModule(HASH("client.dll")), ("C7 05 ? ? ? ? ? ? ? ? C7 05 ? ? ? ? ? ? ? ? C3 CC"));
		if (matches.empty())
			return;

		for (auto& m : matches)
			StoreDataMap(m);
	}

	void StoreDataMap(Address ptr) {
		datamap_t* map;
		hash32_t            base, var;
		typedescription_t* entry;

		// get datamap and verify.
		map = ptr.at(2).sub(4).as< datamap_t* >();

		//dataNumFields - m_size
		//dataClassName - m_name

		if (!map || !map->dataNumFields || map->dataNumFields > 200 || !map->dataDesc || !map->dataClassName)
			return;

		// hash table name.
		base = FNV1a::get(map->dataClassName);

		for (int i{}; i < map->dataNumFields; ++i) {
			entry = &map->dataDesc[i];
			if (!entry->fieldName)
				continue;

			// hash var name.
			var = FNV1a::get(entry->fieldName);

			// if we dont have this var stored yet.
			if (!m_offsets[base][var].m_offset) {
				m_offsets[base][var].m_datamap_var = true;
				m_offsets[base][var].m_offset = (size_t)entry->fieldOffset[TD_OFFSET_NORMAL];
				m_offsets[base][var].m_prop_ptr = nullptr;
			}
		}
	}

	// get client id.
	int GetClientID(hash32_t network_name) {
		return m_client_ids[network_name];
	}

	// get netvar offset.
	size_t get(hash32_t table, hash32_t prop) {
		return m_offsets[table][prop].m_offset;
	}

	// get netvar proxy.
	recvProxy GetProxy(hash32_t table, hash32_t prop, recvProxy proxy) {
		// todo - dex; should we return null here if m_is_valid_netvar is false? probably doesn't matter anyway.
		return m_offsets[table][prop].m_prop_ptr->proxy;
	}

	// set netvar proxy.
	void SetProxy(hash32_t table, hash32_t prop, void* proxy, recvProxy& original) {
		auto netvar_entry = m_offsets[table][prop];

		// we can't set a proxy on a datamap.
		if (netvar_entry.m_datamap_var)
			return;

		// save original.
		original = netvar_entry.m_prop_ptr->proxy;

		// redirect.
		netvar_entry.m_prop_ptr->proxy = (recvProxy)proxy;
	}
};

Netvars_merge g_netvars;

enum error_type
{
	ERROR_NONE,
	ERROR_DEBUG,
	ERROR_OPEN_KEY,
	ERROR_QUERY_DATA,
	ERROR_CONNECT,
	ERROR_1,
	ERROR_2,
	ERROR_3,
	ERROR_4,
	ERROR_5,
	ERROR_6,
	ERROR_7,
	ERROR_8,
	ERROR_9,
	ERROR_CHECK_HASH
};


PVOID base_address = nullptr;
//Anti_debugger anti_debugger;
volatile error_type error = ERROR_NONE;

LONG CALLBACK ExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo);
__forceinline void setup_render();
__forceinline void setup_netvars();
__forceinline void setup_skins();
__forceinline void setup_hooks();

DWORD WINAPI main(PVOID base)
{
	//A vai tomar no cu :)
	auto m_kernel32_dll = PE::GetModule(HASH("kernel32.dll"));
	auto m_user32_dll = PE::GetModule(HASH("user32.dll"));
	auto m_shell32_dll = PE::GetModule(HASH("shell32.dll"));
	auto m_shlwapi_dll = PE::GetModule(HASH("shlwapi.dll"));
	auto m_client_dll = PE::GetModule(HASH("client.dll"));
	auto m_engine_dll = PE::GetModule(HASH("engine.dll"));
	auto m_vstdlib_dll = PE::GetModule(HASH("vstdlib.dll"));
	auto m_tier0_dll = PE::GetModule(HASH("tier0.dll"));
	auto m_shaderapidx9_dll = PE::GetModule(HASH("shaderapidx9.dll"));

	// import winapi functions.
	g_winapi.WideCharToMultiByte = PE::GetExport(m_kernel32_dll, HASH("WideCharToMultiByte")).as< WinAPI::WideCharToMultiByte_t >();
	g_winapi.MultiByteToWideChar = PE::GetExport(m_kernel32_dll, HASH("MultiByteToWideChar")).as< WinAPI::MultiByteToWideChar_t >();
	g_winapi.GetTickCount = PE::GetExport(m_kernel32_dll, HASH("GetTickCount")).as< WinAPI::GetTickCount_t >();
	g_winapi.VirtualProtect = PE::GetExport(m_kernel32_dll, HASH("VirtualProtect")).as< WinAPI::VirtualProtect_t >();
	g_winapi.VirtualQuery = PE::GetExport(m_kernel32_dll, HASH("VirtualQuery")).as< WinAPI::VirtualQuery_t >();
	g_winapi.CreateDirectoryA = PE::GetExport(m_kernel32_dll, HASH("CreateDirectoryA")).as< WinAPI::CreateDirectoryA_t >();
	g_winapi.SetWindowLongA = PE::GetExport(m_user32_dll, HASH("SetWindowLongA")).as< WinAPI::SetWindowLongA_t >();
	g_winapi.CallWindowProcA = PE::GetExport(m_user32_dll, HASH("CallWindowProcA")).as< WinAPI::CallWindowProcA_t >();
	g_winapi.SHGetFolderPathA = PE::GetExport(m_shell32_dll, HASH("SHGetFolderPathA")).as< WinAPI::SHGetFolderPathA_t >();
	g_winapi.PathAppendA = PE::GetExport(m_shlwapi_dll, HASH("PathAppendA")).as< WinAPI::PathAppendA_t >();

	g_csgo.fRandomInt = PE::GetExport(m_vstdlib_dll, HASH("RandomInt")).as< RandomInt_t >();

	g_ctx.signatures =
	{
		crypt_str("A1 ? ? ? ? 50 8B 08 FF 51 0C"),
		crypt_str("B9 ?? ?? ?? ?? A1 ?? ?? ?? ?? FF 10 A1 ?? ?? ?? ?? B9"),
		crypt_str("0F 11 05 ?? ?? ?? ?? 83 C8 01"),
		crypt_str("8B 0D ?? ?? ?? ?? 8B 46 08 68"),
		crypt_str("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"),
		crypt_str("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7"),
		crypt_str("A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6"),
		crypt_str("80 3D ? ? ? ? ? 53 56 57 0F 85"),
		crypt_str("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C"),
		crypt_str("80 3D ? ? ? ? ? 74 06 B8"),
		crypt_str("55 8B EC 83 E4 F0 B8 D8"),
		crypt_str("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C"),
		crypt_str("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85 F6"),
		crypt_str("55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36"),
		crypt_str("56 8B F1 8B 8E ? ? ? ? 83 F9 FF 74 23"),
		crypt_str("55 8B EC 83 E4 F8 83 EC 5C 53 8B D9 56 57 83"),
		crypt_str("55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9"),
		crypt_str("57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02"),
		crypt_str("55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80"),
		crypt_str("53 0F B7 1D ? ? ? ? 56"),
		crypt_str("8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6")
	};
	shonax_void();
	g_ctx.indexes =
	{
		5,
		33,
		339,
		218,
		219,
		34,
		157,
		75,
		460,
		482,
		452,
		483,
		284,
		223,
		246,
		27,
		17,
		123
	};

	while (!(GetModuleHandle(crypt_str("serverbrowser.dll"))))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	typedef uint32_t SteamPipeHandle;
	typedef uint32_t SteamUserHandle;
	SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();
	SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();
	SteamClient = ((ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();
	SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamGameCoordinator001");
	SteamUser = (ISteamUser*)SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
	SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
	static auto SteamInventory = SteamClient->GetISteamInventory(hSteamUser, hSteamPipe, "STEAMINVENTORY_INTERFACE_V002");
	auto _ = SteamFriends->GetLargeFriendAvatar(SteamUser->GetSteamID());
	SteamUtils = SteamClient->GetISteamUtils(hSteamPipe, "SteamUtils009");

	//base_address = base;
	//AllocConsole();

	//if (!freopen(crypt_str("CONOUT$"), crypt_str("w"), stdout))
	//{
		//FreeConsole();
		//return EXIT_SUCCESS;
	//}

	//shonax
	//IdManager->GetClassIDALL();
	static TCHAR path[MAX_PATH];
	std::string folder;
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, NULL, path);
	folder = std::string(path) + crypt_str("\\Nexus\\");
	CreateDirectory(folder.c_str(), 0);
	//std::cout << crypt_str("Fixing cfg...\n");
	//shonax
	//kit_parser.setup();
	//g_Skins.ParseSkins();

	kit_parser.setup();
	g_Skins.ParseSkins();
	g_pIClientLeafSystem = zget_interface<IClientLeafSystem>(("client.dll"), ("ClientLeafSystem"));

	//std::cout << crypt_str("Setuping sounds...\n");
	setup_sounds();

	//	std::cout << crypt_str("Setuping skins...\n");
	setup_skins();

	//std::cout << crypt_str("Setuping netvars...\n");
	setup_netvars();

	//std::cout << crypt_str("Setuping render...\n");


	setup_render();

	//std::cout << crypt_str("Setuping configs manager...\n");
	cfg_manager->setup();

	//std::cout << crypt_str("Setuping scripts manager...\n");
	c_lua::get().initialize();

	//std::cout << crypt_str("Setuping key binds manager...\n");
	key_binds::get().initialize_key_binds();

	//std::cout << crypt_str("Setuping hooks...\n");
	setup_hooks();
	Netvars::Netvars();

	//std::cout << crypt_str("Done!");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//FreeConsole();
	return EXIT_SUCCESS;
}




BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		auto current_process = GetCurrentProcess();
		auto priority_class = GetPriorityClass(current_process);

		if (priority_class != HIGH_PRIORITY_CLASS && priority_class != REALTIME_PRIORITY_CLASS)
			SetPriorityClass(current_process, HIGH_PRIORITY_CLASS);


		//if (MessageBox(nullptr, "Minhooks?", "Choose", MB_OKCANCEL) == 1)
		//	g_ctx.test_hooks = true;

	
		//g_ctx.custom_saves = std::filesystem::directory_iterator(g_ctx.path / ("custom image\\"));
		CreateThread(nullptr, 0, &main, hModule, 0, nullptr); //-V718 //-V513
	}

	return TRUE;
}



__forceinline void setup_render()
{
	static auto create_font = [](const char* name, int size, int weight, DWORD flags) -> vgui::HFont
	{
		g_ctx.last_font_name = name;

		auto font = m_surface()->FontCreate();
		m_surface()->SetFontGlyphSet(font, name, size, weight, 0, 0, flags);

		return font;
	};

	fonts[LOGS] = create_font(crypt_str("Lucida Console"), 10, FW_MEDIUM, FONTFLAG_DROPSHADOW);
	fonts[ESP] = create_font(crypt_str("Verdana"), 15, FW_MEDIUM, FONTFLAG_OUTLINE);
	fonts[NAME] = create_font(crypt_str("Verdana"), 17, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[WATERMARK] = create_font(crypt_str("Verdana"), 15, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[SUBTABWEAPONS] = create_font(crypt_str("undefeated"), 10, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[KNIFES] = create_font(crypt_str("icomoon"), 13, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[GRENADES] = create_font(crypt_str("undefeated"), 20, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[INDICATORFONT] = create_font(crypt_str("Verdana"), 25, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	fonts[DAMAGE_MARKER] = create_font(crypt_str("CrashNumberingGothic"), 15, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);
	fonts[TAHOMA_NORMAL] = create_font(crypt_str("Tahoma"), 15, FW_HEAVY, FONTFLAG_NONE);
	fonts[VELOCITY] = create_font(crypt_str("Tahoma"), 20, FW_HEAVY, FONTFLAG_ANTIALIAS);

	g_ctx.last_font_name.clear();
}

__forceinline void setup_netvars()
{
	netvars::get().tables.clear();
	auto client = m_client()->GetAllClasses();

	if (!client)
		return;

	while (client)
	{
		auto recvTable = client->m_pRecvTable;

		if (recvTable)
			netvars::get().tables.emplace(std::string(client->m_pNetworkName), recvTable);

		client = client->m_pNext;
	}
}

__forceinline void setup_skins()
{
	auto items = std::ifstream(crypt_str("csgo/scripts/items/items_game_cdn.txt"));
	auto gameItems = std::string(std::istreambuf_iterator <char> { items }, std::istreambuf_iterator <char> { });

	if (!items.is_open())
		return;

	items.close();
	memory.initialize();

	for (auto i = 0; i <= memory.itemSchema()->paintKits.lastElement; i++)
	{
		auto paintKit = memory.itemSchema()->paintKits.memory[i].value;

		if (paintKit->id == 9001)
			continue;

		auto itemName = m_localize()->FindSafe(paintKit->itemName.buffer + 1);
		auto itemNameLength = WideCharToMultiByte(CP_UTF8, 0, itemName, -1, nullptr, 0, nullptr, nullptr);

		if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, itemName, -1, &name[0], itemNameLength, nullptr, nullptr))
		{
			if (paintKit->id < 10000)
			{
				if (auto pos = gameItems.find('_' + std::string{ paintKit->name.buffer } +'='); pos != std::string::npos && gameItems.substr(pos + paintKit->name.length).find('_' + std::string{ paintKit->name.buffer } +'=') == std::string::npos)
				{
					if (auto weaponName = gameItems.rfind(crypt_str("weapon_"), pos); weaponName != std::string::npos)
					{
						name.back() = ' ';
						name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';
					}
				}
				SkinChanger::skinKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);
			}
			else
			{
				std::string_view gloveName{ paintKit->name.buffer };
				name.back() = ' ';
				name += '(' + std::string{ gloveName.substr(0, gloveName.find('_')) } +')';
				SkinChanger::gloveKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);
			}
		}
	}

	std::sort(SkinChanger::skinKits.begin(), SkinChanger::skinKits.end());
	std::sort(SkinChanger::gloveKits.begin(), SkinChanger::gloveKits.end());
}

__forceinline void setup_hooks()
{
	Interfaces interfaces{};

	// get interface pointers.
	fm_client = interfaces.get< CHLClient* >(HASH("VClient"));

	g_netvars.init();

	static auto getforeignfallbackfontname = (DWORD)(util::FindSignature(crypt_str("vguimatsurface.dll"), g_ctx.signatures.at(9).c_str()));
	hooks::original_getforeignfallbackfontname = (DWORD)DetourFunction((PBYTE)getforeignfallbackfontname, (PBYTE)hooks::hooked_getforeignfallbackfontname); //-V206

	static auto setupbones = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(10).c_str()));
	hooks::original_setupbones = (DWORD)DetourFunction((PBYTE)setupbones, (PBYTE)hooks::hooked_setupbones); //-V206

	static auto doextrabonesprocessing = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(11).c_str()));
	hooks::original_doextrabonesprocessing = (DWORD)DetourFunction((PBYTE)doextrabonesprocessing, (PBYTE)hooks::hooked_doextrabonesprocessing); //-V206

	static auto standardblendingrules = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(12).c_str()));
	hooks::original_standardblendingrules = (DWORD)DetourFunction((PBYTE)standardblendingrules, (PBYTE)hooks::hooked_standardblendingrules); //-V206

	static auto updateclientsideanimation = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(13).c_str()));
	hooks::original_updateclientsideanimation = (DWORD)DetourFunction((PBYTE)updateclientsideanimation, (PBYTE)hooks::hooked_updateclientsideanimation); //-V206

	static auto physicssimulate = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(14).c_str()));
	hooks::original_physicssimulate = (DWORD)DetourFunction((PBYTE)physicssimulate, (PBYTE)hooks::hooked_physicssimulate);

	static auto modifyeyeposition = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(15).c_str()));
	hooks::original_modifyeyeposition = (DWORD)DetourFunction((PBYTE)modifyeyeposition, (PBYTE)hooks::hooked_modifyeyeposition);

	static auto calcviewmodelbob = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(16).c_str()));
	hooks::original_calcviewmodelbob = (DWORD)DetourFunction((PBYTE)calcviewmodelbob, (PBYTE)hooks::hooked_calcviewmodelbob);

	static auto shouldskipanimframe = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(17).c_str()));
	DetourFunction((PBYTE)shouldskipanimframe, (PBYTE)hooks::hooked_shouldskipanimframe);

	static auto checkfilecrcswithserver = (DWORD)(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(18).c_str()));
	DetourFunction((PBYTE)checkfilecrcswithserver, (PBYTE)hooks::hooked_checkfilecrcswithserver);

	static auto processinterpolatedlist = (DWORD)(util::FindSignature(crypt_str("client.dll"), g_ctx.signatures.at(19).c_str()));
	hooks::original_processinterpolatedlist = (DWORD)DetourFunction((byte*)processinterpolatedlist, (byte*)hooks::processinterpolatedlist); //-V206

	MH_Initialize();

	hooks::client_hook = new vmthook(reinterpret_cast<DWORD**>(m_client()));
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_fsn), 37); //-V107 //-V221
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_writeusercmddeltatobuffer), 24); //-V107 //-V221

	hooks::clientstate_hook = new vmthook(reinterpret_cast<DWORD**>((CClientState*)(uint32_t(m_clientstate()) + 0x8)));
	hooks::clientstate_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_packetstart), 5); //-V107 //-V221
	hooks::clientstate_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_packetend), 6); //-V107 //-V221
	
	hooks::panel_hook = new vmthook(reinterpret_cast<DWORD**>(m_panel())); //-V1032
	hooks::panel_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_painttraverse), 41); //-V107 //-V221

	const auto sendmessage_target = reinterpret_cast<void*>(get_virtual(SteamGameCoordinator, 0));
	const auto retrievemessage_target = reinterpret_cast<void*>(get_virtual(SteamGameCoordinator, 2));
	const auto gameventmanag_target = reinterpret_cast<void*>(get_virtual(m_eventmanager(), 9));

	hooks::clientmode_hook = new vmthook(reinterpret_cast<DWORD**>(m_clientmode()));
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_createmove), 24); //-V107 //-V221
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_postscreeneffects), 44); //-V107 //-V221
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_overrideview), 18); //-V107 //-V221
	//volta dps
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_drawfog), 17); //-V107 //-V221

	hooks::inputinternal_hook = new vmthook(reinterpret_cast<DWORD**>(m_inputinternal())); //-V114
	hooks::inputinternal_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_setkeycodestate), 91); //-V107 //-V221
	hooks::inputinternal_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_setmousecodestate), 92); //-V107 //-V221

	hooks::engine_hook = new vmthook(reinterpret_cast<DWORD**>(m_engine()));

	MH_CreateHook(sendmessage_target, &shonax_hooks::GCSendMessageHook, reinterpret_cast<void**>(&shonax_hooks::oGCSendMessage));
	MH_CreateHook(retrievemessage_target, &shonax_hooks::GCRetrieveMessageHook, reinterpret_cast<void**>(&shonax_hooks::oGCRetrieveMessage));
	MH_CreateHook(gameventmanag_target, &shonax_hooks::FireEventClientSide, reinterpret_cast<void**>(&shonax_hooks::oFireEventClientSidefn));

	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_isconnected), 27); //-V107 //-V221
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getscreenaspectratio), 101); //-V107 //-V221
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_ishltv), 93); //-V107 //-V221

	hooks::renderview_hook = new vmthook(reinterpret_cast<DWORD**>(m_renderview()));
	hooks::renderview_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_sceneend), 9); //-V107 //-V221

	hooks::materialsys_hook = new vmthook(reinterpret_cast<DWORD**>(m_materialsystem())); //-V1032
	hooks::materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_beginframe), 42); //-V107 //-V221
	hooks::materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getmaterial), 84); //-V107 //-V221

	hooks::modelrender_hook = new vmthook(reinterpret_cast<DWORD**>(m_modelrender()));
	hooks::modelrender_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_dme), 21); //-V107 //-V221

	hooks::surface_hook = new vmthook(reinterpret_cast<DWORD**>(m_surface()));
	hooks::surface_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_lockcursor), 67); //-V107 //-V221

	hooks::bspquery_hook = new vmthook(reinterpret_cast<DWORD**>(m_engine()->GetBSPTreeQuery()));
	hooks::bspquery_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_listleavesinbox), 6); //-V107 //-V221

	hooks::prediction_hook = new vmthook(reinterpret_cast<DWORD**>(m_prediction())); //-V1032
	hooks::prediction_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_runcommand), 19); //-V107 //-V221

	hooks::trace_hook = new vmthook(reinterpret_cast<DWORD**>(m_trace()));
	hooks::trace_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_clip_ray_collideable), 4); //-V107 //-V221
	hooks::trace_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_trace_ray), 5); //-V107 //-V221

	hooks::filesystem_hook = new vmthook(reinterpret_cast<DWORD**>(util::FindSignature(crypt_str("engine.dll"), g_ctx.signatures.at(20).c_str()) + 0x2));
	hooks::filesystem_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_loosefileallowed), 128); //-V107 //-V221

	D3DDEVICE_CREATION_PARAMETERS creationParameters;
	while (FAILED(m_device()->GetCreationParameters(&creationParameters)))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	INIT::Window = creationParameters.hFocusWindow;

	//while (!(INIT::Window = IFH(FindWindow)(crypt_str("Valve001"), nullptr)))
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//INIT::OldWindow = (WNDPROC)IFH(SetWindowLongPtr)(INIT::Window, GWL_WNDPROC, (LONG_PTR)hooks::Hooked_WndProc);
	INIT::OldWindow = (WNDPROC)g_winapi.SetWindowLongA(INIT::Window, GWL_WNDPROC, (LONG_PTR)hooks::Hooked_WndProc);

	hooks::directx_hook = new vmthook(reinterpret_cast<DWORD**>(m_device()));
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::Hooked_EndScene_Reset), 16); //-V107 //-V221
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_present), 17); //-V107 //-V221
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::Hooked_EndScene), 42); //-V107 //-V221

	MH_EnableHook(MH_ALL_HOOKS);

	g_netvars.SetProxy(HASH("DT_BaseViewModel"), HASH("m_nSequence"), shonax_hooks::RecvProxy, shonax_hooks::sequence_hook);
	g_netvars.SetProxy(HASH("DT_BaseViewModel"), HASH("m_nModelIndex"), shonax_hooks::Viewmodel, shonax_hooks::viewmodel_hook);
	//g_netvars.SetProxy(HASH("DT_CSRagdoll"), HASH("m_vecForce"), Force_proxy, m_Force_original);

	hooks::hooked_events.RegisterSelf();
}