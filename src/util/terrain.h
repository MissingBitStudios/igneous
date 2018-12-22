#pragma once

#include <FastNoise.h>

#include "model.h"

namespace Terrain
{
	Model* terrain(int seed, int xSize, int zSize);
	Mesh* tile(int seed, int x, int z);

	extern FastNoise noise;
}