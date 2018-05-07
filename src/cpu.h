// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_CPU_H
#define MANDELBROTPP_CPU_H

#include "mpp_lib_export.h"
#include <string>

namespace mpp {

class MPP_LIB_EXPORT CPU final {
public:
  CPU();

  enum IntelMicroArchitecture {
    PENTIUM,
    SSE,
    SSE2,
    SSE3,
    SSE41,
    SSE42,
    AVX,
    AVX2,
    MAX_INTEL_MICRO_ARCHITECTURE
  };

  IntelMicroArchitecture GetIntelMicroArchitecture() const;
  const std::string& vendor() const;
  const std::string& cpu_brand() const;
  int signature() const;
  int stepping() const;
  int model() const;
  int family() const;
  int type() const;
  int extended_model() const;
  int extended_family() const;
  bool has_mmx() const;
  bool has_sse() const;
  bool has_sse2() const;
  bool has_sse3() const;
  bool has_sse41() const;
  bool has_sse42() const;
  bool has_avx() const;
  bool has_avx2() const;

private:
  /// Query the processor for CPUID information
  void Initialize();

  /// Raw form of type, family, model and stepping
  int signature_;
  /// Processor type
  int type_;
  /// Family of the processor
  int family_;
  /// Model of the processor
  int model_;
  /// Processor revision number
  int stepping_;
  int ext_model_;
  int ext_family_;
  bool has_mmx_;
  bool has_sse_;
  bool has_sse2_;
  bool has_sse3_;
  bool has_sse41_;
  bool has_sse42_;
  bool has_avx_;
  bool has_avx2_;
  std::string cpu_vendor_;
  std::string cpu_brand_;
};

} // namespace mpp

#endif // MANDELBROTPP_CPU_H