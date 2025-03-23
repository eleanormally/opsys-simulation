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
  bool operator<(const ID& i) const {
    if (_id[0] == i._id[0]) {
      return _id[1] < i._id[1];
    }
    return _id[0] < i._id[0];
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
  BurstTime predictedTime;
  size_t burstCount;
  std::vector<BurstTime> burstTimes;
  size_t burstIndex;
  friend std::ostream& operator<<(std::ostream& out, const Process& p);

 public:
  Process(ID my_id) { id = my_id; }
  Time getArrivalTime() { return arrivalTime; }
  BurstTime getPredictedBurstTime() { return predictedTime; }
  ID getId() { return id; }
  std::string toString() const { return "Process " + id.toString(); }
  const BurstTime& getCurrentBurst() const { return burstTimes[burstIndex]; }
  void incrementBurst() { burstIndex++; }
  bool hasNextBurst() { return burstIndex < burstTimes.size(); }
  size_t numRemainingBursts() const { return burstCount - burstIndex; }
};

std::vector<Process> generateProcesses(const Arguments args);
