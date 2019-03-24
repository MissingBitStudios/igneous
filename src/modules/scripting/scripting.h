#pragma once

#include <mono/jit/jit.h>

#include "../console/console.h"

class Scripting
{
public:
	Scripting(int _argc, char** _argv);
	~Scripting();

	void update(float dt);
private:
	MonoDomain* domain;
	MonoAssembly* assembly;
	MonoImage* image;
	MonoClass* mainClass;
	MonoMethod* updateMethod;
	MonoMethod* shutdownMethod;

	static void ConsoleWriteLine(MonoString* output, Console::level_enum level = Console::trace);
};
