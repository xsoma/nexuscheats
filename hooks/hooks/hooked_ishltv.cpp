// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "..\hooks.hpp"

using IsHLTV_t = bool(__thiscall*)(void*);

bool __fastcall hooks::hooked_ishltv(void* ecx, void* edx) 
{
	static auto original_fn = engine_hook->get_func_address <IsHLTV_t> (93);

	if (g_ctx.globals.setuping_bones)
		return true;

	//se eu botasse check de alive n ia fazer sentido
	//pq a fun??o de update ? justamente pra trocar
	if (g_ctx.globals.updating_animation && m_clientstate()->iDeltaTick != -1)
		return true;// esse nao ? o original, pode ser isso, talvez. Vc tirou isso daonde?, eu troquei o resolver.cpp/h e pq mudou isso junto? pq tava no yougamebosta, mas nao mudei nada no hook

	static auto accumulate_layers_call = util::FindSignature(crypt_str("client.dll"), crypt_str("84 C0 75 0D F6 87"));
	static auto setup_velocity_call = util::FindSignature(crypt_str("client.dll"), crypt_str("84 C0 75 38 8B 0D ? ? ? ? 8B 01 8B 80 ? ? ? ? FF D0"));

	if (uintptr_t(_ReturnAddress()) == accumulate_layers_call || uintptr_t(_ReturnAddress()) == setup_velocity_call)
		return true;

	return original_fn(ecx);
}