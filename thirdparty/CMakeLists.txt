# Copyright (c) 2016, Kai Wolf. All rights reserved.
# Use of this source code is governed by a MIT license that can be found in
# the LICENSE file.

# Unset compiler flags for thirdparty dependencies
unset(CMAKE_C_FLAGS)
unset(CMAKE_CXX_FLAGS)

# GoogleTest for unit testing
add_subdirectory(googletest-1.7.0)

# libpng for png export
add_subdirectory(libpng-1.6.23)