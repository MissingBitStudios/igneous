#pragma once

#include "../components/modelComponent.h"
#include "../components/transformationComponent.h"
#include <entt/entt.hpp>
#include <bigg.hpp>

namespace RendererSystem
{
	void render(bgfx::ProgramHandle program, entt::registry<> &registry);
}