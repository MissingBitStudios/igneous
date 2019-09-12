# suppress library warnings
if(MSVC)
  add_compile_options(/W0)
else()
  add_compile_options(-w)
endif()

# assimp
set(BUILD_SHARED_LIBS  OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
add_subdirectory(assimp)

# bgfx
set(BGFX_BUILD_TOOLS     ON  CACHE INTERNAL "" )
set(BGFX_BUILD_EXAMPLES  OFF CACHE INTERNAL "" )
set(BGFX_CUSTOM_TARGETS  OFF CACHE INTERNAL "" )
set(BGFX_INSTALL         OFF CACHE INTERNAL "" )
add_subdirectory(bgfx.cmake)

# bullet
set(BUILD_CPU_DEMOS              OFF CACHE INTERNAL "")
set(BUILD_OPENGL3_DEMOS          OFF CACHE INTERNAL "")
set(BUILD_BULLET2_DEMOS          OFF CACHE INTERNAL "")
set(BUILD_UNIT_TESTS             OFF CACHE INTERNAL "")
set(BUILD_EXTRAS                 OFF CACHE INTERNAL "")
set(BUILD_PYBULLET               OFF CACHE INTERNAL "")
set(USE_MSVC_RUNTIME_LIBRARY_DLL ON  CACHE INTERNAL "")
add_subdirectory(bullet)

# entt
set(BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(entt)

# glfw
set(GLFW_BUILD_EXAMPLES  OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS     OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS      OFF CACHE INTERNAL "")
set(GLFW_INSTALL         OFF CACHE INTERNAL "")
add_subdirectory(glfw)

# glm
set(GLM_TEST_ENABLE      OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS    OFF CACHE INTERNAL "")
set(BUILD_STATIC_LIBS    OFF CACHE INTERNAL "")
add_subdirectory(glm)

# googletest
if(IGNEOUS_TESTS)
	set(gtest_force_shared_crt  ON CACHE INTERNAL "")
	set(BUILD_GMOCK             OFF CACHE INTERNAL "")
	set(INSTALL_GTEST           OFF CACHE INTERNAL "")
	add_subdirectory(googletest)
endif()

# imgui
add_subdirectory(imgui)

# openal
find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
	message("OpenAL FOUND")
endif()

# raknet
set(RAKNET_ENABLE_SAMPLES OFF CACHE INTERNAL "")
set(RAKNET_ENABLE_DLL     OFF CACHE INTERNAL "")
add_subdirectory(raknet)

# spdlog
add_subdirectory(spdlog)

# stb
add_subdirectory(stb)

# set libraries list
set(IGNEOUS_LIBRARIES
  bgfx
  BulletDynamics
  BulletCollision
  LinearMath
  Bullet3Common
  BulletInverseDynamics
  BulletSoftBody
  EnTT
  glfw
  glm
  imgui
  ${OPENAL_LIBRARY}
  ${RAKNET_COMMON_LIBS}
  spdlog
  stb
CACHE INTERNAL "")

# set includes list
set(IGNEOUS_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/bullet/src
  ${OPENAL_INCLUDE_DIR}
  ${RAKNET_INCLUDE_DIRS}
CACHE INTERNAL "")
