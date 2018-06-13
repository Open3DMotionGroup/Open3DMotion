#
# Open3DMotion
# Copyright (c) 2004-2018.
# All rights reserved.
# See LICENSE.txt for details.
#
# CMake build file
#

cmake_minimum_required(VERSION 3.5)

project (Open3DMotion)

# Configuration
option(OPEN3DMOTION_BIND_PYTHON "Python bindings" OFF)
option(OPEN3DMOTION_OWNBUILD_LIBB64 "Import our own build of libb64" ON)
option(OPEN3DMOTION_OWNBUILD_PUGIXML "Import our own build of pugixml" ON)
option(OPEN3DMOTION_OWNBUILD_ZLIB "Import our own build of zlib" ON)
if (MSVC)
  option(OPEN3DMOTION_MSVC_RUNTIME_STATIC "Use static (non-DLL) version of C runtime libraries" ON)
endif(MSVC)
set(OPEN3DMOTION_LINEAR_ALGEBRA_CLAPACK_FINDPATH "" CACHE PATH "Find clapack using this path")
option(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN "For linear algebra use the Eigen library instead of CLAPACK" OFF)
set(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN_INCLUDEPATH "/usr/include" CACHE PATH "Path to Eigen library of headers")
if (OPEN3DMOTION_BIND_PYTHON)
  set(OPEN3DMOTION_BIND_PYTHON_VERSION_MAJOR "2" CACHE STRING "Python major version")
  set(OPEN3DMOTION_BIND_PYTHON_VERSION_MINOR "7" CACHE STRING "Python minor version")
  set(OPEN3DMOTION_BIND_PYTHON_INSTALLPATH "C:/Python${OPEN3DMOTION_BIND_PYTHON_VERSION_MAJOR}${OPEN3DMOTION_BIND_PYTHON_VERSION_MINOR}" CACHE PATH "Path to Python library")
endif (OPEN3DMOTION_BIND_PYTHON)
option(OPEN3DMOTION_OWNBUILD_CPPUNIT "Import our own build of CPPUNIT" ON)
option(OPEN3DMOTION_OWNINCLUDE_CLAPACK "Put our own clapack directory on the include path" OFF)

# Append 'd' to end of library name for the debug version of the libraries we build
SET(CMAKE_DEBUG_POSTFIX d)

# Additional warning flags for GCC/Clang on Apple
if(APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG  "-g -Wnon-virtual-dtor -Wno-long-long -Wchar-subscripts -Wall -Wextra -Wpointer-arith -Wformat-security -Woverloaded-virtual -Wcast-align -fno-common")
  if(CMAKE_COMPILER_IS_GNUCXX)
      set(CMAKE_CXX_FLAGS_DEBUG  ${CMAKE_CXX_FLAGS_DEBUG} "-ansi -fno-check-new ")
  endif(CMAKE_COMPILER_IS_GNUCXX)
endif(APPLE)

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

# Disable warnings for sprintf and sscanf on MSC
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
endif(MSVC)

# Python includes
if (OPEN3DMOTION_BIND_PYTHON)
  include_directories (${OPEN3DMOTION_BIND_PYTHON_INSTALLPATH}/include)
  set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -DPy_DEBUG")
  set(CMAKE_C_FLAGS_DEBUG  "${CMAKE_C_FLAGS_DEBUG} -DPy_DEBUG")
endif(OPEN3DMOTION_BIND_PYTHON)

# Option to use Eigen library instead of CLAPACK
if (OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  add_definitions(-DOPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  include_directories(${OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN_INCLUDEPATH})
endif(OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)

if (OPEN3DMOTION_OWNBUILD_LIBB64)
  add_subdirectory(ThirdParty/libb64)
endif (OPEN3DMOTION_OWNBUILD_LIBB64)

if (OPEN3DMOTION_OWNBUILD_PUGIXML)
  add_subdirectory(ThirdParty/pugixml)
endif (OPEN3DMOTION_OWNBUILD_PUGIXML)

if (OPEN3DMOTION_OWNBUILD_ZLIB)
  add_subdirectory(ThirdParty/zlib)
endif (OPEN3DMOTION_OWNBUILD_ZLIB)

# Create CMake definition for CLAPACK using our headers and externally imported lib files
if(OPEN3DMOTION_OWNDEFINITION_CLAPACK)
  add_subdirectory(ThirdParty/clapack)
endif(OPEN3DMOTION_OWNDEFINITION_CLAPACK)

# Build cppunit if required
if (OPEN3DMOTION_OWNBUILD_CPPUNIT)
  add_subdirectory(ThirdParty/cppunit)
endif(OPEN3DMOTION_OWNBUILD_CPPUNIT)

# Source dependencies

file(GLOB_RECURSE Open3DMotion_CPP 
  Open3DMotion/OpenORM/*.cpp
  Open3DMotion/Maths/*.cpp
  Open3DMotion/Biomechanics/*.cpp
  Open3DMotion/MotionBundle/*.cpp
  Open3DMotion/MotionFile/*.cpp)

file(GLOB_RECURSE Open3DMotion_H 
  Open3DMotion/OpenORM/*.h
  Open3DMotion/Maths/*.h
  Open3DMotion/Biomechanics/*.h
  Open3DMotion/MotionFile/*.h
  Open3DMotion/MotionBundle/*.h)

# Optional python dependencies

if(OPEN3DMOTION_BIND_PYTHON)
  file(GLOB_RECURSE Open3DMotion_Python_CPP Open3DMotion/Bindings/Python/*.cpp)
  file(GLOB_RECURSE Open3DMotion_Python_H Open3DMotion/Bindings/Python/*.h)
endif(OPEN3DMOTION_BIND_PYTHON)

# Define the library source files
add_library(Open3DMotion ${Open3DMotion_CPP} ${Open3DMotion_H} ${Open3DMotion_Python_CPP} ${Open3DMotion_Python_H})

# Specify include directories to export
# and optionally use our own clapack headers
target_include_directories(Open3DMotion 
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        $<$<BOOL:OPEN3DMOTION_OWNINCLUDE_CLAPACK>:${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/CLAPACK>
)

# Link dependent library
target_link_libraries(Open3DMotion libb64 pugixml zlib)

# Link CLAPACK unless using Eigen
if(NOT OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  find_package(clapack PATHS ${OPEN3DMOTION_LINEAR_ALGEBRA_CLAPACK_FINDPATH})
  target_link_libraries(Open3DMotion f2c blas lapack)
endif(NOT OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)

# Define tests
enable_testing()
add_subdirectory(Open3DMotionTest)

# Installation
install(TARGETS Open3DMotion EXPORT Open3DMotion-export LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Open3DMotion DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT Open3DMotion-export FILE Open3DMotion-targets.cmake DESTINATION lib/cmake/Open3DMotion)
