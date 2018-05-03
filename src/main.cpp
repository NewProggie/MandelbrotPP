// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include <iostream>
#include "cmdline_flags.h"
#include "mandelbrotpp/exporter.h"

DEFINE_int32(mandelbrotpp_iterations, 30,
             "The maximum number of iterations.");

DEFINE_int_pair(mandelbrotpp_image_dimensions, 640, 480,
                "The default image dimension for the Mandelbrot image");

DEFINE_string(mandelbrotpp_image_type, "ppm",
              "The image format to use for creating a Mandelbrot image. Valid "
              "values are 'png', 'ppm' or 'pgm'");

namespace mpp {
namespace internal {

void PrintUsageAndExit() {
    fprintf(stdout,
            "mandelbrotpp"
            " [--mandelbrotpp_iterations=<num_iterations>]\n"
            "             [--mandelbrotpp_image_dimensions=<width>x<height>]\n"
            "             [--mandelbrotpp_image_type=<png|ppm|pgm>]\n");
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

    if (FLAGS_mandelbrotpp_image_type != "png" &&
        FLAGS_mandelbrotpp_image_type != "ppm" &&
        FLAGS_mandelbrotpp_image_type != "pgm") {
        PrintUsageAndExit();
    }
}
} // namespace internal

std::unique_ptr<MandelbrotPPExporter> GetDefaultExporter() {
    typedef std::unique_ptr<MandelbrotPPExporter> PtrType;
    #if HAS_PNG
    if (FLAGS_mandelbrotpp_image_type == "png") return PtrType(new PNGExporter);
    #endif

    if (FLAGS_mandelbrotpp_image_type == "ppm") return PtrType(new PPMExporter);
    if (FLAGS_mandelbrotpp_image_type == "pgm") return PtrType(new PGMExporter);

    std::cerr << "Unexpected image type: '" << FLAGS_mandelbrotpp_image_type
              << "'\n";
    std::exit(1);
}
} // namespace mpp

void mandel_basic(const mpp::MandelbrotPPExporter::Image& image,
                  const mpp::MandelbrotPPExporter::FractalSpec& spec);

int main(int argc, char *argv[]) {
    using namespace mpp;
    internal::ParseCmdLineFlags(argc, argv);
    int32_t xdim, ydim;
    std::tie(xdim, ydim) = FLAGS_mandelbrotpp_image_dimensions;
    MandelbrotPPExporter::Image img(xdim, ydim);
    MandelbrotPPExporter::FractalSpec spec = {
        .xlim = {-2.0f, 2.0f},
        .ylim = {-2.0f, 2.0f},
        .iterations = FLAGS_mandelbrotpp_iterations
    };
    mandel_basic(img, spec);
    GetDefaultExporter()->WriteImage(img);

    return 0;
}