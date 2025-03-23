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
  out << r.toString();
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

ReadyQueue::ReadyQueue(Arguments args, SchedulingAlgorithm algorithm) {
  algorithmType = algorithm;
  timeSlice = args.timeSlice;
}
std::string ReadyQueueFCFS::toString() const {
  if (readyQueue.size() == 0) {
    return "[Q empty]";
  }
  std::string out = "[Q";
  std::queue<Process*> queueCopy = readyQueue;
  while (queueCopy.size() > 0) {
    out += " " + queueCopy.front()->getId().toString();
    queueCopy.pop();
  }
  out += "]";
  return out;
}
std::string ReadyQueueSJF::toString() const {
  std::priority_queue<Process*, std::vector<Process*>, CompareSJF> queueCopy =
      readyQueue;
  std::string allProcesses = "[Q ";

  while (!queueCopy.empty()) {
    Process* p = (queueCopy.top());
    allProcesses = allProcesses + (*p).getId().toString();
    queueCopy.pop();
  }
  allProcesses = (allProcesses == "") ? "empty" : allProcesses;
  allProcesses += "]";
  return allProcesses;
}
