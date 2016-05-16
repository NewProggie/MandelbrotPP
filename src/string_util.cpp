// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "string_util.h"

#include <sstream>

namespace mpp {

std::string Trim(const std::string& str) {
    auto first = str.find_first_not_of(' ');
    auto last = str.find_last_not_of(' ');

    return str.substr(first, (last - first + 1));
}

StringList SplitString(const std::string& str, char delim) {
    StringList tokens;
    std::stringstream ss(str);

    std::string token;
    while (std::getline(ss, token, delim)) {
        if (!token.empty()) {
            tokens.push_back(Trim(token));
        }
    }

    return tokens;
}
}
