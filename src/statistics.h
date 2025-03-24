#pragma once

#include "process/process.h"
#include "time/time.h"
#include "simulation/simulation.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>

static double divideAndRound(Time sum, size_t count) {
	if (count == 0) {
		return 0;
	} else {
		double average = ((double) sum.getUnderlying()) / count;
		return std::ceil(average * 1000) / 1000; 
	}
}

void outputAlgorithm(std::ofstream &statsFile, SimulationStats stats, Time cpuBurstTime) {
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

	std::ofstream statsFile;
	statsFile.open("simout.txt");
	statsFile << std::fixed << std::setprecision(3) 
		<< "-- number of processes: " << args.processCount
		<< "\n-- number of CPU-bound processes: " << cpuBound
		<< "\n-- number of I/O-bound processes: " << ioBound
		<< "\n-- CPU-bound average CPU burst time: " << divideAndRound(cpuBoundSums.cpuBurstTime, cpuBoundBurstCount) << " ms"
		<< "\n-- I/O-bound average CPU burst time: " << divideAndRound(ioBoundSums.cpuBurstTime, ioBoundBurstCount) << " ms"
		<< "\n-- overall average CPU burst time: " <<  divideAndRound(cpuBoundSums.cpuBurstTime + ioBoundSums.cpuBurstTime, cpuBoundBurstCount + ioBoundBurstCount) << " ms"
		<< "\n-- CPU-bound average I/O burst time: " << divideAndRound(cpuBoundSums.ioBurstTime, cpuBoundBurstCount - cpuBound) << " ms"
		<< "\n-- I/O-bound average I/O burst time: " << divideAndRound(ioBoundSums.ioBurstTime, ioBoundBurstCount - ioBound) << " ms"
		<< "\n-- overall average I/O burst time: " << divideAndRound(cpuBoundSums.ioBurstTime + ioBoundSums.ioBurstTime, cpuBoundBurstCount + ioBoundBurstCount - cpuBound - ioBound) << " ms"
	;
	outputAlgorithm(statsFile, simulationStats[0], cpuBoundSums.cpuBurstTime);
  statsFile.close();
}

