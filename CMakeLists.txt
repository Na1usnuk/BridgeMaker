#------------------------------------------------#
#      Bridge Maker Engine CMakeLists file       #
#------------------------------------------------#
#    I create a buch of sections to have some    #
# visul assist in reading and writing this lists #
#       and tried to make usefull comments.      #
#------------------------------------------------#


# I specify version 4.1 because is newest version for now, 
# but i think it can be used with version 3.29
cmake_minimum_required(VERSION 4.1)

project(BridgeMaker LANGUAGES CXX)


#------------------------------------------------#
#               Variables Section                #
#------------------------------------------------#

set(CMAKE_CXX_STANDARD 23)

# Don`t really required, but for sanity i force it
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Will try to get rid of it and fully use c++20 modules in future
#set(PCH_HEADER ${BridgeMaker_SOURCE_DIR}/include/pch.hpp)

# For now i only interested in debug configuration
set(BM_CONFIG debug)
set(CMAKE_BUILD_TYPE Debug)

# Currently only crossplatform is supported.
# Crossplatform use glfw + opengl
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

	foreach(file ${BM_MOD})
		#set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "/interface /c /TP" LANGUAGE CXX)
	endforeach()

	foreach(file ${BM_IMPL})
		#set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "/internalPartition /c /TP" LANGUAGE CXX)
	endforeach()

else()
	message(WARNING "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Modules can be treated as plain text. ")
endif()


#------------------------------------------------#
#                 Files Section                  #
#------------------------------------------------#


# Sources
file(GLOB_RECURSE BM_SRCS
    ${BridgeMaker_SOURCE_DIR}/src/*.cpp
)

# Headers
# Explicitly collect because there is some files 
# that i dont want to include in variable, like precomp header and "all in" header.
file(GLOB_RECURSE BM_HEADERS
	${BridgeMaker_SOURCE_DIR}/include/core/*.hpp
	${BridgeMaker_SOURCE_DIR}/include/graphic/*.hpp
	${BridgeMaker_SOURCE_DIR}/include/imgui.hpp
	${BridgeMaker_SOURCE_DIR}/include/imgui.hpp
)

# Platorm Headers
file(GLOB_RECURSE BM_PLATFORM_HEADERS CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/include/platform/${BM_PLATFORM}/*.hpp
)

# Platorm Sources
file(GLOB_RECURSE BM_PLATFORM_SRCS CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/src/platform/${BM_PLATFORM}/*.cpp
)

# Graphic Headers
file(GLOB_RECURSE BM_GRAPHIC_HEADERS CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/include/graphic/${BM_GRAPHIC}/*.hpp
)

# Graphic Sources
file(GLOB_RECURSE BM_GRAPHIC_SRCS CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/src/graphic/${BM_GRAPHIC}/*.cpp
)

# Module interfaces
file(GLOB_RECURSE BM_MODULES CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/mod/*.cppm
)

# Module implementations
file(GLOB_RECURSE BM_IMPLS CONFIGURE_DEPENDS 
	${BridgeMaker_SOURCE_DIR}/impl/*.cpp
	${BridgeMaker_SOURCE_DIR}/impl/*.cppp # extension for module partition implementation
)

# Exclude the appropriate config module from BM_MODS based on build type.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	list(FILTER BM_MODULES EXCLUDE REGEX "config_debug\\.cppm$")
else()
	list(FILTER BM_MODULES EXCLUDE REGEX "config_release\\.cppm$")
endif()


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

#target_precompile_headers(BridgeMaker PUBLIC ${PCH_HEADER})

target_sources(BridgeMaker
	PRIVATE
		${BM_SRCS}
		${BM_PLATFORM_SRCS}
		${BM_GRAPHIC_SRCS}
	PRIVATE
	FILE_SET HEADERS BASE_DIRS include FILES
		${BM_HEADERS}
		${BM_PLATFORM_HEADERS}
		${BM_GRAPHIC_HEADERS}
	PRIVATE
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

