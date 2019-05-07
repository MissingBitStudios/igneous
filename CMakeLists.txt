cmake_minimum_required(VERSION 3.8)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the igneous source code and call cmake from there")
endif()

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(igneous VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSharp_FLAGS "/langversion:latest")

add_subdirectory(dependencies)

include(cmake/add_shaders.cmake)

configure_file (
  "${CMAKE_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_BINARY_DIR}/src/version.h"
)

file(GLOB_RECURSE IGNEOUS_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/shaders/*.sc
)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/bin/res)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/capture)

add_executable(${CMAKE_PROJECT_NAME} ${IGNEOUS_SOURCES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${IGNEOUS_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_BINARY_DIR}/src
  ${IGNEOUS_INCLUDES}
)

add_subdirectory(engine)

option(IGNEOUS_SAMPLES "Build samples." ON )
if(IGNEOUS_SAMPLES)
  add_subdirectory(samples)
endif(IGNEOUS_SAMPLES)

add_dependencies(igneous sandbox libigneous)
set_target_properties(igneous PROPERTIES
	FOLDER "igneous/engine"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
)
set_property(TARGET igneous PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_property(TARGET igneous PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "sandbox.exe")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})