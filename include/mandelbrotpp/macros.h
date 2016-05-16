// Copyright (c) 2016 Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef MANDELBROTPP_MACROS_H
#define MANDELBROTPP_MACROS_H

#include <memory>

#if __cplusplus < 201103L
#define CHAIR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);                   \
    TypeName& operator=(const TypeName&)
#else
#define CHAIR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;          \
    TypeName& operator=(const TypeName&) = delete
#endif

// std::make_unique for C++11 implementation taken from N3656 STL
namespace mpp {
#if __cplusplus < 201402L
template <class T>
struct _Unique_if {
  typedef std::unique_ptr<T> _Single_object;
};

template <class T>
struct _Unique_if<T[]> {
  typedef std::unique_ptr<T[]> _Unknown_bound;
};

template <class T, size_t N>
struct _Unique_if<T[N]> {
  typedef void _Known_bound;
};

template <class T, class... Args>
typename _Unique_if<T>::_Single_object make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
typename _Unique_if<T>::_Unknown_bound make_unique(size_t n) {
    typedef typename std::remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[n]());
}

template <class T, class... Args>
typename _Unique_if<T>::_Known_bound make_unique(Args&&...) = delete;
#else
using make_unique = std::make_unique;
#endif
}


// Verify an expression during compile-time
#undef COMPILE_ASSERT
#define COMPILE_ASSERT(expr, msg) static_assert(expr, #msg)

#endif // MANDELBROTPP_MACROS_H
