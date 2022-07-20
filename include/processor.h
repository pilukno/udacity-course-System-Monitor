#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization(); 
 private:
  unsigned long long int preidle; // we define the previous idle utilization here
  unsigned long long int pretotal; // we define the previous total utilization here
};

#endif