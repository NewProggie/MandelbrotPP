// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "logging.h"
#include "mpp_lib_export.h"
#include <iostream>

namespace mpp {
namespace internal {

int& LoggingLevelImpl() {
    static int level = 0;
    return level;
}

void SetLoggingLevel(int value) {
    LoggingLevelImpl() = value;
}

int GetLoggingLevel() {
    return LoggingLevelImpl();
}

class MPP_LIB_EXPORT NullLoggingBuffer : public std::streambuf {
  public:
    int overflow(int c) {
        return c;
    }
};

std::ostream& GetNullLoggingInstance() {
    static NullLoggingBuffer log_buffer;
    static std::ostream null_logger(&log_buffer);
    return null_logger;
}

std::ostream& GetErrorLoggingInstance() {
    return std::clog;
}

}
}
