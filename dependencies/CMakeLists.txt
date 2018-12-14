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

# mono TODO
include(${CMAKE_SOURCE_DIR}/cmake/FindMono.cmake)

# openal
include(FindOpenAL)

# raknet
set(RAKNET_ENABLE_SAMPLES OFF CACHE INTERNAL "")
set(RAKNET_ENABLE_DLL OFF CACHE INTERNAL "")
add_subdirectory(raknet)

# spdlog
add_subdirectory(spdlog)

# stb
include(${CMAKE_SOURCE_DIR}/cmake/UseStb.cmake)

# ttvfs
set(TTVFS_IGNORE_CASE FALSE CACHE INTERNAL "")
set(TTVFS_BUILD_CFILEAPI FALSE CACHE INTERNAL "")
add_subdirectory(ttvfs)

# set libraries list
set(IGNEOUS_LIBRARIES
  assimp
  bigg
  ${BULLET_LIBRARIES}
  EnTT
  ${MONO_LIBRARY}
  ${OPENAL_LIBRARY}
  ${RAKNET_COMMON_LIBS}
  spdlog
  stb
  ttvfs
CACHE INTERNAL "")

# set includes list
set(IGNEOUS_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include
  ${CMAKE_CURRENT_BINARY_DIR}/assimp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet/src
  ${MONO_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${RAKNET_INCLUDE_DIRS}
  ${TTVFS_INCLUDE_DIRS}
CACHE INTERNAL "")