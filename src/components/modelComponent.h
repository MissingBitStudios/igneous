#pragma once

#include <bgfx/bgfx.h>

struct Model
{
	bgfx::VertexBufferHandle mVbh;
	bgfx::IndexBufferHandle mIbh;
	bgfx::TextureHandle texture;
	bgfx::UniformHandle uniform;
	bgfx::ProgramHandle program;
};