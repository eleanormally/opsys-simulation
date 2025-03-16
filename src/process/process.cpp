
#include "process.h"

std::ostream& operator<<(std::ostream& out, const Process& p) {
  //TODO
  out << "process " << p.id.toString();
  return out;
}
