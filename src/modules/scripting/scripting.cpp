#include "scripting.h"

#include <mono/metadata/assembly.h>

#include "../../util/log.h"

Scripting::Scripting(void** _argv)
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
		mono_add_internal_call("Igneous.Console::WriteLine", ConsoleWriteLine);
		image = mono_assembly_get_image(assembly);
		uint32_t entry = mono_image_get_entry_point(image);
		MonoMethod* mainMethod = mono_get_method(image, entry, NULL);
		mainClass = mono_method_get_class(mainMethod);
		updateMethod = mono_class_get_method_from_name(mainClass, "Update", 1);
		mono_runtime_invoke(mainMethod, NULL, _argv, NULL);
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
	MonoMethod* shutdownMethod = mono_class_get_method_from_name(mainClass, "Shutdown", 0);
	mono_runtime_invoke(shutdownMethod, NULL, NULL, NULL);
	mono_jit_cleanup(domain);
}
