// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "benchmark.h"
#include "mandelbrotpp/exporter.h"
#include "timing.h"
#include <iostream>
#include <memory>

class MandelbrotFixture : public mpp::Benchmark {
protected:
  virtual void SetUp() override {
    mpp::Benchmark::SetUp();

    image = std::make_unique<mpp::MandelbrotPPExporter::Image>(640, 480);
    spec = mpp::MandelbrotPPExporter::FractalSpec{-2.0f, 2.0f, -2.0f, 2.0f, 30};
  }

  std::unique_ptr<mpp::MandelbrotPPExporter::Image> image;
  mpp::MandelbrotPPExporter::FractalSpec spec;
};

void mandel_basic(const mpp::MandelbrotPPExporter::Image &image,
                  const mpp::MandelbrotPPExporter::FractalSpec &spec);

BENCHMARK_F(MandelbrotFixture, BasicMandelbrot) {
  while (context.Running()) {
    mandel_basic(*image, spec);
  }
}

#if SSE2_FOUND
void mandel_sse2(const mpp::MandelbrotPPExporter::Image &image,
                 const mpp::MandelbrotPPExporter::FractalSpec &spec);

BENCHMARK_F(MandelbrotFixture, SSE2Mandelbrot) {
  while (context.Running()) {
    mandel_sse2(*image, spec);
  }
}
#endif

void mandel_avx(const mpp::MandelbrotPPExporter::Image &image,
                const mpp::MandelbrotPPExporter::FractalSpec &spec);

BENCHMARK_F(MandelbrotFixture, AVXMandelbrot) {
  while (context.Running()) {
    mandel_avx(*image, spec);
  }
}                

BENCHMARK_MAIN()
