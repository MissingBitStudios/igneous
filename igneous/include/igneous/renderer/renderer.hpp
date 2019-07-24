#pragma once

#include <map>
#include <string>
#include <type_traits>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

#include "igneous/renderer/model.hpp"

namespace igneous {
struct VertexData
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 tex;
	glm::vec4 col;
	glm::vec3 tan;
	glm::vec3 bi;
};

struct Vertex
{
	Vertex() {};
	Vertex(VertexData data) {};
	static void init()
	{
		ms_decl
			.begin()
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};

struct PolyVertex : public Vertex
{
	PolyVertex(VertexData data)
	{
		pos = data.pos;
		norm = data.norm;
		tex = data.tex;
		col = data.col;
		tan = data.tan;
		bi = data.bi;
	}

	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 tex;
	glm::vec4 col;
	glm::vec3 tan;
	glm::vec3 bi;

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
};

class Renderer
{
public:
	static Renderer& getInstance();

	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(std::string path, uint32_t _flags = 0, bool track = true);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	template<typename VertexType>
	Model* loadModel(std::string path);

	void cleanUp();

	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;
private:
	Renderer();
	~Renderer();

	std::map<std::string, bgfx::TextureHandle> textures;
	std::map<std::string, bgfx::ProgramHandle> programs;
	std::map<std::string, Model*> models;

	struct SplashVertex
	{
		float x, y, z;
		float tex_x, tex_y;
		static void init()
		{
			ms_decl
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		}
		static bgfx::VertexDecl ms_decl;
	};
};
} // end namespace igneous

#include "igneous/renderer/renderer.inl"
