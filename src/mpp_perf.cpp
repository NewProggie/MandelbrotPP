// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "mandelbrotpp/exporter.h"
#include "timing.h"
#include <iostream>

extern void (*computeMandelbrot)(
    const mpp::MandelbrotPPExporter::Image &image,
    const mpp::MandelbrotPPExporter::FractalSpec &spec);

int main(int argc, char *argv[]) {

  mpp::MandelbrotPPExporter::Image img(640, 480);
  mpp::MandelbrotPPExporter::FractalSpec spec = {
      .xlim = {-2.0f, 2.0f}, .ylim = {-2.0f, 2.0f}, .iterations = 30};

  Timer t;
  (*computeMandelbrot)(img, spec);
  double elapsed = t.elapsed();
  std::cout << "elapsed: " << elapsed << std::endl;
  mpp::PPMExporter exporter;
  exporter.WriteImage(img);

  return 0;
}