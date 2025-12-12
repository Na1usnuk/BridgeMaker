cmake_minimum_required(VERSION 4.1)

add_library(glm)

target_sources(glm 
	PUBLIC
	FILE_SET CXX_MODULES 
	BASE_DIRS glm/
	FILES glm/glm.cppm
)

target_include_directories(glm
	PRIVATE
		.
)
