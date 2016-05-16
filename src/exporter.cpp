// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in 
// the LICENSE file.

#include "mandelbrotpp/exporter.h"

namespace mpp {

int32_t MandelbrotPPExporter::GetMax(const Image& image) const {
    int32_t max = 0;
    for (int idx = 0; idx < image.width * image.height; idx++) {
        if (image.buffer[idx] > max) {
            max = image.buffer[idx];
        }
    }
    return max;
}

}
