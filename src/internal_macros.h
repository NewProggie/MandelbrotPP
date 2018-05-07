// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_INTERNAL_MACROS_H
#define MANDELBROTPP_INTERNAL_MACROS_H

#include <memory>

namespace internal {
/// The arraysize(arr) macro returns the number of elements in array.
/// The expression is a compile-time constant, and therefore can be used in
/// defining new arrays, for example. If you use arraysize on a pointer by
/// mistake, you will get a compile-time error, since *foo and foo[] are
/// two different things.
template <typename T, std::size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
} // namespace internal

#define arraysize(arr) (sizeof(internal::ArraySizeHelper(arr)))

/// Inside the declaration of a class will make it unassignable
#define DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&) = delete

/// Inside the declaration of a class will make it uncopyable and unassignable
#if __cplusplus < 201103L
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);             \
    void operator=(const TypeName&)
#else
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;    \
    TypeName& operator=(const TypeName&) = delete;
#endif

/// Inside the declaration of a class will make it un-default constructible
#define DISALLOW_DEFAULT_CTOR(TypeName) TypeName() = delete

#define DISALLOW_IMPLICIT_CONSTRUCTOR(TypeName) \
    TypeName() = delete;                        \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

/// Verify an expression during compile-time
#undef COMPILE_ASSERT
#define COMPILE_ASSERT(expr, msg) static_assert(expr, #msg)

/// Give branch indication hints to the (GCC) compiler: whether the if
/// condition is likely to be true or false
#undef LIKELY
#undef UNLIKELY
#if defined(__GNUC__) && __GNUC__ >= 4
#  define LIKELY(x) (__builtin_expect((x), 1))
#  define UNLIKELY(x) (__builtin_expect((x), 0))
#else
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
#endif

/// Determine host os system
#if defined(ANDROID)
#  define OS_ANDROID 1
#elif defined(__APPLE__)
#  include <TargetConditionals.h>
#  define OS_MACOSX 1
#  if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#    define OS_IOS 1
#  endif
#elif defined(__linux__)
#  define OS_LINUX 1
#elif defined(_WIN32)
#  define OS_WINDOWS 1
#elif defined(__FreeBSD__)
#  define OS_FREEBSD 1
#elif defined(__NetBSD__)
#  define OS_NETBSD 1
#elif defined(__OpenBSD__)
#  define OS_OPENBSD 1
#else
#  error Please add support for this platform in internal_macros.h
#endif

/// For accessing standard BSD features, use OS_BSD instead of a more specific
/// macro
#if defined(OS_FREEBSD) || defined(OS_NETBSD) || defined(OS_OPENBSD)
#  define OS_BSD 1
#endif

/// for accessing standard POSIX features, use OS_POSIX instead of a more
/// specific macro
#if defined(OS_ANDROID) || defined(OS_FREEBSD) || defined(OS_NETBSD) ||        \
    defined(OS_OPENBSD) || defined(OS_MACOSX)
#  define OS_POSIX 1
#endif

/// Compiler detection
#if defined(__GNUC__)
#  define COMPILER_GCC 1
#elif defined(__clang__)
#  define COMPILER_CLANG 1
#elif defined(_MSC_VER)
#  define COMPILER_MSVC
#else
#  error Please add support for your compiler in internal_macros.h
#endif

/// Processor architecture detection. For more info, see:
/// http://www.agner.org/optimize/calling_conventions.pdf
#if defined(_M_X64) || defined(__x86_64__)
#  define ARCH_CPU_X86_FAMILY 1
#  define ARCH_CPU_X86_64 1
#  define ARCH_CPU_64_BITS 1
#  define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
#  define ARCH_CPU_X86_FAMILY 1
#  define ARCH_CPU_X86 1
#  define ARCH_CPU_32_BITS 1
#  define ARCH_CPU_LITTLE_ENDIAN 1
#elif (defined(__PPC64__) || defined(__PPC__)) && defined(__BIG_ENDIAN__)
#  define ARCH_CPU_PPC64_FAMILY 1
#  define ARCH_CPU_PPC64 1
#  define ARCH_CPU_64_BITS 1
#  define ARCH_CPU_BIG_ENDIAN 1
#elif defined(__PPC64__)
#  define ARCH_CPU_PPC64_FAMILY 1
#  define ARCH_CPU_PPC64 1
#  define ARCH_CPU_64_BITS 1
#  define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
#  define ARCH_CPU_ARM_FAMILY 1
#  define ARCH_CPU_ARMEL 1
#  define ARCH_CPU_32_BITS 1
#  define ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__aarch64__)
#  define ARCH_CPU_ARM_FAMILY 1
#  define ARCH_CPU_ARM64 1
#  define ARCH_CPU_64_BITS 1
#else
#  error Please add support for your architecture in internal_macros.h
#endif

/// Concatenate symbols s1 and s2
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
#  define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
#  define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

#endif // MANDELBROTPP_INTERNAL_MACROS_H