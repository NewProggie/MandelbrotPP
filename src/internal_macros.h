// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

// determine host os system
#if defined(_WIN32)
# define MANDELBROTPP_OS_WINDOWS 1
#elif defined(__APPLE__)
# define MANDELBROTPP_OS_MACOSX 1
#elif defined(__FreeBSD__)
# define MANDELBROTPP_OS_FREEBSD 1
#elif defined(__linux__)
# define MANDELBROTPP_OS_LINUX 1
#endif

// determine used compiler
#if defined(__clang__)
# define COMPILER_CLANG
#elif defined(_MSC_VER)
# define COMPILER_MSVC
#elif defined(__GNUC__)
# define COMPILER_GCC
#endif
