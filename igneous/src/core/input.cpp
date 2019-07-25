#include "igneous/core/input.hpp"

#include <imgui/imgui.h>

#include "igneous/core/log.hpp"

namespace igneous {
namespace input
{
	void cursorVisibleCallback(float oldValue, float newValue)
	{
		input::setCursorVisible(newValue);
	}

	void quitCallback(const std::string& name, const arg_list& args)
	{
		glfwSetWindowShouldClose(window, true);
	}

	void init(GLFWwindow* win)
	{
		IG_CORE_INFO("Initializing Input");
		window = win;
		Console& console = Console::getInstance();
		console.command("quit", quitCallback);

		console.variable("cursor_visible", 0, cursorVisibleCallback);

		console.bind(GLFW_KEY_T, "^cursor_visible");
		console.bind(GLFW_KEY_ESCAPE, "quit");
		IG_CORE_INFO("Input Initialized");
	}

	void setCursorPos(double xpos, double ypos)
	{
		glfwSetCursorPos(window, xpos, ypos);
	}

	void setCursorVisible(bool visible)
	{
		glfwSetInputMode(window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	const char* getTitle()
	{
		return title;
	}

	void setTitle(const char* newTitle)
	{
		title = newTitle;
		glfwSetWindowTitle(window, title);
	}

	bool keys[GLFW_KEY_LAST + 1] = { 0 };
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
	double scrollX, scrollY = 0;
	double mouseX, mouseY = 0;
	int width, height = 0;
	const char* title;
	GLFWwindow* window = nullptr;
}
} // end namespace igneous
