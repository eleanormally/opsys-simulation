#include "../random/random.h"
#include "process.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

ID intToID(int x) {
  return ID(65 + (x / 10), 48 + (x % 10));
}

std::vector<Process> generateProcesses(const Arguments args) {
  Random random = Random(args);
  std::vector<Process> processes;
  Time tau = Time(std::ceil(1 / args.distributionLambda));

  for (size_t i = 0; i < args.processCount; i++) {
    bool isCpuBound = (i < args.processCountCPUBound);
    ID id = intToID(i);
    Time arrival = std::floor(random.next_exp());
    size_t numBursts = std::ceil(random.next_uniform() * 32);
    std::vector<BurstTime> bursts;
    for (size_t j = 0; j < numBursts; j++) {
      size_t cpuTime = std::ceil(random.next_exp());
      size_t ioTime = (j != (numBursts - 1)) ? std::ceil(random.next_exp()) : 0;
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
  outputAverages(args, processes);
  return processes;
}

double divideAndRound(double sum, size_t count) {
	if (count == 0) {
		return 0;
	} else {
		double average = sum / count;
		return std::ceil(average * 1000) / 1000; 
	}
}

void outputAverages(const Arguments args, std::vector<Process> processes) {
	double cpuBoundCpuBurst = 0;
	double ioBoundCpuBurst = 0;
	double cpuBoundIoBurst = 0;
	double ioBoundIoBurst = 0;
	size_t ioBoundBurstCount = 0;
	size_t cpuBoundBurstCount = 0;

	for (Process p : processes) {
		if (p.isCpuBound()) {
			cpuBoundBurstCount += p.getBurstCount();
		} else {
			ioBoundBurstCount += p.getBurstCount();
		}
		for (BurstTime burst : p.getAllBursts()){
			if (p.isCpuBound()) {
				cpuBoundCpuBurst += burst.cpuBurstTime.getUnderlying();
				cpuBoundIoBurst += burst.ioBurstTime.getUnderlying();
			} else {
				ioBoundCpuBurst += burst.cpuBurstTime.getUnderlying();
				ioBoundIoBurst += burst.ioBurstTime.getUnderlying();
			}
		}
	}

	size_t cpuBound = args.processCountCPUBound;
	size_t ioBound = args.processCount - args.processCountCPUBound;

	std::ofstream statsFile;
	statsFile.open("simout.txt");
	statsFile << std::fixed << std::setprecision(3) 
		<< "-- number of processes: " << args.processCount
		<< "\n-- number of CPU-bound processes: " << cpuBound
		<< "\n-- number of I/O-bound processes: " << ioBound
		<< "\n-- CPU-bound average CPU burst time: " << divideAndRound(cpuBoundCpuBurst, cpuBoundBurstCount) << " ms"
		<< "\n-- I/O-bound average CPU burst time: " << divideAndRound(ioBoundCpuBurst, ioBoundBurstCount) << " ms"
		<< "\n-- overall average CPU burst time: " <<  divideAndRound(cpuBoundCpuBurst + ioBoundCpuBurst, cpuBoundBurstCount + ioBoundBurstCount) << " ms"
		<< "\n-- CPU-bound average I/O burst time: " << divideAndRound(cpuBoundIoBurst, cpuBoundBurstCount - cpuBound) << " ms"
		<< "\n-- I/O-bound average I/O burst time: " << divideAndRound(ioBoundIoBurst, ioBoundBurstCount - ioBound) << " ms"
		<< "\n-- overall average I/O burst time: " << divideAndRound(cpuBoundIoBurst + ioBoundIoBurst, cpuBoundBurstCount + ioBoundBurstCount - cpuBound - ioBound) << " ms"
	;
  statsFile.close();
}
