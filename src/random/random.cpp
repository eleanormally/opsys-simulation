#include "random.h"
#include <cmath>
#include <stdlib.h>

double Random::next_exp() const {
  double exp;
  do {
    exp  = -1 * log(next_uniform()) / lambda;
  } while (exp > upperBound);
  
  return exp;
}

double Random::next_uniform() const {
  return drand48();
}