#include <cstddef>
#include <iostream>

class Time {
 private:
  size_t utime;

 public:
  Time(size_t _utime) : utime(_utime) {}
  Time() : utime(0) {}
  bool isNull() const;
  bool operator<(const Time& t) const;
  bool operator==(const Time& t) const;
  bool operator!=(const Time& t) const;
  void operator+=(const size_t);
  void operator+=(const Time&);
  friend Time operator+(Time, const Time&);

  std::string toString() const;

  friend std::ostream& operator<<(std::ostream& out, const Time& time);
};