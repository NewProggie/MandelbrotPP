// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "mandelbrotpp/exporter.h"
#include <cmath>
#include <immintrin.h>

void mandel_avx(const mpp::MandelbrotPPExporter::Image &image,
                const mpp::MandelbrotPPExporter::FractalSpec &spec) {

  __m256 xmin = _mm256_set1_ps(spec.xlim[0]);
  __m256 ymin = _mm256_set1_ps(spec.ylim[0]);
  __m256 xscale = _mm256_set1_ps((spec.xlim[1] - spec.xlim[0]) / image.width);
  __m256 yscale = _mm256_set1_ps((spec.ylim[1] - spec.ylim[0]) / image.height);
  __m256 thresh = _mm256_set1_ps(4);
  __m256 one = _mm256_set1_ps(1);
  __m256 iter_scale = _mm256_set1_ps(1.0f / spec.iterations);
  __m256 depth_scale = _mm256_set1_ps(255);

#pragma omp parallel for schedule(dynamic, 1)
  for (int y = 0; y < image.height; y++) {
    // Moving to 8-lane AVX, thus incrementing by 8
    for (int x = 0; x < image.width; x += 8) {
      __m256 mx =
          _mm256_set_ps(static_cast<float>(x + 7), static_cast<float>(x + 6),
                        static_cast<float>(x + 5), static_cast<float>(x + 4),
                        static_cast<float>(x + 3), static_cast<float>(x + 2),
                        static_cast<float>(x + 1), static_cast<float>(x + 0));
      __m256 my = _mm256_set1_ps(static_cast<float>(y));
      __m256 cr = _mm256_add_ps(_mm256_mul_ps(mx, xscale), xmin);
      __m256 ci = _mm256_add_ps(_mm256_mul_ps(my, yscale), ymin);
      __m256 zr = cr;
      __m256 zi = ci;

      int k = 1;
      __m256 mk = _mm256_set1_ps(static_cast<float>(k));
      while (++k < spec.iterations) {
        __m256 zr2 = _mm256_mul_ps(zr, zr);
        __m256 zi2 = _mm256_mul_ps(zi, zi);
        __m256 zrzi = _mm256_mul_ps(zr, zi);
        zr = _mm256_add_ps(_mm256_sub_ps(zr2, zi2), cr);
        zi = _mm256_add_ps(_mm256_add_ps(zrzi, zrzi), ci);

        zr2 = _mm256_mul_ps(zr, zr);
        zi2 = _mm256_mul_ps(zi, zi);
        __m256 mag2 = _mm256_add_ps(zr2, zi2);
        __m256 mask = _mm256_cmp_ps(mag2, thresh, _CMP_LT_OS);
        mk = _mm256_add_ps(_mm256_and_ps(mask, one), mk);

        if (_mm256_testz_ps(mask, _mm256_set1_ps(-1))) {
          break;
        }
      }

      mk = _mm256_mul_ps(mk, iter_scale);
      mk = _mm256_sqrt_ps(mk);
      mk = _mm256_mul_ps(mk, depth_scale);

      __m256i pixels = _mm256_cvtps_epi32(mk);
      unsigned char *src = reinterpret_cast<unsigned char *>(&pixels);
      for (int i = 0; i < 8; i++) {
        image.buffer[mpp::Map2DTo1D<int32_t>(x + i, y, image.width)] =
            src[i * 4];
      }
    }
  }
}
