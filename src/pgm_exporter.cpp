// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in 
// the LICENSE file.

#include "mandelbrotpp/exporter.h"

#include <fstream>

namespace mpp {

void PGMExporter::WriteImage(const Image &image) {

    std::ofstream out_file;
    out_file.open("out.pgm");
    out_file << "P2\n";
    out_file << image.width << " " << image.height << "\n";
    out_file << GetMax(image);
    for (int idx = 0; idx < image.width * image.height; idx++) {
        if (idx % image.width == 0) {
            out_file << "\n";
        }
        out_file << image.buffer[idx] << " ";
    }

    out_file.close();
}

}
