#pragma once

#include <math.h>
#include "../arg_parser.h"
#include "../queue/ready_queue.h"

#define TRUNCATE_OUTPUT

typedef struct BurstInstance {
  Process* process;
  bool     isInCpuPhase;
} BurstInstance;

enum class EventType {
  ProcessArrived,
  BurstDone,
  BurstTimeout,
  ProcessSelect,
  ProcessStart,
  ProcessEnqueue,
  ProcessEnqueueFront,
};
template <>
struct std::hash<EventType> {
  bool operator()(const EventType& e) const {
    int h = 0;
    switch (e) {
      case EventType::ProcessArrived:
        h = 1;
        break;
      case EventType::BurstDone:
        h = 2;
        break;
      case EventType::BurstTimeout:
        h = 3;
        break;
      case EventType::ProcessSelect:
        h = 4;
        break;
      case EventType::ProcessStart:
        h = 5;
        break;
      case EventType::ProcessEnqueue:
      case EventType::ProcessEnqueueFront:
        h = 6;
        break;
    }
    return std::hash<int>()(h);
  }
};
typedef struct Event {
  EventType type;
  Time      time;
  union {
    Process*      process;
    BurstInstance burst;
  } value;
  bool         operator<(const Event& e) const;
  bool         operator==(const Event& e) const;
  int          getOrder() const;
  ID           getId() const;
  static Event newSelect(Time t) {
    return Event{
        .type = EventType::ProcessSelect,
        .time = t,
        .value = {},
    };
  }
  static Event newQueue(Process* p, Time t, bool front) {
    return Event{
        .type =
            front ? EventType::ProcessEnqueueFront : EventType::ProcessEnqueue,
        .time = t,
        .value =
            {
                .process = p,
            },
    };
  }

} Event;
template <>
struct std::hash<Event> {
  size_t operator()(const Event& e) const {
    using std::hash;
    return (hash<size_t>()(e.time.getUnderlying()) ^
            (hash<EventType>()(e.type) << 1));
  }
};

typedef struct SimulationStats {
  BurstTime   preemptionCount;
  BurstTime   contextSwitchCount;
  BurstTime   waitSum;
  BurstTime   roundRobinSliceCount;
  Time        totalSimulationTime;
  std::string algorithmString;
} SimulationStats;

class Simulation {
  Time                       globalTime;
  const Arguments&           args;
  SchedulingAlgorithm        algorithm;
  std::vector<Process>       processes;
  std::priority_queue<Event> events;
  ReadyQueue*                queue;
  size_t                     nextProcessIdx;
  Process*                   inCPUBurst;
  Time                       cpuBurstStartTime;
  Event                      burstDoneEvent;
  bool                       selectionStarted;
  bool                       processRunning;
  SimulationStats            stats;

  void log(std::string eventDetails) {
#ifdef TRUNCATE_OUTPUT
    if (globalTime >= Time(10000) &&
        !(eventDetails.find("Simulator ended") != std::string::npos ||
          eventDetails.find("terminated") != std::string::npos)) {
      return;
    }
#endif
    std::cout << "time " << globalTime << ": " << eventDetails << " " << *queue
              << std::endl;
  }
  void log(const Process* const p, std::string eventDetails) {
#ifdef TRUNCATE_OUTPUT
    if (globalTime >= Time(10000) &&
        !(eventDetails.find("Simulator ended") != std::string::npos ||
          eventDetails.find("terminated") != std::string::npos)) {
      return;
    }
#endif
    std::string tauString = "";
    if (!args.ignoreExponential &&
        (algorithm == SchedulingAlgorithm::ShortestJobFirst ||
         algorithm == SchedulingAlgorithm::ShortestRemainingTime)) {
      tauString = " (tau " + p->getTau().toString() + ")";
    }
    std::cout << "time " << globalTime << ": " << p->toString() << tauString
              << " " << eventDetails << " " << *queue << std::endl;
  }

  const Process& nextProcess() { return processes[nextProcessIdx]; }
  void           popProcess() { nextProcessIdx++; }
  bool           hasNextProcess() { return nextProcessIdx < processes.size(); }
  void           addProcess(Process* p);
  void           handleCpuBurst(const Event& e);
  void           handleCpuTimeout(const Event& e);
  void           handleIoBurst(const Event& e);
  void           selectProcess();
  void           startProcess(Process* p);

  Event generateIoBurst(Process* p);
  Event generateCpuBurst(Process* p);

  void addProcessToQueue(Process* p, bool front);

  Event        popNextEvent();
  const Event& peekNextEvent();
  bool         hasNextEvent() const;
  void         addEvent(Event e);

  BurstTime addWaitTime(BurstTime burstTime, Process* p);

 public:
  Simulation(const Arguments& _args, SchedulingAlgorithm _algorithm,
             std::vector<Process> _processes)
      : globalTime(Time(0)),
        args(_args),
        algorithm(_algorithm),
        processes(_processes),
        queue(initReadyQueue(_args, _algorithm)),
        nextProcessIdx(0),
        inCPUBurst(nullptr),
        selectionStarted(false),
        processRunning(false) {
    for (size_t i = 0; i < processes.size(); i++) {
      Process* p = &processes[i];
      Event    e = {
             .type = EventType::ProcessArrived,
             .time = p->getArrivalTime(),
             .value =
              {
                     .process = p,
              },
      };
      addEvent(e);
    }
    stats.algorithmString = toString(_algorithm);
  }
  ~Simulation() { delete queue; }
  SimulationStats run();
};
