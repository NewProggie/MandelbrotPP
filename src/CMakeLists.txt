# Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

find_package(OpenMP REQUIRED)

add_library(mpp_lib STATIC
  check.h
  cmdline_flags.cpp
  cmdline_flags.h
  exporter.cpp
  internal_macros.h
  logging.cpp
  logging.h
  mandel_basic.cpp
  pgm_exporter.cpp
  $<$<BOOL:${PNG_FOUND}>:png_exporter.cpp>
  ppm_exporter.cpp
  string_util.cpp
  string_util.h)
add_library(mpp::lib ALIAS mpp_lib)

target_compile_features(mpp_lib PUBLIC cxx_std_14)
target_compile_definitions(mpp_lib INTERFACE $<$<BOOL:${PNG_FOUND}>:HAS_PNG>)
target_link_libraries(mpp_lib PUBLIC $<$<BOOL:${PNG_FOUND}>:PNG::PNG>)

# Expose public/private API
target_include_directories(mpp_lib
  PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${MandelbrotPP_SOURCE_DIR}/src
  PUBLIC 
    $<BUILD_INTERFACE:${MandelbrotPP_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

include(GenerateExportHeader)
generate_export_header(mpp_lib)

add_executable(mpp main.cpp)
target_link_libraries(mpp PRIVATE mpp::lib)

if(BUILD_BENCHMARK)
  add_executable(mandel_basic mpp_perf.cpp mandel_basic.cpp)
  target_link_libraries(mandel_basic PRIVATE mpp::lib)
  add_test(NAME mandel_basic COMMAND mandel_basic)

  add_executable(mandel_sse2 mpp_perf.cpp mandel_sse2.cpp)
  target_link_libraries(mandel_sse2 PRIVATE mpp::lib OpenMP::OpenMP_CXX)
  add_test(NAME mandel_sse2 COMMAND mandel_basic)
endif()