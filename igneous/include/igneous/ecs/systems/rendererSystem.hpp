#pragma once

#include <bgfx/bgfx.h>
#include <entt/entt.hpp>

namespace igneous {
namespace RendererSystem
{
	void init();
	void render(entt::registry &registry);
	void shutdown();
}
} // end namespace igneous
