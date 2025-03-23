#include "ready_queue.h"

std::ostream& operator<<(std::ostream& out, const ReadyQueue& r) {
  //TODO
  out << "[Q empty]";
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
