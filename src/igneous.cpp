#include <assimp/version.h>
#include <bigg.hpp>
#include <btBulletDynamicsCommon.h>
#include <bx/string.h>
#include <entt/entt.hpp>
#include <entt/config/version.h>
#include <glm/gtc/matrix_transform.hpp>
#include <mono/jit/jit.h>
#include <RakNetVersion.h>
#include <spdlog/version.h>
#include <stb_image.h>

#include "components/cameraComponent.h"
#include "components/cubeComponent.h"
#include "components/modelComponent.h"
#include "components/transformationComponent.h"
#include "servers/audioServer.h"
#include "servers/rendererServer.h"
#include "systems/captureSystem.h"
#include "systems/moveSystem.h"
#include "systems/rendererSystem.h"
#include "util/capture.h"
#include "util/input.h"
#include "util/log.h"
#include "version.h"

struct PosColorVertex
{
	float x;
	float y;
	float z;
	float tex_x;
	float tex_y;
	uint32_t abgr;
	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};
bgfx::VertexDecl PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] =
{
	{ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0xff0000ff },
	{ -1.0f, -1.0f,  1.0f, 0.0f, 1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0xff00ffff },
	{ -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0xffff00ff },
	{ -1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0xffffffff },
};
static const uint16_t s_cubeTriList[] = { 2, 1, 0, 2, 3, 1, 5, 6, 4, 7, 6, 5, 4, 2, 0, 6, 2, 4, 3, 5, 1, 3, 7, 5, 1, 4, 0, 1, 5, 4, 6, 3, 2, 7, 3, 6 };

class Engine : public bigg::Application
{
	void initialize(int _argc, char** _argv)
	{
		glfwSetWindowTitle(mWindow, "Igneous");
		GLFWimage images[3];
		images[0].pixels = stbi_load("res/icons/icon16.png", &images[0].width, &images[0].height, 0, 4);
		images[1].pixels = stbi_load("res/icons/icon32.png", &images[1].width, &images[1].height, 0, 4);
		images[2].pixels = stbi_load("res/icons/icon48.png", &images[2].width, &images[2].height, 0, 4);
		glfwSetWindowIcon(mWindow, 3, images);
		stbi_image_free(images[0].pixels);
		stbi_image_free(images[1].pixels);
		stbi_image_free(images[2].pixels);

		RendererServer* renderer = &RendererServer::getInstance();

		audio = &AudioServer::getInstance();

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
		IG_CORE_INFO("Renderer Vendor: {}", renderer->getVendorName());
		IG_CORE_INFO("Renderer Device: {}", renderer->getDeviceName());

		IG_CORE_INFO("-----OpenAL Info-----");
		IG_CORE_INFO("OpenAL Vendor: {}", audio->getVendor());
		IG_CORE_INFO("OpenAL Renderer: {}", audio->getRenderer());
		IG_CORE_INFO("OpenAL Devices: {}", audio->getDevices());
		IG_CORE_INFO("OpenAL Extensions: {}", audio->getExtensions());

		PosColorVertex::init();

		s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Int1);
		handle = renderer->loadTexture("res/icons/icon48.png", BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP);

		mProgram = renderer->loadProgram("vs_cubes", "fs_cubes");
		mVbh = bgfx::createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), PosColorVertex::ms_decl);
		mIbh = bgfx::createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
		bgfx::setDebug(BGFX_DEBUG_TEXT);
		
		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				glm::mat4 mtx;
				mtx = glm::translate(mtx, glm::vec3(15.0f - float(xx) * 3.0f, -15.0f + float(yy) * 3.0f, 0.0f));
				auto entity = registry.create();
				registry.assign<Model>(entity, mVbh, mIbh, handle, s_tex, mProgram);
				registry.assign<Transformation>(entity, mtx);
				registry.assign<Cube>(entity, xx, yy);
			}
		}

		glm::mat4 mtx;
		mtx = glm::translate(mtx, glm::vec3(0.0f, 0.0f, -35.0f));
		glm::vec3 focus = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 proj = bigg::perspective(glm::radians(60.0f), float(getWidth()) / getHeight(), 0.1f, 100.0f);
		camera = registry.create();
		registry.assign<Transformation>(camera, mtx);
		registry.assign<Camera>(camera, focus, proj);

		Input::keySignal.sink().connect<&CaptureSystem::onKey>();
		Input::keySignal.sink().connect<&MoveSystem::onKey>();

		mReset |= BGFX_RESET_MSAA_X4;
		reset(mReset);
	}

	void onKey(int key, int scancode, int action, int mods)
	{
		Input::onKey(key, scancode, action, mods);
		if (Input::keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(mWindow, true);
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
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL, 0xc0c0c0ff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
	}

	void update(float dt)
	{
		RendererSystem::useCamera(camera, uint16_t(getWidth()), uint16_t(getHeight()), registry);
		MoveSystem::update(dt, registry);
		bgfx::touch(0);
		RendererSystem::render(registry);

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
		bgfx::destroy(handle);
		bgfx::destroy(s_tex);
		bgfx::destroy(mProgram);
		IG_CORE_INFO("Shutdown");
		return 0;
	}
private:
	uint32_t mReset = BGFX_RESET_NONE;
	bgfx::ProgramHandle mProgram;
	bgfx::VertexBufferHandle mVbh;
	bgfx::IndexBufferHandle mIbh;
	entt::registry<> registry;
	uint32_t camera;
	AudioServer* audio;
	bgfx::TextureHandle handle;
	bgfx::UniformHandle s_tex;
};

int main(int argc, char** argv)
{
	Engine igneous;
	return igneous.run(argc, argv, bgfx::RendererType::Count, BGFX_PCI_ID_AMD, 0, new CaptureCallback);
}