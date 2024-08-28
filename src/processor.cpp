#include "processor.h"

#include <linux_parser.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  Update();

  float deltaTotal = float(curTotal) - float(prevTotal);
  float deltaIdle = float(curIdle) - float(prevIdle);

  float utilization = (deltaTotal - deltaIdle) / deltaTotal;
  return utilization;
}

void Processor::Update() {
  prevIdle = curIdle;
  prevActive = curActive;
  prevTotal = curTotal;

  curIdle = CurrentIdle();
  curActive = CurrentActive();
  curTotal = CurrentTotal();
}

long int Processor::CurrentTotal() { return LinuxParser::Jiffies(); }
long int Processor::CurrentActive() { return LinuxParser::ActiveJiffies(); }
long int Processor::CurrentIdle() { return LinuxParser::IdleJiffies(); }