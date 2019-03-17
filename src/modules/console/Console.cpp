#include "Console.h"

#include <cctype>
#include <regex>

#include "../../util/input.h"
#include "../../util/log.h"

Console::Console()
{
	Register("alias", alias_callback);
	Register("bind", bind_callback);
	Register("clear", clear_callback);
	Register("unbind", unbind_callback);
	Register("print", print_callback);

	Set("console", false);

	IN_KEY_SINK(&OnKey);

	colors[trace] = WHITE;
	colors[debug] = CYAN;
	colors[info] = GREEN;
	colors[warn] = YELLOW;
	colors[err] = RED;
	colors[critical] = RED;
	colors[off] = WHITE;
}

Console::~Console()
{
	Clear();
}

Console& Console::GetInstance() {
	static Console instance;
	return instance;
}

void Console::Alias(std::string alias, std::string exe)
{
	RETURN_INVALID(alias, "Alias");
	RETURN_EXISTS(alias);

	if (!exe.empty())
	{
		aliases[alias] = exe;
	}
	else
	{
		aliases.erase(alias);
	}
}

bool Console::AliasExists(std::string alias)
{
	return aliases.count(alias);
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

bool Console::BindExists(int key)
{
	return binds.count(key);
}

void Console::bind_callback(arg_list args)
{
	if (args.size() >= 2)
	{
		Console& cmd = GetInstance();
		cmd.Bind(std::stoi(args[0]), args[1]);
	}
}

void Console::Clear()
{
	for (line l : lines)
	{
		delete l.contents;
	}
	lines.clear();
}

void Console::clear_callback(arg_list args)
{
	Console& cmd = GetInstance();
	cmd.Clear();
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
void Console::Execute(std::string input, bool record)
{
	if (record)
	{
		IG_CONSOLE_TRACE("] {}", input);
	}
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
	if (elements.size() > 0)
	{
		if (record)
		{
			if ((history.size() > 0 && input != history[0]) || history.size() == 0)
			{
				if (history.size() >= max_history)
				{
					history.pop_back();
				}
				history.push_front(input);
			}
		}
		std::string command = elements[0];
		elements.erase(elements.begin());
		Run(command, elements);
	}
}

bool Console::Exists(std::string name)
{
	return AliasExists(name) || CommandExists(name) || VariableExists(name);
}

bool Console::IsValid(std::string name)
{
	return name.find_first_not_of("\t\n\v\f\r ") != std::string::npos;
}

void Console::OnKey(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		Console& cmd = GetInstance();
		cmd.RunBind(key);
	}
}

void Console::Output(std::string contents, level_enum level)
{
	if (lines.size() >= max_lines)
	{
		lines.pop_front();
	}
	lines.push_back(line{ colors[level], _strdup(contents.c_str()) });
}

void Console::print_callback(arg_list args)
{
	if (args.size() > 0)
	{
		IG_CONSOLE_TRACE(args[0]);
	}
}

void Console::Register(std::string command, command_callback callback)
{
	RETURN_INVALID(command, "Command");
	RETURN_EXISTS(command);

	commands[command] = callback;
}

void Console::Remove(std::string command)
{
	RETURN_INVALID(command, "Command");

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
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -(ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y )), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	for (line l : lines)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)l.color);
		ImGui::TextUnformatted(l.contents);
		ImGui::PopStyleColor();
	}
	if (scrollToBottom)
	{
		ImGui::SetScrollHereY(1.0f);
		scrollToBottom = false;
	}
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Separator();
	static char in[256];
	strcpy_s(in, 256, input.c_str());
	ImGui::PushItemWidth(-50.0f);
	bool exe = ImGui::InputText("##input", in, 256, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub);
	ImGui::PopItemWidth();
	input = in;
	ImGui::SameLine(); exe |= ImGui::Button("Exe");
	if (exe)
	{
		Execute(input, true);
		input = "";
		editing = "";
		history_index = -1;
		ImGui::SetKeyboardFocusHere(-1);
		scrollToBottom = true;
	}
	ImGui::End();
}

void Console::Run(std::string command, arg_list args)
{
	RETURN_INVALID(command, "Command");

	if (CommandExists(command))
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
	if (BindExists(key))
	{
		Execute(binds.at(key));
	}
}

void Console::Set(std::string variable, var_type value)
{
	RETURN_INVALID(variable, "Variable");

	RETURN_EXISTS(variable);

	variables[variable] = value;
}

int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackHistory:
	{
		if (history_index == -1)
		{
			editing = input;
		}

		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (history_index < (int)history.size() - 1)
			{
				history_index++;
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history[history_index].c_str());
			}
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (history_index >= -1)
			{
				history_index--;
			}

			if (history_index == -1)
			{
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, editing.c_str());
			}
			else if (history_index == -2)
			{
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, "");
				editing = "";
				history_index = -1;
			}
			else
			{
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history[history_index].c_str());
			}
		}
		break;
	}
	default:
		break;
	}
	return 0;
}

int Console::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	Console& cmd = GetInstance();
	return cmd.TextEditCallback(data);
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
	RETURN_INVALID(variable, "Variable");

	variables.erase(variable);
}

bool Console::VariableExists(std::string variable)
{
	return variables.count(variable);
}
