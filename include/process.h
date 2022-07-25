#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id) : pid_(id){ starttime_ = LinuxParser::UpTime(id); }
  int Pid() const;                               // returns process' id
  std::string User() const;                      // returns user
  std::string Command() const;                   // returns the command that created such process
  float CpuUtilization();                        // calculates & returns %cpu usage of the process
  float Utilization() const;                    // returns the usage
  std::string Ram();                             // returns usage memory
  long int UpTime();                             // returns elapsed time
  bool operator<(Process const& a) const;        // utilization_ > a.Utilization()

 private:
 long int starttime_;                           // starting time of the process
 int pid_;                                      // process' id
 float utilization_;                            // current utilization
 std::string user_ = LinuxParser::User(pid_);   // user
 std::string command_ = LinuxParser::Command(pid_); //command that created the process
};

#endif