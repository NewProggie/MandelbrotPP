// Copyright (c) 2016 Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be
// found in the LICENSE file.

#ifndef MANDELBROTPP_EXPORTER_H
#define MANDELBROTPP_EXPORTER_H

namespace mpp {

// Interface for custom mandelbrot image exporter
// By default, mandelbrot images are exported as ppm images. However,
// one might like to control the output format and destination of the
// generated image.
// This can be done by passing a custom exporter object. The exporter
// object must implement the following interface
class MandelbrotPPExporter {};

// Simple reporter that outputs the mandelbrot image in the ppm format.
// This is the default export format used.
class PPMExporter : public MandelbrotPPExporter {};

class BPMExporter : public MandelbrotPPExporter {};
}

#endif // MANDELBROTPP_EXPORTER_H
