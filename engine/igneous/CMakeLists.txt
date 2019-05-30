cmake_minimum_required(VERSION 3.8)

project(igneous VERSION 0.1.0 LANGUAGES CXX)

include(cmake/add_shaders.cmake)

configure_file (
  "${CMAKE_SOURCE_DIR}/version.h.in"
  "${CMAKE_BINARY_DIR}/version.h"
)

file(GLOB_RECURSE IGNEOUS_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/shaders/*.sc
)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/capture)

add_executable(igneous ${IGNEOUS_SOURCES})

target_link_libraries(igneous ${IGNEOUS_LIBRARIES})

target_include_directories(igneous PRIVATE
  ${CMAKE_BINARY_DIR}/src
  ${IGNEOUS_INCLUDES}
)

target_compile_options(libigneous PRIVATE "/langversion:7.3")

add_dependencies(igneous sandbox libigneous)

set_target_properties(igneous PROPERTIES
	FOLDER "igneous/engine"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
)

set_property(TARGET igneous PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT igneous)