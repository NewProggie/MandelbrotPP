// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef MANDELBROTPP_EXPORTER_H
#define MANDELBROTPP_EXPORTER_H

#include <cstdint>
#include <memory>

namespace mpp {

template <typename T>
constexpr T Map2DTo1D(int32_t x, int32_t y, int32_t width) {
    return width * y + x;
}

// Interface for custom mandelbrot image exporter
// By default, mandelbrot images are exported as ppm images. However,
// one might like to control the output format and destination of the
// generated image.
// This can be done by passing a custom exporter object. The exporter
// object must implement the following interface
class MandelbrotPPExporter {
  public:
    struct FractalSpec {
        float xlim[2];
        float ylim[2];
        int iterations;
    };

    struct Image {
        explicit Image(int32_t width, int32_t height)
            : width(width), height(height) {
            buffer = std::make_unique<int32_t[]>(width * height);
      }

      int32_t width, height;
      // We don't use a std::vector here since we don't want to pay for the
      // runtime cost of value-initializing the vector
      std::unique_ptr<int32_t[]> buffer;
    };

    // Creates image file using the data buffer from the given |image|.
    virtual void WriteImage(const Image &image) = 0;

    virtual int32_t GetMax(const Image& image) const;

    virtual ~MandelbrotPPExporter() = default;
};

// Simple reporter that outputs the mandelbrot image in the png format.
// This is the default export format used.
#if HAS_PNG
class PNGExporter : public MandelbrotPPExporter {
  public:
    void WriteImage(const Image &image) override;
};
#endif

class PPMExporter : public MandelbrotPPExporter {
  public:
    void WriteImage(const Image &image) override;
};

class PGMExporter : public MandelbrotPPExporter {
  public:
    void WriteImage(const Image &image) override;
};
}

#endif // MANDELBROTPP_EXPORTER_H
