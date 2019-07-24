#pragma once

#include <unordered_map>

#include <spdlog/sinks/base_sink.h>

#include "igneous/console/console.hpp"

namespace igneous {
template<typename Mutex>
class console_sink : public spdlog::sinks::base_sink <Mutex>
{
public:
	console_sink()
	{
		levels[spdlog::level::trace] = Console::trace;
		levels[spdlog::level::debug] = Console::debug;
		levels[spdlog::level::info] = Console::info;
		levels[spdlog::level::warn] = Console::warn;
		levels[spdlog::level::err] = Console::err;
		levels[spdlog::level::critical] = Console::critical;
		levels[spdlog::level::off] = Console::off;
	}
protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		fmt::memory_buffer formatted;
		spdlog::sinks::sink::formatter_->format(msg, formatted);
		Console::getInstance().write(fmt::to_string(formatted), levels[msg.level]);
	}

	void flush_() override {}

	std::unordered_map<spdlog::level::level_enum, Console::level_enum> levels;
};
} // end namespace igneous
