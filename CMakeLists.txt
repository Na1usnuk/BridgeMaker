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

add_library(BridgeMaker ${BM_SRC} ${BM_HEADERS})

if(WIN32)
	target_compile_definitions(BridgeMaker PRIVATE BM_PLATFORM_WINDOWS)
	file(GLOB_RECURSE BM_WINDOWS_FILES 
		include/platform/windows/*.hpp
		src/platform/windows/*.cpp
	)
	target_sources(BridgeMaker PRIVATE ${BM_WINDOWS_FILES})
endif()



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
)
target_link_libraries(BridgeMaker
    PRIVATE
	spdlog
	glfw
	glad
)

