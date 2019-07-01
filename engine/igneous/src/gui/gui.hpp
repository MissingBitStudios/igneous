#pragma once

#include <imgui.h>

namespace gui
{
	enum Theme
	{
		CHERRY,
		NUM_THEMES
	};

	void setTheme(Theme theme);
}