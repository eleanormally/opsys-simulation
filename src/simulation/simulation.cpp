#include "simulation.h"

void runSimulation(const Arguments& args, SchedulingAlgorithm algorithm,
                   std::vector<Process> processes) {
  Time globalTime(0);
  ReadyQueue queue(args, algorithm);
  std::cout << globalTime << ": Siumlation started for " << algorithm << " "
            << queue << std::endl;
}
