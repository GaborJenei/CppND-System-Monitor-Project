#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include <linux_parser.h>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
    pid_ = pid;
    // cpu_utilization_ = 0; 
    cpu_utilization_ = float(LinuxParser::ActiveJiffies(pid));
    command_ = Command();
    ram_ = LinuxParser::Ram(pid);
    uptime_ = LinuxParser::UpTime(pid);
    user_ = LinuxParser::User(pid);
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

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return CpuUtilization() < a.CpuUtilization();
}