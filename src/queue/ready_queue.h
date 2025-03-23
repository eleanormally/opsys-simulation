#pragma once
#include <array>
#include <queue>
#include "../arg_parser.h"
#include "../process/process.h"
#include "../queue/algorithms.h"

enum class SchedulingAlgorithm {
  FirstComeFirstServe,
  ShortestJobFirst,
  ShortestRemainingTime,
  RoundRobin,
};
std::ostream& operator<<(std::ostream& out, const SchedulingAlgorithm& algo);
std::array<SchedulingAlgorithm, 4> listSchedulingAlgorithms();
inline const std::string toString(SchedulingAlgorithm a)
{
    switch (a)
    {
    case SchedulingAlgorithm::RoundRobin:
      return "RR";
    case SchedulingAlgorithm::ShortestJobFirst:
      return "SJF";
    case SchedulingAlgorithm::FirstComeFirstServe:
      return "FCFS";
    case SchedulingAlgorithm::ShortestRemainingTime:
      return "SRT";
    }
}

class ReadyQueue {
 protected:
  SchedulingAlgorithm algorithmType;
  size_t timeSlice;

 public:
  ReadyQueue(Arguments args, SchedulingAlgorithm algorithm);
  virtual void add(Process*) {}
  virtual Process* pop() { return NULL; }
  virtual const Process* peek() { return NULL; }
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

ReadyQueue getQueue(Arguments args, SchedulingAlgorithm algorithm);

class ReadyQueueFCFS : public ReadyQueue {
  std::priority_queue<Process*, std::vector<Process*>, CompareFCFS> readyQueue;

 public:
  ReadyQueueFCFS(Arguments args, SchedulingAlgorithm algorithm)
      : ReadyQueue(args, algorithm) {}
  void add(Process* p) { readyQueue.push(p); }
  Process* pop() {
    Process* p = readyQueue.top();
    readyQueue.pop();
    return p;
  }
  const Process* peek() { return readyQueue.top(); }
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

class ReadyQueueSJF : public ReadyQueue {
  std::priority_queue<Process*, std::vector<Process*>, CompareSJF> readyQueue;

 public:
  ReadyQueueSJF(Arguments args, SchedulingAlgorithm algorithm)
      : ReadyQueue(args, algorithm) {}
  void add(Process* p) { readyQueue.push(p); }
  Process* pop() {
    Process* p = readyQueue.top();
    readyQueue.pop();
    return p;
  }
  const Process* peek() { return readyQueue.top(); }
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

  static ReadyQueue initReadyQueue(Arguments args, SchedulingAlgorithm algorithm) {
    switch (algorithm) {
      case SchedulingAlgorithm::FirstComeFirstServe:
        return ReadyQueueFCFS(args, algorithm);
        break;
      case SchedulingAlgorithm::ShortestJobFirst:
        return ReadyQueueSJF(args, algorithm);
        break;
      case SchedulingAlgorithm::ShortestRemainingTime:
      case SchedulingAlgorithm::RoundRobin:
        break;
      default:
        return ReadyQueue(args, algorithm);
    }
  }
