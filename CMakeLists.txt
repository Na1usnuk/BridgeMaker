cmake_minimum_required(VERSION 3.29.0)

project(BridgeMaker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PCH_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.hpp)

file(GLOB_RECURSE BM_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
file(GLOB_RECURSE BM_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/include/core/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/graphic/*.hpp
)

add_library(BridgeMaker ${BM_SRC} ${BM_HEADERS})

if(WIN32)
	target_compile_definitions(BridgeMaker PRIVATE BM_PLATFORM_WINDOWS)
endif()



file(GLOB BM_EVENTS_FILES LIST_DIRECTORIES false include/core/events/*)
source_group(Events FILES ${BM_EVENTS_FILES})

file(GLOB_RECURSE BM_GL_FILES include/graphic/*.hpp src/graphic/*.cpp)
source_group(Graphic FILES ${BM_GL_FILES})

target_precompile_headers(BridgeMaker PUBLIC ${PCH_HEADER})

add_subdirectory(deps/spdlog)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)

target_include_directories(BridgeMaker
    PUBLIC
        deps/spdlog/include 
	deps/glfw/include
	deps/glad/include
    PRIVATE
	include/
	include/core
	include/graphic
)
target_link_libraries(BridgeMaker
    PUBLIC 
        spdlog
    PRIVATE
	glfw
	glad
)

