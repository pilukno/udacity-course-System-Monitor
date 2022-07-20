#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu();                  //returns system's CPU
  std::vector<Process>& Processes();  //returns Processes sorted w.r.t. their cpu usage
  float MemoryUtilization();          // returns memory utilization
  long UpTime();                      // returns up time
  int TotalProcesses();               // returns total processes
  int RunningProcesses();             // returns processes running
  std::string Kernel() const;               // returns kernel
  std::string OperatingSystem() const;      

 private:
  std::string kernel_;              
  std::string operatingSystem_;
  Processor cpu_;
  std::vector<Process> processes_{};
};

#endif