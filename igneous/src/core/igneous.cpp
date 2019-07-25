#include "igneous/igneous.hpp"

#include <assimp/version.h>
#include <btBulletDynamicsCommon.h>
#include <bx/string.h>
#include <entt/entt.hpp>
#include <entt/config/version.h>
#include <glm/gtc/matrix_transform.hpp>
#include <RakNetVersion.h>
#include <spdlog/version.h>
#include <stb/stb_image.h>
#include <bgfx/platform.h>
#include <imgui/imgui.h>

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#	define GLFW_EXPOSE_NATIVE_X11
#	define GLFW_EXPOSE_NATIVE_GLX
#elif BX_PLATFORM_OSX
#	define GLFW_EXPOSE_NATIVE_COCOA
#	define GLFW_EXPOSE_NATIVE_NSGL
#elif BX_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#endif // BX_PLATFORM_
#include <GLFW/glfw3native.h>

namespace igneous {
// Application
Application::Application(const char* title, uint32_t width, uint32_t height)
	: mTitle(title), mWidth(width), mHeight(height)
{

}

// Input callbacks
void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	ImGuiIO& io = ImGui::GetIO();
	if (key != GLFW_KEY_UNKNOWN)
	{
		if (action == GLFW_PRESS)
		{
			io.KeysDown[key] = app->mKeyDown[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			io.KeysDown[key] = app->mKeyDown[key] = false;
		}
	}

	io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

	if (!io.WantCaptureKeyboard)
	{
		app->onKey(key, scancode, action, mods);
	}
}

void Application::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	ImGuiIO& io = ImGui::GetIO();
	io.AddInputCharacter(codepoint);
	app->onChar(codepoint);
}

void Application::charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->onCharMods(codepoint, mods);
}

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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
			app->mMouseButtonDown[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			app->mMouseButtonDown[button] = false;
		}
	}

	if (!io.WantCaptureMouse)
	{
		app->onMouseButton(button, action, mods);
	}
}

void Application::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->onCursorPos(xpos, ypos);
}

void Application::cursorEnterCallback(GLFWwindow* window, int entered)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->onCursorEnter(entered);
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheelH += (float)xoffset;
	io.MouseWheel += (float)yoffset;
	app->mMouseWheelH += (float)xoffset;
	app->mMouseWheel += (float)yoffset;
	app->onScroll(xoffset, yoffset);
}

void Application::dropCallback(GLFWwindow* window, int count, const char** paths)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->onDrop(count, paths);
}

void Application::windowSizeCallback(GLFWwindow* window, int width, int height)
{
	Application* app = (Application*)glfwGetWindowUserPointer(window);
	app->mWidth = width;
	app->mHeight = height;
	app->reset(app->mReset);
	app->onWindowSize(width, height);
}

