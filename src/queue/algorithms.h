#pragma once
#include "../process/process.h"

class Compare {
 public:
  bool operator()(Process* p1, Process* p2);
};

class CompareSJF : Compare {
 public:
  bool operator()(Process* p1, Process* p2) {
    Time t1 = p1->getTau() + p1->getTimeRemaining() -
              p1->getCurrentBurst().cpuBurstTime;
    Time t2 = p2->getTau() + p2->getTimeRemaining() -
              p2->getCurrentBurst().cpuBurstTime;
    if (t1 == t2) {
      return p2->getId() < p1->getId();
    }
    return t1 > t2;
  }
};
