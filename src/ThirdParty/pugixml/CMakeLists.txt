# pugixml build file

cmake_minimum_required(VERSION 3.5)

project (pugixml)

# Specify source files
file(GLOB_RECURSE Source_Files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(pugixml ${Source_Files})

# Specify include directories
target_include_directories(pugixml
    PUBLIC
        $<INSTALL_INTERFACE:include/pugixml>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# Installation
install(TARGETS pugixml EXPORT pugixml-export LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION include/pugixml FILES_MATCHING PATTERN "*.hpp")
install(EXPORT pugixml-export FILE pugixml-targets.cmake DESTINATION lib/cmake/pugixml)
