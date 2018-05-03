# Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

ExternalProject_Add(benchmark_ext
  URL ${benchmark_path}
  URL_MD5 ${benchmark_md5}
  INSTALL_DIR ${MandelbrotPP_INSTALL_PREFIX}/benchmark
  CMAKE_ARGS
    -Wno-dev
    -DBUILD_SHARED_LIBS=OFF
    -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    -DBENCHMARK_ENABLE_TESTING=OFF
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF)

ExternalProject_Get_Property(benchmark_ext install_dir)
set(benchmark_name ${CMAKE_STATIC_LIBRARY_PREFIX}benchmark${CMAKE_STATIC_LIBRARY_SUFFIX})
set(benchmark_ROOT ${install_dir} CACHE INTERNAL "")

file(MAKE_DIRECTORY ${benchmark_ROOT}/include)
set(benchmark_INCLUDE_DIRS ${benchmark_ROOT}/include)
set(benchmark_LIBRARIES ${benchmark_ROOT}/lib/${benchmark_name})

add_library(benchmark STATIC IMPORTED)
add_dependencies(benchmark benchmark_ext)
set_target_properties(benchmark PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${benchmark_INCLUDE_DIRS}
  IMPORTED_LOCATION ${benchmark_LIBRARIES})