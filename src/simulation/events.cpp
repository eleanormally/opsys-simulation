#include "simulation.h"

ID Event::getId() const {
  if (type == EventType::ProcessArrived) {
    return value.process->getId();
  } else {
    return value.burst.process->getId();
  }
}

bool Event::operator<(const Event& e) const {

  if (time == e.time) {
    return getId() < e.getId();
  }
  return time < e.time;
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
