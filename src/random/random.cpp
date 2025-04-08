#include "random.h"
#include <stdlib.h>
#include <cmath>

double Random::next_exp() const {
  if (ignoreExponential) {
    return next_uniform();
  }
  double exp;
  do {
    exp = -1 * log(next_uniform()) / lambda;
  } while (exp > upperBound);

  return exp;
}

double Random::next_uniform() const {
  return drand48();
}
