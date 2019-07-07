#pragma once

#include <map>
#include <string>

#include <bgfx/bgfx.h>

class Renderer
{
public:
	static Renderer& getInstance();

	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(const char* _filePath, uint32_t _flags = 0, bool track = true);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	void cleanUp();

	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;
};