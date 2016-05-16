// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_MANDELBROTPP_API_H
#define MANDELBROTPP_MANDELBROTPP_API_H

#include <cstdint>
#include <complex>

namespace mpp {
class MandelbrotPPExporter;

// Pixel coordinates for a Mandelbrot image
struct Coord {
  explicit Coord(int32_t x, int32_t y) : x(x), y(y) {}
  int32_t x;
  int32_t y;
};

template <typename T>
constexpr T Map2DTo1D(int32_t x, int32_t y, int32_t width) {
    return width * y + x;
}

template <typename T>
class MandelbrotPPPixel {
  public:
    MandelbrotPPPixel(Coord coord, int32_t xdim,
                      int32_t ydim, int32_t max_iterations)
        : max_iterations_(max_iterations), iter_(0),
          c_(coord.x, coord.y) {
        c_ *= static_cast<T>(2.0) / static_cast<T>(ydim);
        c_ -= std::complex<T>(static_cast<T>(1.0) * xdim / ydim + 0.5, 1.0);
    }

    int32_t iterations() const {
        return iter_;
    }

    uint32_t color() {
        auto z = c_;
        while (std::abs(z) < static_cast<T>(2) && iter_ < max_iterations_) {
            z = z*z + c_;
            iter_++;
        }
        return iter_ == max_iterations_ ? 0 : iter_;
    }

  private:
    const int32_t max_iterations_;
    int32_t iter_;
    std::complex<T> c_;
};

}

#endif  // MANDELBROTPP_MANDELBROTPP_API_H
