#pragma once

#include <bigg.hpp>

#include "igneous/core/game.hpp"
#include "igneous/console/console.hpp"
#include "igneous/renderer/renderer.hpp"

namespace igneous {
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
	Engine(Game* game);
private:
	uint32_t mReset = BGFX_RESET_NONE;
	Game* game;
	Console* console;
	Renderer* renderer;
};
} // end namespace igneous

#define IG_IMPLEMENT_MAIN(GAME_CLASS)                                                                                  \
int main(int argc, char** argv)                                                                                        \
{                                                                                                                      \
	return Engine(new GAME_CLASS).run(argc, argv, bgfx::RendererType::Count, BGFX_PCI_ID_AMD, 0, new CaptureCallback); \
}
