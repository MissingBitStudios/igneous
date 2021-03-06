#pragma once

#include <bgfx/bgfx.h>

struct ProceduralSky
{
	void init(int verticalCount, int horizontalCount);

	void shutdown();

	void draw();

	bgfx::VertexBufferHandle m_vbh;
	bgfx::IndexBufferHandle m_ibh;

	bgfx::ProgramHandle m_skyProgram;


	struct ScreenPosVertex
	{
		float m_x;
		float m_y;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
				.end();
		}

		static bgfx::VertexLayout ms_layout;
	};
};