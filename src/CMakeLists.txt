# Copyright (c) 2016, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

add_library(mpp_lib cmdline_flags.cpp colorprint.cpp logging.cpp
      string_util.cpp ppm_exporter.cpp pgm_exporter.cpp exporter.cpp)
add_library(mpp::lib ALIAS mpp_lib)
enable_cxx_11(mpp_lib)

# Expose public/private API
target_include_directories(mpp_lib PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_executable(mpp main.cxx)
enable_cxx_11(mpp)

target_link_libraries(mpp PRIVATE mpp::lib)
