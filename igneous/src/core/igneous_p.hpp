#pragma once

#include <bigg.hpp>

#include "core/game.hpp"
#include "console/console_p.hpp"
#include "renderer/renderer_p.hpp"

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
