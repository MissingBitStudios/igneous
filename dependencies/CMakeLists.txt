cmake_minimum_required(VERSION 3.0)

# assimp
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" )
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" )
add_subdirectory(assimp)

# bigg
set(BIGG_EXAMPLES OFF CACHE INTERNAL "" )
add_subdirectory(bigg)

# bullet
set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "" )
set(BUILD_OPENGL3_DEMOS OFF CACHE INTERNAL "" )
set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "" )
set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "" )
add_subdirectory(bullet)

# openal
include(FindOpenAL)

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
  ${OPENAL_LIBRARY}
CACHE INTERNAL "")

# set includes list
set(IGNEOUS_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/assimp/include
  ${CMAKE_CURRENT_BINARY_DIR}/assimp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet/src
  ${OPENAL_INCLUDE_DIR}
CACHE INTERNAL "")