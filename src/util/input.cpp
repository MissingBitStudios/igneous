#include "input.h"

namespace Input
{
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

	bool keys[GLFW_KEY_LAST + 1] = { 0 };
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
	double scrollX, scrollY = 0;
	double mouseX, mouseY = 0;

	entt::sigh<void(int, int, int, int)> keySignal;
	entt::sigh<void(int, int, int)> mouseSignal;
	entt::sigh<void(double, double)> scrollSignal;
	entt::sigh<void(double, double)> cursorPosSignal;
	entt::sigh<void(int)> cursorEnterSignal;
}