// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_LOGGING_H
#define MANDELBROTPP_LOGGING_H

#include <ostream>

namespace mpp {
namespace internal {
int GetLoggingLevel();
void SetLoggingLevel(int level);

std::ostream& GetNullLoggingInstance();
std::ostream& GetErrorLoggingInstance();

inline std::ostream& GetLoggingInstanceForLevel(int level) {
    if (level <= GetLoggingLevel()) {
        return GetErrorLoggingInstance();
    }
    return GetNullLoggingInstance();
}

}
}

#define LOG(x)                                                          \
    (::mpp::internal::GetLoggingInstanceForLevel(x) << "-- LOG(" << x \
                                                      << "): ")

#endif  // MANDELBROTPP_LOGGING_H
