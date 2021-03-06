#include "igneous/renderer/renderer.hpp"

#include <fstream>

#include <stb/stb_image.h>
#include <bimg/bimg.h>
#include <bx/file.h>
#include <bgfx/platform.h>

#include "igneous/core/log.hpp"
#include "splash_assets.h"
#include "igneous/ecs/components/transformationComponent.hpp"
#include "igneous/console/console.hpp"
#include "igneous/core/input.hpp"
#include "igneous/gui/gui.hpp"
#include "igneous/physics/physics.hpp"

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#	define GLFW_EXPOSE_NATIVE_X11
#	define GLFW_EXPOSE_NATIVE_GLX
#elif BX_PLATFORM_OSX
#	define GLFW_EXPOSE_NATIVE_COCOA
#	define GLFW_EXPOSE_NATIVE_NSGL
#elif BX_PLATFORM_WINDOWS
#	define GLFW_EXPOSE_NATIVE_WIN32
#	define GLFW_EXPOSE_NATIVE_WGL
#endif // BX_PLATFORM_
#include <GLFW/glfw3native.h>

namespace igneous {
namespace renderer
{
	static std::map<std::string, bgfx::TextureHandle> textures;
	static std::map<std::string, bgfx::ProgramHandle> programs;
	static std::map<std::string, MaterialHandle> materials;
	std::map<std::string, ModelHandle> models;

	static bgfx::TextureHandle checkerBoard;
	static uint32_t flags = BGFX_RESET_NONE;
	static bgfx::UniformHandle U_COLOR;

	void screenshotCallback(const std::string& name, const arg_list& args)
	{
		screenshot();
	}

	void recordingCallback(float oldValue, float newValue)
	{
		if ((bool)oldValue != (bool)newValue)
		{
			setRecording(newValue);
		}
	}

	void debugCallback(float oldValue, float newValue)
	{
		setDebugOverlay(newValue);
	}

	static ConVar* recording;
	static ConVar* debug;
	static bgfx::UniformHandle s_tex;

	void init(bgfx::Init init)
	{
		IG_CORE_INFO("Initializing Renderer");
		// Setup bgfx
		IG_CORE_INFO("Initializing bgfx");
		bgfx::PlatformData platformData;
		memset(&platformData, 0, sizeof(platformData));
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
		platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(input::window);
		platformData.ndt = glfwGetX11Display();
#elif BX_PLATFORM_OSX
		platformData.nwh = glfwGetCocoaWindow(input::window);
#elif BX_PLATFORM_WINDOWS
		platformData.nwh = glfwGetWin32Window(input::window);
#endif // BX_PLATFORM_
		bgfx::setPlatformData(platformData);

		// Init bgfx
		bgfx::init(init);
		IG_CORE_INFO("bgfx Initializied");

		reset();
		static float s_splashVertices[] =
		{
			-1.0f,  1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 1.0f
		};
		static const uint16_t s_splashTriList[] = { 2, 1, 0, 2, 3, 1 };

		static bgfx::VertexLayout splashVertexLayout;
		splashVertexLayout.begin()
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();

		bgfx::ShaderHandle fs = bgfx::createShader(bgfx::makeRef(fs_splash(), fs_splash_len()));
		bgfx::ShaderHandle vs = bgfx::createShader(bgfx::makeRef(vs_splash(), vs_splash_len()));
		bgfx::ProgramHandle splashProgram = bgfx::createProgram(vs, fs, true);

		bgfx::UniformHandle s_splash = bgfx::createUniform("s_splash", bgfx::UniformType::Sampler);
		bgfx::TextureHandle splashTexture = loadTexture("res/textures/splash.png", BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP, false);

		bgfx::VertexBufferHandle vb = bgfx::createVertexBuffer(bgfx::makeRef(s_splashVertices, sizeof(s_splashVertices)), splashVertexLayout);
		bgfx::IndexBufferHandle ib = bgfx::createIndexBuffer(bgfx::makeRef(s_splashTriList, sizeof(s_splashTriList)));
		
		bgfx::setVertexBuffer(0, vb);
		bgfx::setIndexBuffer(ib);
		bgfx::setTexture(0, s_splash, splashTexture);
		bgfx::setState(BGFX_STATE_DEFAULT);
		bgfx::submit(0, splashProgram);
		bgfx::frame();
		bgfx::destroy(vb);
		bgfx::destroy(ib);
		bgfx::destroy(splashTexture);
		bgfx::destroy(s_splash);
		bgfx::destroy(splashProgram);

		console::command("screenshot", screenshotCallback);
		console::bind(IG_KEY_F2, "screenshot");
		recording = &console::variable("recording", false, recordingCallback);
		console::bind(IG_KEY_F9, "^recording");
		debug = &console::variable("debug", false, debugCallback);
		console::bind(IG_KEY_F3, "^debug");

		const uint32_t checkerBoardSize = 64;
		{
			const bgfx::Memory* mem = bgfx::alloc(checkerBoardSize * checkerBoardSize * 4);
			bimg::imageCheckerboard(mem->data, checkerBoardSize, checkerBoardSize, 8, 0xffe75480, 0xff000000);
			checkerBoard = bgfx::createTexture2D(checkerBoardSize, checkerBoardSize, false, 1
				, bgfx::TextureFormat::BGRA8
				, 0
				| BGFX_SAMPLER_MIN_POINT
				| BGFX_SAMPLER_MIP_POINT
				| BGFX_SAMPLER_MAG_POINT
				, mem
			);
		}

		s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

		U_COLOR = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);

