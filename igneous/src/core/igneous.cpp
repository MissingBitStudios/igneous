#include "igneous/igneous.hpp"

#include <btBulletDynamicsCommon.h>
#include <bx/string.h>
#include <entt/entt.hpp>
#include <entt/config/version.h>
#include <glm/gtc/matrix_transform.hpp>
#include <RakNetVersion.h>
#include <spdlog/version.h>
#include <bgfx/platform.h>
#include <imgui/imgui.h>

namespace igneous {
// Application
Application::Application(const char* title, uint32_t width, uint32_t height)
{
	input::width = width;
	input::height = height;
	input::title = title;
	input::app = this;
}

int Application::run(int argc, char** argv, bgfx::Init init)
{
	IG_CORE_INFO("Initializing Engine");
	IG_CORE_INFO("Initializing Services");
	input::init();
	console::init();
	renderer::init(init);
	physics::init();
	gui::init();
	audio::init();
	console::runFile("startup.cmd");
	IG_CORE_INFO("Services Initialized");

	IG_CORE_INFO("-----Version Info-----");
	IG_CORE_INFO("Igneous Version: {}", IGNEOUS_VERSION);
	IG_CORE_INFO("Bullet Version: {}", BT_BULLET_VERSION);
	IG_CORE_INFO("Dear ImGui Version: {}", IMGUI_VERSION);
	IG_CORE_INFO("EnTT Version: {}", ENTT_VERSION);
	IG_CORE_INFO("GLFW Version: {}.{}.{}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
	IG_CORE_INFO("GLM Version: {}.{}.{}.{}", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH, GLM_VERSION_REVISION);
	IG_CORE_INFO("RakNet Version: {}", RAKNET_VERSION);
	IG_CORE_INFO("OpenAL Version: {}", audio::getVersion());
	IG_CORE_INFO("spdlog Version: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

	IG_CORE_INFO("-----Renderer Info-----");
	IG_CORE_INFO(renderer::getSupportedRenderers());
	IG_CORE_INFO(renderer::getGpuInfo());

	IG_CORE_INFO("-----OpenAL Info-----");
	IG_CORE_INFO("OpenAL Vendor: {}", audio::getVendor());
	IG_CORE_INFO("OpenAL Renderer: {}", audio::getRenderer());
	std::vector<std::string> devices = audio::getDevices();
	std::string defaultDevice = audio::getDefaultDevice();
	std::string selectedDevice = audio::getSelectedDevice();
	IG_CORE_INFO("OpenAL Devices:");
	for (std::string device : devices)
	{
		IG_CORE_INFO("	{}{}{}", device, device == defaultDevice ? "[Default]" : "", device == selectedDevice ? "[Selected]" : "");
	}
	IG_CORE_INFO("OpenAL Extensions: {}", audio::getExtensions());

	IG_CONSOLE_INFO("Engine Initialized");

	// Initialize the application
	IG_CORE_INFO("Initializing Application");
	initialize(argc, argv);
	IG_CONSOLE_INFO("Application Initialized");

	// Loop until the user closes the window
	float lastTime = 0;
	float dt;
	float time;
	while (!glfwWindowShouldClose(input::window))
	{
		time = (float)glfwGetTime();
		dt = time - lastTime;
		lastTime = time;

		glfwPollEvents();
		gui::update(dt);
		bgfx::touch(0);
		ImGui::NewFrame();
		physics::update(dt);
		update(dt);
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
	audio::shutdown();
	gui::shutdown();
	physics::shutdown();
	renderer::shutdown();
	input::shutdown();
	IG_CORE_INFO("Services Shut Down");
	IG_CORE_INFO("Engine Shut Down");
	return ret;
}

int Application::run(int argc, char** argv, bgfx::RendererType::Enum type, uint16_t vendorId, uint16_t deviceId, bgfx::CallbackI* callback, bx::AllocatorI* allocator)
{
	bgfx::Init init;
	init.type = type;
	init.vendorId = vendorId;
	init.deviceId = deviceId;
	init.callback = new CaptureCallback;
	init.allocator = allocator;
	return run(argc, argv, init);
}

void Application::onReset()
{
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, uint16_t(input::width), uint16_t(input::height));
}
} // end namespace igneous
