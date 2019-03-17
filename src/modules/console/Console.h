#pragma once

#include <deque>
#include <string>
#include <unordered_map>
#include <variant>

#include "../gui/gui.h"

#define RETURN_EXISTS(name) if (Exists(name)) { IG_CONSOLE_ERROR("A token already exists with the name: {}", name); return; }
#define RETURN_INVALID(name, type) if (!IsValid(name)) { IG_CONSOLE_ERROR("{} name is invalid.", type); return; }

typedef std::vector<std::string> arg_list;
typedef void(*command_callback)(arg_list);
typedef std::variant<bool, float, int, std::string> var_type;

class Console
{
public:
	static Console& GetInstance();

	const ImColor RED = ImColor(255, 0, 0);
	const ImColor GREEN = ImColor(0, 255, 0);
	const ImColor BLUE = ImColor(0, 0, 255);
	const ImColor CYAN = ImColor(0, 255, 255);
	const ImColor WHITE = ImColor(255, 255, 255);
	const ImColor YELLOW = ImColor(255, 255, 0);

	enum level_enum
	{
		trace,
		debug,
		info,
		warn,
		err,
		critical,
		off
	};

	void Alias(std::string alias, std::string exe);
	bool AliasExists(std::string alias);
	void Bind(int key, std::string command);
	bool BindExists(int key);
	void Clear();
	bool CommandExists(std::string command);
	void Execute(std::string input, bool record = false);
	bool Exists(std::string name);
	template <typename type>
	inline type Get(std::string variable)
	{
		static_assert(std::holds_alternative<type>(var_type), "type must be a var_type");
		RETURN_INVALID(variable, "Variable");

		if (VariableExists(variable))
		{
			return std::get<type>(variables.at(variable));
		}
		else
		{
			std::cout << "Unrecognized variable: " << variable << "\n";
			return NULL;
		}
	}
	bool IsValid(std::string name);
	static void OnKey(int key, int scancode, int action, int mods);
	void Output(std::string contents, level_enum level);
	void Register(std::string command, command_callback callback);
	void Remove(std::string command);
	void Render();
	void Run(std::string command, arg_list args = {});
	void RunBind(int key);
	void Set(std::string variable, var_type value);
	int TextEditCallback(ImGuiInputTextCallbackData* data);
	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
	void Unbind(int key);
	void Unset(std::string variable);
	bool VariableExists(std::string variable);

	Console(Console const&) = delete;
	void operator=(Console const&) = delete;
private:
	Console();
	~Console();

	static void alias_callback(arg_list args);
	static void bind_callback(arg_list args);
	static void clear_callback(arg_list args);
	static void print_callback(arg_list args);
	static void unbind_callback(arg_list args);

	struct line
	{
		ImColor color;
		const char* contents;
	};

	const unsigned int max_lines = 100;
	bool scrollToBottom = false;

	const unsigned int max_history = 100;
	int history_index = -1;
	std::string input;
	std::string editing;

	std::unordered_map<std::string, std::string> aliases;
	std::unordered_map<int, std::string> binds;
	std::unordered_map<std::string, command_callback> commands;
	std::unordered_map<level_enum, ImColor> colors;
	std::deque<std::string> history;
	std::deque<line> lines;
	std::unordered_map<std::string, var_type> variables;
};
