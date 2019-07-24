#include "igneous/ecs/systems/captureSystem.hpp"

#include "igneous/core/log.hpp"

namespace igneous {
namespace CaptureSystem
{
	void screenshot()
	{
		bgfx::requestScreenShot(BGFX_INVALID_HANDLE, "capture/screenshot");
		IG_CORE_INFO("Screenshot requested");
	}

	void record()
	{
		capture = !capture;
		IG_CORE_INFO("Capture: {}", capture?"ON":"OFF");
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
} // end namespace igneous
