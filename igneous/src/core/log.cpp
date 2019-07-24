#include "igneous/core/log.hpp"

#include <mutex>

#if IG_DEBUG
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

#include "console_sink.hpp"

namespace igneous {
Log::Log()
{
	spdlog::set_pattern("[%T][%n]%^[%l]%$: %v");

#if IG_DEBUG
	//Engine logger
	coreLogger = spdlog::stdout_color_mt("IGNEOUS");
	coreLogger->set_level(spdlog::level::trace);

	//Embedded application logger
	clientLogger = spdlog::stdout_color_mt("APP");
	clientLogger->set_level(spdlog::level::trace);

	//Console logger
	consoleLogger = spdlog::stdout_color_mt("CONSOLE");
	auto con_sink = std::make_shared<console_sink<std::mutex>>();
	con_sink->set_pattern("%v");
	consoleLogger->sinks().push_back(con_sink);
	consoleLogger->set_level(spdlog::level::trace);
#else
	//Console logger
	auto con_sink = std::make_shared<console_sink<std::mutex>>();
	consoleLogger = std::make_shared<spdlog::logger>("CONSOLE", con_sink);
	consoleLogger->set_level(spdlog::level::trace);
#endif
}

Log& Log::getInstance() {
	static Log instance;
	return instance;
}

std::shared_ptr<spdlog::logger>& Log::getCoreLogger() {
	return coreLogger;
}

std::shared_ptr<spdlog::logger>& Log::getClientLogger() {
	return clientLogger;
}

std::shared_ptr<spdlog::logger>& Log::getConsoleLogger() {
	return consoleLogger;
}
} // end namespace igneous
