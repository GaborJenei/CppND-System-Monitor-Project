#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memTotal, MemFree;

  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      string key;
      linestream >> key;

      if (key == "MemTotal:") {
        linestream >> memTotal;
        memTotal = static_cast<float>(memTotal);
      } else if (key == "MemFree:") {
        linestream >> MemFree;
        MemFree = static_cast<float>(MemFree);
        break;
      }
    }
  }

  return (memTotal - MemFree) / memTotal;
  // return 0.0;

  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime;

  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    uptime = static_cast<long>(uptime);
  }

  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {

  long idle_jiffies = LinuxParser::IdleJiffies();
  long active_jiffies = LinuxParser::ActiveJiffies();

  return idle_jiffies + active_jiffies;
  }

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {

  // long system_uptime = LinuxParser::UpTime();

  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;
  // long start_time = 0;
  
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    vector<string> pid_stats;
    string stat;
    while (linestream >> stat) {
      pid_stats.push_back(stat);
    }

    utime = std::stol(pid_stats[13]);
    stime = std::stol(pid_stats[14]);
    cutime = std::stol(pid_stats[15]);
    cstime = std::stol(pid_stats[16]);
    // start_time = std::stol(pid_stats[21]);
  }

  long total_time = utime + stime + cutime + cstime;

  // float seconds = system_uptime - (float(start_time) / float(sysconf(_SC_CLK_TCK)));
  // float cpu_usage = 100.0 * ((float(total_time) / float(sysconf(_SC_CLK_TCK))) / seconds);

  return total_time; // cpu_usage;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  
  vector<string> jiffies = LinuxParser::CpuUtilization();

  long user = std::stol(jiffies[LinuxParser::CPUStates::kUser_]);
  long nice = std::stol(jiffies[LinuxParser::CPUStates::kNice_]);
  long system = std::stol(jiffies[LinuxParser::CPUStates::kSystem_]);
  long irq = std::stol(jiffies[LinuxParser::CPUStates::kIRQ_]);
  long softirq = std::stol(jiffies[LinuxParser::CPUStates::kSoftIRQ_]);
  long steal = std::stol(jiffies[LinuxParser::CPUStates::kSteal_]);
  long guest = std::stol(jiffies[LinuxParser::CPUStates::kGuest_]);
  long guest_nice = std::stol(jiffies[LinuxParser::CPUStates::kGuestNice_]);

  return user + nice + system + irq + softirq + steal + guest + guest_nice; 
  }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {

  vector<string> jiffies = LinuxParser::CpuUtilization();

  long idle = std::stol(jiffies[LinuxParser::CPUStates::kIdle_]);
  long iowait = std::stol(jiffies[LinuxParser::CPUStates::kIOwait_]);
  
  return idle + iowait;
  }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  vector<string> cpu_utilization;

  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      string key;
      linestream >> key;

      if (key == "cpu") {
        while (linestream >> key) {
          cpu_utilization.push_back(key);
        }
        break;
      }
    }
  }
  return cpu_utilization;
  }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int total_processes;

  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      string key;
      linestream >> key;

      if (key == "processes") {
        linestream >> total_processes;
        break;
      }
    }
  }

  return total_processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running_processes;

  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      string key;
      linestream >> key;

      if (key == "procs_running") {
        linestream >> running_processes;
        break;
      }
    }
  }

  return running_processes;
  }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
  }

  return command;
  }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string mem = "0";

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      linestream >> key;

      if (key == "VmSize:") {
        linestream >> mem;

        // Convert from KB to MB
        mem = to_string(std::stol(mem) / 1024);
        break;
      }
    }
  }
  
  return mem;
  }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      linestream >> key;

      if (key == "Uid:") {
        linestream >> key;
        break;
      }
    }
  }
  return key;
  }

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  
  string line;
  string key;

  string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      vector<string> user_info;
      string info;
      while (linestream >> info) {
        user_info.push_back(info);

        if (info == uid) {
          key = user_info[0];
          break;
        }
      }
    }
  }
  return key;
  }

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  
  long uptime = 0;

  string line;
  string key;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    
    vector<string> pid_stats;
    string stat;
    while (linestream >> stat) {
      pid_stats.push_back(stat);
    }

    uptime = std::stol(pid_stats[21]);
  }
  
  return LinuxParser::UpTime() - uptime / sysconf(_SC_CLK_TCK);
}
