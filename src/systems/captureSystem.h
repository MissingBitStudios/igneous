#pragma once

#include <iostream>

#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>

namespace CaptureSystem
{
	void screenshot();

	void record();

	void onKey(int key, int scancode, int action, int mods);

	extern bool capture;
}