# zlib build file

cmake_minimum_required(VERSION 2.6)

project (zlib)

# Inlclude path
include_directories (${PROJECT_SOURCE_DIR})

# Build the library
file(GLOB_RECURSE Source_Files ${PROJECT_SOURCE_DIR}/*.c)
add_library(zlib ${Source_Files})

# Place in output path so other projects can also link
install(TARGETS zlib LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
