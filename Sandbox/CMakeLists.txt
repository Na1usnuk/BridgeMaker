cmake_minimum_required(VERSION 3.29)

project(Sandbox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED Yes)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Sandbox_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Sandbox_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Sandbox_SOURCE_DIR}/bin)

file(GLOB_RECURSE MOD src/*.cppm)
file(GLOB_RECURSE IMPL src/*.cppp)

add_executable(Sandbox main.cpp)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
   	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Sandbox)
	set_target_properties(Sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${Sandbox_SOURCE_DIR})
endif()

add_subdirectory(.. Engine)

target_link_libraries(Sandbox PRIVATE BridgeMaker)
target_sources(Sandbox PRIVATE
	${IMPL}
	FILE_SET CXX_MODULES FILES
	${MOD}
	
)