#include "input.h"

#include <imgui.h>

namespace Input
{
	void Init(GLFWwindow* win)
	{
		IG_CORE_INFO("Initializing Input");
		window = win;
		Console& console = Console::getInstance();
		console.command("toggle_cursor", toggleCursorCallback);
		console.command("quit", quitCallback);

		console.bind(GLFW_KEY_T, "toggle_cursor");
		console.bind(GLFW_KEY_ESCAPE, "quit");
		IG_CORE_INFO("Input Initialized");
	}

	void onKey(int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
		keySignal.publish(key, scancode, action, mods);
	}
	
	void onMouseButton(int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			mouseButtons[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			mouseButtons[button] = false;
		}
		mouseSignal.publish(button, action, mods);
	}

	void onScroll(double xoffset, double yoffset)
	{
		scrollX = xoffset;
		scrollY = yoffset;
		scrollSignal.publish(xoffset, yoffset);
	}

	void onCursorPos(double xpos, double ypos)
	{
		mouseX = xpos;
		mouseY = ypos;
		cursorPosSignal.publish(xpos, ypos);
	}

	void onCursorEnter(int entered)
	{
		cursorEnterSignal.publish(entered);
	}

	void setCursorPos(GLFWwindow* window, double xpos, double ypos)
	{
		glfwSetCursorPos(window, xpos, ypos);
	}

	void setCursorVisible(GLFWwindow* window, bool visible)
	{
		glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	void toggleCursorCallback(arg_list args)
	{
		Input::setCursorVisible(window, glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);
	}

	void quitCallback(arg_list args)
	{
		glfwSetWindowShouldClose(window, true);
	}

	bool keys[GLFW_KEY_LAST + 1] = { 0 };
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
	double scrollX, scrollY = 0;
	double mouseX, mouseY = 0;
	GLFWwindow* window = nullptr;

	entt::sigh<void(int, int, int, int)> keySignal;
	entt::sigh<void(int, int, int)> mouseSignal;
	entt::sigh<void(double, double)> scrollSignal;
	entt::sigh<void(double, double)> cursorPosSignal;
	entt::sigh<void(int)> cursorEnterSignal;
}