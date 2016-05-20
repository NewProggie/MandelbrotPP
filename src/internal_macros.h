// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

// Convenient macro check for definitions, initially taken from:
// https://plus.google.com/+LinusTorvalds/posts/9gntjh57dXt
// Following macro expands to 1 if a preprocessor value was defined to 1 and 0
// if it wasn't or defined to another value. May be used directory as:
//
// #if IS_DEFINED(SOMETHING)
// ...
// #endif
//
// or even directly in the code, which looks much better:
//
// if (IS_DEFINED(SOMETHING)) {
// ...
// }
#define IS_DEFINED(macro) IS_DEFINED_(macro)
#define MACROTEST_1 ,
#define IS_DEFINED_(value) IS_DEFINED__(MACROTEST_##value)
#define IS_DEFINED__(comma) IS_DEFINED___(comma 1, 0)
#define IS_DEFINED___(_, v, ...) v

// determine host os system
#if defined(_WIN32)
# define MANDELBROTPP_OS_WINDOWS 1
# define __func__ __FUNCTION__
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
