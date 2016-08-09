PROJECT(Image)

cmake_minimum_required(VERSION 3.0)

# インクルードフォルダを相対パスの検索対象に設定する
IF(WIN32)
add_definitions("/I${CMAKE_SOURCE_DIR}/include")
ELSE()
add_definitions("-I ${CMAKE_SOURCE_DIR}/include")
ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/external/jpeg)

FILE(GLOB_RECURSE image_files
	${CMAKE_CURRENT_LIST_DIR}/include/image/*.h*
	${CMAKE_CURRENT_LIST_DIR}/include/type/*.h*
	${CMAKE_CURRENT_LIST_DIR}/src/image/*.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/type/*.cpp
)

FILE(GLOB_RECURSE type_files
	${CMAKE_CURRENT_LIST_DIR}/include/type/*.h*
	${CMAKE_CURRENT_LIST_DIR}/src/type/*.cpp
)

SOURCE_GROUP(image FILES ${image_files})
SOURCE_GROUP(type FILES ${type_files})

add_library(image STATIC
	${image_files}
	${type_files}
)

add_subdirectory(external/jpeg)

add_definitions("-Wall -std=c++11")
add_subdirectory(examples)
