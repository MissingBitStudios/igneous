#pragma once

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

namespace Input
{
	void onKey(int key, int scancode, int action, int mods);

	void onMouseButton(int button, int action, int mods);

	void onScroll(double xoffset, double yoffset);

	void onCursorPos(double xpos, double ypos);

	void onCursorEnter(int entered);

	void setCursorPos(GLFWwindow* window, double xpos, double ypos);

	void setCursorVisible(GLFWwindow* window, bool visible);

	extern bool keys[GLFW_KEY_LAST + 1];
	extern bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
	extern double scrollX, scrollY;
	extern double mouseX, mouseY;

	extern entt::sigh<void(int, int, int, int)> keySignal;
	extern entt::sigh<void(int, int, int)> mouseSignal;
	extern entt::sigh<void(double, double)> scrollSignal;
	extern entt::sigh<void(double, double)> cursorPosSignal;
	extern entt::sigh<void(int)> cursorEnterSignal;
}