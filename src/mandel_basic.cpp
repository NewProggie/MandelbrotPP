// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "mandelbrotpp/exporter.h"
#include <cmath>

void mandel_basic(const mpp::MandelbrotPPExporter::Image &image,
                  const mpp::MandelbrotPPExporter::FractalSpec &spec) {

  float xdiff = spec.xlim[1] - spec.xlim[0];
  float ydiff = spec.ylim[1] - spec.ylim[0];
  float iter_scale = 1.0f / spec.iterations;

  for (int y = 0; y < image.height; y++) {
    for (int x = 0; x < image.width; x++) {
      float cr = x * xdiff / image.width + spec.xlim[0];
      float ci = y * ydiff / image.height + spec.ylim[0];
      float zr = cr;
      float zi = ci;
      int k = 0;
      float mk = 0.0f;
      while (++k < spec.iterations) {
        float zr1 = zr * zr - zi * zi + cr;
        float zi1 = zr * zi + zr * zi + ci;
        zr = zr1;
        zi = zi1;
        mk += 1.0f;
        if (zr * zr + zi * zi >= 4.0f) {
          break;
        }
      }
      mk *= iter_scale;
      mk = std::sqrt(mk);
      mk *= 255; // monochrome depth (assuming RGBA image)
      image.buffer[mpp::Map2DTo1D<int32_t>(x, y, image.width)] = static_cast<int32_t>(mk);
    }
  }
}
