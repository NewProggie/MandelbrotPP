# Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

unset(projects)

set(MandelbrotPP_INSTALL_PREFIX ${MandelbrotPP_BINARY_DIR}/install)

# Google Benchmark
list(APPEND projects benchmark)
set(benchmark_version "1.4.0")
set(benchmark_url "https://github.com/google/benchmark/archive/${benchmark_version}.tar.gz")
set(benchmark_path "${MandelbrotPP_SOURCE_DIR}/thirdparty/benchmark-${benchmark_version}.tar.gz")
set(benchmark_md5 "ccfaf2cd93ae20191b94f730b945423e")

# LibPNG
list(APPEND projects libpng)
set(libpng_version "1.6.34")
set(libpng_url "https://github.com/glennrp/libpng/archive/v${libpng_version}.tar.gz")
set(libpng_path "${MandelbrotPP_SOURCE_DIR}/thirdparty/libpng-${libpng_version}.tar.gz")
set(libpng_md5 "b2cc4bddf1d35305606063a19bff3bcb")


option(USE_SYSTEM_BENCHMARK "Use installed version of Google benchmark" OFF)
option(USE_SYSTEM_LIBPNG "Use installed version of LibPNG" OFF)

# We don't even try to compile libpng under Windows
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  find_package(PNG)
elseif(USE_SYSTEM_LIBPNG)
  find_package(PNG REQUIRED)
else()
  include(External_libpng)
endif()