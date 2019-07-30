add_executable(modelc
	modelc.cpp
)

target_link_libraries(modelc
	assimp
	bgfx
)

target_include_directories(modelc PUBLIC
	${CMAKE_SOURCE_DIR}/dependencies/assimp/include
	${CMAKE_BINARY_DIR}/dependencies/assimp/include
	${CMAKE_SOURCE_DIR}/dependencies/bullet/src
)

set_target_properties(modelc PROPERTIES FOLDER "igneous/tools")
