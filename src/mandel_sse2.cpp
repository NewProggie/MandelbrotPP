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
  __m128 iter_scale = _mm_set_ps1(1.0f / spec.iterations);
  __m128 depth_scale = _mm_set_ps1(255);

#pragma omp parallel for schedule(dynamic, 1)
  for (int y = 0; y < image.height; y++) {
    // Moving to 4-lane SIMD, thus incrementing by 4
    for (int x = 0; x < image.width; x += 4) {
      __m128 mx =
          _mm_set_ps(static_cast<float>(x + 3), static_cast<float>(x + 2),
                     static_cast<float>(x + 1), static_cast<float>(x + 0));
      __m128 my = _mm_set_ps1(static_cast<float>(y));
      __m128 cr = _mm_add_ps(_mm_mul_ps(mx, xscale), xmin);
      __m128 ci = _mm_add_ps(_mm_mul_ps(my, yscale), ymin);
      __m128 zr = cr;
      __m128 zi = ci;

      int k = 1;
      __m128 mk = _mm_set_ps1(static_cast<float>(k));
      while (++k < spec.iterations) {
        __m128 zr2 = _mm_mul_ps(zr, zr);
        __m128 zi2 = _mm_mul_ps(zi, zi);
        __m128 zrzi = _mm_mul_ps(zr, zi);
        zr = _mm_add_ps(_mm_sub_ps(zr2, zi2), cr);
        zi = _mm_add_ps(_mm_add_ps(zrzi, zrzi), ci);

        zr2 = _mm_mul_ps(zr, zr);
        zi2 = _mm_mul_ps(zi, zi);
        __m128 mag2 = _mm_add_ps(zr2, zi2);
        __m128 mask = _mm_cmplt_ps(mag2, thresh);
        mk = _mm_add_ps(_mm_and_ps(mask, one), mk);

        if (_mm_movemask_ps(mask) == 0) {
          break;
        }
      }

      mk = _mm_mul_ps(mk, iter_scale);
      mk = _mm_sqrt_ps(mk);
      mk = _mm_mul_ps(mk, depth_scale);

      __m128i pixels = _mm_cvtps_epi32(mk);
      unsigned char *src = reinterpret_cast<unsigned char *>(&pixels);
      for (int i = 0; i < 4; i++) {
        image.buffer[mpp::Map2DTo1D<int32_t>(x + i, y, image.width)] =
            src[i * 4];
      }
    }
  }
}
