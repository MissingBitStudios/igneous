#pragma once

#include <iosfwd>
#include <string>
#include <vector>

namespace igneous {
typedef void(*convar_callback)(float, float);

class ConVar
{
public:
	ConVar();
	ConVar(const std::string& name, float defaultValue, convar_callback callback = nullptr);
	ConVar(const std::string& name, float defaultValue, float initialValue, convar_callback callback = nullptr);
	ConVar(const ConVar&) = delete;

	float getDefaultValue() const;
	const std::string& getName() const;
	void reset();

	ConVar& operator=(bool boolValue);
	ConVar& operator=(float floatValue);
	ConVar& operator=(int intValue);
	ConVar& operator=(const std::string& strValue);

	friend std::ostream& operator<<(std::ostream& os, const ConVar& conVar);
	friend std::istream& operator>>(std::istream& is, ConVar& conVar);

	bool operator!() const;

	operator bool() const;
	operator float() const;
	operator int() const;
};

typedef std::vector<std::string> arg_list;
typedef void(*command_callback)(const std::string&, const arg_list&);

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
	void execute(const std::string& input, bool record = false, bool positive = true);
	bool exists(const std::string& name) const;
	ConVar& get(const std::string& variable);
	bool isValid(const std::string& name) const;
	void remove(const std::string& name);
	void run(const std::string& command, arg_list args = {});
	void runBind(int key, bool positive = true);
	void runFile(const std::string& filePath);
	void unbind(int key);
	ConVar& variable(const std::string& name, float defaultValue, convar_callback callback = nullptr);
	ConVar& variable(const std::string& name, float defaultValue, float initialValue, convar_callback callback = nullptr);
	bool variableExists(const std::string& variable) const;

	Console(Console const&) = delete;
	void operator=(Console const&) = delete;
};
} // end namespace igneous
