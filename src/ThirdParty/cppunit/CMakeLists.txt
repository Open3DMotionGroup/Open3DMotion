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
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-export LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
install(EXPORT ${PROJECT_NAME}-export FILE ${PROJECT_NAME}-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Allow import directly from build directory if required
export(EXPORT ${PROJECT_NAME}-export FILE ${PROJECT_NAME}-config.cmake)
