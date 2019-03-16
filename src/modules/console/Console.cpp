#include "Console.h"

#include <cctype>
#include <regex>

#include "../gui/gui.h"
#include "../../util/input.h"
#include "../../util/log.h"

Console::Console()
{
	Register("alias", alias_callback);
	Register("bind", bind_callback);
	Register("unbind", unbind_callback);
	Register("print", print_callback);

	Set("console", false);

	IN_KEY_SINK(&OnKey);
}

Console& Console::GetInstance() {
	static Console instance;
	return instance;
}

void Console::Alias(std::string alias, std::string exe)
{
	if (!exe.empty())
	{
		aliases[alias] = exe;
	}
	else
	{
		aliases.erase(alias);
	}
}

void Console::alias_callback(arg_list args)
{
	if (args.size() >= 2)
	{
		Console& cmd = GetInstance();
		cmd.Alias(args[0], args[1]);
	}
}

void Console::Bind(int key, std::string command)
{
	binds[key] = command;
}

void Console::bind_callback(arg_list args)
{
	if (args.size() >= 2)
	{
		Console& cmd = GetInstance();
		cmd.Bind(std::stoi(args[0]), args[1]);
	}
}

bool Console::CommandExists(std::string command)
{
	return commands.count(command);
}

/**
 * @brief Parses an input string to run commands and set variables.
 *
 * For commands:
 *   Runs the command with the space separated arguements.
 *   Sections wrapped in " will keep spaces.
 *   \" will escape a ".
 * For variables:
 *   Sets the variable to the first argument using a string constructor if possible.
 *   If no argument is given print the value of the variable.
 * If no variable or function the name exists then don't do anything.
 *
 * @param input The input string to parse.
 */
void Console::Execute(std::string input)
{
	IG_CONSOLE_INFO("] {}", input);
	arg_list elements;
	std::size_t clipStart = 0;
	bool clipping = false;
	bool quoting = false;
	for (std::size_t i = 0; i <= input.length(); i++)
	{
		char currentChar;
		if (i != input.length())
			currentChar = input[i];
		else
			currentChar = ' ';
		if (currentChar == '\"' && input[i - 1] != '\\')
		{
			quoting = !quoting;
			if (quoting)
			{
				clipStart = i + 1;
				clipping = true;
			}
			else
			{
				std::string token = input.substr(clipStart, i - clipStart);
				token = std::regex_replace(token, std::regex("\\\\\""), "\"");
				elements.push_back(token);
				clipping = false;
			}
		}
		else
		{
			if (!std::isspace(currentChar) && !clipping)
			{
				clipStart = i;
				clipping = true;
			}
			else if (std::isspace(currentChar) && clipping && !quoting)
			{
				elements.push_back(input.substr(clipStart, i - clipStart));
				clipping = false;
			}
		}
	}
	std::string command = elements[0];
	elements.erase(elements.begin());
	Run(command, elements);
}

void Console::OnKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		Console& cmd = GetInstance();
		cmd.RunBind(key);
	}
}

void Console::Output(std::string out)
{
	output += out;
}

void Console::print_callback(arg_list args)
{
	if (args.size() > 0)
	{
		IG_CONSOLE_INFO("{}", args[0]);
	}
}

void Console::Register(std::string command, command_callback callback)
{
	if (command.find_first_not_of("\t\n\v\f\r ") == std::string::npos)
	{
		IG_CONSOLE_ERROR("Command names may not contain spaces.");
		return;
	}

	if (!commands.count(command) && !variables.count(command))
	{
		commands[command] = callback;
	}
	else
	{
		IG_CONSOLE_ERROR("A command or variable already exists by the name: {}", command);
	}
}

void Console::Remove(std::string command)
{
	if (command.find_first_not_of("\t\n\v\f\r ") == std::string::npos)
	{
		IG_CONSOLE_ERROR("Command names may not contain spaces.");
		return;
	}

	commands.erase(command);
}

void Console::Render()
{
	if (!ImGui::Begin("Console"))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::InputTextMultiline("##output", (char*)output.c_str(), output.capacity() + 1, ImVec2(-1.0f, -ImGui::GetItemsLineHeightWithSpacing()), ImGuiInputTextFlags_ReadOnly);
	const unsigned short bufsize = 128;
	static char bufpass[bufsize] = "";
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.90);
	bool exe = ImGui::InputText("##input", bufpass, bufsize, ImGuiInputTextFlags_EnterReturnsTrue);
	ImGui::PopItemWidth();
	ImGui::SameLine(); exe |= ImGui::Button("Exe", ImVec2(-1.0f, -1.0f));
	if (exe)
	{
		Execute(bufpass);
		strcpy_s(bufpass, bufsize, "");
		ImGui::SetKeyboardFocusHere(-1);
	}
	ImGui::End();
}

void Console::Run(std::string command, arg_list args)
{
	if (command.find_first_not_of("\t\n\v\f\r ") == std::string::npos)
	{
		IG_CONSOLE_ERROR("Command names may not contain spaces.");
		return;
	}

	if (commands.count(command))
	{
		command_callback callback = commands.at(command);
		callback(args);
	}
	else
	{
		IG_CONSOLE_ERROR("Unrecognized command: {}", command);
	}
}

void Console::RunBind(int key)
{
	if (binds.count(key))
	{
		Execute(binds.at(key));
	}
}

void Console::Set(std::string variable, std::any value)
{
	if (variable.find_first_not_of("\t\n\v\f\r ") == std::string::npos)
	{
		IG_CONSOLE_ERROR("Variable names may not contain spaces.");
		return;
	}

	if (!commands.count(variable) && !variables.count(variable))
	{
		variables[variable] = value;
	}
	else
	{
		IG_CONSOLE_ERROR("A command or variable already exists by the name: {}", variable);
	}
}

void Console::Unbind(int key)
{
	binds.erase(key);
}

void Console::unbind_callback(arg_list args)
{
	Console& cmd = GetInstance();
	cmd.Unbind(std::stoi(args[0]));
}

void Console::Unset(std::string variable)
{
	if (variable.find_first_not_of("\t\n\v\f\r ") == std::string::npos)
	{
		IG_CONSOLE_ERROR("Variable names may not contain spaces.");
		return;
	}

	variables.erase(variable);
}

bool Console::VariableExists(std::string variable)
{
	return variables.count(variable);
}
