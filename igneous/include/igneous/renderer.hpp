#pragma once

#include <map>
#include <string>
#include <vector>

#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <bx/string.h>
#include <bx/filepath.h>
#include <bx/bx.h>
#include <bx/allocator.h>

namespace igneous {
class Renderer
{
public:
	static Renderer& getInstance();

	std::string getSupportedRenderers();
	std::string getGpuInfo();

	bgfx::TextureHandle loadTexture(const char* _filePath, uint32_t _flags = 0, bool track = true);
	bgfx::ProgramHandle loadProgram(const char* vs, const char* fs);

	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;
};

struct CaptureCallback : public bgfx::CallbackI
{
	CaptureCallback();

	virtual ~CaptureCallback();

	virtual void fatal(const char* _filePath, uint16_t _line, bgfx::Fatal::Enum _code, const char* _str) override;

	virtual void traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList) override;

	virtual void profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override;

	virtual void profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/) override;

	virtual void profilerEnd() override;

	virtual uint32_t cacheReadSize(uint64_t _id) override;

	virtual bool cacheRead(uint64_t _id, void* _data, uint32_t _size) override;

	virtual void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) override;

	virtual void screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t /*_size*/, bool _yflip) override;

	virtual void captureBegin(uint32_t _width, uint32_t _height, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool _yflip) override;

	virtual void captureEnd() override;

	virtual void captureFrame(const void* _data, uint32_t /*_size*/) override;

	static bx::AllocatorI* allocator;
	static bx::FileReaderI* s_fileReader;
	static bx::FileWriterI* s_fileWriter;
};

struct Vertex
{
	float pos_x;
	float pos_y;
	float pos_z;
	float norm_x;
	float norm_y;
	float norm_z;
	float tex_x;
	float tex_y;
	float col_r;
	float col_g;
	float col_b;
	float col_a;
	float tan_x;
	float tan_y;
	float tan_z;
	float bi_x;
	float bi_y;
	float bi_z;
	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
			.end();
	}
	static bgfx::VertexDecl ms_decl;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint16_t> indices, std::vector<bgfx::TextureHandle> textures);
	~Mesh();

	bgfx::VertexBufferHandle vbh;
	bgfx::IndexBufferHandle ibh;
	std::vector<bgfx::TextureHandle> textures;
};

class Model
{
public:
	Model(const char* path);
	Model();
	~Model();

	std::vector<Mesh*> meshes;
	std::string directory;
};
} // end namespace igneous
