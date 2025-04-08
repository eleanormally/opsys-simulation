#include "ready_queue.h"

ReadyQueue* initReadyQueue(Arguments args, SchedulingAlgorithm algorithm) {
  switch (algorithm) {
    case SchedulingAlgorithm::FirstComeFirstServe:
    case SchedulingAlgorithm::RoundRobin:
      return new ReadyQueueFCFS(args, algorithm);
      break;
    case SchedulingAlgorithm::ShortestJobFirst:
    case SchedulingAlgorithm::ShortestRemainingTime:
      return new ReadyQueueSJF(args, algorithm);
  }
  return new ReadyQueue(args, algorithm);
}
std::ostream& operator<<(std::ostream& out, const ReadyQueue& r) {
  out << (r).toString();
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
  std::string          out = "[Q";
  std::deque<Process*> queueCopy = readyQueue;
  while (queueCopy.size() > 0) {
    out += " " + queueCopy.front()->getId().toString();
    queueCopy.pop_front();
  }
  out += "]";
  return out;
}
std::string ReadyQueueSJF::toString() const {
  if (readyQueue.size() == 0) {
    return "[Q empty]";
  }
  std::priority_queue<Process*, std::vector<Process*>, CompareSJF> queueCopy =
      readyQueue;
  std::string allProcesses = "[Q";

  while (!queueCopy.empty()) {
    Process* p = (queueCopy.top());
    allProcesses += " " + p->getId().toString();
    /*allProcesses += " " + p->getTau().toString();*/
    queueCopy.pop();
  }
  allProcesses += "]";
  return allProcesses;
}
