#include "random.h"
#include <cmath>

double Random::next_exp() const {
  return -1 * log(next_uniform()) / lambda;
}
double Random::next_uniform() const {
  //TODO
  return 0;
}
