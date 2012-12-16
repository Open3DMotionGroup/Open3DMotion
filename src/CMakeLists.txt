#
# Open3DMotion
# Copyright (c) 2004-2012.
# All rights reserved.
# See LICENSE.txt for details.
#
# CMake build file
#

cmake_minimum_required(VERSION 2.6)

project (Open3DMotion)

option(OPEN3DMOTION_OWNBUILD_CPPUNIT "Link to our own build of CPPUNIT" ON)
option(OPEN3DMOTION_OWNBUILD_LIBB64 "Link to our own build of libb64" ON)
option(OPEN3DMOTION_OWNBUILD_PUGIXML "Link to our own build of pugixml" ON)
option(OPEN3DMOTION_OWNBUILD_ZLIB "Link to our own build of zlib" ON)
option(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN "For linear algebra use the Eigen library instead of CLAPACK" OFF)
if (MSVC)
  option(OPEN3DMOTION_MSVC_RUNTIME_STATIC "Use static (non-DLL) version of C runtime libraries" ON)
endif(MSVC)
set(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN_INCLUDEPATH "/usr/include" CACHE PATH "Path to Eigen library")

SET(CMAKE_DEBUG_POSTFIX d)

# Additional warning flags on GNU C
if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG  "-g -Wnon-virtual-dtor -Wno-long-long -ansi -Wcast-align -Wchar-subscripts -Wall -Wextra -Wpointer-arith -Wformat-security -Woverloaded-virtual -Wunused-parameter -fno-check-new -fno-common")
endif (CMAKE_COMPILER_IS_GNUCC)

# Optionally use static libraries on windows
if (OPEN3DMOTION_MSVC_RUNTIME_STATIC)
  foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO 
	CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif(OPEN3DMOTION_MSVC_RUNTIME_STATIC)

# Inlclude path
include_directories (${PROJECT_SOURCE_DIR})

# Default include paths for third-party libs
include_directories (${PROJECT_SOURCE_DIR}/ThirdParty/CLAPACK)
include_directories (${PROJECT_SOURCE_DIR}/ThirdParty/libb64/include)
include_directories (${PROJECT_SOURCE_DIR}/ThirdParty/pugixml/src)
include_directories (${PROJECT_SOURCE_DIR}/ThirdParty/zlib)
include_directories (${PROJECT_SOURCE_DIR}/ThirdParty/cppunit/include)

# Option to use Eigen library instead of CLAPACK
if (OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  add_definitions(-DOPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  include_directories(${OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN_INCLUDEPATH})
endif(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)

# Build the library
file(GLOB_RECURSE Open3DMotion_CPP Open3DMotion/*.cpp)
file(GLOB_RECURSE Open3DMotion_H Open3DMotion/*.h)
add_library(Open3DMotion ${Open3DMotion_CPP} ${Open3DMotion_H})

# Build test program
file(GLOB_RECURSE Open3DMotionTest_CPP Open3DMotionTest/*.cpp)
file(GLOB_RECURSE Open3DMotionTest_H Open3DMotionTest/*.h)
add_executable(Open3DMotionTest ${Open3DMotionTest_CPP} ${Open3DMotionTest_H})
target_link_libraries (Open3DMotionTest Open3DMotion)

if (OPEN3DMOTION_OWNBUILD_CPPUNIT)
  add_subdirectory(${PROJECT_SOURCE_DIR}/ThirdParty/cppunit)
  target_link_libraries (Open3DMotionTest cppunit)
else(OPEN3DMOTION_OWNBUILD_CPPUNIT)
  link_directories(${PROJECT_SOURCE_DIR}/ThirdParty/cppunit/lib)
  target_link_libraries (Open3DMotionTest debug cppunitd optimized cppunit)
endif(OPEN3DMOTION_OWNBUILD_CPPUNIT)

if (OPEN3DMOTION_OWNBUILD_LIBB64)
  add_subdirectory(${PROJECT_SOURCE_DIR}/ThirdParty/libb64)
  target_link_libraries (Open3DMotionTest libb64)
else(OPEN3DMOTION_OWNBUILD_LIBB64)
  link_directories(${PROJECT_SOURCE_DIR}/ThirdParty/libb64/lib)
  target_link_libraries (Open3DMotionTest debug libb64d optimized libb64)
endif(OPEN3DMOTION_OWNBUILD_LIBB64)

if (OPEN3DMOTION_OWNBUILD_PUGIXML)
  add_subdirectory(${PROJECT_SOURCE_DIR}/ThirdParty/pugixml)
  target_link_libraries (Open3DMotionTest pugixml)
else(OPEN3DMOTION_OWNBUILD_PUGIXML)
  link_directories(${PROJECT_SOURCE_DIR}/ThirdParty/pugixml/lib)
  target_link_libraries (Open3DMotionTest debug pugixmld optimized pugixml)
endif(OPEN3DMOTION_OWNBUILD_PUGIXML)

if (OPEN3DMOTION_OWNBUILD_ZLIB)
  add_subdirectory(${PROJECT_SOURCE_DIR}/ThirdParty/zlib)
  target_link_libraries (Open3DMotionTest zlib)
else(OPEN3DMOTION_OWNBUILD_ZLIB)
  link_directories(${PROJECT_SOURCE_DIR}/ThirdParty/zlib/lib)
  target_link_libraries (Open3DMotionTest debug zlibd optimized zlib)
endif(OPEN3DMOTION_OWNBUILD_ZLIB)

# Optionally use the Eigen library instead of CLAPACK
if (OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  # No library binaries needed here - all relevant things are in header templates
else(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  # Add CLAPACK and supporting libraries
  link_directories(${PROJECT_SOURCE_DIR}/ThirdParty/CLAPACK/lib)
  target_link_libraries(Open3DMotionTest optimized clapack optimized blas optimized libf2c)
  target_link_libraries(Open3DMotionTest debug clapackd debug blasd debug libf2cd)
endif(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)

install(TARGETS Open3DMotion Open3DMotionTest
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

install(DIRECTORY ./Open3DMotion DESTINATION include/Open3DMotion
          FILES_MATCHING PATTERN "*.h" 
          PATTERN ".svn" EXCLUDE
          PATTERN "ipch" EXCLUDE)

enable_testing()
add_test (Open3DMotionTestRun ${CMAKE_CURRENT_BINARY_DIR}/Open3DMotionTest)
set_tests_properties(Open3DMotionTestRun PROPERTIES WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
