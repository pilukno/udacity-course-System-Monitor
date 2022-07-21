#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization(); 
 private:
  unsigned long long int preidle; // previous idle utilization here
  unsigned long long int pretotal; // previous total utilization here
};

#endif