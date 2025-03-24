#pragma once
#include "../process/process.h"

class Compare {
 public:
  bool operator()(Process* p1, Process* p2);
};

class CompareSJF : Compare {
 public:
  bool operator()(Process* p1, Process* p2) {
    return (*p1).getTau() > (*p2).getTau();
  }
};
