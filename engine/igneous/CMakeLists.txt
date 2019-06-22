add_library(igneous STATIC
	audio/audio.cpp
	audio/audio.hpp
	console/console.cpp
	console/console.hpp
	console/console_sink.hpp
	console/conVar.hpp
	core/debug.hpp
	core/game.hpp
	core/igneous.cpp
	core/input.cpp
	core/input.hpp
	core/log.cpp
	core/log.hpp
	core/version.hpp.in
	ecs/components/cubeComponent.hpp
	ecs/components/modelComponent.hpp
	ecs/components/transformationComponent.hpp
	ecs/systems/captureSystem.cpp
	ecs/systems/captureSystem.hpp
	ecs/systems/moveSystem.cpp
	ecs/systems/moveSystem.hpp
	ecs/systems/rendererSystem.cpp
	ecs/systems/rendererSystem.hpp
	ecs/systems/sky/dynamicValueController.cpp
	ecs/systems/sky/dynamicValueController.hpp
	ecs/systems/sky/proceduralSky.cpp
	ecs/systems/sky/proceduralSky.hpp
	ecs/systems/sky/sunController.cpp
	ecs/systems/sky/sunController.hpp
	ecs/systems/skySystem.cpp
	ecs/systems/skySystem.hpp
	gui/gui.cpp
	gui/gui.hpp
	igneous.hpp
	physics/physics.cpp
	physics/physics.hpp
	renderer/aviwriter.hpp
	renderer/camera.hpp
	renderer/capture.cpp
	renderer/capture.hpp
	renderer/fpsCamera.cpp
	renderer/fpsCamera.hpp
	renderer/model.cpp
	renderer/model.hpp
	renderer/renderer.cpp
	renderer/renderer.hpp
	renderer/scene.cpp
	renderer/scene.hpp
)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/core/version.hpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/core/version.hpp"
	@ONLY
)

target_link_libraries(igneous PUBLIC ${IGNEOUS_LIBRARIES})

target_include_directories(igneous PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${IGNEOUS_INCLUDES}
)

set_target_properties(igneous PROPERTIES FOLDER "igneous/engine")
