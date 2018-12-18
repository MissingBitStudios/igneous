#pragma once

#include <bgfx/bgfx.h>

struct ModelComponent
{
	bgfx::VertexBufferHandle mVbh;
	bgfx::IndexBufferHandle mIbh;
	bgfx::TextureHandle texture;
	bgfx::UniformHandle uniform;
	bgfx::ProgramHandle program;
};