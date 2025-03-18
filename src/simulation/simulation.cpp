#include "simulation.h"

bool BurstInstance::operator<(const BurstInstance& b) const {
  return completionTime < b.completionTime;
}

void Simulation::run() {
  std::cout << globalTime << ": Siumlation started for " << algorithm << " "
            << queue << std::endl;
  while (true) {
    Event e = getNextEvent();
    switch (e.type) {
      case EventType::ProcessArrived:
        addProcess(e.value.process);
        break;
      case EventType::BurstDone:
        handleBurst(e.value.burst);
    }
    if (!inCPUBurst) {
      switchToProcess(e.value.process);
    }
  }
}

void Simulation::addProcess(Process* p) {
  log(p->toString() + " arrived; added to ready queue");
  queue.add(p);
}

void Simulation::switchToProcess(Process* p) {
  globalTime += 2;
  Time burstTime = p->getCurrentBurst().cpuBurstTime;
  log(p->toString() + " started using the CPU for " + burstTime.toString() +
      "burst");
  Event e = {
      .type = EventType::BurstDone,
      .value =
          {
              .burst{
                  .completionTime = burstTime + globalTime,
                  .process = p,
                  .isInCpuPhase = true,
              },
          },
  };
  addEvent(e);
}

void Simulation::handleBurst(BurstInstance& b) {
  if (b.isInCpuPhase) {
    std::string numBurstsRemaining =
        std::to_string((b.process->numRemainingBursts() - 1) * 2);
    log(b.process->toString() + " completed a CPU burst; " +
        numBurstsRemaining + " to go");

    //generate IO burst
    const BurstTime& ioBurst = b.process->getCurrentBurst();
    Event e = {
        .type = EventType::BurstDone,
        .value =
            {
                .burst{
                    .completionTime = ioBurst.ioBurstTime + globalTime,
                    .process = b.process,
                    .isInCpuPhase = false,
                },
            },
    };
    log(b.process->toString() +
        " switching out of the CPU; blocking on I/O until time " +
        e.value.burst.completionTime.toString());
    addEvent(e);
    globalTime += 2;
  } else {
    b.process->incrementBurst();
    queue.add(b.process);
    log(b.process->toString() + " completed I/O; added to ready queue");
  }
}
