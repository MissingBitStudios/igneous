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
		levels[spdlog::level::trace] = console::trace;
		levels[spdlog::level::debug] = console::debug;
		levels[spdlog::level::info] = console::info;
		levels[spdlog::level::warn] = console::warn;
		levels[spdlog::level::err] = console::err;
		levels[spdlog::level::critical] = console::critical;
		levels[spdlog::level::off] = console::off;
	}
protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		fmt::memory_buffer formatted;
		spdlog::sinks::sink::formatter_->format(msg, formatted);
		console::write(fmt::to_string(formatted), levels[msg.level]);
	}

	void flush_() override {}

	std::unordered_map<spdlog::level::level_enum, console::level_enum> levels;
};
} // end namespace igneous
