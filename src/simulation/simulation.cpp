#include "simulation.h"

void Simulation::run() {
  std::cout << globalTime << ": Siumlation started for " << algorithm << " "
            << queue << std::endl;
  while (hasNextEvent()) {
    Event e = popNextEvent();
    switch (e.type) {
      case EventType::ProcessArrived:
        addProcess(e.value.process);
        break;
      case EventType::BurstDone:
        handleBurst(e.value.burst);
      case EventType::ProcessSwitchIn:
        switchToNextProcess();
    }
  }
  log("Simulator ended for " + toString(algorithm));
}

void Simulation::addProcess(Process* p) {
  log(p, " arrived; added to ready queue");
  queue.add(p);
}

void Simulation::switchToNextProcess() {
  Process* p = queue.pop();
  globalTime += 2;
  Time burstTime = p->getCurrentBurst().cpuBurstTime;
  log(p, "started using the CPU for " + burstTime.toString() + "burst");
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
        std::to_string((b.process->numRemainingBursts() - 1) * 2);
    if (numBurstsRemaining == "0") {
      log(b.process, "terminated");
      globalTime += 2;
    }
    log(b.process, "completed a CPU burst; " + numBurstsRemaining + " to go");

    //generate IO burst
    const BurstTime& ioBurst = b.process->getCurrentBurst();
    Event e = {
        .type = EventType::BurstDone,
        .time = ioBurst.ioBurstTime + globalTime,
        .value =
            {
                .burst{
                    .process = b.process,
                    .isInCpuPhase = false,
                },
            },
    };
    log(b.process->toString() +
        "switching out of the CPU; blocking on I/O until time " +
        e.time.toString());
    addEvent(e);
    e = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime + 2,
        .value = {},
    };
    addEvent(e);
  } else {
    b.process->incrementBurst();
    queue.add(b.process);
    log(b.process, "completed I/O; added to ready queue");
  }
}
