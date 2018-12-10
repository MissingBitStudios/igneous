#include <iostream>

#include <assimp/version.h>
#include <bigg.hpp>
#include <btBulletDynamicsCommon.h>
#include <RakNetVersion.h>
#include <stb_image.h>
#include <spdlog/version.h>

#include "version.h"

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
		std::cout << "Igneous version: " << IGNEOUS_VERSION << "\n";
		std::cout << "Assimp version: " << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionRevision() << "\n";
		std::cout << "Bullet version: " << BT_BULLET_VERSION << "\n";
		std::cout << "EnTT version: " << "" << "\n";
		std::cout << "GLFW version: " << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION << "\n";
		std::cout << "GLM version: " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "." << GLM_VERSION_PATCH << "." << GLM_VERSION_REVISION << "\n";
		std::cout << "RakNet version: " << RAKNET_VERSION << "\n";
		//std::cout << "Mono version: " << mono_get_runtime_build_info() << "\n";
		std::cout << "OpenAL version: " << "" << "\n";
		std::cout << "spdlog version: " << SPDLOG_VER_MAJOR << "." << SPDLOG_VER_MINOR << "." << SPDLOG_VER_PATCH << "\n";
	}
	
	void onReset()
	{
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xc0c0c0ff, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
	}

	void update(float dt)
	{
		bgfx::touch(0);
		ImGui::ShowDemoWindow();
	}

	int shutdown()
	{
		std::cout << "Shutdown\n";
		return 0;
	}
};

int main(int argc, char** argv)
{
	Engine igneous;
	return igneous.run(argc, argv);
}