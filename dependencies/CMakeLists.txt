# suppress library warnings
if(MSVC)
  add_definitions(/W0)
else()
  add_definitions(-Wno-everything)
endif()

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
set(BUILD_EXTRAS OFF CACHE INTERNAL "")
add_subdirectory(bullet)

# entt
set(BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(entt)

# openal
find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
	message("OpenAL FOUND")
endif()

# raknet
set(RAKNET_ENABLE_SAMPLES OFF CACHE INTERNAL "")
set(RAKNET_ENABLE_DLL OFF CACHE INTERNAL "")
add_subdirectory(raknet)

# spdlog
add_subdirectory(spdlog)

# stb
add_subdirectory(stb)

# set libraries list
set(IGNEOUS_LIBRARIES
  assimp
  bigg
  BulletDynamics
  BulletCollision
  LinearMath
  Bullet3Common
  BulletInverseDynamics
  BulletSoftBody
  EnTT
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
  ${OPENAL_INCLUDE_DIR}
  ${RAKNET_INCLUDE_DIRS}
CACHE INTERNAL "")
