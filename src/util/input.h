#pragma once

#include <entt/entt.hpp>
#include <GLFW/glfw3.h>

namespace Input
{
	void onKey(int key, int scancode, int action, int mods);

	void onMouseButton(int button, int action, int mods);

	extern bool keys[GLFW_KEY_LAST + 1];
	extern bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];

	extern entt::sigh<void(int, int, int, int)> keySignal;
	extern entt::sigh<void(int, int, int)> mouseSignal;
}