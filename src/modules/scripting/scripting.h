#pragma once

#include <mono/jit/jit.h>

#include "../console/console.h"

class Scripting
{
public:
	Scripting(char* path, int _argc, char** _argv);
	~Scripting();

	void update(float dt);
private:
	static MonoDomain* domain;
	static MonoAssembly* assembly;
	static MonoImage* image;
	static MonoImage* systemImage;
	static MonoMethod* updateMethod;
	static MonoMethod* shutdownMethod;
	static MonoMethod* commandMethod;
	static MonoMethod* onKeyMethod;
	static MonoMethod* onMouseButtonMethod;
	static MonoMethod* onScrollMethod;
	static MonoMethod* onCursorPosMethod;

	static Console* console;

	static void commandCallback(const std::string& name, const arg_list& args);
	static MonoMethod* getSpecializedMethod(MonoMethod* genericMethod, MonoClass* klass);

	//Internal calls
	static void consoleWriteLine(MonoString* output, Console::level_enum level = Console::trace);
	static void consoleAddAlias(MonoString* name, MonoString* exe);
	static void consoleAddCommand(MonoString* name);
	static void consoleBind(int key, MonoString* exe);
	static void consoleExecute(MonoString* exe, bool record = false, bool positive = true);
	static void consoleRemove(MonoString* name);
	static void consoleUnbind(int key);

	//Callbacks
	static void onKey(int key, int scancode, int action, int mods);
	static void onMouseButton(int button, int action, int mods);
	static void onScroll(double xoffset, double yoffset);
	static void onCursorPos(double xpos, double ypos);
};
