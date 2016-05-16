// Copyright (c) 2016 Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef MANDELBROTPP_MACROS_H
#define MANDELBROTPP_MACROS_H

#if __cplusplus < 201103L
#define CHAIR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);                   \
    TypeName& operator=(const TypeName&)
#else
#define CHAIR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;          \
    TypeName& operator=(const TypeName&) = delete
#endif

#endif // MANDELBROTPP_MACROS_H
