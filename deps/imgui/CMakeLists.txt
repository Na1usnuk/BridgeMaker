cmake_minimum_required(VERSION 3.29)

project(imgui)

file(GLOB IMGUI_FILES LIST_DIRECTORIES false imgui*.cpp imgui*.h) 

add_library(imgui ${IMGUI_FILES} 
	backends/imgui_impl_opengl3.cpp 
	backends/imgui_impl_opengl3.h 
	backends/imgui_impl_glfw.cpp 
	backends/imgui_impl_glfw.h
)

target_include_directories(imgui PRIVATE . backends ../glfw/include)
