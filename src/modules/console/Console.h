#pragma once

#include <any>
#include <string>
#include <unordered_map>

typedef std::vector<std::string> arg_list;
typedef void(*command_callback)(arg_list);

class Console
{
public:
	static Console& GetInstance();

	void Alias(std::string alias, std::string exe);
	void Bind(int key, std::string command);
	bool CommandExists(std::string command);
	void Execute(std::string input);
	template <typename type>
	inline type Get(std::string variable)
	{
		if (variable.find_first_not_of("\t\n\v\f\r ") == std::string::npos)
		{
			std::cout << "Variable names may not contain spaces.\n";
			return NULL;
		}

		if (variables.count(variable))
		{
			return std::any_cast<type>(variables.at(variable));
		}
		else
		{
			std::cout << "Unrecognized variable: " << variable << "\n";
			return NULL;
		}
	}
	static void OnKey(int key, int scancode, int action, int mods);
	void Output(std::string output);
	void Register(std::string command, command_callback callback);
	void Remove(std::string command);
	void Render();
	void Run(std::string command, arg_list args = {});
	void RunBind(int key);
	void Set(std::string variable, std::any value);
	void Unbind(int key);
	void Unset(std::string variable);
	bool VariableExists(std::string variable);

	Console(Console const&) = delete;
	void operator=(Console const&) = delete;
private:
	Console();
	~Console() {}

	static void alias_callback(arg_list args);
	static void bind_callback(arg_list args);
	static void print_callback(arg_list args);
	static void unbind_callback(arg_list args);

	std::unordered_map<std::string, std::string> aliases;
	std::unordered_map<int, std::string> binds;
	std::unordered_map<std::string, command_callback> commands;
	std::unordered_map<std::string, std::any> variables;

	std::string output;
};
