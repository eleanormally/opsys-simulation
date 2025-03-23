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
