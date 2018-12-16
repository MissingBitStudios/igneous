#include "captureSystem.h"

void CaptureSystem::screenshot()
{
	bgfx::requestScreenShot(BGFX_INVALID_HANDLE, "capture/screenshot");
	std::cout << "Screenshot!" << std::endl;
}

void CaptureSystem::record()
{
	capture = !capture;

	if (capture)
	{
		std::cout << "Capture: " << "On" << "\n";
	}
	else
	{
		std::cout << "Capture: " << "Off" << "\n";
	}
}

void CaptureSystem::onKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_F2:
			screenshot();
			break;
		case GLFW_KEY_F9:
			record();
			break;
		default:
			break;
		}
	}
}

bool CaptureSystem::capture = false;