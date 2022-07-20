
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
    unsigned long long int total, tidle, totald, tidled;
    float utilization;
    total = LinuxParser::Jiffies(); //calculating new total usage
    tidle = LinuxParser::IdleJiffies();//calculating the new idle
    totald = total - Processor::pretotal; 
    tidled = tidle - Processor::preidle;
    utilization = 1 - (float) tidled / (float) totald;
    Processor::pretotal = total;
    Processor::preidle = tidle;
    return utilization; //this is the formula according to https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
}