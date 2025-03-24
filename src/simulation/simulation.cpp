#include "simulation.h"

SimulationStats Simulation::run() {
  std::cout << "time " << globalTime << ": Simulator started for " << algorithm
            << " " << *queue << std::endl;
  while (hasNextEvent()) {
    Event e = popNextEvent();
    globalTime = e.time;
    switch (e.type) {
      case EventType::ProcessArrived:
        addProcess(e.value.process);
        break;
      case EventType::BurstDone:
        if (e.value.burst.isInCpuPhase) {
          handleCpuBurst(e);
        } else {
          handleIoBurst(e);
        }
        break;
      case EventType::BurstTimeout:
        handleCpuTimeout(e);
        break;
      case EventType::ProcessSwitchIn:
        selectProcess();
        break;
      case EventType::ProcessStart:
        startProcess(e.value.process);
    }
  }
  log("Simulator ended for " + toString(algorithm));
  stats.totalSimulationTime = globalTime;
  return stats;
}

void Simulation::addProcess(Process* p) {
  addProcessToQueue(p);
  p->setTimeRemaining(p->getCurrentBurst().cpuBurstTime);
  log(p, "arrived; added to ready queue");
  if (inCPUBurst == nullptr) {
    Event e = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime,
        .value = {},
    };
    addEvent(e);
  }
}

void Simulation::addProcessToQueue(Process* p) {
  queue->add(p);
}

void Simulation::selectProcess() {
  if (queue->isEmpty() || inCPUBurst)
    return;
  Process* p = queue->pop();
  inCPUBurst = p;
  Event e = {
      .type = EventType::ProcessStart,
      .time = globalTime + args.contextSwitchMillis,
      .value =
          {
              .process = p,
          },
  };
  addEvent(e);
}

