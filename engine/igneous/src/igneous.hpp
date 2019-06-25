#pragma once

#include <bigg.hpp>

#include "ecs/components/cubeComponent.hpp"
#include "ecs/components/modelComponent.hpp"
#include "ecs/components/transformationComponent.hpp"
#include "console/console.hpp"
#include "gui/gui.hpp"
#include "audio/audio.hpp"
#include "physics/physics.hpp"
#include "renderer/renderer.hpp"
#include "ecs/systems/captureSystem.hpp"
#include "ecs/systems/rendererSystem.hpp"
#include "ecs/systems/skySystem.hpp"
#include "renderer/fpsCamera.hpp"
#include "renderer/capture.hpp"
#include "core/input.hpp"
#include "core/log.hpp"
#include "renderer/model.hpp"
#include "core/version.hpp"
#include "core/game.hpp"

class Engine : public bigg::Application
{
	void initialize(int _argc, char** _argv);

	//Input callbacks
	void onKey(int key, int scancode, int action, int mods);
	void onMouseButton(int button, int action, int mods);
	void onScroll(double xoffset, double yoffset);
	void onCursorPos(double xpos, double ypos);
	void onCursorEnter(int entered);
	void onWindowSize(int width, int height);

	void onReset();
	void update(float dt);
	int shutdown();
public:
	Engine(Game* game)
		: bigg::Application("Igneous"), game(game) {}
private:
	uint32_t mReset = BGFX_RESET_NONE;
	Audio* audio;
	Physics* physics;
	Console* console;
	Renderer* renderer;
	Game* game;
};

#define IG_IMPLEMENT_MAIN(GAME_CLASS)                                                                                  \
int main(int argc, char** argv)                                                                                        \
{                                                                                                                      \
	return Engine(new GAME_CLASS).run(argc, argv, bgfx::RendererType::Count, BGFX_PCI_ID_AMD, 0, new CaptureCallback); \
}
