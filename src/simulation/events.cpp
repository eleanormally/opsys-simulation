#include "simulation.h"

ID Event::getId() const {
  if (type == EventType::ProcessArrived) {
    return value.process->getId();
  } else {
    return value.burst.process->getId();
  }
}
int Event::getOrder() const {

  switch (type) {
    case EventType::BurstDone:
      if (value.burst.isInCpuPhase) {
        return 4;
      } else {
        return 1;
      }
      break;
    case EventType::BurstTimeout:
      return 3;
    case EventType::ProcessSwitchIn:
      return 2;
      break;
    case EventType::ProcessArrived:
      return 0;
  }
}

bool Event::operator<(const Event& e) const {
  if (time != e.time) {
    return e.time < time;
  }
  int thisOrder = getOrder();
  int eOrder = e.getOrder();
  if (thisOrder != eOrder) {
    return eOrder < thisOrder;
  }
  return e.getId() < getId();
}

Event Simulation::popNextEvent() {
  Event e = std::move(events.top());
  events.pop();
  return e;
}

const Event& Simulation::peekNextEvent() {
  return events.top();
}

bool Simulation::hasNextEvent() const {
  return !events.empty();
}

void Simulation::addEvent(Event& e) {
  events.push(e);
}
