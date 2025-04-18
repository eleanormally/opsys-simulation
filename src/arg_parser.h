#pragma once
#include <cstddef>
#include <iostream>

typedef struct Arguments {
  size_t processCount;
  size_t processCountCPUBound;
  size_t seed;
  double distributionLambda;
  size_t randomNumberUpperBound;
  size_t contextSwitchMillis;
  double burstTimeAlpha;
  bool   ignoreExponential;
  size_t timeSlice;
  bool   roundRobinAlt;
  Arguments(int argc, char** argv);  //throwable
  friend std::ostream& operator<<(std::ostream& out, const Arguments& args);
} Arguments;
