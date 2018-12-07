if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the igneous source code and call cmake from there")
endif()

cmake_minimum_required(VERSION 3.0)
project(igneous VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(dependencies)

configure_file (
  "${CMAKE_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_BINARY_DIR}/src/version.h"
)

file( GLOB IGNEOUS_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h)
add_executable(${CMAKE_PROJECT_NAME} ${IGNEOUS_SOURCES})

target_link_libraries (${CMAKE_PROJECT_NAME} bigg assimp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
  ${CMAKE_BINARY_DIR}/src
)