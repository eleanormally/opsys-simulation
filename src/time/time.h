#include <cstddef>
#include <iostream>

class Time {
 private:
  size_t utime;

 public:
  Time(size_t _utime) : utime(_utime) {}
  bool isNull() const;
  bool operator<(const Time& t) const;
  friend std::ostream& operator<<(std::ostream& out, const Time& time);
};
