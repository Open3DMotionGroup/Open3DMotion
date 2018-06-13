# libb64 build file

cmake_minimum_required(VERSION 3.5)

project (libb64)

# Specify source files
file(GLOB_RECURSE Source_Files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(libb64 ${Source_Files})

# Specify include directories
target_include_directories(libb64
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Installation
install(TARGETS libb64 EXPORT libb64-export LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include FILES_MATCHING PATTERN "b64/*.h")
install(EXPORT libb64-export FILE libb64-targets.cmake DESTINATION lib/cmake/libb64)
