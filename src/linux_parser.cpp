#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <set>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::set;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line1, line2;
  string total_mem, free_mem;
  string total_val, free_val;
  float memory;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()){
    std::getline(filestream, line1);
    std::istringstream linestream1(line1);
    linestream1 >> total_mem >> total_val;
    std::getline(filestream, line2);
    std::istringstream linestream2(line2);
    linestream2 >> free_mem >> free_val;
    memory = std::stof(free_val) / std::stof(total_val);
  }
  return 1.0 - memory; 
}

long LinuxParser::UpTime() { 
  string line;
  string value;
  long utime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    utime = std::stol(value);
  }
  return utime; 
}

long LinuxParser::Jiffies() { // Read and return the number of jiffies for the system
  vector<string> utilization;
  unsigned long int total_jiffies;
  utilization = LinuxParser::CpuUtilization();
  total_jiffies = 0;
  for(auto jiffie : utilization){
    total_jiffies += std::stol(jiffie);
  }
  return total_jiffies; 
}

long LinuxParser::ActiveJiffies(int pid) { // Read and return the number of active jiffies for a PID
  string line;
  string value;
  int i;
  long total_time; //starttime;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){ //finding uptime
    std::getline(filestream, line);
    std::istringstream linestream(line);
    i = 1;
    total_time = 0;
    while(linestream >> value){
      if(i>= 14 && i<=15){ total_time += std::stol(value);}
      if(i == 15){break;}
      i++;
    }
  }
  total_time /= sysconf(_SC_CLK_TCK);
  return total_time; 
}

long LinuxParser::ActiveJiffies() { return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();}

long LinuxParser::IdleJiffies() { 
  vector<string> utilization;
  unsigned long int idle_jiffies;
  utilization = LinuxParser::CpuUtilization();
  idle_jiffies = std::stol(utilization[3]) + std::stol(utilization[4]);
  return idle_jiffies; 
}

vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string user, nice, system, idle, iowait, irq, softirq, steal;
  string cpu;
  std::vector<string> utilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    utilization = {user, nice, system, idle, iowait, irq, softirq, steal};
  }
  return utilization; 
}

int LinuxParser::TotalProcesses() { // Read and return the total number of processes
  string line;
  string value;
  string initial;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> initial >> value;
    if(initial == "processes"){ return stoi(value);}
  }
  return 0; 
}

int LinuxParser::RunningProcesses() { // Read and return the number of running processes
  string line;
  string value;
  string initial;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> initial >> value;
    if(initial == "procs_running"){ 
      return stoi(value);
      break;}
  } 
  return 0;
}

string LinuxParser::Command(int pid) { // Read and return the command associated with a process
  string line;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
  }
  return line; 
}

string LinuxParser::Ram(int pid) { // Read and return the memory used by a process
  string line;
  string key, value; 
  float memory;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  while (filestream.is_open()){ //finding memory
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    if(key == "VmSize:"){ 
      memory = std::stof(value)/ 1000;
      return  std::to_string(memory).substr(0, 6); //returning up to 6 chars
      break; }
  }
  return 0; 
}

string LinuxParser::Uid(int pid) { // Read and return the user ID associated with a process
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  while (filestream.is_open()){ //finding Uid
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    if(key == "Uid:"){ 
      return value; 
      break; }
  }
  return 0; 
}

string LinuxParser::User(int pid) { // Read and return the user associated with a process
  string line;
  string user, value, uid, x;
  uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  while(filestream.is_open()){ //finding user from /etc/Psswd
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> user >> x >> value;
    if(value == uid){
      return user;
      break;}
  }
  return ""; 
}

long LinuxParser::UpTime(int pid) { // Read and return the uptime of a process
  string line;
  string value;
  int i;
  long startime;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){ //finding uptime
    std::getline(filestream, line);
    std::istringstream linestream(line);
    i = 1;
    while(linestream >> value){
      if(i==22){
        startime = std::stol(value);
        break;
      }
      i++;
    }
  }
  return startime / sysconf(_SC_CLK_TCK); 
  }
