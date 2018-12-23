#include "terrain.h"

namespace Terrain
{
	Model* terrain(int seed, int xSize, int zSize)
	{
		noise.SetNoiseType(FastNoise::NoiseType::Perlin);
		noise.SetSeed(seed);
		noise.SetFrequency(5.0f);

		Model* model = new Model();

		for (unsigned int x = 0; x < xSize; x++)
		{
			for (unsigned int z = 0; z < zSize; z++)
			{
				model->meshes.push_back(tile(seed, x, z));
			}
		}

		return model;
	}

	Mesh* tile(int seed, int x, int z)
	{
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;
		std::vector<bgfx::TextureHandle> textures;

		float r = noise.GetValue(x, z) / 4 + 0.75f;

		for (int xx = 0; xx < 2; xx++)
		{
			for (int zz = 0; zz < 2; zz++)
			{
				Vertex vertex = { 0.0f };
				vertex.pos_x = xx * 5 + x * 5;
				vertex.pos_z = zz * 5 + z * 5;
				vertex.pos_y = noise.GetValue(vertex.pos_x, vertex.pos_z);
				// colors
				vertex.col_r = 0.25f * r;
				vertex.col_g = 0.75f * r;
				vertex.col_b = 0.0f;
				vertex.col_a = 1.0f;

				vertices.push_back(vertex);
			}
		}
		
		indices = {0, 1, 2, 3, 2, 1};

		return new Mesh(vertices, indices, textures);
	}

	FastNoise noise;
}