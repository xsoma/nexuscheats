#pragma once

#include "..\..\includes.hpp"
#include "..\..\sdk\structs.hpp"

class weapon_t;
class Box;

class worldesp : public singleton <worldesp> 
{
public:
	void paint_traverse();
	void DrawBeam(Vector src, Vector end, Color color);
	void skybox_changer();
	void fog_changer();
	void no3dcu();
	void Wireframe();
	void NoShadows();
	void noeffects();
	void PlayerTrails();
	void RenderAsus();
	
	void world_modulation(entity_t* entity);
	void molotov_timer(entity_t* entity);
	void smoke_timer(entity_t* entity);
	void grenade_projectiles(entity_t* entity);
	void bomb_timer(entity_t* entity);
	void bomb_timer2(entity_t* entity);

	void dropped_weapons(entity_t* entity);

	bool changed = false;
	std::string backup_skybox = "";
};