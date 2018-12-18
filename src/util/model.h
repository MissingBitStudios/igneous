#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>

#include "mesh.h"

struct Vertex
{
	float pos_x;
	float pos_y;
	float pos_z;
	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

class Model
{
public:
	Model(const char* path);
	/*  Model Data  */
	std::vector<Mesh> meshes;
private:
	/*  Functions   */
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};