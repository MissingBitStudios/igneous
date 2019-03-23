cmake_minimum_required(VERSION 3.8)

project(libigneous VERSION 0.1.0 LANGUAGES CSharp)

include(CSharpUtilities)

add_library(libigneous SHARED
	Console.cs
)

set_property(TARGET libigneous PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.1")

set_property(TARGET libigneous PROPERTY VS_DOTNET_REFERENCES
	"System"
)

target_compile_options(libigneous PRIVATE "/langversion:6")
set_target_properties(libigneous PROPERTIES
	FOLDER "engine"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
)