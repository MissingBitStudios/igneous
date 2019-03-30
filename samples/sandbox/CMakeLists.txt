cmake_minimum_required(VERSION 3.8)

project(sandbox VERSION 0.1.0 LANGUAGES CSharp)

include(CSharpUtilities)

add_executable(sandbox
	Sandbox.cs
)

set_property(TARGET sandbox PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.1")

set_property(TARGET sandbox PROPERTY VS_DOTNET_REFERENCES
	"System"
)

target_compile_options(sandbox PRIVATE "/langversion:6")
target_link_libraries(sandbox SHARED libigneous)
set_target_properties(sandbox PROPERTIES
	FOLDER "igneous/samples"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
)