#include "console/console.hpp"

#include <cctype>
#include <cstring>
#include <fstream>
#include <regex>
#include <sstream>

#include "core/input.hpp"

Console::Console()
{
	IG_CORE_INFO("Initializing Console");
	command("alias", aliasCallback);
	command("bind", bindCallback);
	command("clear", clearCallback);
	command("help", helpCallback);
	command("print", printCallback);
	command("run_file", runFileCallback);
	command("unbind", unbindCallback);

	consoleVar = variable("console", "0");

	alias("+test", "print down;play_sound forest");
	alias("-test", "print up");
	bind(GLFW_KEY_GRAVE_ACCENT, "^console");
	bind(GLFW_KEY_Z, "+test");
	bind(GLFW_KEY_V, "-test");

	IN_KEY_SINK(&onKey);

	colors[trace] = WHITE;
	colors[debug] = CYAN;
	colors[info] = GREEN;
	colors[warn] = YELLOW;
	colors[err] = RED;
	colors[critical] = RED;
	colors[off] = WHITE;

	IG_CORE_INFO("Console Initialized");
}

Console& Console::getInstance() {
	static Console instance;
	return instance;
}

void Console::alias(const std::string& name, const std::string& exe)
{
	RETURN_INVALID(name);
	if (commandExists(name) || variableExists(name)) { IG_CONSOLE_ERROR("A command or variable already exists with the name: {}", name); return; }

	if (!exe.empty())
	{
		aliases[name] = parse(exe);
	}
}

bool Console::aliasExists(const std::string& name) const
{
	return aliases.count(name);
}

void Console::aliasCallback(const std::string& name, const arg_list& args)
{
	Console& cmd = getInstance();
	if (args.size() >= 2)
	{
		cmd.alias(args[0], args[1]);
	}
	else if (args.size() == 1)
	{
		cmd.remove(args[0]);
	}
}

void Console::bind(int key, const std::string& exe)
{
	binds[key] = parse(exe);
}

bool Console::bindExists(int key) const
{
	return binds.count(key);
}

void Console::bindCallback(const std::string& name, const arg_list& args)
{
	if (args.size() >= 2)
	{
		Console& cmd = getInstance();
		cmd.bind(std::stoi(args[0]), args[1]);
	}
}

void Console::clear()
{
	lines.clear();
}

void Console::clearCallback(const std::string& name, const arg_list& args)
{
	Console& cmd = getInstance();
	cmd.clear();
}

void Console::command(const std::string& name, command_callback callback)
{
	RETURN_INVALID(name);
	RETURN_EXISTS(name);

	commands[name] = callback;
}

bool Console::commandExists(const std::string& name) const
{
	return commands.count(name);
}

