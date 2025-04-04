#
# Open3DMotion
# Copyright (c) 2004-2025.
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
  set(OPEN3DMOTION_BIND_PYTHON_VERSION_MAJOR "3" CACHE STRING "Python major version")
  set(OPEN3DMOTION_BIND_PYTHON_VERSION_MINOR "12" CACHE STRING "Python minor version")
  set(OPEN3DMOTION_BIND_PYTHON_VERSION_MICRO "6" CACHE STRING "Python micro version")
  set(OPEN3DMOTION_BIND_PYTHON_CORE_RELEASE "/Python-${OPEN3DMOTION_BIND_PYTHON_VERSION_MAJOR}.${OPEN3DMOTION_BIND_PYTHON_VERSION_MINOR}.${OPEN3DMOTION_BIND_PYTHON_VERSION_MICRO}" CACHE PATH "Path to Python library")
  set(OPEN3DMOTION_BIND_PYTHON_CORE_DEBUG "${OPEN3DMOTION_BIND_PYTHON_CORE_RELEASE}" CACHE PATH "Path to Python library during debug builds")
endif (OPEN3DMOTION_BIND_PYTHON)
option(OPEN3DMOTION_OWNBUILD_CPPUNIT "Import our own build of CPPUNIT" ON)
option(OPEN3DMOTION_OWNINCLUDE_CLAPACK "Put our own clapack directory on the include path" OFF)
set(OPEN3DMOTION_TEST_PATH_RELEASE "" CACHE PATH "Append to PATH environment used during testing (Release)")
set(OPEN3DMOTION_TEST_PATH_DEBUG "" CACHE PATH "Append to PATH environment used during testing  (Debug)")
set(OPEN3DMOTION_TEST_PYTHONPATH_RELEASE "" CACHE PATH "Append to PYTHONPATH environment used during testing (Release)")
set(OPEN3DMOTION_TEST_PYTHONPATH_DEBUG "" CACHE PATH "Append to PYTHONPATH environment used during testing  (Debug)")

# Append 'd' to end of library name for the debug version of the libraries we build
SET(CMAKE_DEBUG_POSTFIX d)

# Optionally use static libraries on windows
if (OPEN3DMOTION_MSVC_RUNTIME_STATIC)
  foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO 
	      CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
    if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      set(${flag_var} "${${flag_var}}" CACHE STRING "" FORCE)
      endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif(OPEN3DMOTION_MSVC_RUNTIME_STATIC)

# Per-architecture (and customiseable) library directories
if (WIN32 AND MSVC)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
     set(INSTALL_ARCHITECTURE "win64")
  else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(INSTALL_ARCHITECTURE "win32")
  endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(INSTALL_COMPILER "msvc")
  string(SUBSTRING ${MSVC_VERSION} 0 2 MSVC_VERSION_MAJOR)
  set(CMAKE_LIBRARY_ARCHITECTURE "${INSTALL_ARCHITECTURE}-${INSTALL_COMPILER}-${MSVC_VERSION_MAJOR}")
  message(${PROJECT_NAME} " building for architecture: " ${CMAKE_LIBRARY_ARCHITECTURE})
endif(WIN32 AND MSVC)
set(CMAKE_INSTALL_LIBDIR "lib/${CMAKE_LIBRARY_ARCHITECTURE}" CACHE STRING "Subdirectory for library")

# Do our own build of dependent libraries if requested

if (OPEN3DMOTION_OWNBUILD_LIBB64)
  add_subdirectory(ThirdParty/libb64)
endif (OPEN3DMOTION_OWNBUILD_LIBB64)

if (OPEN3DMOTION_OWNBUILD_PUGIXML)
  add_subdirectory(ThirdParty/pugixml)
endif (OPEN3DMOTION_OWNBUILD_PUGIXML)

if (OPEN3DMOTION_OWNBUILD_ZLIB)
  add_subdirectory(ThirdParty/zlib)
endif (OPEN3DMOTION_OWNBUILD_ZLIB)

if (OPEN3DMOTION_OWNBUILD_CPPUNIT)
  add_subdirectory(ThirdParty/cppunit)
endif(OPEN3DMOTION_OWNBUILD_CPPUNIT)

# Define the library
add_subdirectory(Open3DMotion)

# Define tests
enable_testing()
add_subdirectory(Open3DMotionTest)
