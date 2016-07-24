# Copyright (c) 2016, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

add_library(mpp_lib
    array_size.h
    check.h
    cmdline_flags.cpp
    cmdline_flags.h
    colorprint.cpp
    colorprint.h
    exporter.cpp
    internal_macros.h
    logging.cpp
    logging.h
    pgm_exporter.cpp
    png_exporter.cpp
    ppm_exporter.cpp
    string_util.cpp
    string_util.h)
add_library(mpp::lib ALIAS mpp_lib)
enable_cxx_11(mpp_lib)

# Expose public/private API
target_include_directories(mpp_lib PUBLIC ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/src)

add_executable(mpp main.cxx)
enable_cxx_11(mpp)

target_link_libraries(mpp PRIVATE mpp::lib png)
