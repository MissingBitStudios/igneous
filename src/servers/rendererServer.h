#pragma once

#include <map>
#include <string>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bx/file.h>

class RendererServer
{
public:
	static RendererServer& getInstance();

	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(const char* _filePath, uint32_t _flags = 0, bool track = true);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	void cleanUp();

	RendererServer(RendererServer const&) = delete;
	void operator=(RendererServer const&) = delete;
private:
	RendererServer();
	~RendererServer();

	std::map<std::string, bgfx::TextureHandle> textures;

	struct SplashVertex
	{
		float x;
		float y;
		float z;
		float tex_x;
		float tex_y;
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