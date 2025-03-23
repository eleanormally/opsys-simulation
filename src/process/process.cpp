#include "process.h"

std::ostream& operator<<(std::ostream& out, const Process& p) {
  //TODO
  std::string cpuOrIo = (p.isCpuBound()) ? "CPU" : "I/O";

  out << cpuOrIo << "-bound process " << p.id.toString() 
    << ": arrival time " << p.getArrivalTime() << "; "
    << p.getBurstCount() << " CPU bursts:" << std::endl;
  
  for (BurstTime burst : p.getAllBursts()) {
    out << "==> CPU burst " << burst.cpuBurstTime;
    if (!(burst.ioBurstTime == Time(0))) {
      out << " ==> I/O burst " << burst.ioBurstTime;
    }
    out << std::endl;
  }
  
  return out;
}