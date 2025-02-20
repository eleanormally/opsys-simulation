#pragma once

#include <cstddef>
typedef struct Arguments {
  size_t processCount;
  size_t processCountCPUBound;
  size_t seed;
  int distributionLambda;
  size_t randomNumberUpperBound;
  size_t contextSwitchMillis;
  double burstTimeAlpha;
  size_t timeSlice;
  Arguments(int argc, char** argv);  //throwable
} Arguments;
