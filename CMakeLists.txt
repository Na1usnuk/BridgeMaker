#------------------------------------------------#
#      Bridge Maker Engine CMakeLists file       #
#------------------------------------------------#
#    I create a buch of sections to have some    #
# visul assist in reading and writing this lists #
#       and tried to make usefull comments.      #
#------------------------------------------------#


# I specify version 4.1 because is newest version for now, 
# but i think it can be used with version 3.30
cmake_minimum_required(VERSION 4.1)

project(BridgeMaker)


#------------------------------------------------#
#               Variables Section                #
#------------------------------------------------#

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# For now i only interested in debug configuration
set(BM_CONFIG debug)
set(CMAKE_BUILD_TYPE Debug)

# Specify directories for output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BridgeMaker_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BridgeMaker_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BridgeMaker_SOURCE_DIR}/bin)


#------------------------------------------------#
#          Platform Selection Section            #
#------------------------------------------------#


# Currently only crossplatform is supported.
# Crossplatform uses glfw + opengl
# When first platform specific implementation will be writen, than this should be removed.
set(BM_PLATFORM xplatform)
set(BM_GRAPHIC opengl)


#------------------------------------------------#
#                 Files Section                  #
#------------------------------------------------#


# Core group
file(GLOB_RECURSE BM_CORE_MODS mod/core/*.cppm)
file(GLOB_RECURSE BM_CORE_IMPLS impl/core/*.cpp)
source_group(Core FILES ${BM_CORE_MODS} ${BM_CORE_IMPLS})

# Graphic group
file(GLOB BM_GFX_MODS LIST_DIRECTORIES NO mod/gfx/${BM_GRAPHIC}/*.cppm mod/gfx/*.cppm)
file(GLOB BM_GFX_IMPLS LIST_DIRECTORIES NO impl/gfx/${BM_GRAPHIC}/*.cpp impl/gfx/*.cpp)
source_group(Graphic FILES ${BM_GFX_MODS} ${BM_GFX_IMPLS})

# Platform group
file(GLOB BM_PLATFORM_MODS LIST_DIRECTORIES NO mod/platform/${BM_PLATFORM}/*.cppm mod/platform/*.cppm)
file(GLOB BM_PLATFORM_IMPLS LIST_DIRECTORIES NO impl/platform/${BM_PLATFORM}/*.cpp impl/platform/*.cpp)
source_group(Platform FILES ${BM_PLATFORM_MODS} ${BM_PLATFORM_IMPLS})

# Audio group
file(GLOB_RECURSE BM_AUDIO_MODS mod/audio/*.cppm)
file(GLOB_RECURSE BM_AUDIO_IMPLS impl/audio/*.cpp)
source_group(Audio FILES ${BM_AUDIO_MODS} ${BM_AUDIO_IMPLS})

# Application group
file(GLOB_RECURSE BM_APP_MODS mod/app/*.cppm)
file(GLOB_RECURSE BM_APP_IMPLS impl/app/*.cpp)
source_group(Application FILES ${BM_APP_MODS} ${BM_APP_IMPLS})

# UI group
file(GLOB_RECURSE BM_UI_MODS mod/ui/*.cppm)
file(GLOB_RECURSE BM_UI_IMPLS impl/ui/*.cpp)
source_group(UI FILES ${BM_UI_MODS} ${BM_UI_IMPLS})

# Resources group
file(GLOB_RECURSE BM_RES_MODS mod/res/*.cppm)
file(GLOB_RECURSE BM_RES_IMPLS impl/res/*.cpp)
source_group(Resources FILES ${BM_RES_MODS} ${BM_RES_IMPLS})


#------------------------------------------------#
#                  Target Here                   #
#------------------------------------------------#


add_library(BridgeMaker)

target_sources(BridgeMaker
	PUBLIC 
	FILE_SET bridge_maker TYPE CXX_MODULES BASE_DIRS mod FILES
		mod/BridgeMaker.cppm
	PRIVATE
	FILE_SET core TYPE CXX_MODULES BASE_DIRS mod/core impl/core FILES
		${BM_CORE_MODS}
		${BM_CORE_IMPLS}
	PRIVATE
	FILE_SET application TYPE CXX_MODULES BASE_DIRS mod/app impl/app FILES
		${BM_APP_MODS}
		${BM_APP_IMPLS}
	PRIVATE 
	FILE_SET graphic TYPE CXX_MODULES BASE_DIRS mod/gfx impl/gfx FILES
		${BM_GFX_MODS}
		${BM_GFX_IMPLS}
	PRIVATE
	FILE_SET platform TYPE CXX_MODULES BASE_DIRS mod/platform impl/platform FILES
		${BM_PLATFORM_MODS}
		${BM_PLATFORM_IMPLS}
	PRIVATE
	FILE_SET audio TYPE CXX_MODULES BASE_DIRS mod/audio impl/audio FILES
		${BM_AUDIO_MODS}
		${BM_AUDIO_IMPLS}
	PRIVATE
	FILE_SET ui TYPE CXX_MODULES BASE_DIRS mod/ui impl/ui FILES
		${BM_UI_MODS}
		${BM_UI_IMPLS}
	PRIVATE
	FILE_SET resources TYPE CXX_MODULES BASE_DIRS mod/res impl/res FILES
		${BM_RES_MODS}
		${BM_RES_IMPLS}
)


#------------------------------------------------#
#             Third Party Section                #
#------------------------------------------------#


add_subdirectory(deps/spdlog)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/imgui)
add_subdirectory(deps/stb)
add_subdirectory(deps/glm)
add_subdirectory(deps/tiny_obj_loader)

target_include_directories(BridgeMaker
	PUBLIC
		deps/stb
	PRIVATE
		deps/imgui/
		deps/imgui/backends
		deps/imgui/misc/cpp
      		deps/spdlog/include 
		deps/glfw/include
		include/
		include/core
		include/graphic
		deps/tiny_obj_loader
		mod/graphic/opengl
		deps/glad/include
		deps/glm
)

target_link_libraries(BridgeMaker
	PUBLIC 
		imgui
		glm
		stb
	PRIVATE
        	spdlog
		glad
		glfw
		tiny_obj_loader
)

if(WIN32)
	target_link_libraries(BridgeMaker PUBLIC ws2_32)
endif()


#------------------------------------------------#
#            Compiler Flags Section              #
#------------------------------------------------#


if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

	foreach(file ${BM_MOD} ${BM_IMPL})
		set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-fmodules-ts" LANGUAGE CXX)
	endforeach()

elseif(MSVC)
	# For cleaner output
	add_compile_options(/nologo)
	add_link_options(/NOLOGO)
	#add_compile_options(/experimental:module)

	set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
		set_target_properties(BridgeMaker PROPERTIES
		COMPILE_PDB_NAME "BridgeMaker"
		COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
	)
endif()

