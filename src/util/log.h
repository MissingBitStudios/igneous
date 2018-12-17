#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

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