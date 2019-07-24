#include "igneous/core/igneous.hpp"

#include <assimp/version.h>
#include <bigg.hpp>
#include <btBulletDynamicsCommon.h>
#include <bx/string.h>
#include <entt/entt.hpp>
#include <entt/config/version.h>
#include <glm/gtc/matrix_transform.hpp>
#include <RakNetVersion.h>
#include <spdlog/version.h>
#include <stb_image.h>

#include "igneous/core/log.hpp"
#include "igneous/core/input.hpp"
#include "igneous/ecs/systems/rendererSystem.hpp"
#include "igneous/core/version.hpp"
#include "igneous/audio/audio.hpp"
#include "igneous/ecs/systems/captureSystem.hpp"
#include "igneous/renderer/model.hpp"

namespace igneous {
Engine::Engine(Game* game)
	: bigg::Application("Igneous"), game(game) {}

void Engine::initialize(int _argc, char** _argv)
{
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

	Input::Init(mWindow);

	IG_CORE_INFO("Initializing Servers");
	renderer = &Renderer::getInstance();
	Audio* audio = &Audio::getInstance();
	RendererSystem::init();
	IG_CORE_INFO("Servers Initialized");

	console = &Console::getInstance();
	console->runFile("startup.cmd");

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

	IN_KEY_SINK(&CaptureSystem::onKey);

	mReset |= BGFX_RESET_MSAA_X4;
	reset(mReset);

	Input::setCursorVisible(mWindow, false);
	Input::width = getWidth();
	Input::height = getHeight();

	IG_CORE_INFO("Initializing game");
	game->initialize(_argc, _argv);
	IG_CORE_INFO("Game Initialized");

	IG_CONSOLE_INFO("Engine Initialized");
}

//Input callbacks
void Engine::onKey(int key, int scancode, int action, int mods) { Input::onKey(key, scancode, action, mods); }
void Engine::onMouseButton(int button, int action, int mods) { Input::onMouseButton(button, action, mods); }
void Engine::onScroll(double xoffset, double yoffset) { Input::onScroll(xoffset, yoffset); }
void Engine::onCursorPos(double xpos, double ypos) { Input::onCursorPos(xpos, ypos); }
void Engine::onCursorEnter(int entered) { Input::onCursorEnter(entered); }
void Engine::onWindowSize(int width, int height) { Input::width = width; Input::height = height; }

void Engine::onReset()
{
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x008080ff, 1.0f, 0);
	bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
}

void Engine::update(float dt)
{
	//Update
	game->update(dt);

	//Render
	bgfx::touch(0);
	game->render();
	console->render();
}

int Engine::shutdown()
{
	game->shutdown();
	delete game;
	Input::setCursorVisible(mWindow, true);
	IG_CORE_INFO("Shutingdown Servers");
	RendererSystem::shutdown();
	renderer->cleanUp();
	return 0;
}
} // end namespace igneous
