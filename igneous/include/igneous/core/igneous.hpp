#pragma once

#include <bigg.hpp>

#include "game.hpp"
#include "../renderer/capture.hpp"

namespace igneous {
class Engine : public bigg::Application
{
public:
	Engine(Game* game);
};

#define IG_IMPLEMENT_MAIN(GAME_CLASS)                                                                                  \
int main(int argc, char** argv)                                                                                        \
{                                                                                                                      \
	return Engine(new GAME_CLASS).run(argc, argv, bgfx::RendererType::Count, BGFX_PCI_ID_AMD, 0, new CaptureCallback); \
}
} // end namespace igneous
