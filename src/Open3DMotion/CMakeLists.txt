#
# Open3DMotion
# Copyright (c) 2004-2018.
# All rights reserved.
# See LICENSE.txt for details.
#
# CMake build file for library
#

# Source dependencies

file(GLOB_RECURSE Open3DMotion_CPP 
  OpenORM/*.cpp
  Maths/*.cpp
  Biomechanics/*.cpp
  MotionBundle/*.cpp
  MotionFile/*.cpp)

file(GLOB_RECURSE Open3DMotion_H 
  OpenORM/*.h
  Maths/*.h
  Biomechanics/*.h
  MotionFile/*.h
  MotionBundle/*.h)

# Optional python dependencies

if(OPEN3DMOTION_BIND_PYTHON)
  file(GLOB_RECURSE Open3DMotion_Python_CPP Bindings/Python/*.cpp)
  file(GLOB_RECURSE Open3DMotion_Python_H Bindings/Python/*.h)
endif(OPEN3DMOTION_BIND_PYTHON)

# Define the library source files
add_library(Open3DMotion ${Open3DMotion_CPP} ${Open3DMotion_H} ${Open3DMotion_Python_CPP} ${Open3DMotion_Python_H})

# Specify include directories to export
# and optionally use our own clapack headers
target_include_directories(Open3DMotion 
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    PRIVATE
        $<$<BOOL:OPEN3DMOTION_OWNINCLUDE_CLAPACK>:${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/CLAPACK>
)

# Link core dependencies
target_link_libraries(Open3DMotion libb64 pugixml zlib)

# Link CLAPACK unless using Eigen
if(NOT OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)
  target_link_libraries(Open3DMotion f2c blas lapack)
endif(NOT OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN)

# Disable warnings for sprintf and sscanf on MSC
target_compile_definitions(Open3DMotion PUBLIC $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>)

# Set Eigen flag if present
target_compile_definitions(Open3DMotion PUBLIC $<$<BOOL:${OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN}>:OPEN3DMOTION_LINEAR_ALGEBRA_EIGEN>)

# Installation
install(TARGETS Open3DMotion EXPORT Open3DMotionExport LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT Open3DMotionExport FILE Open3DMotionConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Open3DMotion)

# Allow import directly from build directory if required
export(EXPORT Open3DMotionExport FILE Open3DMotionConfig.cmake)
