#include "ready_queue.h"

std::ostream& operator<<(std::ostream& out, const ReadyQueue& r) {
  //TODO
  out << "[Q empty]";
  return out;
}

std::ostream& operator<<(std::ostream& out, const SchedulingAlgorithm& algo) {
  switch (algo) {
    case SchedulingAlgorithm::RoundRobin:
      out << "RR";
      break;
    case SchedulingAlgorithm::ShortestJobFirst:
      out << "SJF";
      break;
    case SchedulingAlgorithm::FirstComeFirstServe:
      out << "FCFS";
      break;
    case SchedulingAlgorithm::ShortestRemainingTime:
      out << "SRT";
      break;
  }
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

ReadyQueue::ReadyQueue(Arguments args, SchedulingAlgorithm algorithm) {}
void ReadyQueue::add(Process& p) {}
Process ReadyQueue::pop() {}
const Process& ReadyQueue::peek() {}
