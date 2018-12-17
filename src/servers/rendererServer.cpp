#include "rendererServer.h"

#include <pcil/pcil.hpp>
#include <stb_image.h>

RendererServer& RendererServer::getInstance() {
	static RendererServer instance;
	return instance;
}

const char* RendererServer::getVendorName()
{
	return pcil::vendorLookup(bgfx::getCaps()->vendorId);
}

const char* RendererServer::getDeviceName()
{
	const bgfx::Caps* caps = bgfx::getCaps();
	return pcil::deviceLookup(caps->vendorId, caps->deviceId);
}

bgfx::TextureHandle RendererServer::loadTexture(const char* _filePath, uint32_t _flags)
{
	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

	int width, height;
	stbi_uc* data = stbi_load(_filePath, &width, &height, 0, 4);
	handle = bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::RGBA8, _flags, bgfx::copy(data, width * height * sizeof(stbi_uc*)));

	if (bgfx::isValid(handle))
	{
		bgfx::setName(handle, _filePath);
	}

	return handle;
}