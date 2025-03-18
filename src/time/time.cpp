#include "time.h"

bool Time::operator<(const Time& t) const {
  return utime < t.utime;
}
std::ostream& operator<<(std::ostream& out, const Time& time) {
  out << "time " << time.utime;
  return out;
}
