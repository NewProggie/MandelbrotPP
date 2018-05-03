// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_TIMING_H
#define MANDELBROTPP_TIMING_H

#include <chrono>
#include <ratio>

class Timer {
public:
  Timer() : beg_(clock_::now()) {}
  void reset() { beg_ = clock_::now(); }
  double elapsed() const {
    return std::chrono::duration_cast<ms_>(clock_::now() - beg_).count();
  }

private:
  using clock_ = std::chrono::high_resolution_clock;
  using second_ = std::chrono::duration<double, std::ratio<1>>;
  using ms_ = std::chrono::duration<double, std::milli>;
  std::chrono::time_point<clock_> beg_;
};

#endif // MANDELBROTPP_TIMING_H
