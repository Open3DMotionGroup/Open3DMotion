#
# Open3DMotionTest
# Copyright (c) 2004-2018.
# All rights reserved.
# See LICENSE.txt for details.
#
# CMake build file for tests
#

# Define test source files

file(GLOB_RECURSE Open3DMotionTest_CPP 
  Open3DMotionTest.cpp
  Data/*.cpp  
  OpenORM/*.cpp
  Maths/*.cpp
  Biomechanics/*.cpp
  MotionBundle/*.cpp
  MotionFile/*.cpp)

file(GLOB_RECURSE Open3DMotionTest_H 
  OpenORM/*.h
  Data/*.h  
  Maths/*.h
  Biomechanics/*.h
  MotionFile/*.h
  MotionBundle/*.h)

# Only require Python tests when Python bindings are enabled
if(OPEN3DMOTION_BIND_PYTHON)
  file(GLOB_RECURSE Open3DMotionTest_Python_CPP Bindings/Python/*.cpp)
  file(GLOB_RECURSE Open3DMotionTest_Python_H Bindings/Python/*.h)
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
