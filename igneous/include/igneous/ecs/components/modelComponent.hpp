#pragma once

#include <bgfx/bgfx.h>

#include "../../renderer/model.hpp"

namespace igneous {
struct ModelComponent
{
	Model* model;
	bgfx::ProgramHandle program;
};
} // end namespace igneous
