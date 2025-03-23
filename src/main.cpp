#include <iomanip>
#include "arg_parser.h"
#include "process/process.h"
#include "queue/ready_queue.h"
#include "simulation/simulation.h"

int main(int argc, char** argv) {
  Arguments args(argc, argv);
  std::cout << args << std::endl;
  std::vector<Process> processes = generateProcesses(args);
  for (const Process& p : processes) {
    std::cout << p << std::endl;
  }
  std::cout << "<<< PROJECT SIMULATIONS\n<<< -- t_cs="
            << args.contextSwitchMillis * 2 << "ms; alpha=" << std::fixed
            << std::setprecision(2) << args.burstTimeAlpha
            << "; t_slice=" << args.timeSlice << "ms\n";
  for (SchedulingAlgorithm algorithm : listSchedulingAlgorithms()) {
    Simulation(args, algorithm, processes).run();
  }
  return EXIT_SUCCESS;
}
