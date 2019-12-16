#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(int mPid,float sysUptime);
    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
    int m_pid{0};
    std::string m_user;
    std::string m_command;
    float m_cpuUsage{0};
    std::string m_ram;
    long m_upTime{0};
};

#endif