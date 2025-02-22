#pragma once
#include "../process/process.h"

class ReadyQueue {
  void add(Process& p);
  Process pop();
  const Process& peek();
};
