// Copyright (c) 2016 - 2018, Kai Wolf. All rights reserved.
// Use of this source code is governed by a MIT license that can be found in
// the LICENSE file.

#ifndef MANDELBROTPP_BENCHMARK_H
#define MANDELBROTPP_BENCHMARK_H

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

namespace mpp {

using BenchmarkList = std::vector<class Benchmark *>;

enum class ContextState { Idle, Running, AreaBench };

template <typename Clock> class BasicContext {
public:
  BasicContext(std::chrono::nanoseconds duration)
      : state(ContextState::Idle), duration(duration) {}

  // Keeps the benchmark running for the duration given in the constructor.
  // Return true as long as there is time left.
  bool Running() {
    if (state == ContextState::AreaBench) {
      if (runTime > duration) {
        return false;
      }
      iterations++;
      return true;
    }
    if (state == ContextState::Idle) {
      state = ContextState::Running;
      iterations = 1;
      start = Clock::now();
      return true;
    }
    auto now = Clock::now();
    runTime = now - start;
    if (runTime >= duration) {
      return false;
    }

    iterations++;
    return true;
  }

protected:
  int64_t TimePerIteration(int64_t overhead = 0) const {
    auto perIt = runTime.count() / iterations;

    if (state != ContextState::AreaBench) {
      perIt -= overhead;
    }

    return perIt;
  }

  int64_t Iterations() const { return iterations; }

  void BeginArea() {
    if (state != ContextState::AreaBench) {
      // reset everything set by Running()
      iterations = 1;
      runTime = std::chrono::nanoseconds::zero();
      state = ContextState::AreaBench;
    }
    start = Clock::now();
  }

  void EndArea() {
    auto now = Clock::now();
    runTime += (now - start);
  }

private:
  ContextState state;
  int64_t iterations;
  typename Clock::time_point start;
  std::chrono::nanoseconds duration;
  std::chrono::nanoseconds runTime;

  friend class BenchmarkArea;
  friend class Benchmark;
};

using Context = BasicContext<std::chrono::high_resolution_clock>;

class BenchmarkArea {
public:
  BenchmarkArea(Context &context) : context(context) { context.BeginArea(); }

  ~BenchmarkArea() { context.EndArea(); }

private:
  Context &context;
};

enum class Color { White = 0, Green = 32, Yellow = 33, Cyan = 36 };

class ConsoleModifier {
public:
  ConsoleModifier(Color c) : code(c) {}
  friend std::ostream &operator<<(std::ostream &os, const ConsoleModifier &cm) {
    return os << "\033[" << static_cast<int>(cm.code) << "m";
  }

private:
  Color code;
};

class Benchmark {
private:
  static std::string FilterArgument(int argc, const char *argv[]) {
    // iterate over all arguments and find --benchmark_filter=
    std::regex filterRegex{"--benchmark_filter=(.*)"};
    std::smatch baseMatch;

    for (int i = 0; i < argc; i++) {
      std::string argument{argv[i]};
      if (std::regex_match(argument, baseMatch, filterRegex)) {
        if (baseMatch.size() == 2) {
          std::ssub_match subMatch = baseMatch[1];
          std::string filter = subMatch.str();
          // replace all occurrences of * with .*
          filter = std::regex_replace(filter, std::regex("\\*"), ".*");

          return filter;
        }
      }
    }

    return std::string(".*");
  }

public:
  static BenchmarkList &List() {
    // Currently elements in list are never destroyed, but as
    // the lifetime of list is the lifetime of the process, we
    // don't bother using unique_ptr
    static BenchmarkList list;
    return list;
  }

  // empty benchmark to measure the overhead of context.Running()
  static void EmptyBenchMark(Context &context) {
    while (context.Running())
      ;
  }

