// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_COLORPRINT_H
#define MANDELBROTPP_COLORPRINT_H

namespace mpp {
enum LogColor {
    COLOR_DEFAULT,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE
};

void ColorPrint(LogColor color, const char *format, ...);
}

#endif // MANDELBROTPP_COLORPRINT_H
