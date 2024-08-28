#include "process.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  CpuUtilization(pid);
  command_ = Command();
  ram_ = LinuxParser::Ram(pid);
  uptime_ = LinuxParser::UpTime(pid);
  user_ = LinuxParser::User(pid);
}

// Set CpuUtilization
void Process::CpuUtilization(int pid) {
  float seconds = float(LinuxParser::UpTime(pid));
  float total_time = float(LinuxParser::ActiveJiffies(pid));

  cpu_utilization_ = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_utilization_; }

// Return the command that generated this process
string Process::Command() {
  string line;
  line = LinuxParser::Command(pid_);
  return line;
}

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}