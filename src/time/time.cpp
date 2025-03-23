#include "time.h"

bool Time::operator<(const Time& t) const {
  return utime < t.utime;
}
bool Time::operator==(const Time& t) const {
  return utime == t.utime;
}
void Time::operator+=(const size_t increment) {
  utime += increment;
}
void Time::operator+=(const Time& increment) {
  utime += increment.utime;
}
Time operator+(Time l, const Time& r) {
  l += r;
  return l;
}

std::string Time::toString() const {
  return std::to_string(utime) + "ms";
}

std::ostream& operator<<(std::ostream& out, const Time& time) {
  out << time.toString();
  return out;
}