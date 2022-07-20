
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor::Processor(){
    preidle = LinuxParser::IdleJiffies();
    pretotal = LinuxParser::ActiveJiffies();
}

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  //  std::vector<string> cpu_utilization;
  //  long idle, iowait;
    unsigned long long int total, tidle, totald, tidled;
    float utilization;
  //  cpu_utilization = LinuxParser::CpuUtilization();
    total = LinuxParser::Jiffies();
 //   for (int i = 0; i < 8; i++){ //calculating the new total time
   //     total += std::stol(cpu_utilization[i]);
    //}
  //  idle = std::stol(cpu_utilization[3]);
 //   iowait = std::stol(cpu_utilization[4]);
    tidle = LinuxParser::IdleJiffies();//idle + iowait; //calculating the new idle
    totald = total - Processor::pretotal; 
    tidled = tidle - Processor::preidle;
    utilization = 1 - (float) tidled / (float) totald;
    Processor::pretotal = total;
    Processor::preidle = tidle;
    return utilization; //this is the formula according to https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
}