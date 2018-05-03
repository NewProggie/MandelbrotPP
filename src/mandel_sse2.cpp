// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "mandelbrotpp/exporter.h"
#include <cmath>
#include <xmmintrin.h>

void mandel_sse2(const mpp::MandelbrotPPExporter::Image &image,
                 const mpp::MandelbrotPPExporter::FractalSpec &spec) {

  __m128 xmin = _mm_set_ps1(spec.xlim[0]);
  __m128 ymin = _mm_set_ps1(spec.ylim[0]);
  __m128 xscale = _mm_set_ps1((spec.xlim[1] - spec.xlim[0]) / image.width);
  __m128 yscale = _mm_set_ps1((spec.ylim[1] - spec.ylim[0]) / image.height);
  __m128 thresh = _mm_set_ps1(4);
  __m128 one = _mm_set_ps1(1);
  // --> float iter_scale = 1.0f / spec.iterations;
  __m128 iter_scale = _mm_set_ps1(1.0f / spec.iterations);
  __m128 depth_scale = _mm_set_ps1(255);

  #pragma omp parallel for schedule(dynamic, 1)
  for (int y = 0; y < image.height; y++) {
    // Moving to 4-lane SIMD, thus incrementing by 4
    // --> for (int x = 0; x < image.width; x++) {
    for (int x = 0; x < image.width; x += 4) {
      __m128 mx = _mm_set_ps(x + 3, x + 2, x + 1, x + 0);
      __m128 my = _mm_set_ps1(y);
      // --> float cr = x * xdiff / image.width + spec.xlim[0];
      __m128 cr = _mm_add_ps(_mm_mul_ps(mx, xscale), xmin);
      // --> float ci = y * ydiff / image.height + spec.ylim[0];
      __m128 ci = _mm_add_ps(_mm_mul_ps(my, yscale), ymin);
      // --> float zr = cr;
      __m128 zr = cr;
      // --> float zi = ci;
      __m128 zi = ci;

      int k = 0;
      // --> float mk = 0.0f;
      __m128 mk = _mm_set_ps1(k);
      while (++k < spec.iterations) {
        // --> float zr1 = zr * zr - zi * zi + cr;
        // --> float zi1 = zr * zi + zr * zi + ci;
        // --> zr = zr1;
        // --> zi = zi1;
        __m128 zr2 = _mm_mul_ps(zr, zr);
        __m128 zi2 = _mm_mul_ps(zi, zi);
        __m128 zrzi = _mm_mul_ps(zr, zi);
        zr = _mm_add_ps(_mm_sub_ps(zr2, zi2), cr);
        zi = _mm_add_ps(_mm_add_ps(zrzi, zrzi), ci);

        // --> mk += 1.0f;
        zr2 = _mm_mul_ps(zr, zr);
        zi2 = _mm_mul_ps(zi, zi);
        __m128 mag2 = _mm_add_ps(zr2, zi2);
        __m128 mask = _mm_cmplt_ps(mag2, thresh);
        mk = _mm_add_ps(_mm_and_ps(mask, one), mk);

        // --> if (zr * zr + zi * zi >= 4.0f) {
        // -->   break;
        // --> }
        if (_mm_movemask_ps(mask) == 0) {
          break;
        }
      }
      // --> mk *= iter_scale;
      mk = _mm_mul_ps(mk, iter_scale);
      // --> mk = std::sqrt(mk);
      mk = _mm_sqrt_ps(mk);
      // --> mk *= 256; // monochrome depth (assuming RGBA image)
      mk = _mm_mul_ps(mk, depth_scale);

      // --> image.buffer[mpp::Map2DTo1D<int32_t>(x, y, image.width)] = mk;
      __m128i pixels = _mm_cvtps_epi32(mk);
      unsigned char *src = (unsigned char *)&pixels;
      for (int i = 0; i < 4; i++) {
        image.buffer[mpp::Map2DTo1D<int32_t>(x + i, y, image.width)] =
            src[i * 4];
      }
    }
  }
}

void (*computeMandelbrot)(const mpp::MandelbrotPPExporter::Image &image,
                          const mpp::MandelbrotPPExporter::FractalSpec &spec) =
    &mandel_sse2;