#pragma once

#include <array>
#include <vector>
#include "../arg_parser.h"
#include "../time/time.h"

class ID {
 private:
  std::array<char, 2> _id;

 public:
};

typedef struct BurstTime {
  Time cpuBurstTime;
  Time ioBurstTime;
} BurstTime;

class Process {
 private:
  ID id;
  Time arrivalTime;
  size_t burstCount;
  std::vector<BurstTime> burstTimes;
};

std::vector<Process> generateProcesses(const Arguments args);
