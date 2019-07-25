cmake_minimum_required(VERSION 3.8)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the igneous source code and call cmake from there")
endif()

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

project(igneous VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(IGNEOUS_ASSET_GEN   "Build assets."        OFF)
option(IGNEOUS_DOCS        "Build documentation." OFF)
option(IGNEOUS_EDITOR      "Build editor."        ON)
option(IGNEOUS_INSTALL     "Install Igneous."     OFF)
option(IGNEOUS_SAMPLES     "Build samples."       ON)
option(IGNEOUS_TESTS       "Build tests."         ON)

add_subdirectory(dependencies)

include(cmake/macros.cmake)

if(IGNEOUS_DOCS)
  add_subdirectory(docs)
endif()

add_subdirectory(tools)

if(IGNEOUS_EDITOR)
  add_subdirectory(editor)
endif()

add_subdirectory(igneous)

if(IGNEOUS_SAMPLES)
  add_subdirectory(samples)
endif()

if(IGNEOUS_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
