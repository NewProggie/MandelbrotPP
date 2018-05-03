# Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

ExternalProject_Add(PNG
  URL ${libpng_path}
  URL_MD5 ${libpng_md5}
  INSTALL_DIR ${MandelbrotPP_INSTALL_PREFIX}/libpng
  CMAKE_ARGS
    -Wno-dev
    -DBUILD_SHARED_LIBS=OFF
    -DPNG_SHARED=OFF
    -DPNG_STATIC=ON)

ExternalProject_Get_Property(PNG install_dir)
set(PNG_name ${CMAKE_STATIC_LIBRARY_PREFIX}png${CMAKE_STATIC_LIBRARY_SUFFIX})
set(PNG_ROOT ${install_dir} CACHE INTERNAL "")
set(PNG_FOUND TRUE)

file(MAKE_DIRECTORY ${PNG_ROOT}/include)
set(PNG_INCLUDE_DIRS ${PNG_ROOT}/include)
set(PNG_LIBRARIES ${PNG_ROOT}/lib/${PNG_name})
set(PNG_VERSION_STRING ${libpng_version})

add_library(PNG::PNG STATIC IMPORTED)
add_dependencies(PNG::PNG PNG)
set_target_properties(PNG::PNG PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${PNG_INCLUDE_DIRS}
  IMPORTED_LOCATION ${PNG_LIBRARIES})
