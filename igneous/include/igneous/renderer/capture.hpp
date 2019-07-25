#pragma once

#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <bx/string.h>
#include <bx/filepath.h>
#include <bx/bx.h>
#include "igneous/core/igneous.hpp"
#include "igneous/renderer/aviwriter.hpp"
#include <bx/allocator.h>

namespace igneous {
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

	static AviWriter* m_writer;

	static bx::AllocatorI* allocator;
	static bx::FileReaderI* s_fileReader;
	static bx::FileWriterI* s_fileWriter;
};
} // end namespace igneous
