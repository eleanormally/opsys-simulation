#include "arg_parser.h"
#include <stdexcept>
Arguments::Arguments(int argc, char** argv) {
  if (argc != 7) {
    throw std::runtime_error("Incorrect number of arguments given.");
  }
  processCount = std::atoi(argv[1]);
  if (processCount == 0) {
    throw std::runtime_error("Cannot parse given process count.");
  }
  processCountCPUBound = std::atoi(argv[2]);
  if (processCountCPUBound == 0) {
    throw std::runtime_error("Cannot parse given CPU-bound process count.");
  }
  seed = std::atoi(argv[3]);
  if (seed == 0) {
    throw std::runtime_error(
        "Cannot parse given pseudo-random generator seed.");
  }
  distributionLambda = std::atoi(argv[4]);
  if (distributionLambda == 0) {
    throw std::runtime_error("Cannot parse given distribution lambda.");
  }
  randomNumberUpperBound = std::atoi(argv[5]);
  if (randomNumberUpperBound == 0) {
    throw std::runtime_error("Cannot parse given random number upper bound.");
  }
  contextSwitchMillis = std::atoi(argv[6]);
  if (contextSwitchMillis == 0) {
    throw std::runtime_error("Cannot parse given context switching time.");
  }
  burstTimeAlpha = std::atof(argv[7]);
  if (burstTimeAlpha == 0.0) {
    throw std::runtime_error("Cannot parse given burst time alpha value.");
  }
  timeSlice = std::atoi(argv[8]);
  if (timeSlice == 0) {
    throw std::runtime_error("Cannot parse given time slice.");
  }
}
