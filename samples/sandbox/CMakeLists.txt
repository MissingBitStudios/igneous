cmake_minimum_required(VERSION 3.8)

project(sandbox VERSION 0.1.0 LANGUAGES CSharp)

include(CSharpUtilities)

add_executable(sandbox
	Sandbox.cs
)

set_property(TARGET sandbox PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.7.2")

set_property(TARGET sandbox PROPERTY VS_DOTNET_REFERENCES
	"Microsoft.CSharp"
	"PresentationCore"
	"PresentationFramework"
	"System"
	"System.Core"
	"System.Data"
	"System.Data.DataSetExtensions"
	"System.IO"
	"System.IO.Pipes"
	"System.Net.Http"
	"System.Xaml"
	"System.Xml"
	"System.Xml.Linq"
	"WindowsBase"
)

set_property(TARGET sandbox PROPERTY 
	VS_DOTNET_REFERENCE_STEAMWORKS ${STEAMWORKS_PATH}
)

set_property(TARGET sandbox PROPERTY 
	VS_DOTNET_REFERENCE_DISCORD ${DISCORD_PATH}
)

set(CMAKE_CSharp_FLAGS "/langversion:7.3")

target_link_libraries(sandbox SHARED libigneous)
set_target_properties(sandbox PROPERTIES
	FOLDER "igneous/samples"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
)
