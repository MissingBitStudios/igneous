add_executable(modelc
	modelc.cpp
)

target_link_libraries(modelc
	assimp
	bgfx
)

target_include_directories(modelc PUBLIC
	${CMAKE_SOURCE_DIR}/dependencies/assimp/include
	${CMAKE_BINARY_DIR}/dependencies/assimp/include
	${CMAKE_SOURCE_DIR}/dependencies/bullet/src
)

set_target_properties(modelc PROPERTIES FOLDER "igneous/tools")

macro(add_model ARG_TARGET)
	cmake_parse_arguments(ARG "" "VERTEX;MODEL;OUTPUT" "" ${ARGN})
	target_sources(${ARG_TARGET} PRIVATE ${ARG_MODEL})
	add_custom_command(
		MAIN_DEPENDENCY ${ARG_MODEL}
		OUTPUT ${ARG_OUTPUT}
		COMMAND "$<TARGET_FILE:modelc>" -v "${ARG_VERTEX}" -m "${ARG_MODEL}" -o "${ARG_OUTPUT}"
		COMMENT "Compiling model ${ARG_MODEL}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endmacro()
