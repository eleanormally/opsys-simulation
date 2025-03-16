#pragma once
#include "../arg_parser.h"
#include "../process/process.h"

enum SchedulingAlgorithm {
  FirstComeFirstServe,
  ShortestJobFirst,
  ShortestRemainingTime,
  RoundRobin,
};
std::array<SchedulingAlgorithm, 4> listSchedulingAlgorithms();

class ReadyQueue {
 public:
  ReadyQueue(Arguments args, SchedulingAlgorithm algorithm);
  void add(Process& p);
  Process pop();
  const Process& peek();
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};
