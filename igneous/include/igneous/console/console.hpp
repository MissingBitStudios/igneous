#pragma once

#include "igneous/console/conVar.hpp"

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include <imgui/imgui.h>

#define RETURN_EXISTS(name) if (exists(name)) { IG_CONSOLE_ERROR("A token already exists with the name: {}", name); return; }
#define RETURN_INVALID(name) if (!isValid(name)) { IG_CONSOLE_ERROR("Token name is invalid."); return; }

namespace igneous {
typedef std::vector<std::string> arg_list;
typedef void(*command_callback)(const std::string&, const arg_list&);

namespace console
{
	enum CF_enum
	{
		CF_NONE = 0,
		CF_TOGGLE = 1 << 0,
		CF_ONOFF = 1 << 1
	};

	struct call
	{
		std::string name;
		arg_list args;
		int flags = CF_NONE;
	};

	typedef std::vector<call> call_sequence;

	enum level_enum
	{
		trace,
		debug,
		info,
		warn,
		err,
		critical,
		off,
		NUM_LEVELS
	};

	void init();
	void alias(const std::string& name, const std::string& exe);
	bool aliasExists(const std::string& name);
	void bind(int key, const std::string& exe);
	bool bindExists(int key);
	void clear();
	void command(const std::string& name, command_callback callback);
	bool commandExists(const std::string& name);
	void execute(call_sequence calls, bool positive = true);
	void execute(const std::string& input, bool record = false, bool positive = true);
	bool exists(const std::string& name);
	call_sequence parse(std::string input);
	void printInfo();
	ConVar& get(const std::string& variable);
	bool isValid(const std::string& name);
	void onKey(int key, int scancode, int action, int mods);
	void remove(const std::string& name);
	void render();
	void run(const std::string& command, arg_list args = {});
	void runBind(int key, bool positive = true);
	void runFile(const std::string& filePath);
	void unbind(int key);
	std::string unparse(call_sequence calls);
	ConVar& variable(const std::string& name, float defaultValue, convar_callback callback = nullptr);
	ConVar& variable(const std::string& name, float defaultValue, float initialValue, convar_callback callback = nullptr);
	bool variableExists(const std::string& variable);
	void write(const std::string& contents, level_enum level);
}
} // end namespace igneous
