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
)

list(LENGTH BM_HEADERS BM_HEADERS_LENGTH)

message(${BM_HEADERS})

add_library(BridgeMaker ${BM_SRC} ${BM_HEADERS})

target_precompile_headers(BridgeMaker PUBLIC ${PCH_HEADER})

if(WIN32)
	target_compile_definitions(BridgeMaker PRIVATE BM_PLATFORM_WINDOWS)
endif()

add_subdirectory(deps/spdlog)
add_subdirectory(deps/glfw)

target_include_directories(BridgeMaker
    PUBLIC
        deps/spdlog/include 
	deps/glfw/include
    PRIVATE
	include/
)
target_link_libraries(BridgeMaker
    PRIVATE
	spdlog
	glfw
)

