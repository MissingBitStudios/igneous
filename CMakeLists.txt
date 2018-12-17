cmake_minimum_required(VERSION 3.0)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the igneous source code and call cmake from there")
endif()

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(igneous VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dependencies)

file(GLOB_RECURSE shaders "${CMAKE_SOURCE_DIR}/shaders/*.sc")
foreach(shader ${shaders})
  if(${shader} MATCHES "/vs_[^/]+\\.sc$")
    add_shader(${shader} VERTEX OUTPUT ${CMAKE_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
  elseif(${shader} MATCHES "/fs_[^/]+\\.sc$")
    add_shader(${shader} FRAGMENT OUTPUT ${CMAKE_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130)
  endif(${shader} MATCHES "/vs_[^/]+\\.sc$")
endforeach(shader)

configure_file (
  "${CMAKE_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_BINARY_DIR}/src/version.h"
)

file(GLOB_RECURSE IGNEOUS_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h
  ${CMAKE_SOURCE_DIR}/shaders/*.sc
)

file(COPY res/ DESTINATION ${CMAKE_BINARY_DIR}/res)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/capture)

add_executable(${CMAKE_PROJECT_NAME} ${IGNEOUS_SOURCES})

target_link_libraries (${CMAKE_PROJECT_NAME} ${IGNEOUS_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_BINARY_DIR}/src
  ${IGNEOUS_INCLUDES}
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})