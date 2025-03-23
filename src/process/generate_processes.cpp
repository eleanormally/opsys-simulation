#include "process.h"
#include "../random/random.h"
#include <math.h>

ID intToID(int x) {
  return ID(65 + (x/10), 48 + (x%10));
}

std::vector<Process> generateProcesses(const Arguments args) {
  Random random = Random(args);
  std::vector<Process> processes;
  Time tau = Time(std::ceil(1/args.distributionLambda));

  for (size_t i = 0; i < args.processCount; i++) {
    bool isCpuBound = (i < args.processCountCPUBound);
    ID id = intToID(i);
    Time arrival = std::floor(random.next_exp());
    size_t numBursts = std::ceil(random.next_uniform() * 32);
    std::vector<BurstTime> bursts;
    for (size_t j = 0; j < numBursts; j++) {
      size_t cpuTime = std::ceil(random.next_exp());
      size_t ioTime = (j != (numBursts-1)) ? std::ceil(random.next_exp()) : 0;
      if (isCpuBound) {
        // CPU bound
        cpuTime *= 4;
      } else {
        // IO bound
        ioTime *= 8;
      }
      bursts.push_back(BurstTime{cpuTime, ioTime});
    }
    Process p(id, arrival, numBursts, bursts, tau, isCpuBound);
    processes.push_back(p);
  }
  return processes;
}

