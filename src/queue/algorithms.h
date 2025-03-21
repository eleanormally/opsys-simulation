#pragma once
#include "../process/process.h"

class Compare{
  public:
    bool operator() (Process* p1, Process* p2);
};

class CompareFCFS : Compare{
  public:
    bool operator() (Process* p1, Process* p2) {
      return (*p1).getArrivalTime() < (*p2).getArrivalTime();
    }
};

class CompareSJF : Compare{
  public:
    bool operator() (Process* p1, Process* p2) {
      return (*p1).getPredictedBurstTime().cpuBurstTime < (*p2).getPredictedBurstTime().cpuBurstTime;
    }
};
