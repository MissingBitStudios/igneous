#pragma once

#include <vector>

#include <bgfx/bgfx.h>

namespace igneous {
enum MATERIAL_ATTRIBUTE : uint8_t
{
	COLOR,
	DIFFUSE,
	NORMAL,
	SPECULAR
};

struct Material
{
	bgfx::ProgramHandle shader;
	uint64_t state;
	std::vector<std::pair<bgfx::UniformHandle, void*>> uniforms;
	std::vector<std::pair<bgfx::UniformHandle, bgfx::TextureHandle>> textures;
};

typedef Material* MaterialHandle;
} // end namespace igneous
