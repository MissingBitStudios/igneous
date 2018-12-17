#pragma once

#include <bgfx/bgfx.h>
#include <entt/entt.hpp>

namespace RendererSystem
{
	void render(entt::registry<> &registry);

	void useCamera(uint32_t entity, uint16_t width, uint16_t height, entt::registry<> &registry);
}