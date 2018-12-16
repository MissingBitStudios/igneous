#include "captureSystem.h"

namespace CaptureSystem
{
	void screenshot()
	{
		bgfx::requestScreenShot(BGFX_INVALID_HANDLE, "capture/screenshot");
		std::cout << "Screenshot!" << std::endl;
	}

	void record()
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

	void onKey(int key, int scancode, int action, int mods)
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

	bool capture = false;
}