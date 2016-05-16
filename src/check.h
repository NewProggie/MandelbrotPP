// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_CHECK_H
#define MANDELBROTPP_CHECK_H

#include <cstdlib>
#include <ostream>

#include "internal_macros.h"
#include "logging.h"

namespace mpp {
namespace internal {

// CheckHandler is the class constructed by failing CHECK macros. CheckHandler
// will log information about the failures and abort when it is destructed.
class CheckHandler {
  public:
    CheckHandler(const char *check, const char *file, const char *func,
                 int line) : log_(GetErrorLoggingInstance()) {
        log_ << file << ":" << line << ": " << func << ": Check `" << check
             << "' failed. ";
    }

    std::ostream &GetLog() {
        return log_;
    }

    ~CheckHandler() {
        log_ << std::endl;
        std::abort();
    }

    CheckHandler &operator=(const CheckHandler &) = delete;
    CheckHandler(const CheckHandler &) = delete;
    CheckHandler() = delete;

  private:
    std::ostream &log_;
};

}
}

// The CHECK macro returns a std::ostream object that can have extra information
// written to it.
#ifndef NDEBUG
#define CHECK(b)                                                         \
    (b ? ::mpp::internal::GetNullLogInstance()                           \
       : ::mpp::internal::CheckHandler(#b, __FILE__, __func__, __LINE__) \
             .GetLog())
#else
#define CHECK(b) ::mpp::internal::GetNullLogInstance()
#endif

#define CHECK_EQ(a, b) CHECK((a) == (b))
#define CHECK_NE(a, b) CHECK((a) != (b))
#define CHECK_GE(a, b) CHECK((a) >= (b))
#define CHECK_LE(a, b) CHECK((a) <= (b))
#define CHECK_GT(a, b) CHECK((a) > (b))
#define CHECK_LT(a, b) CHECK((a) < (b))

#endif // MANDELBROTPP_CHECK_H
