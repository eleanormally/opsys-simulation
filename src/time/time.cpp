#include "time.h"

std::ostream& operator<<(std::ostream& out, const Time& time) {
  out << "time " << time.utime;
  return out;
}
