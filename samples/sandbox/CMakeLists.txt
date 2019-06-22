add_executable(sandbox
	src/sandbox.cpp
	shaders/fs_splash.sc
	shaders/vs_splash.sc
	shaders/vs_bunny.sc
	shaders/fs_bunny.sc
	shaders/vs_sky.sc
	shaders/fs_sky.sc
	shaders/vs_cubes.sc
	shaders/fs_cubes.sc
)

target_link_libraries(sandbox igneous)

add_custom_command(TARGET sandbox POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res
)

add_shader(shaders/vs_splash.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/fs_splash.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/vs_bunny.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/fs_bunny.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/vs_sky.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/fs_sky.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/vs_cubes.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
add_shader(shaders/fs_cubes.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)

set_target_properties(sandbox PROPERTIES FOLDER "igneous/samples")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT sandbox)
