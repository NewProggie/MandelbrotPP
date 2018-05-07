# Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

find_package(OpenMP)

cmake_host_system_information(RESULT SSE2_FOUND QUERY HAS_SSE2)

add_library(mpp_lib STATIC
  check.h
  cmdline_flags.cpp
  cmdline_flags.h
  cpu.h
  cpu.cpp
  exporter.cpp
  internal_macros.h
  logging.cpp
  logging.h
  mandel_basic.cpp
  $<$<BOOL:${SSE2_FOUND}>:mandel_sse2.cpp>
  mandel_avx.cpp
  pgm_exporter.cpp
  $<$<BOOL:${PNG_FOUND}>:png_exporter.cpp>
  ppm_exporter.cpp
  string_util.cpp
  string_util.h)
add_library(mpp::lib ALIAS mpp_lib)

target_compile_features(mpp_lib PUBLIC cxx_std_14)
target_compile_options(mpp_lib PUBLIC -mavx)
target_compile_definitions(mpp_lib
  PUBLIC
    $<$<BOOL:${PNG_FOUND}>:HAS_PNG> $<$<BOOL:${SSE2_FOUND}>:SSE2_FOUND>)
target_link_libraries(mpp_lib
  PUBLIC
    $<$<BOOL:${PNG_FOUND}>:PNG::PNG>
    $<$<BOOL:${OpenMP_FOUND}>:OpenMP::OpenMP_CXX>)

# Expose public/private API
target_include_directories(mpp_lib
  PRIVATE
    ${MandelbrotPP_SOURCE_DIR}/src
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${MandelbrotPP_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

include(GenerateExportHeader)
generate_export_header(mpp_lib)

add_executable(mpp main.cpp)
target_link_libraries(mpp PRIVATE mpp::lib)

if(BUILD_BENCHMARK)
  set(PERF_SOURCES
    benchmark.h
    mpp_perf.cpp)

  add_executable(mpp_perf ${PERF_SOURCES})
  target_compile_features(mpp_perf PRIVATE cxx_std_14)
  target_link_libraries(mpp_perf PRIVATE mpp::lib)
endif()
