cmake_minimum_required(VERSION 3.0)

# assimp
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
add_subdirectory(assimp)

# bigg
set(BIGG_EXAMPLES OFF CACHE INTERNAL "")
add_subdirectory(bigg)

# bullet
set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "")
set(BUILD_OPENGL3_DEMOS OFF CACHE INTERNAL "")
set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "")
set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "")
add_subdirectory(bullet)

# entt
set(BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(entt)

# raknet
set(RAKNET_ENABLE_SAMPLES OFF CACHE INTERNAL "")
set(RAKNET_ENABLE_DLL OFF CACHE INTERNAL "")
add_subdirectory(raknet)

# mono TODO
#include(${CMAKE_SOURCE_DIR}/cmake/FindMono.cmake)

# openal
include(FindOpenAL)

# set libraries list
set(IGNEOUS_LIBRARIES
  assimp
  bigg
  ${BULLET_LIBRARIES}
  EnTT
  ${RAKNET_COMMON_LIBS}
  #${MONO_LIBRARIES}
  ${OPENAL_LIBRARY}
CACHE INTERNAL "")

# set includes list
set(IGNEOUS_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include
  ${CMAKE_CURRENT_BINARY_DIR}/assimp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet/src
  ${RAKNET_INCLUDE_DIRS}
  #${MONO_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIR}
CACHE INTERNAL "")