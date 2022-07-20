#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

System::System(){
    vector<int> process;
    process = LinuxParser::Pids();
    for(auto pid : process){
        System::processes_.push_back(Process(pid));
    kernel_ = LinuxParser::Kernel();
    operatingSystem_ = LinuxParser::OperatingSystem();
    }
}

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 
    sort(processes_.begin(), processes_.end());
    return processes_; 
}

std::string System::Kernel() const { return kernel_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() const { return operatingSystem_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
