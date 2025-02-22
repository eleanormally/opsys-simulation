#include "arg_parser.h"
#include "process/process.h"

int main(int argc, char** argv) {
  Arguments args(argc, argv);
  std::vector<Process> processes = generateProcesses(args);

  // runSimulation(args, SchedulingAlgorithm::TODO);
}
