#pragma once

#include "process/process.h"
#include "time/time.h"
#include "simulation/simulation.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

static double divideAndRound(Time sum, Time count) {
	return divideAndRound(sum, count.getUnderlying());
} 

static double divideAndRound(Time sum, size_t count) {
	if (count == 0) {
		return 0;
	} else {
		double average = ((double) sum.getUnderlying()) / count;
		return std::ceil(average * 1000) / 1000; 
	}
}

std::string getAlgorithmName(size_t algorithmNum) {
	switch (algorithmNum) {
	    case 0:
	      return "FCFS";
	    case 1:
	      return "SJF";
	    case 2:
	      return "SRT";
	    case 3:
	      return "RR";
	    default:
	      return "ERROR";
	  }
}

void outputAlgorithm(std::ofstream &statsFile, SimulationStats stats, Time cpuBurstTime, size_t algorithmNum) {
	std::string algorithmName = getAlgorithmName(algorithmNum);
	
	statsFile << "\n\nAlgorithm " << algorithmName
		<< "\n-- CPU utilization: " << divideAndRound((cpuBurstTime*((size_t)100)), stats.totalSimulationTime.getUnderlying()) << "%"
		<< "\n-- CPU-bound average wait time: " << divideAndRound(stats.waitSum.cpuBurstTime, stats.waitCount.cpuBurstTime) << " ms"
		<< "\n-- I/O-bound average wait time: " << divideAndRound(stats.waitSum.ioBurstTime, stats.waitCount.ioBurstTime) << " ms"
		<< "\n-- overall average wait time: " << divideAndRound(stats.waitSum.cpuBurstTime + stats.waitSum.ioBurstTime, stats.waitCount.cpuBurstTime + stats.waitCount.ioBurstTime) << " ms"
		<< "\n-- CPU-bound average turnaround time: " << divideAndRound(stats.turnaroundSum.cpuBurstTime, stats.turnaroundCount.cpuBurstTime) << " ms"
		<< "\n-- I/O-bound average turnaround time: " << divideAndRound(stats.turnaroundSum.ioBurstTime, stats.turnaroundCount.ioBurstTime) << " ms"
		<< "\n-- overall average turnaround time: " << divideAndRound(stats.turnaroundSum.cpuBurstTime + stats.turnaroundSum.ioBurstTime, stats.turnaroundCount.cpuBurstTime + stats.turnaroundCount.ioBurstTime) << " ms"
		<< "\n-- CPU-bound number of context switches: " << stats.contextSwitchCount.cpuBurstTime
		<< "\n-- I/O-bound number of context switches: " << stats.contextSwitchCount.ioBurstTime
		<< "\n-- overall number of context switches: " << stats.contextSwitchCount.cpuBurstTime + stats.contextSwitchCount.ioBurstTime
		<< "\n-- CPU-bound number of preemptions: " << stats.preemptionCount.cpuBurstTime
		<< "\n-- I/O-bound number of preemptions: " << stats.preemptionCount.ioBurstTime
		<< "\n-- overall number of preemptions: " << stats.preemptionCount.cpuBurstTime + stats.preemptionCount.ioBurstTime
	;
}

static void outputAverages(const Arguments args, std::vector<Process> processes, std::vector<SimulationStats> simulationStats) {
	BurstTime cpuBoundSums;
	BurstTime ioBoundSums;
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
				cpuBoundSums.cpuBurstTime += burst.cpuBurstTime;
				cpuBoundSums.ioBurstTime += burst.ioBurstTime;
			} else {
				ioBoundSums.cpuBurstTime += burst.cpuBurstTime;
				ioBoundSums.ioBurstTime += burst.ioBurstTime;
			}
		}
	}

	size_t cpuBound = args.processCountCPUBound;
	size_t ioBound = args.processCount - args.processCountCPUBound;
	Time totalCpuUse = cpuBoundSums.cpuBurstTime + ioBoundSums.cpuBurstTime;

	std::ofstream statsFile;
	statsFile.open("simout.txt");
	statsFile << std::fixed << std::setprecision(3) 
		<< "-- number of processes: " << args.processCount
		<< "\n-- number of CPU-bound processes: " << cpuBound
		<< "\n-- number of I/O-bound processes: " << ioBound
		<< "\n-- CPU-bound average CPU burst time: " << divideAndRound(cpuBoundSums.cpuBurstTime, cpuBoundBurstCount) << " ms"
		<< "\n-- I/O-bound average CPU burst time: " << divideAndRound(ioBoundSums.cpuBurstTime, ioBoundBurstCount) << " ms"
		<< "\n-- overall average CPU burst time: " <<  divideAndRound(totalCpuUse, cpuBoundBurstCount + ioBoundBurstCount) << " ms"
		<< "\n-- CPU-bound average I/O burst time: " << divideAndRound(cpuBoundSums.ioBurstTime, cpuBoundBurstCount - cpuBound) << " ms"
		<< "\n-- I/O-bound average I/O burst time: " << divideAndRound(ioBoundSums.ioBurstTime, ioBoundBurstCount - ioBound) << " ms"
		<< "\n-- overall average I/O burst time: " << divideAndRound(cpuBoundSums.ioBurstTime + ioBoundSums.ioBurstTime, cpuBoundBurstCount + ioBoundBurstCount - cpuBound - ioBound) << " ms"
	;
	for (size_t i = 0; i < 4; i++) {
		outputAlgorithm(statsFile, simulationStats[i], totalCpuUse, i);
	}
  statsFile.close();
}

