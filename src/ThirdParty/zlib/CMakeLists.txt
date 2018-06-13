# zlib build file

cmake_minimum_required(VERSION 3.5)

project (zlib)

# Specify source files
file(GLOB_RECURSE Source_Files ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(zlib ${Source_Files})

# Specify include directories
target_include_directories(zlib
    PUBLIC
        $<INSTALL_INTERFACE:include/zlib>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

# Installation
install(TARGETS zlib EXPORT zlib-export LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ DESTINATION include/zlib FILES_MATCHING PATTERN "*.h")
install(EXPORT zlib-export FILE zlib-targets.cmake DESTINATION lib/cmake/zlib)
