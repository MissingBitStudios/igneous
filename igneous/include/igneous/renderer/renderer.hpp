#pragma once

#include <map>
#include <string>

#include <bgfx/bgfx.h>

#include "igneous/renderer/model.hpp"

namespace igneous {
class Renderer
{
public:
	static Renderer& getInstance();

	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(std::string path, uint32_t _flags = 0, bool track = true);
	const bgfx::Memory* loadMemory(const char* filename);
	bgfx::ShaderHandle loadShader(const char* shader);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	template<typename VertexType>
	Model* loadModel(std::string path, bgfx::ProgramHandle program = BGFX_INVALID_HANDLE);

	void cleanUp();

	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;
private:
	Renderer();
	~Renderer();

	std::map<std::string, bgfx::TextureHandle> textures;
	std::map<std::string, bgfx::ProgramHandle> programs;
	std::map<std::string, Model*> models;
};
} // end namespace igneous

#include "igneous/renderer/renderer.inl"
