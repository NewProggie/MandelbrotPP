// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_STRING_UTIL_H
#define MANDELBROTPP_STRING_UTIL_H

#include <string>
#include <vector>

using StringList = std::vector<std::string>;

namespace mpp {

StringList SplitString(const std::string& str, char delim);
}

#endif  // MANDELBROTPP_STRING_UTIL_H
