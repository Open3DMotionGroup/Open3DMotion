# cppunit build file

cmake_minimum_required(VERSION 2.6)

project (cppunit)

# Inlclude path
include_directories (${PROJECT_SOURCE_DIR}/include)

# Build the library
file(GLOB_RECURSE Source_Files ${PROJECT_SOURCE_DIR}/src/cppunit/*.cpp)
add_library(cppunit ${Source_Files})

# Place in output path so other projects can also link
install(TARGETS cppunit LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
