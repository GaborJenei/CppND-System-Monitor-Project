#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  
  void Update();

  long int CurrentTotal();
  long int CurrentActive();
  long int CurrentIdle();

  // TODO: Declare any necessary private members
 private:
   long int prevIdle = 0;
    long int prevActive = 0;
    long int prevTotal = 0;

    long int curIdle = 0;
    long int curActive = 0;
    long int curTotal = 0;
};

#endif