#pragma once

#include "console/conVar.hpp"

#include <string>
#include <vector>

typedef std::vector<std::string> arg_list;
typedef void(*command_callback)(const std::string&, const arg_list&);

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

class Console
{
public:
	static Console& getInstance();

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

	void alias(const std::string& name, const std::string& exe);
	bool aliasExists(const std::string& name) const;
	void bind(int key, const std::string& exe);
	bool bindExists(int key) const;
	void clear();
	void command(const std::string& name, command_callback callback);
	bool commandExists(const std::string& name) const;
	void execute(call_sequence calls, bool positive = true);
	void execute(const std::string& input, bool record = false, bool positive = true);
	bool exists(const std::string& name) const;
	static call_sequence parse(std::string input);
	void printInfo() const;
	ConVar& get(const std::string& variable);
	bool isValid(const std::string& name) const;
	static void onKey(int key, int scancode, int action, int mods);
	void remove(const std::string& name);
	void render();
	void run(const std::string& command, arg_list args = {});
	void runBind(int key, bool positive = true);
	void runFile(const std::string& filePath);
	void unbind(int key);
	static std::string unparse(call_sequence calls);
	ConVar& variable(const std::string& name, float defaultValue, convar_callback callback = nullptr);
	ConVar& variable(const std::string& name, float defaultValue, float initialValue, convar_callback callback = nullptr);
	bool variableExists(const std::string& variable) const;
	void write(const std::string& contents, level_enum level);

	Console(Console const&) = delete;
	void operator=(Console const&) = delete;
};
