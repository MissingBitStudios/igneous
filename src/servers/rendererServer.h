#pragma once

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bx/file.h>

class RendererServer
{
public:
	static RendererServer& getInstance();

	const char* getVendorName();
	const char* getDeviceName();

	bgfx::TextureHandle loadTexture(const char* _filePath, uint32_t _flags);

	RendererServer(RendererServer const&) = delete;
	void operator=(RendererServer const&) = delete;
private:
	RendererServer() {}
	~RendererServer() {}
};