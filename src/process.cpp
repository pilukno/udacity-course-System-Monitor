#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() const { return pid_; }


float Process::CpuUtilization(){ // Return this process's CPU utilization
    float seconds, active_jiffies;
    seconds = Process::UpTime();
    active_jiffies = LinuxParser::ActiveJiffies(pid_);
    utilization_ = active_jiffies / seconds;
    return utilization_;
}

float Process::Utilization() const { return utilization_;}

string Process::Command() const { return command_; }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() const { return user_; }

// we define Up time of a process as the elapsed time.
long int Process::UpTime() { return LinuxParser::UpTime() - starttime_;}

bool Process::operator<(Process const& a) const { return Process::Utilization() > a.Process::Utilization();}