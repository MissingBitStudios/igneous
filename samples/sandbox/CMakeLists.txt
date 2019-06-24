add_executable(sandbox
	src/sandbox.cpp
)

target_link_libraries(sandbox igneous)

copy_directory(sandbox ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)

add_shaders(sandbox OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
	SHADERS
	shaders/fs_splash.sc
	shaders/vs_splash.sc
	shaders/vs_bunny.sc
	shaders/fs_bunny.sc
	shaders/vs_sky.sc
	shaders/fs_sky.sc
	shaders/vs_cubes.sc
	shaders/fs_cubes.sc
)

set_target_properties(sandbox PROPERTIES FOLDER "igneous/samples")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
