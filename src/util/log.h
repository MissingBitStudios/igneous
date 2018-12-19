#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "debug.h"

class Log
{
public:
	static Log& getInstance();

	inline std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
	inline std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }

	Log(Log const&) = delete;
	void operator=(Log const&) = delete;
private:
	Log();
	std::shared_ptr<spdlog::logger> coreLogger;
	std::shared_ptr<spdlog::logger> clientLogger;
};

#if IG_DEBUG
#define IG_CORE_TRACE(...)    Log::getInstance().getCoreLogger()->trace(__VA_ARGS__)
#define IG_CORE_INFO(...)     Log::getInstance().getCoreLogger()->info(__VA_ARGS__)
#define IG_CORE_WARN(...)     Log::getInstance().getCoreLogger()->warn(__VA_ARGS__)
#define IG_CORE_ERROR(...)    Log::getInstance().getCoreLogger()->error(__VA_ARGS__)
#define IG_CORE_FATAL(...)    Log::getInstance().getCoreLogger()->fatal(__VA_ARGS__)

#define IG_CLIENT_TRACE(...)  Log::getInstance().getClientLogger()->trace(__VA_ARGS__)
#define IG_CLIENT_INFO(...)   Log::getInstance().getClientLogger()->info(__VA_ARGS__)
#define IG_CLIENT_WARN(...)   Log::getInstance().getClientLogger()->warn(__VA_ARGS__)
#define IG_CLIENT_ERROR(...)  Log::getInstance().getClientLogger()->error(__VA_ARGS__)
#define IG_CLIENT_FATAL(...)  Log::getInstance().getClientLogger()->fatal(__VA_ARGS__)
#else
#define IG_CORE_TRACE(...)
#define IG_CORE_INFO(...)
#define IG_CORE_WARN(...)
#define IG_CORE_ERROR(...)
#define IG_CORE_FATAL(...)

#define IG_CLIENT_TRACE(...)
#define IG_CLIENT_INFO(...)
#define IG_CLIENT_WARN(...)
#define IG_CLIENT_ERROR(...)
#define IG_CLIENT_FATAL(...)
#endif