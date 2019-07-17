set(SOURCE_FILES
	src/audio/audio.cpp
	src/audio/audio.hpp
	src/console/console.cpp
	src/console/console.hpp
	src/console/console_sink.hpp
	src/console/conVar.cpp
	src/console/conVar.hpp
	src/core/debug.hpp
	src/core/game.hpp
	src/core/igneous.cpp
	src/core/igneous.hpp
	src/core/input.cpp
	src/core/input.hpp
	src/core/log.cpp
	src/core/log.hpp
	src/core/version.hpp
	src/ecs/components/modelComponent.hpp
	src/ecs/components/transformationComponent.hpp
	src/ecs/systems/captureSystem.cpp
	src/ecs/systems/captureSystem.hpp
	src/ecs/systems/rendererSystem.cpp
	src/ecs/systems/rendererSystem.hpp
	src/gui/gui.cpp
	src/gui/gui.hpp
	src/physics/physics.cpp
	src/physics/physics.hpp
	src/renderer/asset_shaders.hpp
	src/renderer/aviwriter.hpp
	src/renderer/camera.cpp
	src/renderer/camera.hpp
	src/renderer/capture.cpp
	src/renderer/capture.hpp
	src/renderer/fpsCamera.cpp
	src/renderer/fpsCamera.hpp
	src/renderer/model.cpp
	src/renderer/model.hpp
	src/renderer/renderer.cpp
	src/renderer/renderer.hpp
	src/renderer/splash_shaders.hpp
)

set(INCLUDE_HEADERS
	include/igneous/audio.hpp
	include/igneous/camera/camera.hpp
	include/igneous/camera/fpsCamera.hpp
	include/igneous/console.hpp
	include/igneous/core.hpp
	include/igneous/debug.hpp
	include/igneous/ecs.hpp
	include/igneous/gui.hpp
	include/igneous/igneous.hpp
	include/igneous/input.hpp
	include/igneous/log.hpp
	include/igneous/physics.hpp
	include/igneous/renderer.hpp
	include/igneous/version.hpp
)

add_library(igneous STATIC
	${SOURCE_FILES}
	${INCLUDE_HEADERS}
)

add_asset_shaders(igneous
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
	HEADER src/renderer/igneous_assets.h
	SHADERS
	shaders/fs_splash.sc
	shaders/vs_splash.sc
)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/src/core/version.hpp.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/core/version.hpp"
	@ONLY
)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/src/core/version.hpp.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/igneous/version.hpp"
	@ONLY
)

target_link_libraries(igneous PUBLIC ${IGNEOUS_LIBRARIES})
target_include_directories(igneous PUBLIC ${IGNEOUS_INCLUDES})
target_include_directories(igneous PRIVATE src)
target_include_directories(igneous INTERFACE include)

source_group("Include Headers" FILES ${INCLUDE_HEADERS})
set_target_properties(igneous PROPERTIES FOLDER "igneous")
