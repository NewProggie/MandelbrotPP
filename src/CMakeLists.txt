# Copyright (c) 2016, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

# Define all the source files
set(SOURCE_FILES "cmdline_flags.cpp" "colorprint.cpp" "logging.cpp" string_util.cpp)

add_library(mpp_lib ${SOURCE_FILES})
enable_cxx_11(mpp_lib)

add_executable(mpp "mandelbrotpp.cxx")
enable_cxx_11(mpp)

# Expose public API
target_include_directories(mpp PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(mpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mpp mpp_lib ${CPR_LIBRARIES})
