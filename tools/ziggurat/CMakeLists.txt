add_executable(ziggurat
	ziggurat.cpp
	ziggurat.hpp
	model.cpp
	copy.cpp
)

target_link_libraries(ziggurat
	assimp
	bgfx
	stb
)

target_include_directories(ziggurat PUBLIC
	${CMAKE_SOURCE_DIR}/dependencies/assimp/include
	${CMAKE_BINARY_DIR}/dependencies/assimp/include
)

set_target_properties(ziggurat PROPERTIES FOLDER "igneous/tools")

macro(add_assets ARG_TARGET ARG_CONFIG_FILE)
	add_custom_target(
		${ARG_TARGET}_ziggurat ALL
		"$<TARGET_FILE:ziggurat>" ${ARG_CONFIG_FILE} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
		DEPENDS ziggurat
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	set_target_properties(${ARG_TARGET}_ziggurat PROPERTIES FOLDER "assets" )
	add_dependencies(${ARG_TARGET} ${ARG_TARGET}_ziggurat)
endmacro()
