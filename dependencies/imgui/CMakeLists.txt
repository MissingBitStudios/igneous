cmake_minimum_required(VERSION 3.0)

project(imgui LANGUAGES CXX)

add_library(imgui STATIC
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(imgui PROPERTIES FOLDER "imgui")