int Application::run(int argc, char** argv, bgfx::RendererType::Enum type, uint16_t vendorId, uint16_t deviceId, bgfx::CallbackI* callback, bx::AllocatorI* allocator)
{
	// Initialize the glfw
	if (!glfwInit())
	{
		return -1;
	}

	// Create a window
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWindow = glfwCreateWindow(getWidth(), getHeight(), getTitle(), NULL, NULL);
	if (!mWindow)
	{
		glfwTerminate();
		return -1;
	}

	// Setup input callbacks
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetKeyCallback(mWindow, keyCallback);
	glfwSetCharCallback(mWindow, charCallback);
	glfwSetCharModsCallback(mWindow, charModsCallback);
	glfwSetMouseButtonCallback(mWindow, mouseButtonCallback);
	glfwSetCursorPosCallback(mWindow, cursorPosCallback);
	glfwSetCursorEnterCallback(mWindow, cursorEnterCallback);
	glfwSetScrollCallback(mWindow, scrollCallback);
	glfwSetDropCallback(mWindow, dropCallback);
	glfwSetWindowSizeCallback(mWindow, windowSizeCallback);

	// Setup bgfx
	bgfx::PlatformData platformData;
	memset(&platformData, 0, sizeof(platformData));
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
	platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(mWindow);
	platformData.ndt = glfwGetX11Display();
#elif BX_PLATFORM_OSX
	platformData.nwh = glfwGetCocoaWindow(mWindow);
#elif BX_PLATFORM_WINDOWS
	platformData.nwh = glfwGetWin32Window(mWindow);
#endif // BX_PLATFORM_
	bgfx::setPlatformData(platformData);

	// Init bgfx
	bgfx::Init init;
	init.type = type;
	init.vendorId = vendorId;
	init.deviceId = deviceId;
	init.callback = callback;
	init.allocator = allocator;
	bgfx::init(init);

	IG_CORE_INFO("Initializing Engine");
	IG_CORE_INFO("Setting window title and icon");
	GLFWimage images[4];
	images[0].pixels = stbi_load("res/icons/icon16.png", &images[0].width, &images[0].height, 0, 4);
	images[1].pixels = stbi_load("res/icons/icon32.png", &images[1].width, &images[1].height, 0, 4);
	images[2].pixels = stbi_load("res/icons/icon48.png", &images[2].width, &images[2].height, 0, 4);
	images[3].pixels = stbi_load("res/icons/icon256.png", &images[3].width, &images[3].height, 0, 4);
	glfwSetWindowIcon(mWindow, 4, images);
	stbi_image_free(images[0].pixels);
	stbi_image_free(images[1].pixels);
	stbi_image_free(images[2].pixels);
	stbi_image_free(images[3].pixels);
	IG_CORE_INFO("Window title and icon set");

	IG_CORE_INFO("Initializing Services");
	Renderer* renderer = &Renderer::getInstance();
	Audio* audio = &Audio::getInstance();
	RendererSystem::init();
	gui::init(mWindow);
	Console* console = &Console::getInstance();
	console->runFile("startup.cmd");
	IG_CORE_INFO("Services Initialized");

	IG_CORE_INFO("-----Version Info-----");
	IG_CORE_INFO("Igneous Version: {}", IGNEOUS_VERSION);
	IG_CORE_INFO("Assimp Version: {}.{}.{}", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
	IG_CORE_INFO("Bullet Version: {}", BT_BULLET_VERSION);
	IG_CORE_INFO("Dear ImGui Version: {}", IMGUI_VERSION);
	IG_CORE_INFO("EnTT Version: {}", ENTT_VERSION);
	IG_CORE_INFO("GLFW Version: {}.{}.{}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
	IG_CORE_INFO("GLM Version: {}.{}.{}.{}", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH, GLM_VERSION_REVISION);
	IG_CORE_INFO("RakNet Version: {}", RAKNET_VERSION);
	IG_CORE_INFO("OpenAL Version: {}", audio->getVersion());
	IG_CORE_INFO("spdlog Version: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

	IG_CORE_INFO("-----Renderer Info-----");
	IG_CORE_INFO(renderer->getSupportedRenderers());
	IG_CORE_INFO(renderer->getGpuInfo());

	IG_CORE_INFO("-----OpenAL Info-----");
	IG_CORE_INFO("OpenAL Vendor: {}", audio->getVendor());
	IG_CORE_INFO("OpenAL Renderer: {}", audio->getRenderer());
	std::vector<std::string> devices = audio->getDevices();
	std::string defaultDevice = audio->getDefaultDevice();
	std::string selectedDevice = audio->getSelectedDevice();
	IG_CORE_INFO("OpenAL Devices:");
	for (std::string device : devices)
	{
		IG_CORE_INFO("	{}{}{}", device, device == defaultDevice ? "[Default]" : "", device == selectedDevice ? "[Selected]" : "");
	}
	IG_CORE_INFO("OpenAL Extensions: {}", audio->getExtensions());

	Input::setCursorVisible(mWindow, false);

	IG_CONSOLE_INFO("Engine Initialized");

	// Initialize the application
	IG_CORE_INFO("Initializing Application");
	reset();
	initialize(argc, argv);
	IG_CONSOLE_INFO("Application Initialized");

	// Loop until the user closes the window
	float lastTime = 0;
	float dt;
	float time;
	while (!glfwWindowShouldClose(mWindow))
	{
		time = (float)glfwGetTime();
		dt = time - lastTime;
		lastTime = time;

		glfwPollEvents();
		gui::update(dt);
		update(dt);
		ImGui::NewFrame();
		bgfx::touch(0);
		render();
		ImGui::Render();
		bgfx::frame();
	}

	// Shutdown application and glfw
	IG_CORE_INFO("Shuting Down Engine");
	IG_CORE_INFO("Shuting Down Application");
	int ret = shutdown();
	IG_CORE_INFO("Application Shut Down");
	IG_CORE_INFO("Shuting Down Services");
	gui::shutdown();
	RendererSystem::shutdown();
	renderer->cleanUp();
	IG_CORE_INFO("Services Shut Down");
	bgfx::shutdown();
	Input::setCursorVisible(mWindow, true);
	glfwTerminate();
	IG_CORE_INFO("Engine Shut Down");
	return ret;
}

void Application::reset(uint32_t flags)
{
	mReset = flags;
	bgfx::reset(mWidth, mHeight, mReset);
	gui::reset(uint16_t(getWidth()), uint16_t(getHeight()));
	onReset();
}

uint32_t Application::getWidth() const
{
	return mWidth;
}

uint32_t Application::getHeight() const
{
	return mHeight;
}

void Application::setSize(int width, int height)
{
	glfwSetWindowSize(mWindow, width, height);
}

const char* Application::getTitle() const
{
	return mTitle;
}

void Application::setTitle(const char* title)
{
	mTitle = title;
	glfwSetWindowTitle(mWindow, title);
}
} // end namespace igneous
