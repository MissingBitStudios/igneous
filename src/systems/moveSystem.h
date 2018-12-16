#pragma once

#include "../components/transformationComponent.h"
#include <entt/entt.hpp>

namespace MoveSystem
{
	void update(float dt, entt::registry<> &registry);

	void onKey(int key, int scancode, int action, int mods);

	extern float time;
	extern int dir;
	extern bool shouldMove;
}