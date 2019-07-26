#pragma once

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

namespace igneous {
namespace gui
{
	/*! @cond */
	void init();
	void update(float dt);
	void reset();
	void render(ImDrawData* drawData);
	void shutdown();
	/*! @endcond */

	/*! Themes to use with `setTheme(Theme theme)` */
	enum Theme
	{
		CHERRY, /*!< Dark with cherry red accent */
		CLASSIC, /*!< Default theme*/
		DARK, /*!< Dark */
		LIGHT, /*!< Light */
		NUM_THEMES /*!< Number of gui themes */
	};

	/*! Sets the color theme for all gui elements.
	* \param theme the `Theme` to use.
	*/
	void setTheme(Theme theme);
}
} // end namespace igneous
