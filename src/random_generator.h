#pragma once

#include <cstddef>
class RNGenerator {
 private:
  size_t seed;

 public:
  RNGenerator(size_t _seed) : seed(_seed) {};
  size_t next_exp();
};
