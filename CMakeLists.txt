PROJECT(Image)

cmake_minimum_required(VERSION 3.0)
add_definitions("-Wall -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_SOURCE_DIR}/include)

FILE(GLOB_RECURSE algorithms
	${CMAKE_SOURCE_DIR}/include/image/*.h*
	${CMAKE_SOURCE_DIR}/include/type/*.h*
	${CMAKE_SOURCE_DIR}/src/image/*.cpp
	${CMAKE_SOURCE_DIR}/src/type/*.cpp
)
#SOURCE_GROUP(external_math FILES ${external_math_lib})

add_library(image STATIC
	${algorithms}
)

add_subdirectory(Examples)