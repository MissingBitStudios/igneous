#pragma once

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "console/console.hpp"

#define IN_KEY_SINK(callback) Input::keySignal.sink().connect<callback>()
#define IN_MOUSE_SINK(callback) Input::mouseSignal.sink().connect<callback>()
#define IN_SCROLL_SINK(callback) Input::scrollSignal.sink().connect<callback>()
#define IN_CURSOR_POS_SINK(callback) Input::cursorPosSignal.sink().connect<callback>()
#define IN_CURSOR_ENTER_SINK(callback) Input::cursorEnterSignal.sink().connect<callback>()

namespace Input
{
	void Init(GLFWwindow* window);

	void cursorVisibleCallback(const std::string& oldValue, const std::string& newValue);
	void onKey(int key, int scancode, int action, int mods);
	void onMouseButton(int button, int action, int mods);
	void onScroll(double xoffset, double yoffset);
	void onCursorPos(double xpos, double ypos);
	void onCursorEnter(int entered);
	void setCursorPos(GLFWwindow* window, double xpos, double ypos);
	void setCursorVisible(GLFWwindow* window, bool visible);
	void quitCallback(const std::string& name, const arg_list& args);

	extern bool keys[GLFW_KEY_LAST + 1];
	extern bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
	extern double scrollX, scrollY;
	extern double mouseX, mouseY;
	extern GLFWwindow* window;

	extern entt::sigh<void(int, int, int, int)> keySignal;
	extern entt::sigh<void(int, int, int)> mouseSignal;
	extern entt::sigh<void(double, double)> scrollSignal;
	extern entt::sigh<void(double, double)> cursorPosSignal;
	extern entt::sigh<void(int)> cursorEnterSignal;
}