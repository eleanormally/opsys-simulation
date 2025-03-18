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
  }
}

void Simulation::addProcess(Process* p) {
  log("Process " + p->getId().toString() + " arrived; added to ready queue");
  queue.add(p);
}
