cmake_minimum_required(VERSION 3.8)

project(libigneous VERSION 0.1.0 LANGUAGES CSharp)

add_library(libigneous SHARED
	Console.cs
	Events.cs
	Input.cs
)

set_property(TARGET libigneous PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.1")

set_property(TARGET libigneous PROPERTY VS_DOTNET_REFERENCES
	"System"
)

set_target_properties(libigneous PROPERTIES
	FOLDER "igneous/engine"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
)