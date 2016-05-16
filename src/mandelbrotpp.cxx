// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "mandelbrotpp/mandelbrotpp.h"

#include <complex>
#include <cstdint>
#include <cstdio>
#include <iostream>

#include "check.h"
#include "cmdline_flags.h"
#include "logging.h"

DEFINE_int32(mandelbrotpp_iterations, 30,
        "The maximum number of iterations.");

DEFINE_int_pair(mandelbrotpp_image_dimensions, 1200, 800,
         "The default image dimension for the Mandelbrot image");

DEFINE_string(mandelbrotpp_image_type, "ppm",
         "The image format to use for creating a Mandelbrot image. Valid "
         "values are 'ppm', 'bpm', or 'pgm'");

namespace mpp {
namespace internal {

// Pixel coordinates for a Mandelbrot image
struct Coord {
  unsigned x;
  unsigned y;
};

// Image dimensions
struct Dim {
  unsigned width;
  unsigned height;
};

template <typename T>
class MandelbrotPPPixel {
  public:
    MandelbrotPPPixel(MandelbrotPPExporter *exporter, Coord coord, Dim dim,
                      int32_t max_iterations)
        : exporter_(exporter), max_iterations_(max_iterations), iter_(0),
          c_(coord.x, coord.y) {
        c_ *= static_cast<T>(2.4) / static_cast<T>(dim.height);
        c_ -= std::complex<T>(
            static_cast<T>(1.2) * dim.width / dim.height + 0.5, 1.2);
    }

    int32_t iterations() const {
        return iter_;
    }

  private:
    void iterate() {
        auto z = c_;
        for (; iter_ < max_iterations_ && std::norm(z) <= static_cast<T>(4);
             ++iter_) {
            z = z * z + c_;
        }
    }

    MandelbrotPPExporter *exporter_;
    const int32_t max_iterations_;
    int32_t iter_;
    std::complex<T> c_;
};

void PrintUsageAndExit() {
    fprintf(stdout,
            "mandelbrotpp"
                " [--mandelbrotpp_iterations=<num_iterations>]\n"
                "             [--mandelbrotpp_image_dimensions=<width>x<height>]\n"
                "             [--mandelbrotpp_image_type=<ppm|bpm|pgm>]\n");
    std::exit(0);
}

void ParseCmdLineFlags(int argc, char *argv[]) {
    using namespace mpp;
    for (int idx = 1; idx < argc; ++idx) {
        if (ParseInt32Flag(argv[idx], "mandelbrotpp_iterations",
                           &FLAGS_mandelbrotpp_iterations) ||
            ParseIntPairFlag(argv[idx], "mandelbrotpp_image_dimensions",
                             &FLAGS_mandelbrotpp_image_dimensions) ||
            ParseStringFlag(argv[idx], "mandelbrotpp_image_type",
                            &FLAGS_mandelbrotpp_image_type)) {
            for (int jdx = idx; jdx != argc; jdx++) {
                argv[jdx] = argv[jdx + 1];
            }
            --(argc);
            --idx;
        } else if (IsFlag(argv[idx], "help")) {
            PrintUsageAndExit();
        }
    }

    if (FLAGS_mandelbrotpp_image_type != "ppm" &&
        FLAGS_mandelbrotpp_image_type != "bpm" &&
        FLAGS_mandelbrotpp_image_type != "pgm") {
        PrintUsageAndExit();
    }
}
}

}

int main(int argc, char *argv[]) {

    mpp::internal::ParseCmdLineFlags(argc, argv);
    std::cout << "Hello, Mandelbrot" << std::endl;

    return 0;
}
