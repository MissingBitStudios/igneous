#include "log.h"

#include <mutex>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/dist_sink.h>

#include "../modules/console/console_sink.h"

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
#endif

	//Console logger
	consoleLogger = spdlog::stdout_color_mt("CONSOLE");
	auto con_sink = std::make_shared<console_sink<std::mutex>>();
	con_sink->set_pattern("%v");
	consoleLogger->sinks().push_back(con_sink);
	consoleLogger->set_level(spdlog::level::trace);
}

Log& Log::getInstance() {
	static Log instance;
	return instance;
}