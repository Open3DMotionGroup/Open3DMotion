# cppunit build file

cmake_minimum_required(VERSION 3.5)

project (cppunit)

# Specify source files
file(GLOB_RECURSE Source_Files ${CMAKE_CURRENT_SOURCE_DIR}/src/cppunit/*.cpp)
add_library(cppunit ${Source_Files})

# Specify include directories
target_include_directories(cppunit
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Installation
install(TARGETS cppunit EXPORT cppunit-export LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "cppunit/*.h")
install(EXPORT cppunit-export FILE cppunit-targets.cmake DESTINATION lib/cmake/cppunit)
