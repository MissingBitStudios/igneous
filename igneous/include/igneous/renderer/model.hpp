#pragma once

#include <vector>

#include <bgfx/bgfx.h>

namespace igneous {
struct Mesh
{
	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	std::vector<bgfx::TextureHandle> textures;
};

struct Model
{
	std::vector<Mesh> meshes;
	bgfx::ProgramHandle program;
};

typedef Model* ModelHandle;
} // end namespace igneous