		IG_CORE_INFO("Renderer Initialized");
	}

	std::string getSupportedRenderers()
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

	std::string getGpuInfo()
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

	bgfx::TextureHandle loadTexture(std::string path, uint32_t _flags, bool track)
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

	const bgfx::Memory* loadMemory(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (file.fail())
		{
			IG_CORE_CRITICAL("Could not open shader file: {}", filename);
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		const bgfx::Memory* mem = bgfx::alloc(uint32_t(size + 1));
		if (file.read((char*)mem->data, size))
		{
			mem->data[mem->size - 1] = '\0';
			return mem;
		}
		return nullptr;
	}

	bgfx::ShaderHandle loadShader(const std::string& shader)
	{
		return bgfx::createShader(loadMemory(shader));
	}

	bgfx::ProgramHandle loadProgram(const std::string& vsName, const std::string& fsName)
	{
		std::string shaderPath;

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

		return bgfx::createProgram(loadShader(shaderPath + vsName + ".bin"), loadShader(shaderPath + fsName + ".bin"), true);
	}

	bgfx::ProgramHandle loadProgram(const std::string& name)
	{
		return loadProgram("vs_" + name, "fs_" + name);
	}

	ModelHandle loadModel(std::string path)
	{
		if (models.count(path))
		{
			return models.at(path);
		}
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (file.fail())
		{
			IG_CORE_CRITICAL("Could not open model file: {}", path);
		}

		file.seekg(sizeof(uint64_t), std::ios::beg);
		uint8_t numAttributes;
		file.read((char*)&numAttributes, sizeof(uint8_t));
		bgfx::VertexLayout vertexLayout;
		vertexLayout.begin();
		for (int i = 0; i < numAttributes; i++)
		{
			uint8_t attribute;
			file.read((char*)&attribute, sizeof(uint8_t));

			if (attribute == bgfx::Attrib::Position)
			{
				vertexLayout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
			}
			else if (attribute == bgfx::Attrib::Normal)
			{
				vertexLayout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float);
			}
			else if (attribute == bgfx::Attrib::Tangent)
			{
				vertexLayout.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float);
			}
			else if (attribute == bgfx::Attrib::Bitangent)
			{
				vertexLayout.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float);
			}
			else if (attribute >= bgfx::Attrib::TexCoord0 && attribute <= bgfx::Attrib::TexCoord7)
			{
				vertexLayout.add((bgfx::Attrib::Enum)attribute, 2, bgfx::AttribType::Float);
			}
			else if (attribute >= bgfx::Attrib::Color0 && attribute <= bgfx::Attrib::Color3)
			{
				vertexLayout.add((bgfx::Attrib::Enum)attribute, 4, bgfx::AttribType::Uint8, true);
			}
		}
		vertexLayout.end();

		unsigned int numMeshes;
		file.read((char*)&numMeshes, sizeof(unsigned int));

		std::streampos pos = file.tellg();

		uint32_t modelSize = 0;
		for (unsigned int i = 0; i < numMeshes; i++)
		{
			unsigned int numVerticies;
			unsigned int numIndicies;
			file.read((char*)&numVerticies, sizeof(unsigned int));
			file.read((char*)&numIndicies, sizeof(unsigned int));
			modelSize += (uint32_t)numVerticies * (uint32_t)vertexLayout.getStride() + (uint32_t)numIndicies * sizeof(uint16_t);
			size_t materialNameLength;
			file.read((char*)&materialNameLength, sizeof(size_t));
			std::string materialName;
			materialName.resize(materialNameLength);
			file.read(materialName.data(), materialNameLength);
		}

		const void* modelData = malloc(modelSize);
		file.read((char*)modelData, modelSize);

		file.seekg(pos, std::ios::beg);

		std::vector<Mesh> meshes;
		uint32_t amountRead = 0;
		for (unsigned int i = 0; i < numMeshes; i++)
		{
			unsigned int numVerticies;
			unsigned int numIndicies;
			file.read((char*)&numVerticies, sizeof(unsigned int));
			file.read((char*)&numIndicies, sizeof(unsigned int));
			size_t materialNameLength;
			file.read((char*)&materialNameLength, sizeof(size_t));
			std::string materialName;
			materialName.resize(materialNameLength);
			file.read(materialName.data(), materialNameLength);

			uint32_t verticiesSize = (uint32_t)numVerticies * (uint32_t)vertexLayout.getStride();
			uint32_t indiciesSize = (uint32_t)numIndicies * sizeof(uint16_t);
			Mesh mesh;
			mesh.vbh = bgfx::createVertexBuffer(bgfx::makeRef((uint8_t*)modelData + amountRead, verticiesSize), vertexLayout);
			amountRead += verticiesSize;
			mesh.ibh = bgfx::createIndexBuffer(bgfx::makeRef((uint8_t*)modelData + amountRead, indiciesSize));
			amountRead += indiciesSize;
			mesh.material = loadMaterial(materialName);

			meshes.push_back(mesh);
		}

		file.close();

		Model* model = new Model;
		model->meshes = meshes;

		models[path] = model;
		return model;
	}

	MaterialHandle loadMaterial(std::string name)
	{
		if (materials.count(name))
		{
			return materials.at(name);
		}
		MaterialHandle material = new Material;

		std::ifstream file("res/materials/" + name + ".bin", std::ios::in | std::ios::binary);
		
		if (file.fail())
		{
			IG_CORE_CRITICAL("Could not open material file: {}", name);
		}

		uint8_t shaderNameLength;
		file.read((char*)&shaderNameLength, sizeof(uint8_t));
		std::string shaderName;
		shaderName.resize(shaderNameLength);
		file.read(shaderName.data(), shaderNameLength);

		material->shader = loadProgram(shaderName);
		material->state = BGFX_STATE_DEFAULT;

		MATERIAL_ATTRIBUTE materialAttribute;
		while (file.read((char*)&materialAttribute, sizeof(materialAttribute)))
		{
			if (materialAttribute == COLOR)
			{
				float* vec4 = new float[4];
				file.read((char*)vec4, 4 * sizeof(float));
				material->uniforms.push_back(std::make_pair(U_COLOR, vec4));
			}
		}

		materials[name] = material;
		return material;
	}

	void reset()
	{
		bgfx::reset(input::width, input::height, flags);
		gui::reset();
		input::app->onReset();
	}

	void render()
	{
		ecs::registry.view<ModelHandle, Transformation>().each([&](const auto, auto& model, auto& transformation)
		{
			for (Mesh mesh : model->meshes)
			{
				bgfx::setTransform(&transformation);
				bgfx::setVertexBuffer(0, mesh.vbh);
				bgfx::setIndexBuffer(mesh.ibh);
				for (auto uniform : mesh.material->uniforms)
				{
					bgfx::setUniform(uniform.first, uniform.second);
				}
				for (auto texture : mesh.material->textures)
				{
					bgfx::setTexture(0, texture.first, texture.second);
				}
				bgfx::setState(mesh.material->state);
				bgfx::submit(0, mesh.material->shader);
			}
		});

		if (*debug)
		{
			physics::renderDebug(DBG_DrawWireframe | DBG_DrawAabb);
		}
	}

	void screenshot()
	{
		bgfx::requestScreenShot(BGFX_INVALID_HANDLE, "capture/screenshot");
		IG_CORE_INFO("Screenshot requested");
	}

	void setRecording(bool record)
	{
		setFlag(BGFX_RESET_CAPTURE, record);
		//*recording = record;
		IG_CORE_INFO("Capture: {}", *recording ? "ON" : "OFF");
	}

	bool isRecording()
	{
		return *recording;
	}

	void setDebugOverlay(bool debugOverlay)
	{
		//set reset
		IG_CORE_INFO("Debug Overlay: {}", *debug ? "ON" : "OFF");
	}

	void setFlag(uint32_t flag, bool value)
	{
		flags = (value) ? (flags | flag) : (flags & ~flag);
		reset();
	}

	bool getFlag(uint32_t flag)
	{
		return flags & flag;
	}

	void setFlags(uint32_t newFlags)
	{
		flags = newFlags;
		reset();
	}

	uint32_t getFlags()
	{
		return flags;
	}

	void shutdown()
	{
		IG_CORE_INFO("Shutingdown Renderer");
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

		bgfx::destroy(checkerBoard);
		bgfx::destroy(s_tex);
		bgfx::shutdown();
		IG_CORE_INFO("Renderer Shutdown");
	}
}
} // end namespace igneous
