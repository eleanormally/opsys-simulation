#include <iomanip>
#include "arg_parser.h"
#include "process/process.h"
#include "queue/ready_queue.h"
#include "simulation/simulation.h"
#include "statistics.h"

int main(int argc, char** argv) {
  Arguments args(argc, argv);
  std::cout << args << std::endl;
  std::vector<Process> processes = generateProcesses(args);
  for (const Process& p : processes) {
    std::cout << p << std::endl;
  }
  std::vector<SimulationStats> allStats;
  std::cout << "<<< PROJECT SIMULATIONS\n<<< -- t_cs="
            << args.contextSwitchMillis * 2 << "ms; alpha=";
  if (args.ignoreExponential) {
    std::cout << "<n/a>";
  } else {
    std::cout << std::fixed << std::setprecision(2) << args.burstTimeAlpha;
  }
  std::cout << "; t_slice=" << args.timeSlice << "ms";
  if (args.roundRobinAlt) {
    std::cout << "; RR_ALT";
  }
  for (SchedulingAlgorithm algorithm : listSchedulingAlgorithms()) {
    std::cout << std::endl;
    allStats.push_back(Simulation(args, algorithm, processes).run());
  }
  outputAverages(args, processes, allStats);
  return EXIT_SUCCESS;
}
