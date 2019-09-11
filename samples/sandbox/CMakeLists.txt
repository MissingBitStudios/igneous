add_executable(sandbox
	src/sandbox.cpp
	src/ecs/systems/sky/dynamicValueController.cpp
	src/ecs/systems/sky/dynamicValueController.hpp
	src/ecs/systems/sky/proceduralSky.cpp
	src/ecs/systems/sky/proceduralSky.hpp
	src/ecs/systems/sky/sunController.cpp
	src/ecs/systems/sky/sunController.hpp
	src/ecs/systems/skySystem.cpp
	src/ecs/systems/skySystem.hpp
)

target_link_libraries(sandbox igneous)

add_shaders(sandbox OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
	SHADERS
	res/shaders/vs_bunny.sc
	res/shaders/fs_bunny.sc
	res/shaders/vs_sky.sc
	res/shaders/fs_sky.sc
	res/shaders/vs_poly.sc
	res/shaders/fs_poly.sc
)

add_assets(sandbox ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(sandbox PROPERTIES FOLDER "igneous/samples")
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
