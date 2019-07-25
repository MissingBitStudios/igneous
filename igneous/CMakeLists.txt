set(SOURCES
	src/audio/audio.cpp
	src/audio/channel.cpp
	src/audio/source.cpp
	src/console/console.cpp
	src/console/conVar.cpp
	src/core/consoleSink.hpp
	src/core/igneous.cpp
	src/core/input.cpp
	src/core/log.cpp
	src/ecs/systems/captureSystem.cpp
	src/ecs/systems/rendererSystem.cpp
	src/gui/gui.cpp
	src/physics/physics.cpp
	src/renderer/camera.cpp
	src/renderer/capture.cpp
	src/renderer/fpsCamera.cpp
	src/renderer/igneous_assets.h
	src/renderer/renderer.cpp
	src/renderer/splashShaders.hpp
)

set(INCLUDES
	include/igneous/audio/audio.hpp
	include/igneous/audio/channel.hpp
	include/igneous/audio/listener.hpp
	include/igneous/audio/source.hpp
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
	include/igneous/renderer/aviwriter.hpp
	include/igneous/renderer/camera.hpp
	include/igneous/renderer/capture.hpp
	include/igneous/renderer/fpsCamera.hpp
	include/igneous/renderer/model.hpp
	include/igneous/renderer/renderer.hpp
	include/igneous/renderer/renderer.inl
	include/igneous/renderer/vertex.hpp
)

set(AMALGAMATED_INCLUDES
	include/igneous/audio.hpp
	include/igneous/console.hpp
	include/igneous/core.hpp
	include/igneous/ecs.hpp
	include/igneous/gui.hpp
	include/igneous/igneous.hpp
	include/igneous/physics.hpp
	include/igneous/renderer.hpp
)

add_library(igneous STATIC
	${SOURCES}
	${INCLUDES}
	${AMALGAMATED_INCLUDES}
)

add_asset_shaders(igneous
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
	HEADER src/renderer/igneous_assets.h
	SHADERS
	shaders/fs_splash.sc
	shaders/vs_splash.sc
)

configure_file (include/igneous/core/version.hpp.in include/igneous/core/version.hpp @ONLY)

target_link_libraries(igneous PUBLIC ${IGNEOUS_LIBRARIES})
target_include_directories(igneous PUBLIC include ${IGNEOUS_INCLUDES})

source_group("Amalgamated Includes" FILES ${AMALGAMATED_INCLUDES})
set_target_properties(igneous PROPERTIES FOLDER "igneous")
