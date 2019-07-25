#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>

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

struct GenericVertex : public Vertex
{
	GenericVertex(VertexData data)
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
} // end namespace igneous
