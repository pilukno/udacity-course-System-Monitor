#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id) : pid_(id){ starttime_ = LinuxParser::UpTime(id); }
  int Pid() const;                               
  std::string User() const;                      
  std::string Command() const;                   
  float CpuUtilization();
  float Utilization() const;                 
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

 private:
 long int starttime_; 
 int pid_;
 float utilization_;
 std::string user_ = LinuxParser::User(pid_); 
 std::string command_ = LinuxParser::Command(pid_);
};

#endif