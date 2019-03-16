#include <spdlog/sinks/base_sink.h>

#include "Console.h"

template<typename Mutex>
class console_sink : public spdlog::sinks::base_sink <Mutex>
{
protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		fmt::memory_buffer formatted;
		sink::formatter_->format(msg, formatted);
		Console::GetInstance().Output(fmt::to_string(formatted));
	}

	void flush_() override {}
};