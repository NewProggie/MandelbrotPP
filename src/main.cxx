// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include <iostream>
#include "mandelbrotpp/mandelbrotpp.h"

#include "cmdline_flags.h"

DEFINE_int32(mandelbrotpp_iterations, 30,
             "The maximum number of iterations.");

DEFINE_int_pair(mandelbrotpp_image_dimensions, 640, 480,
                "The default image dimension for the Mandelbrot image");

DEFINE_string(mandelbrotpp_image_type, "ppm",
              "The image format to use for creating a Mandelbrot image. Valid "
              "values are 'ppm' or 'pgm'");

namespace mpp {
namespace internal {

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
        FLAGS_mandelbrotpp_image_type != "bmp" &&
        FLAGS_mandelbrotpp_image_type != "pgm") {
        PrintUsageAndExit();
    }
}

}

std::unique_ptr<MandelbrotPPExporter> GetDefaultExporter() {
    typedef std::unique_ptr<MandelbrotPPExporter> PtrType;
    if (FLAGS_mandelbrotpp_image_type == "ppm") {
        return PtrType(new PPMExporter);
    } else if (FLAGS_mandelbrotpp_image_type == "pgm") {
        return PtrType(new PGMExporter);
    } else {
        std::cerr << "Unexpected image type: '" << FLAGS_mandelbrotpp_image_type
            << "'\n";
        std::exit(1);
    }
}

}

int main(int argc, char *argv[]) {
    using namespace mpp;
    internal::ParseCmdLineFlags(argc, argv);
    int32_t xdim, ydim;
    std::tie(xdim, ydim) = FLAGS_mandelbrotpp_image_dimensions;
    MandelbrotPPExporter::Image image(xdim, ydim);
    for (auto y = 0; y < ydim; y++) {
        for (auto x = 0; x < xdim; x++) {
            MandelbrotPPPixel<float> m(Coord(x, y), xdim, ydim,
                                       FLAGS_mandelbrotpp_iterations);
            image.buffer[Map2DTo1D<int32_t>(x, y, xdim)] = m.color();
        }
    }
    GetDefaultExporter()->WriteImage(image);

    return 0;
}
