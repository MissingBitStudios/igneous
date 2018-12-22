#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>

struct Vertex
{
	float pos_x;
	float pos_y;
	float pos_z;
	float norm_x;
	float norm_y;
	float norm_z;
	float tex_x;
	float tex_y;
	float col_r;
	float col_g;
	float col_b;
	float col_a;
	float tan_x;
	float tan_y;
	float tan_z;
	float bi_x;
	float bi_y;
	float bi_z;
	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, std::vector<bgfx::TextureHandle> textures);
	~Mesh();

	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	std::vector<bgfx::TextureHandle> textures;
private:
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

class Model
{
public:
	Model(const char* path);
	Model() {}
	~Model();

	std::vector<Mesh*> meshes;
private:
	void processNode(aiNode *node, const aiScene *scene);
	Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<bgfx::TextureHandle> loadMaterialTextures(aiMaterial *mat, aiTextureType type);

	std::string directory;
};