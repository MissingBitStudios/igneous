#include "input.h"

void Input::onKey(int key, int scancode, int action, int mods)
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

void Input::onMouseButton(int button, int action, int mods)
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

void Input::onScroll(double xoffset, double yoffset)
{
	scrollX = xoffset;
	scrollY = yoffset;
	scrollSignal.publish(xoffset, yoffset);
}

void Input::onCursorPos(double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	cursorPosSignal.publish(xpos, ypos);
}

void Input::onCursorEnter(int entered)
{
	cursorEnterSignal.publish(entered);
}

bool Input::keys[GLFW_KEY_LAST + 1] = { 0 };
bool Input::mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
double Input::scrollX, Input::scrollY = 0;
double Input::mouseX, Input::mouseY = 0;

entt::sigh<void(int, int, int, int)> Input::keySignal;
entt::sigh<void(int, int, int)> Input::mouseSignal;
entt::sigh<void(double, double)> Input::scrollSignal;
entt::sigh<void(double, double)> Input::cursorPosSignal;
entt::sigh<void(int)> Input::cursorEnterSignal;