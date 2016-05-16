// Copyright (c) 2016, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_CMDLINE_FLAGS_H
#define MANDELBROTPP_CMDLINE_FLAGS_H

#include <cstdint>
#include <string>
#include <tuple>
#include <utility>

using int_pair = std::tuple<int32_t, int32_t>;

// Macro for referencing flags
#define FLAG(name) FLAGS_##name

// Macro for declaring flags
#define DECLARE_bool(name) extern bool FLAG(name)
#define DECLARE_int32(name) extern int32_t FLAG(name)
#define DECLARE_int64(name) extern int64_t FLAG(name)
#define DECLARE_float(name) extern float FLAG(name)
#define DECLARE_double(name) extern double FLAG(name)
#define DECLARE_string(name) extern std::string FLAG(name)

// Macro for defining flags
#define DEFINE_bool(name, def_val, doc) bool FLAG(name) = (def_val)
#define DEFINE_int32(name, def_val, doc) int32_t FLAG(name) = (def_val)
#define DEFINE_int64(name, def_val, doc) int64_t FLAG(name) = (def_val)
#define DEFINE_int_pair(name, def_val1, def_val2, doc) \
    int_pair FLAG(name) = (std::make_tuple(def_val1, def_val2))
#define DEFINE_float(name, def_val, doc) float FLAG(name) = (def_val)
#define DEFINE_double(name, def_val, doc) double FLAG(name) = (def_val)
#define DEFINE_string(name, def_val, doc) std::string FLAG(name) = (def_val)

namespace mpp {

// Parses |str| for a 32-bit signed integer and writes the result to |value|,
// if successful.
bool ParseInt32(const std::string &src, const char *str, int32_t *value);

// Parses a string for a bool flag in the form of '--flag=value' or '--flag'.
// In the former case, the value is taken as true, as long as it does not start
// with '0', 'f', or 'F'. In the latter case, the value is taken as true.
bool ParseBoolFlag(const char *str, const char *flag, bool *value);

// Parses a string for an int32 flag, in the form of '--flag=value'.
// On success it stores the value of the flag in |value|.
bool ParseInt32Flag(const char *str, const char *flag, int32_t *value);

// Parses a pair of int, in the form of '--flag=value1 value2'.
// On success it stores the value of the flag in |value|.
bool ParseIntPairFlag(const char *str, const char *flag, int_pair *value);

// Parses a string for a double flag, in the form of '--flag=value'.
// On success it stores the value of the flag in |value|.
bool ParseDoubleFlag(const char *str, const char *flag, double *value);

// Parses a string for a string flag, in the form of '--flag=value'.
// On success it stores the value of the flag in |value|.
bool ParseStringFlag(const char *str, const char *flag, std::string *value);

// Returns true, if the string matches the |flag|.
bool IsFlag(const char *str, const char *flag);
}

#endif // MANDELBROTPP_CMDLINE_FLAGS_H
