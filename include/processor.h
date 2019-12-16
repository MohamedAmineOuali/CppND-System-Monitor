#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();

 private:
    std::vector<float> previous;

    static float getSysActiveCpuTime(std::vector<float> cpuStat);
    static float getSysIdleCpuTime(std::vector<float> cpuStat);
};

#endif