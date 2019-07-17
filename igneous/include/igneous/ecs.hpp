#pragma once

#include <bgfx/bgfx.h>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "renderer.hpp"

namespace igneous {
struct Transformation
{
	glm::mat4 mtx;
};

struct ModelComponent
{
	Model* model;
	bgfx::ProgramHandle program;
};

namespace CaptureSystem
{
	void screenshot();

	void record();

	void onKey(int key, int scancode, int action, int mods);

	extern bool capture;
}

namespace RendererSystem
{
	void init();
	void render(entt::registry& registry);
	void shutdown();
}
} // end namespace igneous
