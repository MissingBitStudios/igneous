#include "proceduralSky.hpp"

#include <bx/allocator.h>

#include <igneous/igneous.hpp>

using namespace igneous;

void ProceduralSky::init(int verticalCount, int horizontalCount)
{
	ScreenPosVertex::init();

	m_skyProgram = renderer::loadProgram("vs_sky", "fs_sky");

	bx::AllocatorI* allocator = new bx::DefaultAllocator();

	ScreenPosVertex* vertices = (ScreenPosVertex*)BX_ALLOC(allocator
		, verticalCount * horizontalCount * sizeof(ScreenPosVertex)
	);

	for (int i = 0; i < verticalCount; i++)
	{
		for (int j = 0; j < horizontalCount; j++)
		{
			ScreenPosVertex& v = vertices[i * verticalCount + j];
			v.m_x = float(j) / (horizontalCount - 1) * 2.0f - 1.0f;
			v.m_y = float(i) / (verticalCount - 1) * 2.0f - 1.0f;
		}
	}

	uint16_t* indices = (uint16_t*)BX_ALLOC(allocator
		, (verticalCount - 1) * (horizontalCount - 1) * 6 * sizeof(uint16_t)
	);

	int k = 0;
	for (int i = 0; i < verticalCount - 1; i++)
	{
		for (int j = 0; j < horizontalCount - 1; j++)
		{
			indices[k++] = (uint16_t)(j + 0 + horizontalCount * (i + 0));
			indices[k++] = (uint16_t)(j + 1 + horizontalCount * (i + 0));
			indices[k++] = (uint16_t)(j + 0 + horizontalCount * (i + 1));

			indices[k++] = (uint16_t)(j + 1 + horizontalCount * (i + 0));
			indices[k++] = (uint16_t)(j + 1 + horizontalCount * (i + 1));
			indices[k++] = (uint16_t)(j + 0 + horizontalCount * (i + 1));
		}
	}

	m_vbh = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(ScreenPosVertex) * verticalCount * horizontalCount), ScreenPosVertex::ms_layout);
	m_ibh = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(uint16_t) * k));

	BX_FREE(allocator, indices);
	BX_FREE(allocator, vertices);
}

void ProceduralSky::shutdown()
{
	bgfx::destroy(m_ibh);
	bgfx::destroy(m_vbh);
	bgfx::destroy(m_skyProgram);
}

void ProceduralSky::draw()
{
	bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_EQUAL);
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setVertexBuffer(0, m_vbh);
	bgfx::submit(0, m_skyProgram);
}

bgfx::VertexLayout ProceduralSky::ScreenPosVertex::ms_layout;