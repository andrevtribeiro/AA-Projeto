#include "benchmark/benchmark.h"
#include "benchmarks/util/benchmark_assertions.hpp"


// Example main with a custom reporter
int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
  TestReporter rep;
  benchmark::RunSpecifiedBenchmarks(&rep);
}