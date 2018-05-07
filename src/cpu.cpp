// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#include "cpu.h"
#include "internal_macros.h"
#include <algorithm>
#include <cstddef>
#include <cstring>

#if defined(ARCH_CPU_X86_FAMILY) && defined(COMPILER_MSVC)
#include <immintrin.h> // for _xgetbv()
#include <intrin.h>
#endif

namespace mpp {

namespace {

#if defined(ARCH_CPU_X86_FAMILY)
#if !defined(COMPILER_MSVC)

#if defined(__pic__) && defined(__i386__)

void __cpuid(int cpu_info[4], int info_type) {
  __asm__ volatile("mov %%ebx, %%edi\n"
                   "cpuid\n"
                   "xchg %%edi, %%ebx\n"
                   : "=a"(cpu_info[0]), "=D"(cpu_info[1]), "=c"(cpu_info[2]),
                     "=d"(cpu_info[3])
                   : "a"(info_type, "c"(0));)
}

#else

void __cpuid(int cpu_info[4], int info_type) {
  __asm__ volatile("cpuid\n"
                   : "=a"(cpu_info[0]), "=b"(cpu_info[1]), "=c"(cpu_info[2]),
                     "=d"(cpu_info[3])
                   : "a"(info_type), "c"(0));
}

#endif
// _xgetbv returns the valie of an Intel Extented Control Register (XCR).
// Currently only XCR0 is defined by Intel, thus |xcr| should always be zero.
uint64_t _xgetbv(uint32_t xcr) {
  uint32_t eax, edx;

  __asm__ volatile("xgetbv" : "=a"(eax), "=d"(edx) : "c"(xcr));
  return (static_cast<uint64_t>(edx) << 32) | eax;
}

#endif // !defined(COMPILER_MSVC)
#endif // defined(ARCH_CPU_X86_FAMILY)

} // namespace

CPU::CPU()
    : signature_(0), type_(0), family_(0), model_(0), stepping_(0),
      ext_model_(0), ext_family_(0), has_mmx_(false), has_sse_(false),
      has_sse2_(false), has_sse3_(false), has_sse41_(false), has_sse42_(false),
      has_avx_(false), has_avx2_(false), cpu_vendor_("unknown") {
  Initialize();
}

void CPU::Initialize() {
  int cpu_info[4] = {-1};

  // this array temporarily holds the vendor name as well as the brand name.
  // Thus, it has to be large enough for both use cases.
  char cpu_string[sizeof(cpu_info) * 3 + 1];

  // __cpuid() with an |info_type| argument of 0 returns the number of valid
  // IDs in |cpu_info[0]| and the CPU identification string in the other three
  // elements. The CPU identification string is not in linear order. The code
  // below arranges the information in a human readable form as follows:
  //   cpu_info[1] | cpu_info[3] | cpu_info[2]. Both 2, 3 are swapped before
  // using memcpy() to copy these three array elements to |cpu_string|.
  __cpuid(cpu_info, 0);
  int num_ids = cpu_info[0];
  std::swap(cpu_info[2], cpu_info[3]);
  static constexpr std::size_t kVendorNameSize = 3 * sizeof(cpu_info[1]);
  COMPILE_ASSERT(kVendorNameSize < arraysize(cpu_string),
                 "cpu_string too small");
  std::memcpy(cpu_string, &cpu_info[1], kVendorNameSize);
  cpu_string[kVendorNameSize] = '\0';
  cpu_vendor_ = cpu_string;

  // interpret CPU feature information
  if (num_ids > 0) {
    int cpu_info7[4] = {0};
    __cpuid(cpu_info, 1);
    if (num_ids >= 7) {
      __cpuid(cpu_info7, 7);
    }
    signature_ = cpu_info[0];
    stepping_ = cpu_info[0] & 0xf;
    model_ = ((cpu_info[0] >> 4) & 0xf) + ((cpu_info[0] >> 12) & 0xf0);
    family_ = (cpu_info[0] >> 8) & 0xf;
    type_ = (cpu_info[0] >> 12) & 0x3;
    ext_model_ = (cpu_info[0] >> 16) & 0xf;
    ext_family_ = (cpu_info[0] >> 20) & 0xff;
    has_mmx_ = (cpu_info[3] & 0x00800000) != 0;
    has_sse_ = (cpu_info[3] & 0x02000000) != 0;
    has_sse2_ = (cpu_info[3] & 0x04000000) != 0;
    has_sse3_ = (cpu_info[2] & 0x00000001) != 0;
    has_sse41_ = (cpu_info[2] & 0x00080000) != 0;
    has_sse42_ = (cpu_info[2] & 0x00100000) != 0;

    // AVX instructions will generate an illegal instruction exception, unless
    //  a) they are supported by the CPU
    //  b) XSAVE is supported by the CPU
    //  c) XSAVE is enabled by the kernel
    // See http://software.intel.com/en-us/blogs/2011/04/14/is-avx-enabled
    has_avx_ = (cpu_info[2] & 0x10000000) != 0 &&
               (cpu_info[2] & 0x04000000) != 0 /* XSAVE */ &&
               (cpu_info[2] & 0x08000000) != 0 /* OSXSAVE */ &&
               (_xgetbv(0) & 6) == 6 /* XSAVE enabled by kernel */;
    has_avx2_ = has_avx_ && (cpu_info7[1] & 0x00000020) != 0;

    // get the brand string of the cpu
    __cpuid(cpu_info, 0x80000000);
    const int max_parameter = cpu_info[0];

    static constexpr int kParameterStart = 0x80000002;
    static constexpr int kParameterEnd = 0x80000004;
    static constexpr int kParameterSize = kParameterEnd - kParameterStart + 1;
    COMPILE_ASSERT(kParameterSize * sizeof(cpu_info) + 1 ==
                       arraysize(cpu_string),
                   "cpu_string has wrong size");

    if (max_parameter >= kParameterEnd) {
      size_t i = 0;
      for (int parameter = kParameterStart; parameter <= kParameterEnd;
           ++parameter) {
        __cpuid(cpu_info, parameter);
        memcpy(&cpu_string[i], cpu_info, sizeof(cpu_info));
        i += sizeof(cpu_info);
      }
      cpu_string[i] = '\0';
      cpu_brand_ = cpu_string;
    }
  }
}

CPU::IntelMicroArchitecture CPU::GetIntelMicroArchitecture() const {
  if (has_avx2())
    return AVX2;
  if (has_avx())
    return AVX;
  if (has_sse42())
    return SSE42;
  if (has_sse41())
    return SSE41;
  if (has_sse3())
    return SSE3;
  if (has_sse2())
    return SSE2;
  if (has_sse())
    return SSE2;
  return PENTIUM;
}

const std::string &CPU::vendor() const { return cpu_vendor_; }

const std::string &CPU::cpu_brand() const { return cpu_brand_; }

int CPU::signature() const { return signature_; }

int CPU::stepping() const { return stepping_; }

int CPU::model() const { return model_; }

int CPU::family() const { return family_; }

int CPU::type() const { return type_; }

int CPU::extended_model() const { return ext_model_; }

int CPU::extended_family() const { return ext_family_; }

bool CPU::has_mmx() const { return has_mmx_; }

bool CPU::has_sse() const { return has_sse_; }

bool CPU::has_sse2() const { return has_sse2_; }

bool CPU::has_sse3() const { return has_sse3_; }

bool CPU::has_sse41() const { return has_sse41_; }

bool CPU::has_sse42() const { return has_sse42_; }

bool CPU::has_avx() const { return has_avx_; }

bool CPU::has_avx2() const { return has_avx2_; }

} // namespace mpp