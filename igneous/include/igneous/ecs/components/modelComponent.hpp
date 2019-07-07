#pragma once

#include <bgfx/bgfx.h>

#include "../../renderer/model.hpp"

struct ModelComponent
{
	Model* model;
	bgfx::ProgramHandle program;
};