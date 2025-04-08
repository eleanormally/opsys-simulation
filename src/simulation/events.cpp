#include "simulation.h"

ID Event::getId() const {
  if (type == EventType::ProcessArrived) {
    return value.process->getId();
  } else if (type != EventType::ProcessSelect) {
    return value.burst.process->getId();
  }
  return ID('Z', 'Z');
}

/* ORDER
 *            CPU Burst | Process Start | IO Burst | Process Arrival
 * Process Enqueue | Timeout Burst                       Process Select
 *
 */
int Event::getOrder() const {
  switch (type) {
    case EventType::BurstDone:
      if (value.burst.isInCpuPhase) {
        return 6;
      } else {
        return 3;
      }
      break;
    case EventType::BurstTimeout:
      return 5;
      break;
    case EventType::ProcessSelect:
      return 1;
      break;
    case EventType::ProcessStart:
      return 4;
      break;
    case EventType::ProcessArrived:
      return 2;
      break;
    case EventType::ProcessEnqueue:
      return 7;
      break;
    case EventType::ProcessEnqueueFront:
      return 0;
      break;
  }
  return 1000;  //THIS NEVER ACTUALLY HAPPENS GCC JUST CANT STATICALLY ANALYZE
}

bool Event::operator<(const Event& e) const {
  if (time != e.time) {
    return e.time < time;
  }
  int thisOrder = getOrder();
  int eOrder = e.getOrder();
  if (thisOrder != eOrder) {
    return thisOrder < eOrder;
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

void Simulation::addEvent(Event e) {
  if (e.type == EventType::ProcessSelect) {
    if (selectionStarted) {
      return;
    }
    selectionStarted = true;
  }
  events.push(e);
}
bool Event::operator==(const Event& e) const {
  return time == e.time && type == e.type && getId() == e.getId();
}
