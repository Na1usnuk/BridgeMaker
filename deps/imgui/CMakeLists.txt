cmake_minimum_required(VERSION 4.1)

set(CMAKE_CXX_STANDARD 23)

add_library(imgui)

target_sources(imgui
	PUBLIC
	FILE_SET CXX_MODULES
	FILES
		mod/imgui.cppm
		#mod/imgui_internal.cppm
		#mod/imgui_freetype.cppm
		mod/backends/imgui_impl_glfw.cppm
		mod/backends/imgui_impl_opengl3.cppm
	PRIVATE
	FILE_SET HEADERS 
	FILES
		src/imgui.h
		#src/imgui_internal.h
		src/imstb_rectpack.h
		src/imstb_textedit.h
		src/imstb_truetype.h
		#src/misc/freetype/imgui_freetype.h
		src/backends/imgui_impl_glfw.h
		src/backends/imgui_impl_opengl3.h
	PRIVATE
		src/imgui.cpp
		src/imgui_draw.cpp
		src/imgui_demo.cpp
		src/imgui_tables.cpp
		src/imgui_widgets.cpp
		#src/misc/freetype/imgui_freetype.cpp
		src/backends/imgui_impl_glfw.cpp
		src/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
	PRIVATE
		src/
		src/backends
		../glfw/include
		#src/misc/freetype
)



