#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include <c++/7/regex>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel,version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel>> version;
  }
  return kernel+" "+version;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;

  for(auto& p: std::filesystem::directory_iterator(kProcDirectory.c_str())){
      if (p.is_directory()){
          string filename(p.path().filename());
          if (std::all_of(filename.begin(), filename.end(), isdigit)) {
              int pid = stoi(filename);
              pids.push_back(pid);
          }
      }
  }

  return pids;
}

float LinuxParser::MemoryUtilization() {
    long totalMem(-1),freeMem(-1),buffer(-1);

    std::ifstream memoryFile(kProcDirectory+kMeminfoFilename);
    string line;
    string tmp;
    while (getline(memoryFile,line)){

        if(line.rfind("MemFree",0)==0)
        {
            std::istringstream cur(line);
            cur>>tmp>>freeMem>>tmp;
        }
        else if(line.rfind("MemAvailable",0)==0)
        {
            std::istringstream cur(line);
            cur>>tmp>>totalMem>>tmp;
        }
        else if(line.rfind("Buffers",0)==0)
        {
            std::istringstream cur(line);
            cur>>tmp>>buffer>>tmp;
        }

        if(totalMem!=-1 && freeMem!=-1 && buffer!=-1 )
            break;
    }
    buffer=std::max(buffer, static_cast<long>(0));

    return (1-(freeMem*1.0/(totalMem-buffer)));
}

long LinuxParser::UpTime() {
    float upTime;
    std::ifstream file(kProcDirectory+kUptimeFilename);
    file>>upTime;
    return static_cast<int>(upTime);
}

vector<string> LinuxParser::CpuUtilization() {
    std::ifstream statFile(kProcDirectory+kStatFilename);
    string line;
    while (getline(statFile,line)){
        if(line.rfind("cpu ",0)==0)
        {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf),end;
            return vector<string>(++beg,end);
        }
    }
    return {};
}

int LinuxParser::TotalProcesses() {

    std::ifstream statFile(kProcDirectory+kStatFilename);
    string line;
    while (getline(statFile,line)){
        if(line.rfind("processes",0)==0)
            break;
    }
    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf);
    beg++;
    return std::stoi(*beg);
}

int LinuxParser::RunningProcesses() {
    std::ifstream statFile(kProcDirectory+kStatFilename);
    string line;
    while (getline(statFile,line)){
        if(line.rfind("procs_running",0)==0)
            break;
    }
    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf);
    beg++;
    return std::stoi(*beg);
}

string LinuxParser::Command(int pid) {
    std::ifstream cmdFile(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
    string cmd;
    cmdFile>>cmd;
    return cmd;
}

std::unordered_map<string, float> LinuxParser::ParseStateFile(int pid) {
    std::ifstream statFile(kProcDirectory+to_string(pid)+kStatFilename);
    string line;
    getline(statFile,line);
    std::istringstream buf(line);
    std::istream_iterator<string> beg(buf),end;
    vector<string> data(beg,end);
    std::unordered_map<string, float> result;

    result["utime"]=stof(data[13]);
    result["stime"]=stof(data[14]);
    result["cutime"]=stof(data[15]);
    result["cstime"]=stof(data[16]);
    result["startTime"]=stof(data[21]);

    return result;
}

string LinuxParser::Ram(int pid) {
    std::ifstream statFile(kProcDirectory+std::to_string(pid)+kStatusFilename);
    string line;
    while (getline(statFile,line)){
        if(line.rfind("VmData:",0)==0)
        {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf);
            return *(++beg);
        }
    }
    return "0";
}

string LinuxParser::Uid(int pid) {
    std::ifstream statFile(kProcDirectory+std::to_string(pid)+kStatusFilename);
    string line;
    while (getline(statFile,line)){
        if(line.rfind("Uid:",0)==0)
        {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf);
            return *(++beg);
        }
    }
    return "0";
}

string LinuxParser::User(int pid) {
    auto uid=Uid(pid);
    std::ifstream file(kPasswordPath);

    string line;
    while (file>>line){
        if(line.find("x:"+uid+":")!=std::string::npos){
            return line.substr(0,line.find(":"));
        }
    }

    return "";
}


