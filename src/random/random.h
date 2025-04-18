#pragma once

#include <stdlib.h>
#include <cstddef>
#include "../arg_parser.h"

class Random {
 private:
  size_t seed;
  double lambda;
  size_t upperBound;

 public:
  Random(const Arguments args)
      : seed(args.seed),
        lambda(args.distributionLambda),
        upperBound(args.randomNumberUpperBound) {
    srand48(seed);
  }
  double next_exp() const;
  double next_uniform() const;
};
