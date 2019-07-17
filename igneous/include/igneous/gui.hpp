#pragma once

namespace igneous {
	namespace gui
	{
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
