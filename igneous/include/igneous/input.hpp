#pragma once

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

namespace igneous {
#define IN_KEY_SINK(callback) Input::keySignal.sink().connect<callback>()
#define IN_MOUSE_SINK(callback) Input::mouseSignal.sink().connect<callback>()
#define IN_SCROLL_SINK(callback) Input::scrollSignal.sink().connect<callback>()
#define IN_CURSOR_POS_SINK(callback) Input::cursorPosSignal.sink().connect<callback>()
#define IN_CURSOR_ENTER_SINK(callback) Input::cursorEnterSignal.sink().connect<callback>()

namespace Input
{
	void Init(GLFWwindow* window);

	void onKey(int key, int scancode, int action, int mods);
	void onMouseButton(int button, int action, int mods);
	void onScroll(double xoffset, double yoffset);
	void onCursorPos(double xpos, double ypos);
	void onCursorEnter(int entered);
	void setCursorPos(GLFWwindow* window, double xpos, double ypos);
	void setCursorVisible(GLFWwindow* window, bool visible);

	extern bool keys[GLFW_KEY_LAST + 1];
	extern bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
	extern double scrollX, scrollY;
	extern double mouseX, mouseY;
	extern int width;
	extern int height;
	extern GLFWwindow* window;

	extern entt::sigh<void(int, int, int, int)> keySignal;
	extern entt::sigh<void(int, int, int)> mouseSignal;
	extern entt::sigh<void(double, double)> scrollSignal;
	extern entt::sigh<void(double, double)> cursorPosSignal;
	extern entt::sigh<void(int)> cursorEnterSignal;
}

/*! @defgroup keys Keyboard keys
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  These key codes are inspired by the _USB HID Usage Tables v1.12_ (p. 53-60),
 *  but re-arranged to map to 7-bit ASCII for printable keys (function keys are
 *  put in the 256+ range).
 *
 *  The naming of the key codes follow these rules:
 *   - The US keyboard layout is used
 *   - Names of printable alpha-numeric characters are used (e.g. "A", "R",
 *     "3", etc.)
 *   - For non-alphanumeric characters, Unicode:ish names are used (e.g.
 *     "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
 *     correspond to the Unicode standard (usually for brevity)
 *   - Keys that lack a clear US mapping are named "WORLD_x"
 *   - For non-printable keys, custom names are used (e.g. "F4",
 *     "BACKSPACE", etc.)
 *
 *  @ingroup input
 *  @{
 */

