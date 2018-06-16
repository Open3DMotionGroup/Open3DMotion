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
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION include/pugixml FILES_MATCHING PATTERN "*.hpp")
install(EXPORT ${PROJECT_NAME}-export FILE ${PROJECT_NAME}-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Allow import directly from build directory if required
export(EXPORT ${PROJECT_NAME}-export FILE ${PROJECT_NAME}-config.cmake)
