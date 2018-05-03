# Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(MandelbrotPP VERSION 0.1 
  DESCRIPTION "C++ based implementation of the famous Mandelbrot set"
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set the default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Set up directory structure for output libraries and binaries
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  endif()
endif()

# Setup a few default arguments and options for the following projects
option(BUILD_TESTING "Build with tests" OFF)
option(BUILD_BENCHMARK "Build with performance tests" ON)
option(BUILD_DOCUMENTATION "Build with documentation" OFF)

# Setup project defaults
include(InstallLocation)
include(ExternalProject)
include(Projects)
enable_testing()

add_subdirectory(src)
