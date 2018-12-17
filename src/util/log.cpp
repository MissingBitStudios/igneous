#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

Log::Log()
{
	spdlog::set_pattern("%^[%T][%l][%n]: %v%$");
	coreLogger = spdlog::stdout_color_mt("IGNEOUS");
	coreLogger->set_level(spdlog::level::trace);

	clientLogger = spdlog::stdout_color_mt("APP");
	clientLogger->set_level(spdlog::level::trace);
}

Log& Log::getInstance() {
	static Log instance;
	return instance;
}