 /* The unknown key */
#define IG_KEY_UNKNOWN GLFW_KEY_UNKNOWN

/* Printable keys */
#define IG_KEY_SPACE GLFW_KEY_SPACE
#define IG_KEY_APOSTROPHE GLFW_KEY_APOSTROPHE  /* ' */
#define IG_KEY_COMMA GLFW_KEY_COMMA  /* , */
#define IG_KEY_MINUS GLFW_KEY_MINUS  /* - */
#define IG_KEY_PERIOD GLFW_KEY_PERIOD  /* . */
#define IG_KEY_SLASH GLFW_KEY_SLASH  /* / */
#define IG_KEY_0 GLFW_KEY_0
#define IG_KEY_1 GLFW_KEY_1
#define IG_KEY_2 GLFW_KEY_2
#define IG_KEY_3 GLFW_KEY_3
#define IG_KEY_4 GLFW_KEY_4
#define IG_KEY_5 GLFW_KEY_5
#define IG_KEY_6 GLFW_KEY_6
#define IG_KEY_7 GLFW_KEY_7
#define IG_KEY_8 GLFW_KEY_8
#define IG_KEY_9 GLFW_KEY_9
#define IG_KEY_SEMICOLON GLFW_KEY_SEMICOLON  /* ; */
#define IG_KEY_EQUAL GLFW_KEY_EQUAL  /* = */
#define IG_KEY_A GLFW_KEY_A
#define IG_KEY_B GLFW_KEY_B
#define IG_KEY_C GLFW_KEY_C
#define IG_KEY_D GLFW_KEY_D
#define IG_KEY_E GLFW_KEY_E
#define IG_KEY_F GLFW_KEY_F
#define IG_KEY_G GLFW_KEY_G
#define IG_KEY_H GLFW_KEY_H
#define IG_KEY_I GLFW_KEY_I
#define IG_KEY_J GLFW_KEY_J
#define IG_KEY_K GLFW_KEY_K
#define IG_KEY_L GLFW_KEY_L
#define IG_KEY_M GLFW_KEY_M
#define IG_KEY_N GLFW_KEY_N
#define IG_KEY_O GLFW_KEY_O
#define IG_KEY_P GLFW_KEY_P
#define IG_KEY_Q GLFW_KEY_Q
#define IG_KEY_R GLFW_KEY_R
#define IG_KEY_S GLFW_KEY_S
#define IG_KEY_T GLFW_KEY_T
#define IG_KEY_U GLFW_KEY_U
#define IG_KEY_V GLFW_KEY_V
#define IG_KEY_W GLFW_KEY_W
#define IG_KEY_X GLFW_KEY_X
#define IG_KEY_Y GLFW_KEY_Y
#define IG_KEY_Z GLFW_KEY_Z
#define IG_KEY_LEFT_BRACKET GLFW_KEY_LEFT_BRACKET  /* [ */
#define IG_KEY_BACKSLASH GLFW_KEY_BACKSLASH  /* \ */
#define IG_KEY_RIGHT_BRACKET GLFW_KEY_RIGHT_BRACKET  /* ] */
#define IG_KEY_GRAVE_ACCENT GLFW_KEY_GRAVE_ACCENT  /* ` */
#define IG_KEY_WORLD_1 GLFW_KEY_WORLD_1 /* non-US #1 */
#define IG_KEY_WORLD_2 GLFW_KEY_WORLD_2 /* non-US #2 */

/* Function keys */
#define IG_KEY_ESCAPE GLFW_KEY_ESCAPE
#define IG_KEY_ENTER GLFW_KEY_ENTER
#define IG_KEY_TAB GLFW_KEY_TAB
#define IG_KEY_BACKSPACE GLFW_KEY_BACKSPACE
#define IG_KEY_INSERT GLFW_KEY_INSERT
#define IG_KEY_DELETE GLFW_KEY_DELETE
#define IG_KEY_RIGHT GLFW_KEY_RIGHT
#define IG_KEY_LEFT GLFW_KEY_LEFT
#define IG_KEY_DOWN GLFW_KEY_DOWN
#define IG_KEY_UP GLFW_KEY_UP
#define IG_KEY_PAGE_UP GLFW_KEY_PAGE_UP
#define IG_KEY_PAGE_DOWN GLFW_KEY_PAGE_DOWN
#define IG_KEY_HOME GLFW_KEY_HOME
#define IG_KEY_END GLFW_KEY_END
#define IG_KEY_CAPS_LOCK GLFW_KEY_CAPS_LOCK
#define IG_KEY_SCROLL_LOCK GLFW_KEY_SCROLL_LOCK
#define IG_KEY_NUM_LOCK GLFW_KEY_NUM_LOCK
#define IG_KEY_PRINT_SCREEN GLFW_KEY_PRINT_SCREEN
#define IG_KEY_PAUSE GLFW_KEY_PAUSE
#define IG_KEY_F1 GLFW_KEY_F1
#define IG_KEY_F2 GLFW_KEY_F2
#define IG_KEY_F3 GLFW_KEY_F3
#define IG_KEY_F4 GLFW_KEY_F4
#define IG_KEY_F5 GLFW_KEY_F5
#define IG_KEY_F6 GLFW_KEY_F6
#define IG_KEY_F7 GLFW_KEY_F7
#define IG_KEY_F8 GLFW_KEY_F8
#define IG_KEY_F9 GLFW_KEY_F9
#define IG_KEY_F10 GLFW_KEY_F10
#define IG_KEY_F11 GLFW_KEY_F11
#define IG_KEY_F12 GLFW_KEY_F12
#define IG_KEY_F13 GLFW_KEY_F13
#define IG_KEY_F14 GLFW_KEY_F14
#define IG_KEY_F15 GLFW_KEY_F15
#define IG_KEY_F16 GLFW_KEY_F16
#define IG_KEY_F17 GLFW_KEY_F17
#define IG_KEY_F18 GLFW_KEY_F18
#define IG_KEY_F19 GLFW_KEY_F19
#define IG_KEY_F20 GLFW_KEY_F20
#define IG_KEY_F21 GLFW_KEY_F21
#define IG_KEY_F22 GLFW_KEY_F22
#define IG_KEY_F23 GLFW_KEY_F23
#define IG_KEY_F24 GLFW_KEY_F24
#define IG_KEY_F25 GLFW_KEY_F25
#define IG_KEY_KP_0 GLFW_KEY_KP_0
#define IG_KEY_KP_1 GLFW_KEY_KP_1
#define IG_KEY_KP_2 GLFW_KEY_KP_2
#define IG_KEY_KP_3 GLFW_KEY_KP_3
#define IG_KEY_KP_4 GLFW_KEY_KP_4
#define IG_KEY_KP_5 GLFW_KEY_KP_5
#define IG_KEY_KP_6 GLFW_KEY_KP_6
#define IG_KEY_KP_7 GLFW_KEY_KP_7
#define IG_KEY_KP_8 GLFW_KEY_KP_8
#define IG_KEY_KP_9 GLFW_KEY_KP_9
#define IG_KEY_KP_DECIMAL GLFW_KEY_KP_DECIMAL
#define IG_KEY_KP_DIVIDE GLFW_KEY_KP_DIVIDE
#define IG_KEY_KP_MULTIPLY GLFW_KEY_KP_MULTIPLY
#define IG_KEY_KP_SUBTRACT GLFW_KEY_KP_SUBTRACT
#define IG_KEY_KP_ADD GLFW_KEY_KP_ADD
#define IG_KEY_KP_ENTER GLFW_KEY_KP_ENTER
#define IG_KEY_KP_EQUAL GLFW_KEY_KP_EQUAL
#define IG_KEY_LEFT_SHIFT GLFW_KEY_LEFT_SHIFT
#define IG_KEY_LEFT_CONTROL GLFW_KEY_LEFT_CONTROL
#define IG_KEY_LEFT_ALT GLFW_KEY_LEFT_ALT
#define IG_KEY_LEFT_SUPER GLFW_KEY_LEFT_SUPER
#define IG_KEY_RIGHT_SHIFT GLFW_KEY_RIGHT_SHIFT
#define IG_KEY_RIGHT_CONTROL GLFW_KEY_RIGHT_CONTROL
#define IG_KEY_RIGHT_ALT GLFW_KEY_RIGHT_ALT
#define IG_KEY_RIGHT_SUPER GLFW_KEY_RIGHT_SUPER
#define IG_KEY_MENU GLFW_KEY_MENU

#define IG_KEY_LAST IG_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *
 *  See [key input](@ref input_key) for how these are used.
 *
 *  @ingroup input
 *  @{ */

 /*! @brief If this bit is set one or more Shift keys were held down.
  */
#define IG_MOD_SHIFT GLFW_MOD_SHIFT
  /*! @brief If this bit is set one or more Control keys were held down.
   */
#define IG_MOD_CONTROL GLFW_MOD_CONTROL
   /*! @brief If this bit is set one or more Alt keys were held down.
	*/
#define IG_MOD_ALT GLFW_MOD_ALT
	/*! @brief If this bit is set one or more Super keys were held down.
	 */
#define IG_MOD_SUPER GLFW_MOD_SUPER

	 /*! @} */

	 /*! @defgroup buttons Mouse buttons
	  *
	  *  See [mouse button input](@ref input_mouse_button) for how these are used.
	  *
	  *  @ingroup input
	  *  @{ */
#define IG_MOUSE_BUTTON_1 GLFW_MOUSE_BUTTON_1
#define IG_MOUSE_BUTTON_2 GLFW_MOUSE_BUTTON_2
#define IG_MOUSE_BUTTON_3 GLFW_MOUSE_BUTTON_3
#define IG_MOUSE_BUTTON_4 GLFW_MOUSE_BUTTON_4
#define IG_MOUSE_BUTTON_5 GLFW_MOUSE_BUTTON_5
#define IG_MOUSE_BUTTON_6 GLFW_MOUSE_BUTTON_6
#define IG_MOUSE_BUTTON_7 GLFW_MOUSE_BUTTON_7
#define IG_MOUSE_BUTTON_8 GLFW_MOUSE_BUTTON_8
#define IG_MOUSE_BUTTON_LAST GLFW_MOUSE_BUTTON_LAST
#define IG_MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_LEFT
#define IG_MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_RIGHT
#define IG_MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_MIDDLE
	  /*! @} */

	  /*! @defgroup joysticks Joysticks
	   *
	   *  See [joystick input](@ref joystick) for how these are used.
	   *
	   *  @ingroup input
	   *  @{ */
#define IG_JOYSTICK_1 GLFW_JOYSTICK_1
#define IG_JOYSTICK_2 GLFW_JOYSTICK_2
#define IG_JOYSTICK_3 GLFW_JOYSTICK_3
#define IG_JOYSTICK_4 GLFW_JOYSTICK_4
#define IG_JOYSTICK_5 GLFW_JOYSTICK_5
#define IG_JOYSTICK_6 GLFW_JOYSTICK_6
#define IG_JOYSTICK_7 GLFW_JOYSTICK_7
#define IG_JOYSTICK_8 GLFW_JOYSTICK_8
#define IG_JOYSTICK_9 GLFW_JOYSTICK_9
#define IG_JOYSTICK_10 GLFW_JOYSTICK_10
#define IG_JOYSTICK_11 GLFW_JOYSTICK_11
#define IG_JOYSTICK_12 GLFW_JOYSTICK_12
#define IG_JOYSTICK_13 GLFW_JOYSTICK_13
#define IG_JOYSTICK_14 GLFW_JOYSTICK_14
#define IG_JOYSTICK_15 GLFW_JOYSTICK_15
#define IG_JOYSTICK_16 GLFW_JOYSTICK_16
#define IG_JOYSTICK_LAST IG_JOYSTICK_LAST
} // end namespace igneous
