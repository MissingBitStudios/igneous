#pragma once

#include <bgfx/bgfx.h>

#include "../util/model.h"

struct ModelComponent
{
	Model* model;
	bgfx::ProgramHandle program;
};