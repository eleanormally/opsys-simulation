#pragma once

#include <array>
#include <iostream>
#include <vector>
#include "../arg_parser.h"
#include "../time/time.h"

class ID {
 private:
  std::array<char, 2> _id;

 public:
  std::string toString() const {
    std::string out = "  ";
    out[0] = _id[0];
    out[1] = _id[1];
    return out;
  }
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
  friend std::ostream& operator<<(std::ostream& out, const Process& p);
};

std::vector<Process> generateProcesses(const Arguments args);
