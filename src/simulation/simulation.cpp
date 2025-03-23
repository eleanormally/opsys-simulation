#include "simulation.h"

void Simulation::run() {
  std::cout << globalTime << ": Siumlation started for " << algorithm << " "
            << *queue << std::endl;
  while (hasNextEvent()) {
    Event e = popNextEvent();
    globalTime = e.time;
    switch (e.type) {
      case EventType::ProcessArrived:
        addProcess(e.value.process);
        break;
      case EventType::BurstDone:
        handleBurst(e.value.burst);
        break;
      case EventType::ProcessSwitchIn:
        switchToNextProcess();
    }
  }
  log("Simulator ended for " + toString(algorithm));
}

void Simulation::addProcess(Process* p) {
  queue->add(p);
  log(p, "arrived; added to ready queue");
  if (!inCPUBurst) {
    switchToNextProcess();
  }
}

void Simulation::switchToNextProcess() {
  Process* p = queue->pop();
  globalTime += 2;
  Time burstTime = p->getCurrentBurst().cpuBurstTime;
  log(p, "started using the CPU for " + burstTime.toString() + " burst");
  inCPUBurst = true;
  Event e = {
      .type = EventType::BurstDone,
      .time = burstTime + globalTime,
      .value =
          {
              .burst{
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
        std::to_string((b.process->numRemainingBursts() - 1));
    if (numBurstsRemaining == "0") {
      log(b.process, "terminated");
      globalTime += 2;
      return;
    }
    inCPUBurst = false;
    std::string burstWord = "burst";
    if (numBurstsRemaining != "1") {
      burstWord += "s";
    }
    log(b.process, "completed a CPU burst; " + numBurstsRemaining + " " +
                       burstWord + " to go");

    //generate IO burst
    const BurstTime& ioBurst = b.process->getCurrentBurst();
    Event e = {
        .type = EventType::BurstDone,
        .time = ioBurst.ioBurstTime + globalTime + 2,
        .value =
            {
                .burst{
                    .process = b.process,
                    .isInCpuPhase = false,
                },
            },
    };
    log(b.process->toString() +
        " switching out of the CPU; blocking on I/O until time " +
        e.time.toString());
    addEvent(e);
    Event e2 = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime + 2,
        .value = {},
    };
    addEvent(e2);
  } else {
    b.process->incrementBurst();
    queue->add(b.process);
    log(b.process, "completed I/O; added to ready queue");
  }
}
