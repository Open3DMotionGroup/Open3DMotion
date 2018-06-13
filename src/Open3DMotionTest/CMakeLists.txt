#
# Open3DMotionTest
# Copyright (c) 2004-2018.
# All rights reserved.
# See LICENSE.txt for details.
#
# CMake build file for tests
#

cmake_minimum_required(VERSION 3.5)

# Note no project set (assumes Open3DMotion is project)

# Define test source files

file(GLOB_RECURSE Open3DMotionTest_CPP 
  ${CMAKE_CURRENT_SOURCE_DIR}/Open3DMotionTest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Data/*.cpp  
  ${CMAKE_CURRENT_SOURCE_DIR}/OpenORM/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Maths/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Biomechanics/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MotionBundle/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MotionFile/*.cpp)

file(GLOB_RECURSE Open3DMotionTest_H 
  ${CMAKE_CURRENT_SOURCE_DIR}/OpenORM/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Data/*.h  
  ${CMAKE_CURRENT_SOURCE_DIR}/Maths/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Biomechanics/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MotionFile/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MotionBundle/*.h)

# Only require Python tests when Python bindings are enabled
if(OPEN3DMOTION_BIND_PYTHON)
  file(GLOB_RECURSE Open3DMotionTest_Python_CPP ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/Python/*.cpp)
  file(GLOB_RECURSE Open3DMotionTest_Python_H ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/Python/*.h)
endif(OPEN3DMOTION_BIND_PYTHON)

# Define all dependencies of test executable
add_executable(Open3DMotionTest ${Open3DMotionTest_CPP} ${Open3DMotionTest_H} ${Open3DMotionTest_Python_CPP} ${Open3DMotionTest_Python_H})

# And include directory
target_include_directories(Open3DMotionTest 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Dependencies
target_link_libraries(Open3DMotionTest Open3DMotion cppunit)

# Describe how to run the test
# - note that working directory must be set to src for tests to succeed
#   because they expect to see Open3DMotionTest/Data present (relative to working directory)
add_test (NAME Open3DMotionTestRun COMMAND $<TARGET_FILE:Open3DMotionTest>)
set_tests_properties(Open3DMotionTestRun PROPERTIES WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
