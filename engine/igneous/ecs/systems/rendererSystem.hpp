#pragma once

#include <bgfx/bgfx.h>
#include <entt/entt.hpp>

namespace RendererSystem
{
	void render(entt::registry &registry, bgfx::TextureHandle handle);
}