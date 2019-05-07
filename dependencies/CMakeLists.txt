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

# Discord.Net
file(COPY "${CMAKE_CURRENT_LIST_DIR}/Discord RPC DotNet" DESTINATION ${CMAKE_BINARY_DIR}/bin)
set(DISCORD_PATH "${CMAKE_BINARY_DIR}/bin/Discord RPC DotNet/Discord RPC DotNet.dll" CACHE INTERNAL "")

# entt
set(BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(entt)

# Facepunch.Steamworks
file(COPY ${CMAKE_CURRENT_LIST_DIR}/Facepunch.Steamworks DESTINATION ${CMAKE_BINARY_DIR}/bin)
set(STEAMWORKS_PATH ${CMAKE_BINARY_DIR}/bin/Facepunch.Steamworks/Facepunch.Steamworks.dll CACHE INTERNAL "")

# fastnoise
include(${CMAKE_SOURCE_DIR}/cmake/UseFastnoise.cmake)

# mono
include(${CMAKE_SOURCE_DIR}/cmake/FindMono.cmake)

# openal
include(FindOpenAL)

# pcil
add_subdirectory(pcil)

# raknet
set(RAKNET_ENABLE_SAMPLES OFF CACHE INTERNAL "")
set(RAKNET_ENABLE_DLL OFF CACHE INTERNAL "")
add_subdirectory(raknet)

# spdlog
add_subdirectory(spdlog)

# stb
include(${CMAKE_SOURCE_DIR}/cmake/UseStb.cmake)

# set libraries list
set(IGNEOUS_LIBRARIES
  assimp
  bigg
  LinearMath
  Bullet3Common
  BulletInverseDynamics
  BulletCollision
  BulletDynamics
  BulletSoftBody
  EnTT
  ${MONO_LIBRARY}
  fastnoise
  pcil
  ${OPENAL_LIBRARY}
  ${RAKNET_COMMON_LIBS}
  spdlog
  stb
CACHE INTERNAL "")

# set includes list
set(IGNEOUS_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include
  ${CMAKE_CURRENT_BINARY_DIR}/assimp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet/src
  ${MONO_INCLUDE_DIR}
  ${OPENAL_INCLUDE_DIR}
  ${RAKNET_INCLUDE_DIRS}
CACHE INTERNAL "")