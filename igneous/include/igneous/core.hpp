#pragma once

#include <bigg.hpp>

#include "renderer.hpp"

namespace igneous {
class Game
{
public:
	virtual void initialize(int _argc, char** _argv) {}
	virtual void update(float dt) {}
	virtual void render() {}
	virtual void shutdown() {}
};

class Engine : public bigg::Application
{
public:
	Engine(Game* game);
};
} // end namespace igneous

#define IG_IMPLEMENT_MAIN(GAME_CLASS)                                                                                  \
int main(int argc, char** argv)                                                                                        \
{                                                                                                                      \
	return Engine(new GAME_CLASS).run(argc, argv, bgfx::RendererType::Count, BGFX_PCI_ID_AMD, 0, new CaptureCallback); \
}
