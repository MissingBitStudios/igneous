add_library(igneous STATIC
	src/audio/audio.cpp
	src/audio/audio.hpp
	src/console/console.cpp
	src/console/console.hpp
	src/console/console_sink.hpp
	src/console/conVar.hpp
	src/core/debug.hpp
	src/core/game.hpp
	src/core/igneous.cpp
	src/core/input.cpp
	src/core/input.hpp
	src/core/log.cpp
	src/core/log.hpp
	src/core/version.hpp.in
	src/ecs/components/cubeComponent.hpp
	src/ecs/components/modelComponent.hpp
	src/ecs/components/transformationComponent.hpp
	src/ecs/systems/captureSystem.cpp
	src/ecs/systems/captureSystem.hpp
	src/ecs/systems/moveSystem.cpp
	src/ecs/systems/moveSystem.hpp
	src/ecs/systems/rendererSystem.cpp
	src/ecs/systems/rendererSystem.hpp
	src/ecs/systems/sky/dynamicValueController.cpp
	src/ecs/systems/sky/dynamicValueController.hpp
	src/ecs/systems/sky/proceduralSky.cpp
	src/ecs/systems/sky/proceduralSky.hpp
	src/ecs/systems/sky/sunController.cpp
	src/ecs/systems/sky/sunController.hpp
	src/ecs/systems/skySystem.cpp
	src/ecs/systems/skySystem.hpp
	src/gui/gui.cpp
	src/gui/gui.hpp
	src/igneous.hpp
	src/physics/physics.cpp
	src/physics/physics.hpp
	src/renderer/aviwriter.hpp
	src/renderer/camera.hpp
	src/renderer/capture.cpp
	src/renderer/capture.hpp
	src/renderer/fpsCamera.cpp
	src/renderer/fpsCamera.hpp
	src/renderer/model.cpp
	src/renderer/model.hpp
	src/renderer/renderer.cpp
	src/renderer/renderer.hpp
	src/renderer/scene.cpp
	src/renderer/scene.hpp
	src/renderer/splash_shaders.hpp
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
	"${CMAKE_CURRENT_BINARY_DIR}/src/core/version.hpp"
	@ONLY
)

target_link_libraries(igneous PUBLIC ${IGNEOUS_LIBRARIES})

target_include_directories(igneous PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_BINARY_DIR}/src
	${IGNEOUS_INCLUDES}
)

set_target_properties(igneous PROPERTIES FOLDER "igneous/engine")
