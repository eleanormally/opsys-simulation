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
static double divideAndRound(Time sum, Time count) {
	return divideAndRound(sum, count.getUnderlying());
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

static void outputAverages(const Arguments args, std::vector<Process> processes, std::vector<SimulationStats> simulationStats) {
	size_t contextSwitch = 2*args.contextSwitchMillis;
	size_t hundred = 100;
	BurstTime cpuBoundSums;
	BurstTime ioBoundSums;
	BurstTime burstCounts;

	for (Process p : processes) {
		if (p.isCpuBound()) {
			burstCounts.cpuBurstTime += p.getBurstCount();
		} else {
			burstCounts.ioBurstTime += p.getBurstCount();
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
		<< "\n-- CPU-bound average CPU burst time: " << divideAndRound(cpuBoundSums.cpuBurstTime, burstCounts.cpuBurstTime) << " ms"
		<< "\n-- I/O-bound average CPU burst time: " << divideAndRound(ioBoundSums.cpuBurstTime, burstCounts.ioBurstTime) << " ms"
		<< "\n-- overall average CPU burst time: " <<  divideAndRound(totalCpuUse, burstCounts.cpuBurstTime + burstCounts.ioBurstTime) << " ms"
		<< "\n-- CPU-bound average I/O burst time: " << divideAndRound(cpuBoundSums.ioBurstTime, burstCounts.cpuBurstTime - cpuBound) << " ms"
		<< "\n-- I/O-bound average I/O burst time: " << divideAndRound(ioBoundSums.ioBurstTime, burstCounts.ioBurstTime - ioBound) << " ms"
		<< "\n-- overall average I/O burst time: " << divideAndRound(cpuBoundSums.ioBurstTime + ioBoundSums.ioBurstTime, burstCounts.cpuBurstTime + burstCounts.ioBurstTime - cpuBound - ioBound) << " ms"
	;

	for (size_t i = 0; i < 4; i++) {
		// Calculate turnaround as (# cpu bursts + # preemptions) * (context switch time) + wait time + burst time
		BurstTime turnaroundSums;
		turnaroundSums.cpuBurstTime = simulationStats[i].waitSum.cpuBurstTime + (burstCounts.cpuBurstTime + simulationStats[i].preemptionCount.cpuBurstTime)*contextSwitch + cpuBoundSums.cpuBurstTime;
		turnaroundSums.ioBurstTime = simulationStats[i].waitSum.ioBurstTime + (burstCounts.ioBurstTime + simulationStats[i].preemptionCount.ioBurstTime)*contextSwitch + ioBoundSums.cpuBurstTime;

		statsFile << "\n\nAlgorithm " << simulationStats[i].algorithmString
			<< "\n-- CPU utilization: " << divideAndRound(totalCpuUse*hundred, simulationStats[i].totalSimulationTime.getUnderlying()) << "%"
			<< "\n-- CPU-bound average wait time: " << divideAndRound(simulationStats[i].waitSum.cpuBurstTime, burstCounts.cpuBurstTime) << " ms"
			<< "\n-- I/O-bound average wait time: " << divideAndRound(simulationStats[i].waitSum.ioBurstTime, burstCounts.ioBurstTime) << " ms"
			<< "\n-- overall average wait time: " << divideAndRound(simulationStats[i].waitSum.cpuBurstTime + simulationStats[i].waitSum.ioBurstTime, burstCounts.cpuBurstTime + burstCounts.ioBurstTime) << " ms"
			<< "\n-- CPU-bound average turnaround time: " << divideAndRound(turnaroundSums.cpuBurstTime, burstCounts.cpuBurstTime) << " ms"
			<< "\n-- I/O-bound average turnaround time: " << divideAndRound(turnaroundSums.ioBurstTime, burstCounts.ioBurstTime) << " ms"
			<< "\n-- overall average turnaround time: " << divideAndRound(turnaroundSums.cpuBurstTime + turnaroundSums.ioBurstTime, burstCounts.cpuBurstTime + burstCounts.ioBurstTime) << " ms"
			<< "\n-- CPU-bound number of context switches: " << simulationStats[i].contextSwitchCount.cpuBurstTime.getUnderlying()
			<< "\n-- I/O-bound number of context switches: " << simulationStats[i].contextSwitchCount.ioBurstTime.getUnderlying()
			<< "\n-- overall number of context switches: " << simulationStats[i].contextSwitchCount.cpuBurstTime.getUnderlying() + simulationStats[i].contextSwitchCount.ioBurstTime.getUnderlying()
			<< "\n-- CPU-bound number of preemptions: " << simulationStats[i].preemptionCount.cpuBurstTime.getUnderlying()
			<< "\n-- I/O-bound number of preemptions: " << simulationStats[i].preemptionCount.ioBurstTime.getUnderlying()
			<< "\n-- overall number of preemptions: " << simulationStats[i].preemptionCount.cpuBurstTime.getUnderlying() + simulationStats[i].preemptionCount.ioBurstTime.getUnderlying()
		;

		if (simulationStats[i].algorithmString == toString(SchedulingAlgorithm::RoundRobin)) {
			statsFile
			<< "\n-- CPU-bound percentage of CPU bursts completed within one time slice: " << divideAndRound(simulationStats[i].roundRobinSliceCount.cpuBurstTime*hundred, burstCounts.cpuBurstTime) << "%"
			<< "\n-- I/O-bound percentage of CPU bursts completed within one time slice: " << divideAndRound(simulationStats[i].roundRobinSliceCount.ioBurstTime*hundred, burstCounts.ioBurstTime) << "%"
			<< "\n-- overall percentage of CPU bursts completed within one time slice: " << divideAndRound(simulationStats[i].roundRobinSliceCount.cpuBurstTime*hundred + simulationStats[i].roundRobinSliceCount.ioBurstTime*hundred, burstCounts.cpuBurstTime + burstCounts.ioBurstTime) << "%"
			;
		}

	}
  statsFile.close();
}

