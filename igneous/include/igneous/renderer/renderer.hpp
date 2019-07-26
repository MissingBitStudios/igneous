#pragma once

#include <map>
#include <string>

#include <bgfx/bgfx.h>

#include "igneous/renderer/model.hpp"

namespace igneous {
namespace renderer
{
	void init();
	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(std::string path, uint32_t _flags = 0, bool track = true);
	const bgfx::Memory* loadMemory(const char* filename);
	bgfx::ShaderHandle loadShader(const char* shader);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	template<typename VertexType>
	Model* loadModel(std::string path, bgfx::ProgramHandle program = BGFX_INVALID_HANDLE);

	void render();
	void screenshot();
	void setRecording(bool recording);
	bool isRecording();

	void shutdown();

	extern std::map<std::string, Model*> models;
}
} // end namespace igneous

#include "igneous/renderer/renderer.inl"
