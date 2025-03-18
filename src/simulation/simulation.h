#include "../arg_parser.h"
#include "../queue/ready_queue.h"

typedef struct BurstInstance {
  Time completionTime;
  Process* process;
  bool isInCpuPhase;
  bool operator<(const BurstInstance& b) const;
} BurstInstance;

enum class EventType {
  ProcessArrived,
  BurstDone,
};
typedef struct Event {
  EventType type;
  union {
    Process* process;
    BurstInstance burst;
  } value;
} Event;

class Simulation {
  Time globalTime;
  const Arguments& args;
  SchedulingAlgorithm algorithm;
  std::vector<Process> processes;
  ReadyQueue queue;
  size_t nextProcessIdx;
  bool inCPUBurst;

  void log(std::string eventDetails) {
    std::cout << "time " << globalTime << ": " << eventDetails << queue
              << std::endl;
  }

  const Process& nextProcess() { return processes[nextProcessIdx]; }
  void popProcess() { nextProcessIdx++; }
  bool hasNextProcess() { return nextProcessIdx < processes.size(); }
  void addProcess(Process* p);
  void handleBurst(BurstInstance& b);
  void switchToProcess(Process* p);

  Event getNextEvent();
  void addEvent(Event& e);

 public:
  Simulation(const Arguments& _args, SchedulingAlgorithm _algorithm,
             const std::vector<Process>& _processes)
      : globalTime(Time(0)),
        args(_args),
        algorithm(_algorithm),
        processes(_processes),
        queue(ReadyQueue(_args, _algorithm)),
        nextProcessIdx(0),
        inCPUBurst(false) {}
  void run();
};