Event Simulation::generateCpuBurst(Process* p) {
  Time burstTime = p->getTimeRemaining();
  bool doTimeout = false;
  if (algorithm == SchedulingAlgorithm::RoundRobin) {
    doTimeout = burstTime > args.timeSlice;
  }
  if (doTimeout) {
    return {
        .type = EventType::BurstTimeout,
        .time = globalTime + args.timeSlice,
        .value =
            {
                .burst{
                    .process = p,
                    .isInCpuPhase = true,
                },
            },
    };
  } else {
    return {
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
  }
}
Event Simulation::generateIoBurst(Process* p) {
  const BurstTime b = p->getCurrentBurst();
  Event nextEvent = {
      .type = EventType::BurstDone,
      .time = b.ioBurstTime + globalTime + args.contextSwitchMillis,
      .value =
          {
              .burst{
                  .process = p,
                  .isInCpuPhase = false,
              },
          },
  };
  return nextEvent;
}

void Simulation::startProcess(Process* p) {
  Time burstTime = p->getTimeRemaining();
  Time totalBurstTime = p->getCurrentBurst().cpuBurstTime;
  Event e = generateCpuBurst(p);
  cpuBurstStartTime = globalTime;
  inCPUBurst = p;
  burstDoneEvent = e;
  if (totalBurstTime != burstTime) {
    log(p, "started using the CPU for remaining " + burstTime.toString() +
               " of " + totalBurstTime.toString() + " burst");
  } else {
    log(p, "started using the CPU for " + burstTime.toString() + " burst");
  }
  addEvent(e);
}

void Simulation::handleCpuTimeout(const Event& e) {
  BurstInstance b = e.value.burst;
  if (!(e == burstDoneEvent)) {
    return;
    //do not handle a preempted process
  }
  Time remaining = b.process->getTimeRemaining() - args.timeSlice;
  inCPUBurst = nullptr;
  b.process->setTimeRemaining(remaining);
  if (queue->isEmpty()) {
    log("Time slice expired; no preemption because ready queue is empty");
    inCPUBurst = b.process;
    Event e = generateCpuBurst(b.process);
    burstDoneEvent = e;
    addEvent(e);
    return;
  } else {
    log("Time slice expired; preempting process " +
        b.process->getId().toString() + " with " + remaining.toString() +
        " remaining");
    addProcessToQueue(b.process);
  }
  Event nextEvent = {
      .type = EventType::ProcessSwitchIn,
      .time = globalTime + args.contextSwitchMillis,
      .value = {},
  };
  addEvent(nextEvent);
}

void Simulation::handleCpuBurst(const Event& e) {
  BurstInstance b = e.value.burst;
  if (b.process != inCPUBurst || !(e == burstDoneEvent)) {
    return;
    //do not handle a preempted process
  }
  std::string numBurstsRemaining =
      std::to_string((b.process->numRemainingBursts() - 1));
  inCPUBurst = nullptr;
  if (numBurstsRemaining == "0") {
    log(b.process->toString() + " terminated");
    Event nextEvent = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime + args.contextSwitchMillis,
        .value = {},
    };
    addEvent(nextEvent);
    return;
  }
  std::string burstWord = "burst";
  if (numBurstsRemaining != "1") {
    burstWord += "s";
  }
  log(b.process, "completed a CPU burst; " + numBurstsRemaining + " " +
                     burstWord + " to go");

  if (algorithm == SchedulingAlgorithm::ShortestJobFirst ||
      algorithm == SchedulingAlgorithm::ShortestRemainingTime) {
    double oldTau = (double)b.process->getTau().getUnderlying();
    double cpuTime =
        (double)b.process->getCurrentBurst().cpuBurstTime.getUnderlying();
    double tauUnderlying =
        (oldTau * (1 - args.burstTimeAlpha) + cpuTime * args.burstTimeAlpha);
    Time newTau = Time(ceil(tauUnderlying));
    log("Recalculated tau for process " + b.process->getId().toString() +
        ": old tau " + b.process->getTau().toString() + " ==> new tau " +
        newTau.toString());
    b.process->setTau(newTau);
    b.process->setTimeRemaining(b.process->getTimeRemaining() - cpuTime);
  }

  //generate IO burst
  Event nextEvent = generateIoBurst(b.process);
  log(b.process->toString() +
      " switching out of CPU; blocking on I/O until time " +
      nextEvent.time.toString());
  addEvent(nextEvent);
  Event switchIn = {
      .type = EventType::ProcessSwitchIn,
      .time = globalTime + args.contextSwitchMillis,
      .value = {},
  };
  addEvent(switchIn);
}

void Simulation::handleIoBurst(const Event& e) {
  BurstInstance b = e.value.burst;
  b.process->incrementBurst();
  b.process->setTimeRemaining(b.process->getCurrentBurst().cpuBurstTime);

  addProcessToQueue(b.process);
  Time currentBurstDuration = globalTime - cpuBurstStartTime;
  Time predictedRemaining;
  if (inCPUBurst != nullptr) {
    predictedRemaining = inCPUBurst->getTau() - currentBurstDuration +
                         inCPUBurst->getTimeRemaining() -
                         inCPUBurst->getCurrentBurst().cpuBurstTime;
    if (predictedRemaining > inCPUBurst->getTau()) {
      //do not preempt for underflow
      predictedRemaining = Time(0);
    }
  }
  if (algorithm == SchedulingAlgorithm::ShortestRemainingTime &&
      predictedRemaining > b.process->getTau()) {
    log(b.process, "completed I/O; preempting " +
                       inCPUBurst->getId().toString() +
                       " (predicted remaining time " +
                       predictedRemaining.toString() + ")");
    inCPUBurst->setTimeRemaining(inCPUBurst->getTimeRemaining() -
                                 currentBurstDuration);
    addProcessToQueue(inCPUBurst);
    inCPUBurst = nullptr;
    Event e = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime + args.contextSwitchMillis,
        .value = {},
    };
    addEvent(e);
  } else {
    log(b.process, "completed I/O; added to ready queue");
    Event e = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime,
        .value = {},
    };
    addEvent(e);
  }
}
