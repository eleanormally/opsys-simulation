#pragma once
#include "../arg_parser.h"
#include "../process/process.h"
#include "../queue/algorithms.h"
#include <queue>
#include <array>

enum class SchedulingAlgorithm {
  FirstComeFirstServe,
  ShortestJobFirst,
  ShortestRemainingTime,
  RoundRobin,
};
std::ostream& operator<<(std::ostream& out, const SchedulingAlgorithm& algo);
std::array<SchedulingAlgorithm, 4> listSchedulingAlgorithms();

class ReadyQueue {
  protected:
   SchedulingAlgorithm algorithmType;
   size_t timeSlice;

 public:
  ReadyQueue(Arguments args, SchedulingAlgorithm algorithm);
  virtual void add(Process* p) {}
  virtual Process* pop() { return NULL; }
  virtual const Process* peek() { return NULL; }
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

ReadyQueue getQueue(Arguments args, SchedulingAlgorithm algorithm);

class ReadyQueueFCFS : public ReadyQueue {
  std::priority_queue<Process*, std::vector<Process*>, CompareFCFS> readyQueue;

  public:
    ReadyQueueFCFS(Arguments args, SchedulingAlgorithm algorithm) : ReadyQueue(args, algorithm) {}
    void add(Process* p) { readyQueue.push(p); }
    Process* pop() { Process* p = readyQueue.top(); readyQueue.pop(); return p; }
    const Process* peek() { return readyQueue.top();  }
    friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

class ReadyQueueSJF : public ReadyQueue {
  std::priority_queue<Process*, std::vector<Process*>, CompareSJF> readyQueue;
  
  public:
    ReadyQueueSJF(Arguments args, SchedulingAlgorithm algorithm) : ReadyQueue(args, algorithm) {}
    void add(Process* p) { readyQueue.push(p); }
    Process* pop() { Process* p = readyQueue.top(); readyQueue.pop(); return p; }
    const Process* peek() { return readyQueue.top();  }
    friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};
  