#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "igneous/core/debug.hpp"

namespace igneous {
class Log
{
public:
	static Log& getInstance();

#if IG_DEBUG
	std::shared_ptr<spdlog::logger>& getCoreLogger();
	std::shared_ptr<spdlog::logger>& getClientLogger();
#endif
	std::shared_ptr<spdlog::logger>& getConsoleLogger();

	Log(Log const&) = delete;
	void operator=(Log const&) = delete;
private:
	Log();
#if IG_DEBUG
	std::shared_ptr<spdlog::logger> coreLogger;
	std::shared_ptr<spdlog::logger> clientLogger;
#endif
	std::shared_ptr<spdlog::logger> consoleLogger;
};
} // end namespace igneous

#if IG_DEBUG
#define IG_CORE_TRACE(...)    igneous::Log::getInstance().getCoreLogger()->trace(__VA_ARGS__)
#define IG_CORE_INFO(...)     igneous::Log::getInstance().getCoreLogger()->info(__VA_ARGS__)
#define IG_CORE_WARN(...)     igneous::Log::getInstance().getCoreLogger()->warn(__VA_ARGS__)
#define IG_CORE_ERROR(...)    igneous::Log::getInstance().getCoreLogger()->error(__VA_ARGS__)
#define IG_CORE_CRITICAL(...)    {igneous::Log::getInstance().getCoreLogger()->critical(__VA_ARGS__); abort();}
#define IG_CORE_LOG(msg, level)  igneous::Log::getInstance().getCoreLogger()->log((spdlog::level::level_enum)level, msg)

#define IG_CLIENT_TRACE(...)  igneous::Log::getInstance().getClientLogger()->trace(__VA_ARGS__)
#define IG_CLIENT_INFO(...)   igneous::Log::getInstance().getClientLogger()->info(__VA_ARGS__)
#define IG_CLIENT_WARN(...)   igneous::Log::getInstance().getClientLogger()->warn(__VA_ARGS__)
#define IG_CLIENT_ERROR(...)  igneous::Log::getInstance().getClientLogger()->error(__VA_ARGS__)
#define IG_CLIENT_CRITICAL(...)  {igneous::Log::getInstance().getClientLogger()->critical(__VA_ARGS__); abort();}
#define IG_CLIENT_LOG(msg, level)  igneous::Log::getInstance().getClientLogger()->log((spdlog::level::level_enum)level, msg)
#else
#define IG_CORE_TRACE(...)
#define IG_CORE_INFO(...)
#define IG_CORE_WARN(...)
#define IG_CORE_ERROR(...)
#define IG_CORE_CRITICAL(...)
#define IG_CORE_LOG(msg, level)

#define IG_CLIENT_TRACE(...)
#define IG_CLIENT_INFO(...)
#define IG_CLIENT_WARN(...)
#define IG_CLIENT_ERROR(...)
#define IG_CLIENT_CRITICAL(...)
#define IG_CLIENT_LOG(msg, level)
#endif

#define IG_CONSOLE_TRACE(...)  igneous::Log::getInstance().getConsoleLogger()->trace(__VA_ARGS__)
#define IG_CONSOLE_INFO(...)   igneous::Log::getInstance().getConsoleLogger()->info(__VA_ARGS__)
#define IG_CONSOLE_WARN(...)   igneous::Log::getInstance().getConsoleLogger()->warn(__VA_ARGS__)
#define IG_CONSOLE_ERROR(...)  igneous::Log::getInstance().getConsoleLogger()->error(__VA_ARGS__)
#define IG_CONSOLE_CRITICAL(...)  {igneous::Log::getInstance().getConsoleLogger()->critical(__VA_ARGS__); abort();}
#define IG_CONSOLE_LOG(msg, level)  igneous::Log::getInstance().getConsoleLogger()->log((spdlog::level::level_enum)level, msg)
