#include "benchmark/benchmark.h"
#include <iostream>

#define MAXIMUM_ACCEPTABLE_ERROR (0.15)

class TestReporter : public benchmark::ConsoleReporter {
public:
  using Base = benchmark::ConsoleReporter;
  using Base::Base;

  void ReportRuns(const std::vector<Run>& report) {
    Base::ReportRuns(report);
  
    for (auto& R : report) assert(CheckRun(R));
  }
private:
  bool CheckRun(Run const& R) const {
    if (!(R.complexity == benchmark::oLambda)) return true;
    
    if (R.report_rms) {
      if ((R.cpu_accumulated_time * GetTimeUnitMultiplier(R.time_unit)) >= MAXIMUM_ACCEPTABLE_ERROR) {
        std::cout << "Complexity for test " << R.benchmark_name << ", with error " << (R.cpu_accumulated_time * GetTimeUnitMultiplier(R.time_unit)) << " does not check out" << std::endl;
        return false;
      }
    }
    
    if (R.report_big_o) {
      if ((R.cpu_accumulated_time) >= 2.0) {
        std::cout << "Complexity for test " << R.benchmark_name << ", with error " << R.cpu_accumulated_time << " does not check out" << std::endl;
        return false;
      }
    }
    return true;
  }
};