void Console::execute(call_sequence calls, bool positive) const
{
	for (call c : calls)
	{
		if (c.flags & CF_TOGGLE && positive)
		{
			if (variableExists(c.name))
			{
				ConVar& var = *get(c.name);
				var = !var;
			}
			else
			{
				IG_CONSOLE_ERROR("No variable exists with the name: {}", c.name);
			}
		}
		else if (c.flags & CF_ONOFF)
		{
			std::string name = positive ? "+" + c.name : "-" + c.name;
			if (aliasExists(name) || commandExists(name))
			{
				run(name, c.args);
			}
			else
			{
				IG_CONSOLE_ERROR("No alias or command exists with the name: {}", c.name);
			}
		}
		else if (positive)
		{
			if (aliasExists(c.name) || commandExists(c.name))
			{
				run(c.name, c.args);
			}
			else if (variableExists(c.name))
			{
				ConVar& var = *get(c.name);
				if (c.args.empty())
				{
					IG_CONSOLE_TRACE(var);
				}
				else
				{
					var = c.args[0];
				}
			}
			else
			{
				IG_CONSOLE_ERROR("No alias, command, or variable exists with the name: {}", c.name);
			}
		}
	}
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
void Console::execute(const std::string& input, bool record, bool positive)
{
	if (record)
	{
		IG_CONSOLE_TRACE("] {}", input);
	}

	if (input.size() > 0)
	{
		execute(parse(input), positive);

		if (record && ((history.size() > 0 && input != history[0]) || history.size() == 0))
		{
			if (history.size() >= max_history)
			{
				history.pop_back();
			}
			history.push_front(input);
		}
	}
}

bool Console::exists(const std::string& name) const
{
	return aliasExists(name) || commandExists(name) || variableExists(name);
}

ConVar* Console::get(const std::string& variable) const
{
	if (!isValid(variable)) { IG_CONSOLE_ERROR("Variable name is invalid."); return nullptr; }

	if (variables.count(variable))
	{
		return variables.at(variable);
	}
	else
	{
		return nullptr;
	}
}

void Console::helpCallback(const std::string& name, const arg_list& args)
{
	Console& cmd = getInstance();
	cmd.printInfo();
}

bool Console::isValid(const std::string& name) const
{
	return name.find_first_not_of("\t\n\v\f\r ") != std::string::npos;
}

void Console::onKey(int key, int scancode, int action, int mods)
{
	Console& cmd = getInstance();
	if (action == GLFW_PRESS)
	{
		cmd.runBind(key, true);
	}
	else if (action == GLFW_RELEASE)
	{
		cmd.runBind(key, false);
	}
}

call_sequence Console::parse(std::string input)
{
	call_sequence seq;
	if (input.empty()) return seq;
	arg_list elements;
	size_t clipStart = 0;
	bool clipping = false;
	bool quoting = false;
	for (size_t i = 0; i <= input.size(); i++)
	{
		char cchar = i == input.size() ? ' ' : input[i];

		if (cchar == '"')
		{
			if (input[i - 1] == '\\')
			{
				input.erase(i - 1, 1);
				i--;
			}
			else
			{
				if (quoting)
				{
					elements.push_back(input.substr(clipStart, i - clipStart));
					clipping = false;
				}
				quoting = !quoting;
			}
		}
		else
		{
			if ((std::isspace(cchar) || cchar == ';') && clipping && !quoting)
			{
				elements.push_back(input.substr(clipStart, i - clipStart));
				clipping = false;
			}
			else if (!clipping && !std::isspace(cchar))
			{
				clipStart = i;
				clipping = true;
			}

			if ((i == input.size() || cchar == ';') && !clipping && !quoting)
			{
				std::string name = elements[0];
				elements.erase(elements.begin());
				int flags = CF_NONE;
				if (name[0] == '^') { flags |= CF_TOGGLE; name.erase(name.begin()); }
				else if (name[0] == '+') { flags |= CF_ONOFF; name.erase(name.begin()); }
				seq.push_back(call{ name, elements, flags });
				elements.clear();
			}
		}
	}
	return seq;
}

void Console::printCallback(const std::string& name, const arg_list& args)
{
	if (args.size() > 0)
	{
		IG_CONSOLE_TRACE(args[0]);
	}
}

void Console::printInfo() const
{
	IG_CONSOLE_TRACE("Aliases:");
	for (auto it = aliases.begin(); it != aliases.end(); it++)
	{
		IG_CONSOLE_TRACE("  {} = {}", it->first, unparse(it->second));
	}

	IG_CONSOLE_TRACE("Binds:");
	for (auto it = binds.begin(); it != binds.end(); it++)
	{
		IG_CONSOLE_TRACE("  {} = {}", glfwGetKeyName(it->first, 0), unparse(it->second));
	}

	IG_CONSOLE_TRACE("Commands:");
	for (auto it = commands.begin(); it != commands.end(); it++)
	{
		IG_CONSOLE_TRACE("  {}", it->first);
	}

	IG_CONSOLE_TRACE("Variables:");
	for (auto it = variables.begin(); it != variables.end(); it++)
	{
		IG_CONSOLE_TRACE("  {} = {}", it->first, *it->second);
	}
}

void Console::remove(const std::string& name)
{
	RETURN_INVALID(name);

	if (aliasExists(name))
	{
		aliases.erase(name);
	}
	else if (commandExists(name))
	{
		commands.erase(name);
	}
	else if (variableExists(name))
	{
		delete variables[name];
		variables.erase(name);
	}
}

void Console::render()
{
	if (!*consoleVar) return;
	static bool open;
	if (!ImGui::Begin("Console", &open, ImVec2(ImGui::GetWindowSize().x * 0.5f, ImGui::GetWindowSize().y * 0.5f)) || ImGui::IsWindowAppearing())
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
		ImGui::TextUnformatted(l.contents.c_str());
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
	ImGui::PushItemWidth(-50.0f);
	static char buf[max_input];
	std::strcpy(buf, input.c_str());
	bool exe = ImGui::InputText("##input", buf, max_input, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, &textEditCallbackStub);
	ImGui::SetItemDefaultFocus();
	input = buf;
	ImGui::PopItemWidth();
	ImGui::SameLine(); exe |= ImGui::Button("Exe", ImVec2(-1.0f, -1.0f));
	if (exe)
	{
		execute(input, true);
		input = "";
		editing = "";
		history_index = -1;
		ImGui::SetKeyboardFocusHere(-1);
		scrollToBottom = true;
	}
	ImGui::End();
}

void Console::run(const std::string& name, arg_list args) const
{
	RETURN_INVALID(name);

	if (aliasExists(name))
	{
		execute(aliases.at(name));
	}
	else if (commandExists(name))
	{
		command_callback callback = commands.at(name);
		callback(name, args);
	}
	else
	{
		IG_CONSOLE_ERROR("Unrecognized command: {}", name);
	}
}

void Console::runBind(int key, bool positive) const
{
	if (bindExists(key))
	{
		execute(binds.at(key), positive);
	}
}

void Console::runFile(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (file.is_open()) {
		for (std::string line; getline(file, line);) {
			execute(line);
		}
		file.close();
	}
	else
	{
		IG_CONSOLE_ERROR("Could not open file: {}", filePath);
	}
}

void Console::runFileCallback(const std::string& name, const arg_list& args)
{
	if (args.size() >= 1)
	{
		Console& cmd = getInstance();
		cmd.runFile(args[0]);
	}
}

int Console::textEditCallback(ImGuiInputTextCallbackData* data)
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

			data->DeleteChars(0, data->BufTextLen);
			if (history_index == -1)
			{
				data->InsertChars(0, editing.c_str());
			}
			else if (history_index == -2)
			{
				editing = "";
				history_index = -1;
			}
			else
			{
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

int Console::textEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	Console& cmd = getInstance();
	return cmd.textEditCallback(data);
}

void Console::unbind(int key)
{
	binds.erase(key);
}

void Console::unbindCallback(const std::string& name, const arg_list& args)
{
	Console& cmd = getInstance();
	cmd.unbind(std::stoi(args[0]));
}

std::string Console::unparse(call_sequence calls)
{
	std::string out;
	for (size_t i = 0; i < calls.size(); i++)
	{
		const call& c = calls[i];
		// add name with prefix if any
		if (c.flags & CF_TOGGLE) out += '^' + c.name;
		else if (c.flags & CF_ONOFF) out += '+' + c.name;
		else out += c.name;

		for (const std::string& arg : c.args)
		{
			out += ' ';
			//add arg with quotes if theres a space
			if (arg.find(' ') == std::string::npos) out += arg;
			else out += '"' + arg + '"';
		}
		//add ; if not last call
		if (i < calls.size() - 1) out += ';';
	}
	return out;
}

ConVar* Console::variable(const std::string& name, const std::string& defaultValue, convar_callback callback)
{
	return variables[name] = new ConVar(name, defaultValue, callback);
}

ConVar* Console::variable(const std::string& name, const std::string& defaultValue, const std::string& initialValue, convar_callback callback)
{
	return variables[name] = new ConVar(name, defaultValue, initialValue, callback);
}

bool Console::variableExists(const std::string& variable) const
{
	return variables.count(variable);
}

void Console::write(const std::string& contents, level_enum level)
{
	if (lines.size() >= max_lines)
	{
		lines.pop_front();
	}
	lines.push_back(line{ colors[level], contents });
}