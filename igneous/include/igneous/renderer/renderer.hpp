#pragma once

#include <map>
#include <string>

#include <bgfx/bgfx.h>

#include "igneous/ecs/ecs.hpp"
#include "igneous/renderer/model.hpp"

namespace igneous {
namespace renderer
{
	void init(bgfx::Init init);
	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(std::string path, uint32_t _flags = 0, bool track = true);
	const bgfx::Memory* loadMemory(const char* filename);
	bgfx::ShaderHandle loadShader(const char* shader);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	ModelHandle loadModel(std::string path, bgfx::ProgramHandle program);

	void render();
	void screenshot();
	void setRecording(bool record);
	bool isRecording();
	void setDebugOverlay(bool debugOverlay);

	void reset();

	void setFlag(uint32_t flag, bool value);
	bool getFlag(uint32_t newFlags);
	void setFlags(uint32_t flags);
	uint32_t getFlags();

	void shutdown();

	extern std::map<std::string, Model*> models;
}
} // end namespace igneous
