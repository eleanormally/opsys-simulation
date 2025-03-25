#pragma once
#include "../process/process.h"

class Compare {
 public:
  bool operator()(Process* p1, Process* p2);
};

class CompareSJF : Compare {
 public:
  bool operator()(Process* p1, Process* p2) {
    Time t1 = p1->getPredictedTime();
    Time t2 = p2->getPredictedTime();
    if (t1 == t2) {
      return p2->getId() < p1->getId();
    }
    return t1 > t2;
  }
};
