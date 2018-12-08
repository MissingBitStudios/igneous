cmake_minimum_required(VERSION 3.0)

# assimp
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

set(IGNEOUS_LIBRARIES
  assimp
  bigg
  LinearMath
  Bullet3Common
  BulletInverseDynamics
  BulletCollision
  BulletDynamics
  BulletSoftBody
CACHE INTERNAL "")

set(IGNEOUS_INCLUDES
  ${CMAKE_SOURCE_DIR}/dependencies/bullet/src
CACHE INTERNAL "")