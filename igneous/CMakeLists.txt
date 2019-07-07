add_library(igneous STATIC
	src/audio/audio.cpp
	src/audio/audio_p.hpp
	src/console/console.cpp
	src/console/console_p.hpp
	src/console/console_sink.hpp
	src/console/conVar.cpp
	src/console/conVar_p.hpp
	src/core/igneous.cpp
	src/core/igneous_p.hpp
	src/core/input.cpp
	src/core/input_p.hpp
	src/core/log.cpp
	src/ecs/systems/captureSystem.cpp
	src/ecs/systems/rendererSystem.cpp
	src/gui/gui.cpp
	src/physics/physics.cpp
	src/physics/physics_p.hpp
	src/renderer/asset_shaders.hpp
	src/renderer/aviwriter.hpp
	src/renderer/camera.cpp
	src/renderer/capture.cpp
	src/renderer/capture_p.hpp
	src/renderer/fpsCamera.cpp
	src/renderer/model.cpp
	src/renderer/renderer.cpp
	src/renderer/renderer_p.hpp
	src/renderer/splash_shaders.hpp

	include/igneous/audio/audio.hpp
	include/igneous/console/console.hpp
	include/igneous/console/conVar.hpp
	include/igneous/core/debug.hpp
	include/igneous/core/game.hpp
	include/igneous/core/igneous.hpp
	include/igneous/core/input.hpp
	include/igneous/core/log.hpp
	include/igneous/core/version.hpp
	include/igneous/ecs/components/modelComponent.hpp
	include/igneous/ecs/components/transformationComponent.hpp
	include/igneous/ecs/systems/captureSystem.hpp
	include/igneous/ecs/systems/rendererSystem.hpp
	include/igneous/gui/gui.hpp
	include/igneous/physics/physics.hpp
	include/igneous/renderer/camera.hpp
	include/igneous/renderer/capture.hpp
	include/igneous/renderer/fpsCamera.hpp
	include/igneous/renderer/model.hpp
	include/igneous/renderer/renderer.hpp
)

add_asset_shaders(igneous
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
	HEADER src/renderer/igneous_assets.h
	SHADERS
	shaders/fs_splash.sc
	shaders/vs_splash.sc
)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/include/igneous/core/version.hpp.in"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/igneous/core/version.hpp"
	@ONLY
)

target_link_libraries(igneous PUBLIC ${IGNEOUS_LIBRARIES})
target_include_directories(igneous PUBLIC ${IGNEOUS_INCLUDES})
target_include_directories(igneous PRIVATE src include/igneous)
target_include_directories(igneous INTERFACE include)

set_target_properties(igneous PROPERTIES FOLDER "igneous")
