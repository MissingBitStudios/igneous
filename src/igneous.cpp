#include <assimp/version.h>
#include <bigg.hpp>
#include <btBulletDynamicsCommon.h>
#include <bx/string.h>
#include <entt/entt.hpp>
#include <entt/config/version.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <mono/jit/jit.h>
#include <RakNetVersion.h>
#include <spdlog/version.h>
#include <stb_image.h>

#include "components/cubeComponent.h"
#include "components/modelComponent.h"
#include "components/transformationComponent.h"
#include "servers/audioServer.h"
#include "servers/physicsServer.h"
#include "servers/rendererServer.h"
#include "systems/captureSystem.h"
#include "systems/rendererSystem.h"
#include "systems/skySystem.h"
#include "util/camera.h"
#include "util/capture.h"
#include "util/input.h"
#include "util/log.h"
#include "util/model.h"
#include "util/terrain.h"
#include "version.h"

class Engine : public bigg::Application
{
	void initialize(int _argc, char** _argv)
	{
		IG_CORE_INFO("Setting window title and icon");
		glfwSetWindowTitle(mWindow, "Igneous");
		GLFWimage images[3];
		images[0].pixels = stbi_load("res/icons/icon16.png", &images[0].width, &images[0].height, 0, 4);
		images[1].pixels = stbi_load("res/icons/icon32.png", &images[1].width, &images[1].height, 0, 4);
		images[2].pixels = stbi_load("res/icons/icon48.png", &images[2].width, &images[2].height, 0, 4);
		glfwSetWindowIcon(mWindow, 3, images);
		stbi_image_free(images[0].pixels);
		stbi_image_free(images[1].pixels);
		stbi_image_free(images[2].pixels);
		IG_CORE_INFO("Window title and icon set");

		IG_CORE_INFO("Initializing Servers");
		RendererServer* renderer = &RendererServer::getInstance();
		audio = &AudioServer::getInstance();
		physics = &PhysicsServer::getInstance();
		IG_CORE_INFO("Servers Initialized");

		sky = new SkySystem;

		IG_CORE_INFO("-----Version Info-----");
		IG_CORE_INFO("Igneous Version: {}", IGNEOUS_VERSION);
		IG_CORE_INFO("Assimp Version: {}.{}.{}", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());
		IG_CORE_INFO("Bullet Version: {}", BT_BULLET_VERSION);
		IG_CORE_INFO("Dear ImGui Version: {}", IMGUI_VERSION);
		IG_CORE_INFO("EnTT Version: {}", ENTT_VERSION);
		IG_CORE_INFO("GLFW Version: {}.{}.{}", GLFW_VERSION_MAJOR, GLFW_VERSION_MINOR, GLFW_VERSION_REVISION);
		IG_CORE_INFO("GLM Version: {}.{}.{}.{}", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH, GLM_VERSION_REVISION);
		IG_CORE_INFO("RakNet Version: {}", RAKNET_VERSION);
		IG_CORE_INFO("Mono Version: {}", mono_get_runtime_build_info());
		IG_CORE_INFO("OpenAL Version: {}", audio->getVersion());
		IG_CORE_INFO("spdlog Version: {}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

		IG_CORE_INFO("-----Renderer Info-----");
		IG_CORE_INFO("{}", renderer->getSupportedRenderers());
		IG_CORE_INFO("{}", renderer->getGpuInfo());

		IG_CORE_INFO("-----OpenAL Info-----");
		IG_CORE_INFO("OpenAL Vendor: {}", audio->getVendor());
		IG_CORE_INFO("OpenAL Renderer: {}", audio->getRenderer());
		IG_CORE_INFO("OpenAL Devices: {}", audio->getDevices());
		IG_CORE_INFO("OpenAL Extensions: {}", audio->getExtensions());

		Vertex::init();

		p = renderer->loadProgram("vs_bunny", "fs_bunny");
		c = renderer->loadProgram("vs_cubes", "fs_cubes");
		bunny = new Model("res/models/sponza/sponza.obj");
		barn = new Model("res/models/BigBarn/BigBarn.obj");

		glm::mat4 mat;
		mat = glm::translate(mat, glm::vec3(0.0f, 0.0f, -35.0f));
		mat = glm::scale(mat, glm::vec3(0.01f));

		auto entity = registry.create();
		registry.assign<ModelComponent>(entity, bunny, p);
		registry.assign<Transformation>(entity, mat);

		glm::mat4 mat0;
		entity = registry.create();
		registry.assign<ModelComponent>(entity, barn, c);
		registry.assign<Transformation>(entity, mat0);

		Model* terrain = Terrain::terrain(0, 10, 10);

		glm::mat4 mat1;
		mat1 = glm::translate(mat1, glm::vec3(-35.0f, 0.0f, 0.0f));
		entity = registry.create();
		registry.assign<ModelComponent>(entity, terrain, c);
		registry.assign<Transformation>(entity, mat1);

		glm::mat4 mat2;
		mat2 = glm::translate(mat1, glm::vec3(50.0f, 0.0f, 0.0f));
		entity = registry.create();
		registry.assign<ModelComponent>(entity, terrain, c);
		registry.assign<Transformation>(entity, mat2);

		handle = renderer->loadTexture("res/icons/icon48.png", false);

		camera = new Camera(glm::vec3(-35.0f, 5.0f, 0.0f), getWidth(), getHeight());

		Input::keySignal.sink().connect<&CaptureSystem::onKey>();

		mReset |= BGFX_RESET_MSAA_X4;
		reset(mReset);

		Input::setCursorVisible(mWindow, false);
	}

	void onKey(int key, int scancode, int action, int mods)
	{
		Input::onKey(key, scancode, action, mods);
		if (Input::keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(mWindow, true);
		if (Input::keys[GLFW_KEY_R]) Input::setCursorVisible(mWindow, true);
		if (Input::keys[GLFW_KEY_H]) Input::setCursorVisible(mWindow, false);
	}
	
	void onMouseButton(int button, int action, int mods)
	{
		Input::onMouseButton(button, action, mods);
	}

	void onScroll(double xoffset, double yoffset)
	{
		Input::onScroll(xoffset, yoffset);
	}

	void onCursorPos(double xpos, double ypos)
	{
		Input::onCursorPos(xpos, ypos);
	}

	void onCursorEnter(int entered)
	{
		Input::onCursorEnter(entered);
	}

	void onReset()
	{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xc0c0c0ff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
	}

	void update(float dt)
	{
		physics->step(dt);
		camera->update(dt);
		camera->use(getWidth(), getHeight());
		bgfx::touch(0);
		RendererSystem::render(registry, handle);

		sky->update(dt);

		ImGui::ShowDemoWindow();

		ImGui::Begin("Audio Test");
		if (ImGui::Button("bell"))
		{
			audio->playSound(audio->loadSound("res/audio/test.ogg"));
		}
		if (ImGui::Button("forest"))
		{
			audio->playSound(audio->loadSound("res/audio/forest.ogg"));
		}
		ImGui::End();

		if (CaptureSystem::capture && (mReset & BGFX_RESET_CAPTURE) != BGFX_RESET_CAPTURE)
		{
			mReset |= BGFX_RESET_CAPTURE;
			reset(mReset);
		}
		else if (!CaptureSystem::capture && (mReset & BGFX_RESET_CAPTURE) == BGFX_RESET_CAPTURE)
		{
			mReset &= ~BGFX_RESET_CAPTURE;
			reset(mReset);
		}
	}

	int shutdown()
	{
		Input::setCursorVisible(mWindow, true);
		IG_CORE_INFO("Shutingdown Servers");
		bgfx::destroy(handle);
		bgfx::destroy(p);
		bgfx::destroy(c);
		RendererServer* renderer = &RendererServer::getInstance();
		renderer->cleanUp();
		delete bunny;
		delete barn;
		delete sky;
		return 0;
	}
private:
	uint32_t mReset = BGFX_RESET_NONE;
	entt::registry<> registry;
	AudioServer* audio;
	bgfx::TextureHandle handle;
	SkySystem* sky;
	bgfx::ProgramHandle p;
	bgfx::ProgramHandle c;
	Model* bunny;
	Model* barn;
	Camera* camera;
	PhysicsServer* physics;
};

int main(int argc, char** argv)
{
	Engine igneous;
	return igneous.run(argc, argv, bgfx::RendererType::Count, BGFX_PCI_ID_AMD, 0, new CaptureCallback);
}