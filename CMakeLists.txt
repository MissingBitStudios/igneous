cmake_minimum_required(VERSION 3.8)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the igneous source code and call cmake from there")
endif()

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(igneous VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(IGNEOUS_EDITOR  "Build editor."    ON)
option(IGNEOUS_SAMPLES "Build samples."   ON)
option(IGNEOUS_TESTS   "Build tests."     ON)
option(IGNEOUS_TOOLS   "Build tools."     ON)

add_subdirectory(dependencies)

if(IGNEOUS_EDITOR)
  add_subdirectory(engine/editor)
endif(IGNEOUS_EDITOR)

add_subdirectory(engine/igneous)

if(IGNEOUS_SAMPLES)
  add_subdirectory(samples)
endif(IGNEOUS_SAMPLES)

if(IGNEOUS_TESTS)
  add_subdirectory(test)
endif(IGNEOUS_TESTS)

if(IGNEOUS_TOOLS)
  add_subdirectory(engine/tools)
endif(IGNEOUS_TOOLS)
