# Copyright (c) 2016, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(mandelbrotpp VERSION 0.1 LANGUAGES CXX)

include(cmake/options.cmake REQUIRED)
include(cmake/compiler_options.cmake REQUIRED)
include(cmake/dynamic_analyzer_options.cmake REQUIRED)
include(cmake/documentation_targets.cmake REQUIRED)
include(cmake/macros.cmake REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(src)
