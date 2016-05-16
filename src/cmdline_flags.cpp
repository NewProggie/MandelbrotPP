// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "cmdline_flags.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <tuple>

#include "check.h"
#include "string_util.h"

namespace mpp {

// Parses a string as a cmdline flag. The string should have the format
// '--flag=value'. If |def_optional| is true, the "=value" part can be
// neglected.
// Returns the value of the flag of nullptr, if the parsing failed
const char *ParseFlagValue(const char *str,
                           const char *flag,
                           bool def_optional) {
    // |str| and |flag| must not be nullptr.
    if (str == nullptr || flag == nullptr)
        return nullptr;

    // The flag must start with '--'
    auto flag_str = std::string("--") + std::string(flag);

    auto flag_len = flag_str.length();
    if (std::strncmp(str, flag_str.c_str(), flag_len) != 0)
        return nullptr;

    // Skip the flag name
    auto flag_end = str + flag_len;

    // If |def_optional| is set, it's ok to omit the '=value' part
    if (def_optional && (flag_end[0] == '\0'))
        return flag_end;

    // If |def_optional| is true and there are more chars after the flag name,
    // or if |def_optional| is false, there must be a '=' after the flag name
    if (flag_end[0] != '=')
        return nullptr;

    // Returns the string after '='
    return flag_end + 1;
}

// Parses |str| for a double. If successful, it writes the result to |value|.
bool ParseDouble(const std::string &src, const char *str, double *value) {
    // Parses |str| as a double
    char *end = nullptr;
    auto double_val = strtod(str, &end);

    // Did we consume all characters in the string?
    if (*end != '\0') {
        // No, we encountered an invalud character
        std::cerr << src << " is expected to be a double, but actually has the "
                  << "value: " << str << "\n";
        return false;
    }

    *value = double_val;
    return true;
}

bool ParseInt32(const std::string &src, const char *str, int32_t *value) {
    // Parses |str| as decimal integer
    char *end = nullptr;
    const auto long_val = std::strtol(str, &end, 10);

    // Did we consume all characters in the string?
    if (*end != '\0') {
        // No, we encountered an invalid character
        std::cerr << src << " is expected to be a 32-bit integer, but actually "
                  << "has the value: " << str << "\n";
        return false;
    }

    // Check, if the parsed value is in the range of an int32
    auto result = static_cast<int32_t>(long_val);
    if (std::numeric_limits<long>::min() == long_val ||
        std::numeric_limits<long>::max() == long_val ||
        // Since strtol() returns LONG_MIN or LONG_MAX if the input overflows,
        // we check, if the parsed value overflows as an int32
        result != long_val) {
        std::cerr << src << " is expected to be a 32-bit integer, but actually "
                  << "has the value: " << str << " which overflows"
                  << "\n";
        return false;
    }

    *value = result;
    return true;
}

bool ParseBoolFlag(const char *str, const char *flag, bool *value) {
    // Gets the value of the flag as a string
    auto value_str = ParseFlagValue(str, flag, true);

    // Abort, if the parsing failed
    if (value_str == nullptr)
        return false;

    // Convert the string value to a boolean
    *value = !(*value_str == '0' || *value_str == 'f' || *value_str == 'F');
    return true;
}

bool ParseInt32Flag(const char *str, const char *flag, int32_t *value) {
    // Gets the value of the flag as a string
    auto value_str = ParseFlagValue(str, flag, false);

    // Abort, if the parsing failed
    if (value_str == nullptr)
        return false;

    // Set |value| to the value of the flag
    return ParseInt32(
        std::string("The value of flag --") + flag, value_str, value);
}

bool ParseIntPairFlag(const char *str, const char *flag, int_pair *value) {
    auto value_str = ParseFlagValue(str, flag, false);

    if (value_str == nullptr)
        return false;

    // split value_str from '1200x800' to '1200' and '800'
    auto values = SplitString(value_str, 'x');
    CHECK_EQ(values.size(), 2);

    return (ParseInt32(std::string("The value of flag lhs --") + flag,
                       values[0].c_str(), &std::get<0>(*value)) &&
            ParseInt32(std::string("The value of flag rhs --") + flag,
                       values[1].c_str(), &std::get<1>(*value)));
}

bool ParseDoubleFlag(const char *str, const char *flag, double *value) {
    // Gets the value of the flag as a string
    auto value_str = ParseFlagValue(str, flag, false);

    // Abort, if the parsing failed
    if (value_str == nullptr)
        return false;

    // Set |value| to the value of the flag
    return ParseDouble(
        std::string("The value of flag --") + flag, value_str, value);
}

bool ParseStringFlag(const char *str, const char *flag, std::string *value) {
    // Get the value of the flag as a string
    auto value_str = ParseFlagValue(str, flag, false);

    // Abort, if the parsing failed
    if (value_str == nullptr)
        return false;

    *value = value_str;
    return true;
}

bool IsFlag(const char *str, const char *flag) {
    return (ParseFlagValue(str, flag, true) != nullptr);
}
}
