// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a personal license file that can be
// found in the LICENSE file in the top directory.

#include "mandelbrotpp/exporter.h"

#include <png.h>
#include <cstdio>
#include <cstdlib>
#include "check.h"

namespace mpp {

void PNGExporter::WriteImage(const MandelbrotPPExporter::Image &image) {

    std::FILE *fp = std::fopen("out.png", "wb");
    CHECK(fp != nullptr) << "Could not open file to write png";

    // initialize write structure
    auto png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr,
                                       nullptr);
    CHECK(png != nullptr) << "Could not create png write struct";

    // initialize info structure
    auto info = png_create_info_struct(png);
    CHECK(info != nullptr) << "Could not create png info struct";

    // setup error handling
    if (setjmp(png_jmpbuf(png))) {
        std::fclose(fp);
        png_destroy_write_struct(&png, &info);
        CHECK(false) << "Could not configure png error handling";
    }

    png_init_io(png, fp);

    // write header (32 bit color depth)
    png_set_IHDR(png, info, image.width, image.height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    // allocate memory for one row (3 bytes per pixel - RGB)
    auto row =
        static_cast<png_bytep>(std::malloc(3 * image.width * sizeof(png_byte)));

    for (auto y = 0; y < image.height; y++) {
        for (auto x = 0; x < image.width; x++) {
            row[x * 3] = image.buffer[y * image.width + x];
            row[x * 3 + 1] = image.buffer[y * image.width + x];
            row[x * 3 + 2] = image.buffer[y * image.width + x];
        }
        png_write_row(png, row);
    }
    png_write_end(png, nullptr);
    png_free_data(png, info, PNG_FREE_ALL, -1);
    png_destroy_write_struct(&png, (png_infopp) nullptr);
    std::free(row);

    std::fclose(fp);
}
}
