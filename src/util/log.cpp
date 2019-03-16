#include "log.h"

#include <mutex>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/dist_sink.h>

#include "../modules/console/console_sink.h"

Log::Log()
{
	spdlog::set_pattern("%^[%T][%n][%l]: %v%$");

	coreLogger = spdlog::stdout_color_mt("IGNEOUS");
	coreLogger->set_level(spdlog::level::trace);

	clientLogger = spdlog::stdout_color_mt("APP");
	clientLogger->set_level(spdlog::level::trace);

	auto dist_sink = std::make_shared<spdlog::sinks::dist_sink_st>();

	auto con_sink = std::make_shared<console_sink<std::mutex>>();
	con_sink->set_pattern("%^[%l] %v%$");
	dist_sink->add_sink(con_sink);

	auto color_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	color_sink->set_pattern("%^[%T][%n][%l]: %v%$");
	dist_sink->add_sink(color_sink);

	consoleLogger = std::make_shared<spdlog::logger>("CONSOLE", dist_sink);
	consoleLogger->set_level(spdlog::level::trace);
	spdlog::register_logger(consoleLogger);
}

Log& Log::getInstance() {
	static Log instance;
	return instance;
}