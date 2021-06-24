#pragma once
#include "utils/math.hpp"
#include "hooks/hooks.hpp"
#include "sdk/math/Vector.hpp"
#include "sdk/structs.hpp"
#include "utils/singleton.h"

class walk_bot : public singleton<walk_bot>
{
public:
	Vector vSamplingSpacesS[257], vNewLocation[257];
	int oldOne = 0, newTwo = 0, current = 0;
	void WalkBot_painttraverse();
	bool MarksIsVisible(player_t* local, Vector& vTo);
	void MakeFebsMove(CUserCmd* cmd, player_t* local);
};
