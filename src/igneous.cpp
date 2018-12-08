#include <iostream>

#include <assimp/version.h>
#include <bigg.hpp>
#include <btBulletDynamicsCommon.h>
#include <RakNetVersion.h>

#include "version.h"

class Engine : public bigg::Application
{
	void initialize(int _argc, char** _argv)
	{
		std::cout << "Igneous version: " << IGNEOUS_VERSION << "\n";
		std::cout << "Assimp version: " << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionRevision() << "\n";
		std::cout << "Bullet version: " << BT_BULLET_VERSION << "\n";
		std::cout << "EnTT version: " << "" << "\n";
		std::cout << "RakNet version: " << RAKNET_VERSION << "\n";
		//std::cout << "Mono version: " << "" << "\n";
		std::cout << "OpenAL version: " << "" << "\n";
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