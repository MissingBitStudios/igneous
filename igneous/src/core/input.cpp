#include "igneous/core/input.hpp"

#include <imgui/imgui.h>
#include <stb/stb_image.h>

#include "igneous/core/log.hpp"
#include "igneous/renderer/renderer.hpp"

namespace igneous {
namespace input
{
	// Input callbacks
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		if (key != GLFW_KEY_UNKNOWN)
		{
			if (action == GLFW_PRESS)
			{
				io.KeysDown[key] = input::keys[key] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				io.KeysDown[key] = input::keys[key] = false;
			}
		}

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

		if (!io.WantCaptureKeyboard)
		{
			app->onKey(key, scancode, action, mods);
			console::onKey(key, scancode, action, mods);
		}
	}

	void charCallback(GLFWwindow* window, unsigned int codepoint)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacter(codepoint);
		app->onChar(codepoint);
	}

	void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		app->onCharMods(codepoint, mods);
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		if (button >= 0 && button < IM_ARRAYSIZE(io.MouseDown))
		{
			if (action == GLFW_PRESS)
			{
				io.MouseDown[button] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				io.MouseDown[button] = false;
			}
		}

		if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST)
		{
			if (action == GLFW_PRESS)
			{
				input::mouseButtons[button] = true;
			}
			else if (action == GLFW_RELEASE)
			{
				input::mouseButtons[button] = false;
			}
		}

		if (!io.WantCaptureMouse)
		{
			app->onMouseButton(button, action, mods);
		}
	}

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		input::mouseX = xpos;
		input::mouseY = ypos;
		app->onCursorPos(xpos, ypos);
	}

	void cursorEnterCallback(GLFWwindow* window, int entered)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		app->onCursorEnter(entered);
	}

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += (float)xoffset;
		io.MouseWheel += (float)yoffset;
		input::scrollX += (float)xoffset;
		input::scrollY += (float)yoffset;
		app->onScroll(xoffset, yoffset);
	}

	void dropCallback(GLFWwindow* window, int count, const char** paths)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		app->onDrop(count, paths);
	}

	void windowSizeCallback(GLFWwindow* window, int width, int height)
	{
		Application* app = (Application*)glfwGetWindowUserPointer(window);
		input::width = width;
		input::height = height;
		renderer::reset();
		app->onWindowSize(width, height);
	}

	void cursorVisibleCallback(float oldValue, float newValue)
	{
		input::setCursorVisible(newValue);
	}

	void quitCallback(const std::string& name, const arg_list& args)
	{
		glfwSetWindowShouldClose(window, true);
	}

	void init()
	{
		IG_CORE_INFO("Initializing Input");
		IG_CORE_INFO("Initializing GLFW");

		// Initialize glfw
		if (!glfwInit())
		{
			IG_CORE_CRITICAL("Could not initialize GLFW");
		}

		// Create a window
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(input::width, input::height, input::title, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			IG_CORE_CRITICAL("Could not open a window");
		}

		IG_CORE_INFO("Setting window title and icon");
		GLFWimage images[4];
		images[0].pixels = stbi_load("res/icons/icon16.png", &images[0].width, &images[0].height, 0, 4);
		images[1].pixels = stbi_load("res/icons/icon32.png", &images[1].width, &images[1].height, 0, 4);
		images[2].pixels = stbi_load("res/icons/icon48.png", &images[2].width, &images[2].height, 0, 4);
		images[3].pixels = stbi_load("res/icons/icon256.png", &images[3].width, &images[3].height, 0, 4);
		glfwSetWindowIcon(window, 4, images);
		stbi_image_free(images[0].pixels);
		stbi_image_free(images[1].pixels);
		stbi_image_free(images[2].pixels);
		stbi_image_free(images[3].pixels);
		IG_CORE_INFO("Window title and icon set");

		// Setup input callbacks
		glfwSetWindowUserPointer(window, app);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCharCallback(window, charCallback);
		glfwSetCharModsCallback(window, charModsCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
		glfwSetCursorEnterCallback(window, cursorEnterCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetDropCallback(window, dropCallback);
		glfwSetWindowSizeCallback(window, windowSizeCallback);
		IG_CORE_INFO("GLFW Initializied");

		console::command("quit", quitCallback);

		console::variable("cursor_visible", 0, cursorVisibleCallback);

		console::bind(GLFW_KEY_T, "^cursor_visible");
		console::bind(GLFW_KEY_ESCAPE, "quit");
		IG_CORE_INFO("Input Initialized");
	}

	void shutdown()
	{
		IG_CORE_INFO("Shutingdown Input");
		glfwTerminate();
		IG_CORE_INFO("Input Shutdown");
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

	void setSize(int width, int height)
	{
		glfwSetWindowSize(window, width, height);
	}

	bool keys[GLFW_KEY_LAST + 1] = { 0 };
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { 0 };
	double scrollX, scrollY = 0;
	double mouseX, mouseY = 0;
	int width, height = 0;
	const char* title;
	GLFWwindow* window = nullptr;
	Application* app;
}
} // end namespace igneous
