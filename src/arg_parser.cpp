#include "arg_parser.h"
#include <iomanip>
#include <stdexcept>
Arguments::Arguments(int argc, char** argv) {
  if (argc != 9) {
    throw std::runtime_error("Incorrect number of arguments given.");
  }
  processCount = std::atoi(argv[1]);
  if (processCount == 0 && argv[1][0] != '0') {
    throw std::runtime_error("Cannot parse given process count.");
  }
  processCountCPUBound = std::atoi(argv[2]);
  if (processCountCPUBound == 0 && argv[2][0] != '0') {
    throw std::runtime_error("Cannot parse given CPU-bound process count.");
  }
  seed = std::atoi(argv[3]);
  if (seed == 0 && argv[3][0] != '0') {
    throw std::runtime_error(
        "Cannot parse given pseudo-random generator seed.");
  }
  distributionLambda = std::atof(argv[4]);
  if (distributionLambda == 0) {
    throw std::runtime_error("Cannot parse given distribution lambda.");
  }
  randomNumberUpperBound = std::atoi(argv[5]);
  if (randomNumberUpperBound == 0 && argv[5][0] != '0') {
    throw std::runtime_error("Cannot parse given random number upper bound.");
  }
  contextSwitchMillis = std::atoi(argv[6]);
  contextSwitchMillis /= 2;
  if (contextSwitchMillis == 0 && argv[6][0] != '0') {
    throw std::runtime_error("Cannot parse given context switching time.");
  }
  burstTimeAlpha = std::atof(argv[7]);
  if (burstTimeAlpha == 0.0 && argv[7][0] != '0') {
    throw std::runtime_error("Cannot parse given burst time alpha value.");
  }
  timeSlice = std::atoi(argv[8]);
  if (timeSlice == 0 && argv[8][0] != '0') {
    throw std::runtime_error("Cannot parse given time slice.");
  }
}

std::ostream& operator<<(std::ostream& out, const Arguments& args) {
  std::string pluralCpuProcess = "";
  if (args.processCountCPUBound != 1) {
    pluralCpuProcess = "es";
  }
  out << "<<< -- process set (n=" << args.processCount << ") with "
      << args.processCountCPUBound
      << " CPU-bound process" + pluralCpuProcess + "\n";
  out << "<<< -- seed=" << args.seed << "; lambda=" << std::fixed
      << args.distributionLambda << "; bound=" << args.randomNumberUpperBound
      << std::endl;
  return out;
}
