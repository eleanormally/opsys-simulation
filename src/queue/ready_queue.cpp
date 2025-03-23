#include "ready_queue.h"

ReadyQueue* initReadyQueue(Arguments args, SchedulingAlgorithm algorithm) {
  ReadyQueueFCFS* rq = new ReadyQueueFCFS(args, algorithm);
  return rq;
  switch (algorithm) {
    case SchedulingAlgorithm::FirstComeFirstServe:
      break;
    case SchedulingAlgorithm::ShortestJobFirst:
      return new ReadyQueueSJF(args, algorithm);
      break;
    case SchedulingAlgorithm::ShortestRemainingTime:
    case SchedulingAlgorithm::RoundRobin:
      break;
  }
  return new ReadyQueue(args, algorithm);
}
std::ostream& operator<<(std::ostream& out, const ReadyQueue& r) {
  out << "[Q" << (r).toString() << "]";
  return out;
}

std::ostream& operator<<(std::ostream& out, const SchedulingAlgorithm& algo) {
  out << toString(algo);
  return out;
}

std::array<SchedulingAlgorithm, 4> listSchedulingAlgorithms() {
  return {
      SchedulingAlgorithm::FirstComeFirstServe,
      SchedulingAlgorithm::ShortestJobFirst,
      SchedulingAlgorithm::ShortestRemainingTime,
      SchedulingAlgorithm::RoundRobin,
  };
}

ReadyQueue getQueue(Arguments args, SchedulingAlgorithm algorithm) {
  switch (algorithm) {
    case SchedulingAlgorithm::FirstComeFirstServe:
    case SchedulingAlgorithm::RoundRobin:
      return ReadyQueueFCFS(args, algorithm);
    case SchedulingAlgorithm::ShortestJobFirst:
      return ReadyQueueSJF(args, algorithm);
    case SchedulingAlgorithm::ShortestRemainingTime:
      break;  // TODO
  }
}

ReadyQueue::ReadyQueue(Arguments args, SchedulingAlgorithm algorithm) {
  algorithmType = algorithm;
  timeSlice = args.timeSlice;
}

std::string ReadyQueueSJF::toString() const {
  std::priority_queue<Process*, std::vector<Process*>, CompareSJF> queueCopy = readyQueue;
  std::string allProcesses = "";

  while(!queueCopy.empty()) {
    Process* p = (queueCopy.top());
    allProcesses = allProcesses + (*p).getId().toString();
    queueCopy.pop();
  }
  return (allProcesses == "") ? "empty" : allProcesses;
}

std::string ReadyQueueFCFS::toString() const {
  std::priority_queue<Process*, std::vector<Process*>, CompareFCFS> queueCopy = readyQueue;
  std::string allProcesses = "";

  while(!queueCopy.empty()) {
    Process* p = (queueCopy.top());
    allProcesses += " " + (*p).getId().toString();
    queueCopy.pop();
  }
  return (allProcesses == "") ? "empty" : allProcesses;
}