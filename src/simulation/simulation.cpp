#include "simulation.h"

void Simulation::run() {
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
      case EventType::BurstTimeout:
        handleBurst(e.value.burst, e.type == EventType::BurstTimeout);
        break;
      case EventType::ProcessSwitchIn:
        selectProcess();
        break;
      case EventType::ProcessStart:
        startProcess(e.value.process);
    }
  }
  log("Simulator ended for " + toString(algorithm));
}

void Simulation::addProcess(Process* p) {
  addProcessToQueue(p);
  p->setTimeRemaining(p->getCurrentBurst().cpuBurstTime);
  log(p, "arrived; added to ready queue");
  if (!inCPUBurst) {
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

Event generateBurstEvent(Process* p, SchedulingAlgorithm algorithm,
                         const Arguments& args, Time currentTime) {
  Time burstTime = p->getTimeRemaining();
  bool doTimeout = false;
  if (algorithm == SchedulingAlgorithm::RoundRobin) {
    doTimeout = burstTime > args.timeSlice;
  }
  if (doTimeout) {
    return {
        .type = EventType::BurstTimeout,
        .time = currentTime + args.timeSlice,
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
        .time = burstTime + currentTime,
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

void Simulation::selectProcess() {
  if (queue->isEmpty() || inCPUBurst)
    return;
  Process* p = queue->pop();
  inCPUBurst = true;
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
void Simulation::startProcess(Process* p) {
  Time burstTime = p->getTimeRemaining();
  Time totalBurstTime = p->getCurrentBurst().cpuBurstTime;
  Event e = generateBurstEvent(p, algorithm, args, globalTime);
  if (totalBurstTime != burstTime) {

    log(p, "started using the CPU for remaining " + burstTime.toString() +
               " of " + totalBurstTime.toString() + " burst");
  } else {
    log(p, "started using the CPU for " + burstTime.toString() + " burst");
  }
  addEvent(e);
}

void Simulation::handleBurst(BurstInstance& b, bool timeout) {
  if (b.isInCpuPhase) {
    std::string numBurstsRemaining =
        std::to_string((b.process->numRemainingBursts() - 1));
    inCPUBurst = false;
    if (numBurstsRemaining == "0" && !timeout) {
      log(b.process->toString() + " terminated");
      Event e = {
          .type = EventType::ProcessSwitchIn,
          .time = globalTime + args.contextSwitchMillis,
          .value = {},
      };
      addEvent(e);
      return;
    }
    std::string burstWord = "burst";
    if (numBurstsRemaining != "1") {
      burstWord += "s";
    }
    if (algorithm == SchedulingAlgorithm::ShortestJobFirst) {
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
    }
    if (timeout) {
      Time remaining = b.process->getTimeRemaining() - args.timeSlice;
      b.process->setTimeRemaining(remaining);
      if (queue->isEmpty()) {
        log("Time slice expired; no preemption because ready queue is empty");
        inCPUBurst = true;
        Event e = generateBurstEvent(b.process, algorithm, args, globalTime);
        addEvent(e);
        return;
      } else {
        log("Time slice expired; preempting process " +
            b.process->getId().toString() + " with " + remaining.toString() +
            " remaining");
        addProcessToQueue(b.process);
      }
    } else {
      log(b.process, "completed a CPU burst; " + numBurstsRemaining + " " +
                         burstWord + " to go");

      //generate IO burst
      const BurstTime& ioBurst = b.process->getCurrentBurst();
      Event e = {
          .type = EventType::BurstDone,
          .time = ioBurst.ioBurstTime + globalTime + args.contextSwitchMillis,
          .value =
              {
                  .burst{
                      .process = b.process,
                      .isInCpuPhase = false,
                  },
              },
      };
      log(b.process->toString() +
          " switching out of CPU; blocking on I/O until time " +
          e.time.toString());
      addEvent(e);
    }

    Event e = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime + args.contextSwitchMillis,
        .value = {},
    };
    addEvent(e);
  } else {
    b.process->incrementBurst();
    b.process->setTimeRemaining(b.process->getCurrentBurst().cpuBurstTime);
    addProcessToQueue(b.process);
    log(b.process, "completed I/O; added to ready queue");
    Event e = {
        .type = EventType::ProcessSwitchIn,
        .time = globalTime,
        .value = {},
    };
    addEvent(e);
  }
}
