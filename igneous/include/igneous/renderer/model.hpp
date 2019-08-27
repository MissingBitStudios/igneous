#pragma once

#include <vector>

#include <bgfx/bgfx.h>

#include "igneous/renderer/material.hpp"

namespace igneous {
struct Mesh
{
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	MaterialHandle material;
};

struct Model
{
	std::vector<Mesh> meshes;
};

typedef Model* ModelHandle;
} // end namespace igneous