  static void RunAllBenchmarks(int argc = 0, const char *argv[] = nullptr) {
    auto maxLength = MaxNameLength();
    auto nanoLength = std::string("Time").length() + 10;
    auto itLength = std::string("Iterations").length();

    auto filter = FilterArgument(argc, argv);

    std::regex filterRegex;
    try {
      filterRegex = std::regex(filter);
    } catch (std::regex_error) {
      std::cout << "Invalid filter: " << filter << std::endl;
      exit(1);
    }

    Context overheadContext(std::chrono::seconds(1));
    EmptyBenchMark(overheadContext);

    std::cout << std::left << std::setw(maxLength) << "Name"
              << "               "
              << "Time  Iterations" << std::endl;
    std::string dashes;
    dashes.insert(0, maxLength + 3 + nanoLength + 4 + itLength, '-');
    std::cout << dashes << std::endl;

    for (auto benchmark : List()) {
      std::smatch baseMatch;
      if (!std::regex_match(benchmark->Name(), baseMatch, filterRegex))
        continue;

      Context context(std::chrono::seconds(1));
      benchmark->SetUp();
      benchmark->RunBenchmark(context);
      benchmark->TearDown();
      std::ostringstream ost;
      int64_t timePerIteration =
          context.TimePerIteration(overheadContext.TimePerIteration());
      timePerIteration = std::max<int64_t>(timePerIteration, 0ll);
      ost << timePerIteration;
      auto nanoStr = ost.str();

      std::cout << ConsoleModifier(Color::Green);
      std::cout << std::left << std::setw(maxLength);
      std::cout << benchmark->Name();
      std::cout << "  ";

      std::cout << ConsoleModifier(Color::Yellow);
      std::cout << std::right << std::setw(nanoLength) << nanoStr << " ns";

      std::ostringstream itstrstr;
      itstrstr << context.Iterations();
      auto itStr = itstrstr.str();

      std::cout << "  ";
      std::cout << ConsoleModifier(Color::Cyan);
      std::cout << std::right << std::setw(itLength) << itStr << std::endl;
      std::cout << "\033[0m";
    }
  }

  static size_t MaxNameLength() {
    size_t length = 0;
    for (const auto &benchmark : List()) {
      length = benchmark->Name().length() > length ? benchmark->Name().length()
                                                   : length;
    }
    return length;
  }

  const std::string &Name() const { return name; }

  std::string &Name() { return name; }

protected:
  virtual void RunBenchmark(Context &context){};
  virtual void SetUp() {}
  virtual void TearDown() {}

protected:
  std::string name;
};

#define _BM_CONCATX(A, B) A##B
#define _BM_CONCAT(A, B) _BM_CONCATX(A, B)
#define _BM_STRX(X) #X
#define _BM_STR(X) _BM_STRX(X)

#define BENCHMARK(NAME)                                                        \
  class NAME : public mpp::Benchmark {                                         \
  public:                                                                      \
    static class _init {                                                       \
    public:                                                                    \
      _init() {                                                                \
        mpp::Benchmark *bench = new NAME();                                    \
        bench->Name() = #NAME;                                                 \
        NAME::List().push_back(bench);                                         \
      }                                                                        \
    } _initializer;                                                            \
                                                                               \
  protected:                                                                   \
    void RunBenchmark(mpp::Context &context) override;                         \
  };                                                                           \
  NAME::_init NAME::_initializer;                                              \
                                                                               \
  void NAME::RunBenchmark(mpp::Context &context)

#define BENCHMARK_F(FIXTURE, NAME)                                             \
  class _BM_CONCAT(FIXTURE, NAME) : public FIXTURE {                           \
  public:                                                                      \
    static class _init {                                                       \
    public:                                                                    \
      _init() {                                                                \
        mpp::Benchmark *bench = new _BM_CONCAT(FIXTURE, NAME)();               \
        bench->Name() = _BM_STR(FIXTURE) "." _BM_STR(NAME);                    \
        _BM_CONCAT(FIXTURE, NAME)::List().push_back(bench);                    \
      }                                                                        \
    } _initializer;                                                            \
                                                                               \
  protected:                                                                   \
    void RunBenchmark(mpp::Context &context) override;                         \
  };                                                                           \
  _BM_CONCAT(FIXTURE, NAME)::_init _BM_CONCAT(FIXTURE, NAME)::_initializer;    \
                                                                               \
  void _BM_CONCAT(FIXTURE, NAME)::RunBenchmark(mpp::Context &context)

} // namespace mpp

#define BENCHMARK_MAIN()                                                       \
  int main(int argc, const char *argv[]) {                                     \
    mpp::Benchmark::RunAllBenchmarks(argc, argv);                              \
  }

#endif // MANDELBROTPP_BENCHMARK_H
