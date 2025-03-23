#include "../arg_parser.h"
#include "../queue/ready_queue.h"

typedef struct BurstInstance {
  Process* process;
  bool isInCpuPhase;
} BurstInstance;

enum class EventType {
  ProcessArrived,
  BurstDone,
};
typedef struct Event {
  EventType type;
  Time time;
  union {
    Process* process;
    BurstInstance burst;
  } value;
  bool operator<(const Event& e) const;
  ID getId() const;
} Event;

class Simulation {
  Time globalTime;
  const Arguments& args;
  SchedulingAlgorithm algorithm;
  std::vector<Process> processes;
  std::priority_queue<Event> events;
  ReadyQueue queue;
  size_t nextProcessIdx;
  bool inCPUBurst;

  void log(std::string eventDetails) {
    std::cout << "time " << globalTime << ": " << eventDetails << queue
              << std::endl;
  }
  void log(const Process* const p, std::string eventDetails) {
    std::cout << "time " << globalTime << ": " << p->toString() << " "
              << eventDetails << queue << std::endl;
  }

  const Process& nextProcess() { return processes[nextProcessIdx]; }
  void popProcess() { nextProcessIdx++; }
  bool hasNextProcess() { return nextProcessIdx < processes.size(); }
  void addProcess(Process* p);
  void handleBurst(BurstInstance& b);
  void switchToProcess(Process* p);

  Event popNextEvent();
  const Event& peekNextEvent();
  bool hasNextEvent() const;
  void addEvent(Event& e);

 public:
  Simulation(const Arguments& _args, SchedulingAlgorithm _algorithm,
             const std::vector<Process>& _processes)
      : globalTime(Time(0)),
        args(_args),
        algorithm(_algorithm),
        processes(_processes),
        queue(initReadyQueue(_args, _algorithm)),
        nextProcessIdx(0),
        inCPUBurst(false) {
    for (Process& p : processes) {
      Event e = {
          .type = EventType::ProcessArrived,
          .time = p.getArrivalTime(),
          .value =
              {
                  .process = &p,
              },
      };
      addEvent(e);
    }
  }
  void run();
};
