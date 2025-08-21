cmake_minimum_required(VERSION 3.29.0)

project(BridgeMaker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PCH_HEADER ${BridgeMaker_SOURCE_DIR}/include/pch.hpp)

file(GLOB_RECURSE BM_SRC
    ${BridgeMaker_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE BM_HEADERS
	${BridgeMaker_SOURCE_DIR}/include/core/*.hpp
	${BridgeMaker_SOURCE_DIR}/include/graphic/*.hpp
	${BridgeMaker_SOURCE_DIR}/include/imgui.hpp
	${BridgeMaker_SOURCE_DIR}/include/imgui.hpp
)
#for now only crossplatform
file(GLOB_RECURSE BM_PLATFORM_SPECIFIC 
	${BridgeMaker_SOURCE_DIR}/include/platform/*.hpp
	${BridgeMaker_SOURCE_DIR}/src/platform/*.hpp
	${BridgeMaker_SOURCE_DIR}/include/platform/xplatform/*.hpp
	${BridgeMaker_SOURCE_DIR}/src/platform/xplatform/*.cpp
)

add_library(BridgeMaker ${BM_SRC} ${BM_HEADERS} ${BM_PLATFORM_SPECIFIC})

if(WIN32)
	target_compile_definitions(BridgeMaker PRIVATE BM_PLATFORM_WINDOWS)
endif()

target_compile_definitions(BridgeMaker PRIVATE BM_USE_OPENGL)

file(GLOB BM_EVENTS_FILES LIST_DIRECTORIES false include/core/events/*)
source_group(Events FILES ${BM_EVENTS_FILES})

source_group(Platform FILES ${BM_PLATFORM_SPECIFIC})

file(GLOB_RECURSE BM_GL_FILES include/graphic/*.hpp src/graphic/*.cpp)
source_group(Graphic FILES ${BM_GL_FILES})

target_precompile_headers(BridgeMaker PUBLIC ${PCH_HEADER})

add_subdirectory(deps/spdlog)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/imgui)
add_subdirectory(deps/stb)
add_subdirectory(deps/glm)

target_include_directories(BridgeMaker
    PUBLIC
	deps/imgui/
	deps/imgui/backends
	deps/glm
    PRIVATE
        deps/spdlog/include 
	deps/glad/include
	deps/glfw/include
	deps/stb
	include/
	include/core
	include/graphic
)
target_link_libraries(BridgeMaker
    PUBLIC 
        spdlog
	imgui
	glm
    PRIVATE
	glfw
	glad
	stb
)


if(MSVC)
    add_compile_options(/nologo)
endif()

