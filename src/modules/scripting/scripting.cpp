#include "scripting.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/debug-helpers.h>

#include "../../util/input.h"
#include "../../util/log.h"

Scripting::Scripting(int _argc, char** _argv)
{
	//Configure mono
	mono_config_parse(NULL);
	mono_set_dirs("mono\\lib", "mono\\etc");

	//Init the domain
	domain = mono_jit_init("sandbox.exe");

	//Add internal calls
	mono_add_internal_call("Igneous.Console::WriteLine", consoleWriteLine);
	mono_add_internal_call("Igneous.Console::AddCommandInternal", consoleAddCommand);
	mono_add_internal_call("Igneous.Console::Execute", consoleExecute);

	//Open the assembly
	assembly = mono_domain_assembly_open(domain, "sandbox.exe");
	if (!assembly)
		IG_CORE_CRITICAL("Could not load the exe!");
	MonoAssembly* libAssembly = mono_domain_assembly_open(domain, "libigneous.dll");
	if (!libAssembly)
		IG_CORE_CRITICAL("Could not load the exe!");
	MonoAssembly* mscorlibAssembly = mono_domain_assembly_open(domain,
		"mono/lib/mono/4.7-api/mscorlib.dll");
	if (!libAssembly)
		IG_CORE_CRITICAL("Could not load the mscorlib assembly!");

	//Get the image
	image = mono_assembly_get_image(assembly);
	MonoImage* libImage = mono_assembly_get_image(libAssembly);
	systemImage = mono_assembly_get_image(mscorlibAssembly);

	//If main entry point exists
	uint32_t entry = mono_image_get_entry_point(image);
	MonoMethod* entryMethod = mono_get_method(image, entry, NULL);
	if (!entryMethod)
		IG_CORE_CRITICAL("App must have a void Main(string[] args) method");

	//Get the main class
	MonoClass* mainClass = mono_method_get_class(entryMethod);
	mono_free_method(entryMethod);

	//Mono method flag constants
	const uint32_t MF_STATIC = 1 << 4;

	//If update exists and is static
	updateMethod = mono_class_get_method_from_name(mainClass, "Update", 1);
	if (!updateMethod || !(mono_method_get_flags(updateMethod, NULL) & MF_STATIC))
		IG_CORE_CRITICAL("App must have a void Update(float dt) method");

	//If shutdown exists and is static
	shutdownMethod = mono_class_get_method_from_name(mainClass, "Shutdown", 0);
	if (!shutdownMethod || !(mono_method_get_flags(shutdownMethod, NULL) & MF_STATIC))
		IG_CORE_CRITICAL("App must have a void Shutdown() method");

	//Load necessary library methods
	MonoMethodDesc* desc = mono_method_desc_new("Igneous.Console:RunCommandCallback", TRUE);
	commandMethod = mono_method_desc_search_in_image(desc, libImage);
	mono_method_desc_free(desc);
	if (!commandMethod)
		IG_CORE_CRITICAL("Could not find command callback method!");

	desc = mono_method_desc_new("Igneous.Events:Call", TRUE);
	MonoMethod* eventCallMethod = mono_method_desc_search_in_image(desc, libImage);
	mono_method_desc_free(desc);
	if (!eventCallMethod)
		IG_CORE_CRITICAL("Could not find event call method!");

	MonoClass* klass = mono_class_from_name(libImage, "Igneous", "Input/KeyEvent");
	onKeyMethod = getSpecializedMethod(eventCallMethod, klass);
	if (!onKeyMethod)
		IG_CORE_CRITICAL("Could not find event call specialized method!");

	klass = mono_class_from_name(libImage, "Igneous", "Input/MouseButtonEvent");
	onMouseButtonMethod = getSpecializedMethod(eventCallMethod, klass);
	if (!onMouseButtonMethod)
		IG_CORE_CRITICAL("Could not find event call specialized method!");

	klass = mono_class_from_name(libImage, "Igneous", "Input/ScrollEvent");
	onScrollMethod = getSpecializedMethod(eventCallMethod, klass);
	if (!onScrollMethod)
		IG_CORE_CRITICAL("Could not find event call specialized method!");

	klass = mono_class_from_name(libImage, "Igneous", "Input/CursorMoveEvent");
	onCursorPosMethod = getSpecializedMethod(eventCallMethod, klass);
	if (!onCursorPosMethod)
		IG_CORE_CRITICAL("Could not find event call specialized method!");

	IN_KEY_SINK(&onKey);
	IN_MOUSE_SINK(&onMouseButton);
	IN_SCROLL_SINK(&onScroll);
	IN_CURSOR_POS_SINK(&onCursorPos);

	console = &Console::getInstance();

	//Run the main entry point with args
	mono_jit_exec(domain, assembly, _argc, _argv);
}

void Scripting::update(float dt)
{
	void* args[1] = { &dt };
	mono_runtime_invoke(updateMethod, NULL, args, NULL);
}

Scripting::~Scripting()
{
	//Run app's shutdown routine
	mono_runtime_invoke(shutdownMethod, NULL, NULL, NULL);
	//Free data
	mono_free_method(updateMethod);
	mono_free_method(shutdownMethod);
	mono_free_method(commandMethod);
	//Cleanup mono
	mono_jit_cleanup(domain);
}

void Scripting::commandCallback(const std::string& name, const arg_list& args)
{
	MonoArray* argsStringArray = mono_array_new(domain, mono_get_string_class(), args.size());
	for (size_t i = 0; i < args.size(); i++)
		mono_array_set(argsStringArray, MonoString*, i, mono_string_new(domain, args[i].c_str()));
	MonoString* n = mono_string_new(domain, name.c_str());
	static void* _args[] = { n, argsStringArray };
	mono_runtime_invoke(commandMethod, NULL, _args, NULL);
}

