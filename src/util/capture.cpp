#include "capture.h"

#include <bx/debug.h>
#include <bimg/bimg.h>
#include <inttypes.h>
#include <iostream>
#include <string>

void savePng(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _srcPitch, const void* _src, bimg::TextureFormat::Enum _format, bool _yflip)
{
	bx::FileWriter writer;
	bx::Error err;
	if (bx::open(&writer, _filePath, false, &err))
	{
		bimg::imageWritePng(&writer, _width, _height, _srcPitch, _src, _format, _yflip, &err);
		bx::close(&writer);
	}
}

void saveTga(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _srcPitch, const void* _src, bool _grayscale, bool _yflip)
{
	bx::FileWriter writer;
	bx::Error err;
	if (bx::open(&writer, _filePath, false, &err))
	{
		bimg::imageWriteTga(&writer, _width, _height, _srcPitch, _src, _grayscale, _yflip, &err);
		bx::close(&writer);
	}
}

CaptureCallback::CaptureCallback()
{
	s_fileReader = BX_NEW(allocator, bx::FileReader);
	s_fileWriter = BX_NEW(allocator, bx::FileWriter);
}

CaptureCallback::~CaptureCallback()
{
	BX_DELETE(allocator, s_fileReader);
	s_fileReader = NULL;

	BX_DELETE(allocator, s_fileWriter);
	s_fileWriter = NULL;
}

void CaptureCallback::fatal(bgfx::Fatal::Enum _code, const char* _str)
{
	// Something unexpected happened, inform user and bail out.
	bx::debugPrintf("Fatal error: 0x%08x: %s", _code, _str);

	// Must terminate, continuing will cause crash anyway.
	abort();
}

void CaptureCallback::traceVargs(const char* _filePath, uint16_t _line, const char* _format, va_list _argList)
{
	bx::debugPrintf("%s (%d): ", _filePath, _line);
	bx::debugPrintfVargs(_format, _argList);
}

void CaptureCallback::profilerBegin(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/)
{
}

void CaptureCallback::profilerBeginLiteral(const char* /*_name*/, uint32_t /*_abgr*/, const char* /*_filePath*/, uint16_t /*_line*/)
{
}

void CaptureCallback::profilerEnd()
{
}

uint32_t CaptureCallback::cacheReadSize(uint64_t _id)
{
	char filePath[256];
	bx::snprintf(filePath, sizeof(filePath), "temp/%016" PRIx64, _id);

	// Use cache id as filename.
	bx::FileReaderI* reader = s_fileReader;
	bx::Error err;
	if (bx::open(reader, filePath, &err))
	{
		uint32_t size = (uint32_t)bx::getSize(reader);
		bx::close(reader);
		// Return size of shader file.
		return size;
	}

	// Return 0 if shader is not found.
	return 0;
}

bool CaptureCallback::cacheRead(uint64_t _id, void* _data, uint32_t _size)
{
	char filePath[256];
	bx::snprintf(filePath, sizeof(filePath), "temp/%016" PRIx64, _id);

	// Use cache id as filename.
	bx::FileReaderI* reader = s_fileReader;
	bx::Error err;
	if (bx::open(reader, filePath, &err))
	{
		// Read shader.
		uint32_t result = bx::read(reader, _data, _size, &err);
		bx::close(reader);

		// Make sure that read size matches requested size.
		return result == _size;
	}

	// Shader is not found in cache, needs to be rebuilt.
	return false;
}

void CaptureCallback::cacheWrite(uint64_t _id, const void* _data, uint32_t _size)
{
	char filePath[256];
	bx::snprintf(filePath, sizeof(filePath), "temp/%016" PRIx64, _id);

	// Use cache id as filename.
	bx::FileWriterI* writer = s_fileWriter;
	bx::Error err;
	if (bx::open(writer, filePath, false, &err))
	{
		// Write shader to cache location.
		bx::write(writer, _data, _size, &err);
		bx::close(writer);
	}
}

void CaptureCallback::screenShot(const char* _filePath, uint32_t _width, uint32_t _height, uint32_t _pitch, const void* _data, uint32_t /*_size*/, bool _yflip)
{
	char temp[1024];

	// Save screen shot as PNG.
	bx::snprintf(temp, BX_COUNTOF(temp), "%s.png", _filePath);
	savePng(temp, _width, _height, _pitch, _data, bimg::TextureFormat::BGRA8, _yflip);

	// Save screen shot as TGA.
	bx::snprintf(temp, BX_COUNTOF(temp), "%s.tga", _filePath);
	saveTga(temp, _width, _height, _pitch, _data, false, _yflip);
}

void CaptureCallback::captureBegin(uint32_t _width, uint32_t _height, uint32_t /*_pitch*/, bgfx::TextureFormat::Enum /*_format*/, bool _yflip)
{
	m_writer = BX_NEW(allocator, AviWriter)(s_fileWriter);
	if (!m_writer->open("capture/capture.avi", _width, _height, 30, _yflip))
	{
		BX_DELETE(allocator, m_writer);
		m_writer = NULL;
	}
}

void CaptureCallback::captureEnd()
{
	if (NULL != m_writer)
	{
		m_writer->close();
		BX_DELETE(allocator, m_writer);
		m_writer = NULL;
	}
}

void CaptureCallback::captureFrame(const void* _data, uint32_t /*_size*/)
{
	if (NULL != m_writer)
	{
		m_writer->frame(_data);
	}
}

AviWriter* CaptureCallback::m_writer;
bx::AllocatorI* CaptureCallback::allocator = new Allocator;
bx::FileReaderI* CaptureCallback::s_fileReader = NULL;
bx::FileWriterI* CaptureCallback::s_fileWriter = NULL;