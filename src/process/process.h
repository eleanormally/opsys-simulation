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
    friend std::ostream& operator<<(std::ostream& out, const Process& p);
   
  public:
    Process(ID _id, Time _time, size_t _burstCount, std::vector<BurstTime> _burstTimes, Time _tau, bool _cpuBound) :
      id(_id),
      arrivalTime(_time),
      burstCount(_burstCount),
      burstTimes(_burstTimes),
      tau(_tau),
      cpuBound(_cpuBound) {}
    Time getArrivalTime() const { return arrivalTime; }
    ID getId() { return id; }
    bool isCpuBound() const { return cpuBound; }
    std::string toString() const { return "Process " + id.toString() + "  Time " + arrivalTime.toString(); }
    Time getTau() { return tau; }
    const BurstTime& getCurrentBurst() const { return burstTimes[burstIndex]; }
    void incrementBurst() { burstIndex++; }
    bool hasNextBurst() { return burstIndex < burstTimes.size(); }
    std::vector<BurstTime> getAllBursts() const { return burstTimes; }
    size_t getBurstCount() const { return burstCount; }
    size_t numRemainingBursts() const { return burstCount - burstIndex; }
};

std::vector<Process> generateProcesses(const Arguments args);
