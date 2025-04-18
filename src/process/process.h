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
  ID(char first, char second) : _id{first, second} {}
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
  bool operator==(const ID& i) const {
    return _id[0] == i._id[0] && _id[1] == i._id[1];
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
  size_t burstIndex;
  Time tau;
  bool cpuBound;
  Time timeRemainingInBurst;
  Time lastAddedToQueue;
  bool firstSlice;
  friend std::ostream& operator<<(std::ostream& out, const Process& p);

 public:
  Process(ID _id, Time _time, size_t _burstCount,
          std::vector<BurstTime> _burstTimes, Time _tau, bool _cpuBound)
      : id(_id),
        arrivalTime(_time),
        burstCount(_burstCount),
        burstTimes(_burstTimes),
        burstIndex(0),
        tau(_tau),
        cpuBound(_cpuBound),
        firstSlice(true) {
    timeRemainingInBurst = _burstTimes[0].cpuBurstTime;
  }
  Time getArrivalTime() const { return arrivalTime; }
  ID getId() { return id; }
  bool isCpuBound() const { return cpuBound; }
  std::string toString() const { return "Process " + id.toString(); }
  Time getTau() const { return tau; }
  Time getPredictedTime() const {
    return tau + timeRemainingInBurst - getCurrentBurst().cpuBurstTime;
  }
  void setTau(Time t) { tau = t; }
  Time getTimeRemaining() const { return timeRemainingInBurst; }
  void setTimeRemaining(Time t) { timeRemainingInBurst = t; }
  Time getLastAddedToQueueTime() { return lastAddedToQueue; }
  void setLastAddedToQueueTime(Time t) { lastAddedToQueue = t; }
  const BurstTime& getCurrentBurst() const { return burstTimes[burstIndex]; }
  void incrementBurst() { burstIndex++; firstSlice = true; }
  bool hasNextBurst() { return burstIndex < burstTimes.size(); }
  std::vector<BurstTime> getAllBursts() const { return burstTimes; }
  size_t getBurstCount() const { return burstCount; }
  size_t getBurstIndex() const { return burstIndex; }
  size_t numRemainingBursts() const { return burstCount - burstIndex; }
  bool isFirstSlice() { return firstSlice; }
  void setFirstSlice(bool b) { firstSlice = b; }
};

std::vector<Process> generateProcesses(const Arguments args);
