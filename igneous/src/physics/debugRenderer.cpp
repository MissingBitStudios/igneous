#include "debugRenderer.hpp"

#include <vector>

#include "igneous/core/log.hpp"
#include "igneous/core/input.hpp"

#include "debug_assets.h"

namespace igneous {
namespace physics
{
	struct DebugVertex
	{
		float x, y, z;
		float r, g, b;
		static void init()
		{
			ms_decl
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
				.end();
		}
		static bgfx::VertexDecl ms_decl;
	};

	bgfx::VertexDecl DebugVertex::ms_decl;

	static bgfx::ProgramHandle debugProgram;
	static std::vector<DebugVertex> debugVertices;
	static std::vector<uint16_t> debugIndicies;

	DebugRenderer::DebugRenderer()
		: debugMode(DBG_NoDebug)
	{
		bgfx::ShaderHandle fs = bgfx::createShader(bgfx::makeRef(fs_debug(), fs_debug_len()));
		bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_debug(), vs_debug_len()));
		debugProgram = bgfx::createProgram(vs, fs, true);
		DebugVertex::init();
	}

	DebugRenderer::~DebugRenderer()
	{
		bgfx::destroy(debugProgram);
	}

	void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		DebugVertex fromVertex;
		fromVertex.x = from.getX();
		fromVertex.y = from.getY();
		fromVertex.z = from.getZ();
		fromVertex.r = color.getX();
		fromVertex.g = color.getY();
		fromVertex.b = color.getZ();

		DebugVertex toVertex;
		toVertex.x = to.getX();
		toVertex.y = to.getY();
		toVertex.z = to.getZ();
		toVertex.r = color.getX();
		toVertex.g = color.getY();
		toVertex.b = color.getZ();
		
		debugVertices.push_back(fromVertex);
		debugVertices.push_back(toVertex);

		debugIndicies.push_back(debugIndicies.size());
		debugIndicies.push_back(debugIndicies.size());
	}

	void DebugRenderer::render()
	{
		bgfx::VertexBufferHandle vb = bgfx::createVertexBuffer(bgfx::makeRef(debugVertices.data(), (uint32_t)debugVertices.size() * DebugVertex::ms_decl.getStride()), DebugVertex::ms_decl);
		bgfx::setVertexBuffer(0, vb);
		bgfx::IndexBufferHandle ib = bgfx::createIndexBuffer(bgfx::makeRef(debugIndicies.data(), (uint32_t)debugIndicies.size() * sizeof(uint16_t)));
		bgfx::setIndexBuffer(ib);
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_PT_LINES | BGFX_STATE_LINEAA);
		bgfx::setViewRect(100, 0, 0, input::width, input::height);
		bgfx::submit(100, debugProgram);
		bgfx::destroy(vb);
		bgfx::destroy(ib);
		debugVertices.clear();
		debugIndicies.clear();
		bgfx::setState(BGFX_STATE_DEFAULT);
	}

	void DebugRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{

	}

	void DebugRenderer::reportErrorWarning(const char* warningString)
	{
		IG_CORE_ERROR(warningString);
	}

	void DebugRenderer::draw3dText(const btVector3& location, const char* textString)
	{

	}

	void DebugRenderer::setDebugMode(int debugMode)
	{
		this->debugMode = debugMode;
	}

	int DebugRenderer::getDebugMode() const
	{
		return debugMode;
	}
}
} // end namespace igneous
