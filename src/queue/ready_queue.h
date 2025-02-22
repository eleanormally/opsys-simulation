#pragma once
#include "../arg_parser.h"
#include "../process/process.h"

enum SchedulingAlgorithm {
  FirstComeFirstServe,
  ShortestJobFirst,
  ShortestRemainingTime,
  RoundRobin,
};

class ReadyQueue {
  ReadyQueue(Arguments args, SchedulingAlgorithm algorithm);
  void add(Process& p);
  Process pop();
  const Process& peek();
};
