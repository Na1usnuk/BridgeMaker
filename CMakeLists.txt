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

# Currently only crossplatform is supported.
# Crossplatform uses glfw + opengl
# When first platform specific implementation will be writen, than this should be removed.
set(BM_PLATFORM xplatform)
set(BM_GRAPHIC opengl)


#------------------------------------------------#
#          Platform Selection Section            #
#------------------------------------------------#


if(NOT DEFINED BM_PLATFORM)

	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")                         #use directx + win32

		set(BM_PLATFORM windows)
		set(BM_GRAPHIC metal)

	elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")                       #use vulcan + x11

		set(BM_PLATFORM linux)
		set(BM_GRAPHIC vulcan)

	elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")                     #use vulcan + android.native_window

		set(BM_PLATFORM android)
		set(BM_GRAPHIC vulcan)

	elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

		if(CMAKE_OSX_SYSROOT MATCHES "iphoneos|iphonesimulator") #use metal + uikit
			set(BM_PLATFORM ios)
		else() #MacOS                                            #use metal + cocoa
			set(BM_PLATFORM macos)
		endif()

		set(BM_GRAPHIC metal)

	else()

		set(BM_PLATFORM xplatform)
		set(BM_GRAPHIC opengl)

	endif()

endif() #NOT DEFINED BM_PLATFORM


#------------------------------------------------#
#            Compiler Flags Section              #
#------------------------------------------------#


if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

	foreach(file ${BM_MOD} ${BM_IMPL})
		set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-fmodules-ts" LANGUAGE CXX)
	endforeach()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# For cleaner output
	add_compile_options(/nologo)
	add_link_options(/NOLOGO)
	#add_compile_options(/experimental:module)
endif()


#------------------------------------------------#
#                 Files Section                  #
#------------------------------------------------#


# Module interfaces
file(GLOB_RECURSE BM_MODULES CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/mod/*.cppm
)

# Module implementations
file(GLOB_RECURSE BM_IMPLS CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/impl/*.cpp
)


#------------------------------------------------#
#                 Group Section                  #
#------------------------------------------------#


# Event group
file(GLOB BM_EVENT_FILES LIST_DIRECTORIES false mod/core/events/*.cppm impl/core/events/*.cpp)
source_group(Events FILES ${BM_EVENT_FILES})

# Layer group
file(GLOB BM_LAYER_FILES LIST_DIRECTORIES false mod/core/layers/*.cppm impl/core/layers/*.cpp)
source_group(Layers FILES ${BM_LAYER_FILES})

# Graphic group
file(GLOB_RECURSE BM_GL_FILES mod/graphic/*.cppm impl/graphic/*.cpp)
source_group(Graphic FILES ${BM_GL_FILES})

# Platform group
file(GLOB BM_PLATFORM_FILES LIST_DIRECTORIES false mod/platform/${BM_PLATFORM}/*.cppm impl/platform/${BM_PLATFORM}/*.cpp)
source_group(Platform FILES ${BM_PLATFORM_FILES})



#------------------------------------------------#
#                  Target Here                   #
#------------------------------------------------#


add_library(BridgeMaker)

target_sources(BridgeMaker
	PUBLIC
	FILE_SET CXX_MODULES BASE_DIRS impl mod FILES
		${BM_MODULES}
		${BM_IMPLS}
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

target_include_directories(BridgeMaker
	PUBLIC
		deps/imgui/
		deps/imgui/backends
		deps/imgui/misc/cpp
		deps/glm
		deps/stb
	PRIVATE
      		deps/spdlog/include 
		deps/glad/include
		deps/glfw/include
		include/
		include/core
		include/graphic
)

target_link_libraries(BridgeMaker
	PUBLIC 
        	spdlog
		imgui
		glm
		stb
	PRIVATE
		glfw
		glad
)

if(MSVC)
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set_target_properties(BridgeMaker PROPERTIES
	COMPILE_PDB_NAME "BridgeMaker"
	COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>"
)
endif()