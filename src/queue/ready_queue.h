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
inline const std::string toString(SchedulingAlgorithm a) {
  switch (a) {
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
  virtual ~ReadyQueue() {}
  ReadyQueue(Arguments args, SchedulingAlgorithm algorithm);
  virtual bool isEmpty() const { return true; }
  virtual void add(Process*) { std::cout << "ERROR: USING BASE READY QUEUE\n"; }
  virtual Process* pop() { return NULL; }
  virtual const Process* peek() { return NULL; }
  virtual std::string toString() const { return ""; }
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

class ReadyQueueFCFS : public ReadyQueue {
  std::queue<Process*> readyQueue;

 public:
  ~ReadyQueueFCFS() {}
  ReadyQueueFCFS(Arguments args, SchedulingAlgorithm algorithm)
      : ReadyQueue(args, algorithm) {}
  bool isEmpty() const { return readyQueue.size() == 0; }
  void add(Process* p) { readyQueue.push(p); }
  Process* pop() {
    Process* p = readyQueue.front();
    readyQueue.pop();
    return p;
  }
  const Process* peek() { return readyQueue.front(); }
  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

class ReadyQueueSJF : public ReadyQueue {
  std::priority_queue<Process*, std::vector<Process*>, CompareSJF> readyQueue;

 public:
  ~ReadyQueueSJF() {}
  ReadyQueueSJF(Arguments args, SchedulingAlgorithm algorithm)
      : ReadyQueue(args, algorithm) {}
  bool isEmpty() const { return readyQueue.size() == 0; }
  void add(Process* p) { readyQueue.push(p); }
  Process* pop() {
    Process* p = readyQueue.top();
    readyQueue.pop();
    return p;
  }
  const Process* peek() { return readyQueue.top(); }
  std::string toString() const;
  friend std::ostream& operator<<(std::ostream& out, const ReadyQueue& r);
};

ReadyQueue* initReadyQueue(Arguments args, SchedulingAlgorithm algorithm);
