#pragma once

#include <cstddef>
#include "../arg_parser.h"

class Random {
 private:
  size_t seed;
  int lambda;

 public:
  Random(const Arguments args);
  double next_exp() const;
  double next_uniform() const;
};
