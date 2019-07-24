#include "igneous/renderer/renderer.hpp"

#include <bigg.hpp>
#include <stb_image.h>

#include <bimg/bimg.h>
#include <bx/file.h>

#include "igneous/core/log.hpp"
#include "igneous/renderer/splash_shaders.hpp"

namespace igneous {
Renderer::Renderer()
{
	IG_CORE_INFO("Initializing Renderer");
	static SplashVertex s_splashVertices[] =
	{
		{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f },
		{ 1.0f,  1.0f, 0.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, -1.0f, 0.0f, 1.0f, 1.0f },
	};
	static const uint16_t s_splashTriList[] = { 2, 1, 0, 2, 3, 1 };

	SplashVertex::init();

	bgfx::ShaderHandle fs = bgfx::createShader(bgfx::makeRef(fs_splash(), fs_splash_len()));
	bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_splash(), vs_splash_len()));
	bgfx::ProgramHandle splashProgram = bgfx::createProgram(vs, fs, true);

	bgfx::UniformHandle s_splash = bgfx::createUniform("s_splash", bgfx::UniformType::Sampler);
	bgfx::TextureHandle splashTexture = loadTexture("res/textures/splash.png", BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP, false);

	bgfx::setVertexBuffer(0, bgfx::createVertexBuffer(bgfx::makeRef(s_splashVertices, sizeof(s_splashVertices)), SplashVertex::ms_decl));
	bgfx::setIndexBuffer(bgfx::createIndexBuffer(bgfx::makeRef(s_splashTriList, sizeof(s_splashTriList))));
	bgfx::setTexture(0, s_splash, splashTexture);
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(0, splashProgram);
	bgfx::frame();
	bgfx::destroy(splashTexture);
	bgfx::destroy(s_splash);
	bgfx::destroy(splashProgram);

	Vertex::init();
	PolyVertex::init();

	IG_CORE_INFO("Renderer Initialized");
}

Renderer& Renderer::getInstance() {
	static Renderer instance;
	return instance;
}

std::string Renderer::getSupportedRenderers()
{
	std::string supportedRenderers = "Supported Renderers: ";
	bgfx::RendererType::Enum rendererTypes[bgfx::RendererType::Count];
	uint8_t num = bgfx::getSupportedRenderers(bgfx::RendererType::Count, rendererTypes);

	for (uint8_t i = 0; i < num; i++)
	{
		supportedRenderers += bgfx::getRendererName(rendererTypes[i]);
		supportedRenderers += i != num - 1 ? ", " : "";
	}

	return supportedRenderers;
}

std::string Renderer::getGpuInfo()
{
	const bgfx::Caps* caps = bgfx::getCaps();
	std::string gpuInfo = "Number of GPUs: " + std::to_string(caps->numGPUs) + "\n";
	std::string isSelected;

	for (uint8_t i = 0; i < caps->numGPUs; i++)
	{
		isSelected = caps->deviceId == caps->gpu[i].deviceId ? "(Selected)" : "          ";
		gpuInfo += "GPU[" + std::to_string(i) + "]" + isSelected + "| Vendor ID: " + std::to_string(caps->gpu[i].vendorId) + " | Device ID: " + std::to_string(caps->gpu[i].deviceId);
		gpuInfo += i < caps->numGPUs - 1 ? "\n" : "";
	}

	return gpuInfo;
}

bgfx::TextureHandle Renderer::loadTexture(std::string path, uint32_t _flags, bool track)
{
	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

	IG_CORE_INFO("Searching for texture: {}", path);

	if (textures.count(path))
	{
		IG_CORE_INFO("Found identical texture: {}", path);
		handle = textures.at(path);
	}
	else
	{
		IG_CORE_INFO("Loading texture: {}", path);
		int width, height;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, 0, 4);
		const bgfx::Memory* mem = bgfx::copy(data, width * height * 4);
		if (nullptr != mem)
		{
			handle = bgfx::createTexture2D((uint16_t)width, (uint16_t)height, false, 1, bgfx::TextureFormat::RGBA8, _flags, mem);
		}
		else
		{
			IG_CORE_ERROR("Could not load texture: {}", path);
		}
		stbi_image_free(data);

#if IG_DEBUG
		if (bgfx::isValid(handle))
		{
			bgfx::setName(handle, path.c_str());
		}
#endif
		if (track)
		{
			textures[path] = handle;
		}
	}

	return handle;
}

bgfx::ProgramHandle Renderer::loadProgram(const char* vs, const char* fs)
{
	char vsName[512];
	char fsName[512];

	const char* shaderPath = "???";

	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Noop:
	case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
	case bgfx::RendererType::Gnm:                                       break;
	case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
	case bgfx::RendererType::Nvn:                                       break;
	case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
	case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
	case bgfx::RendererType::Vulkan:                                    break;
	case bgfx::RendererType::Count:                                     break;
	}

	bx::strCopy(vsName, BX_COUNTOF(vsName), shaderPath);
	bx::strCat(vsName, BX_COUNTOF(vsName), vs);
	bx::strCat(vsName, BX_COUNTOF(vsName), ".bin");

	bx::strCopy(fsName, BX_COUNTOF(fsName), shaderPath);
	bx::strCat(fsName, BX_COUNTOF(fsName), fs);
	bx::strCat(fsName, BX_COUNTOF(fsName), ".bin");

	return bigg::loadProgram(vsName, fsName);
}

void Renderer::cleanUp()
{
	IG_CORE_INFO("Cleaning up Renderer");
	for (auto it = textures.begin(); it != textures.end(); ++it)
	{
		IG_CORE_INFO("Destroying texture: {}", it->first);
		bgfx::destroy(it->second);
	}

	for (auto it = models.begin(); it != models.end(); ++it)
	{
		IG_CORE_INFO("Destroying model: {}", it->first);
		for (Mesh mesh : it->second->meshes)
		{
			bgfx::destroy(mesh.vbh);
			bgfx::destroy(mesh.ibh);
		}
		delete it->second;
	}
	IG_CORE_INFO("Renderer Cleaned up ");
}

Renderer::~Renderer()
{
	IG_CORE_INFO("Shutingdown Renderer");
	IG_CORE_INFO("Renderer Shutdown");
}

bgfx::VertexDecl Renderer::SplashVertex::ms_decl;
bgfx::VertexDecl Vertex::ms_decl;
} // end namespace igneous
