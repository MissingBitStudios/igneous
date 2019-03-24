#include "scripting.h"

#include <mono/metadata/assembly.h>

#include "../../util/log.h"

Scripting::Scripting(int _argc, char** _argv)
{
	mono_set_dirs("mono\\lib", "mono\\etc");
	domain = mono_jit_init("igneous");
	assembly = mono_domain_assembly_open(domain, "sandbox.exe");
	if (!assembly)
	{
		IG_CORE_ERROR("Could not load the dll!");
	}
	else
	{
		//Mono method flag constants
		static const uint32_t MF_STATIC = 1 << 4;

		//Get the image
		image = mono_assembly_get_image(assembly);

		//If main entry point exists
		uint32_t entry = mono_image_get_entry_point(image);
		MonoMethod* mainMethod = mono_get_method(image, entry, NULL);
		if (!mainMethod)
			IG_CORE_CRITICAL("App must have a static void Main(string[] args) method");

		//Get the main class
		mainClass = mono_method_get_class(mainMethod);

		//If update exists and is static
		updateMethod = mono_class_get_method_from_name(mainClass, "Update", 1);
		if (!updateMethod || !(mono_method_get_flags(updateMethod, NULL) & MF_STATIC))
			IG_CORE_CRITICAL("App must have a static void Update(float dt) method");

		//If shutdown exists and is static
		shutdownMethod = mono_class_get_method_from_name(mainClass, "Shutdown", 0);
		if (!shutdownMethod || !(mono_method_get_flags(shutdownMethod, NULL) & MF_STATIC))
			IG_CORE_CRITICAL("App must have a static void Shutdown() method");

		//Add internal calls
		mono_add_internal_call("Igneous.Console::WriteLine", ConsoleWriteLine);

		//Run the main entry point with args
		MonoArray* argsStringArray = mono_array_new(domain, mono_get_string_class(), _argc);
		for (int i = 0; i < _argc; i++)
			mono_array_set(argsStringArray, MonoString*, i, mono_string_new(domain, _argv[i]));
		void* args[1] = { argsStringArray };
		mono_runtime_invoke(mainMethod, NULL, args, NULL);
	}
}

void Scripting::ConsoleWriteLine(MonoString* output, Console::level_enum level)
{
	char* out = mono_string_to_utf8(output);
	IG_CONSOLE_LOG(out, level);
	mono_free(out);
}

void Scripting::update(float dt)
{
	void* args[1] = { &dt };
	mono_runtime_invoke(updateMethod, NULL, args, NULL);
}

Scripting::~Scripting()
{
	mono_runtime_invoke(shutdownMethod, NULL, NULL, NULL);
	mono_jit_cleanup(domain);
}