MonoMethod* Scripting::getSpecializedMethod(MonoMethod* genericMethod, MonoClass* klass)
{
	// find the MethodInfo class
	MonoClass* methodInfoClass = mono_class_from_name(systemImage,
		"System.Reflection",
		"MonoMethod");

	// find the MethodInfo.MakeGenericMethod(Type[]) method
	MonoMethod* makeGenericMethod = mono_class_get_method_from_name(methodInfoClass,
		"MakeGenericMethod",
		1);

	MonoReflectionMethod* monoReflectionMethod = mono_method_get_object(domain,
		genericMethod,
		klass);

	MonoType* monoType = mono_class_get_type(klass);
	MonoReflectionType* monoReflectionType = mono_type_get_object(domain,
		monoType);

	// create an array of Types, that will be the argument to MethodInfo.MakeGenericMethod(Type[])
	MonoObject* exception = NULL;
	MonoArray* a = mono_array_new(domain, mono_class_from_mono_type(monoType), 1);
	mono_array_set(a, MonoReflectionType*, 0, monoReflectionType);
	void* makeGenArgs[] = { a };

	MonoObject* methodInfo = mono_runtime_invoke(makeGenericMethod, monoReflectionMethod, makeGenArgs, &exception);

	MonoClass* monoGenericMethodClass = mono_object_get_class(methodInfo);

	// MethodHandle property of MethodInfo object
	MonoProperty* methodHandleProperty = mono_class_get_property_from_name(monoGenericMethodClass, "MethodHandle");
	MonoMethod* getMethodHandleMethod = mono_property_get_get_method(methodHandleProperty);

	MonoObject* methodHandle = mono_runtime_invoke(getMethodHandleMethod, methodInfo, NULL, &exception);

	MonoMethod* specializedMethod = *(MonoMethod**)mono_object_unbox(methodHandle);
	return specializedMethod;
}

//Internal calls

void Scripting::consoleAddCommand(MonoString* name)
{
	char* n = mono_string_to_utf8(name);
	console->command(n, commandCallback);
	mono_free(n);
}

void Scripting::consoleExecute(MonoString* exe, bool record, bool positive)
{
	char* e = mono_string_to_utf8(exe);
	console->execute(e, record, positive);
	mono_free(e);
}

void Scripting::consoleWriteLine(MonoString* output, Console::level_enum level)
{
	char* out = mono_string_to_utf8(output);
	IG_CONSOLE_LOG(out, level);
	mono_free(out);
}

//Callbacks

void Scripting::onKey(int key, int scancode, int action, int mods)
{
	MonoArray* argsObjectArray = mono_array_new(domain, mono_get_object_class(), 4);
	mono_array_set(argsObjectArray, MonoObject*, 0, mono_value_box(domain, mono_get_int32_class(), &key));
	mono_array_set(argsObjectArray, MonoObject*, 1, mono_value_box(domain, mono_get_int32_class(), &scancode));
	mono_array_set(argsObjectArray, MonoObject*, 2, mono_value_box(domain, mono_get_int32_class(), &action));
	mono_array_set(argsObjectArray, MonoObject*, 3, mono_value_box(domain, mono_get_int32_class(), &mods));
	void* args[] = { argsObjectArray };
	mono_runtime_invoke(onKeyMethod, NULL, args, NULL);
}

void Scripting::onMouseButton(int button, int action, int mods)
{

	MonoArray* argsObjectArray = mono_array_new(domain, mono_get_object_class(), 3);
	mono_array_set(argsObjectArray, MonoObject*, 0, mono_value_box(domain, mono_get_int32_class(), &button));
	mono_array_set(argsObjectArray, MonoObject*, 1, mono_value_box(domain, mono_get_int32_class(), &action));
	mono_array_set(argsObjectArray, MonoObject*, 2, mono_value_box(domain, mono_get_int32_class(), &mods));
	void* args[] = { argsObjectArray };
	mono_runtime_invoke(onMouseButtonMethod, NULL, args, NULL);
}

void Scripting::onScroll(double xoffset, double yoffset)
{
	MonoArray* argsObjectArray = mono_array_new(domain, mono_get_object_class(), 2);
	mono_array_set(argsObjectArray, MonoObject*, 0, mono_value_box(domain, mono_get_double_class(), &xoffset));
	mono_array_set(argsObjectArray, MonoObject*, 1, mono_value_box(domain, mono_get_double_class(), &yoffset));
	void* args[] = { argsObjectArray };
	mono_runtime_invoke(onScrollMethod, NULL, args, NULL);
}

void Scripting::onCursorPos(double xpos, double ypos)
{
	MonoArray* argsObjectArray = mono_array_new(domain, mono_get_object_class(), 2);
	mono_array_set(argsObjectArray, MonoObject*, 0, mono_value_box(domain, mono_get_double_class(), &xpos));
	mono_array_set(argsObjectArray, MonoObject*, 1, mono_value_box(domain, mono_get_double_class(), &ypos));
	void* args[] = { argsObjectArray };
	mono_runtime_invoke(onCursorPosMethod, NULL, args, NULL);
}

MonoDomain* Scripting::domain;
MonoAssembly* Scripting::assembly;
MonoImage* Scripting::image;
MonoImage* Scripting::systemImage;
MonoMethod* Scripting::updateMethod;
MonoMethod* Scripting::shutdownMethod;
MonoMethod* Scripting::commandMethod;
MonoMethod* Scripting::onKeyMethod;
MonoMethod* Scripting::onMouseButtonMethod;
MonoMethod* Scripting::onScrollMethod;
MonoMethod* Scripting::onCursorPosMethod;

Console* Scripting::console;