#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
public:
    System();

    void refresh();

    Processor &Cpu();
    std::vector<Process> &Processes();

    float MemoryUtilization();

    long UpTime();

    int TotalProcesses();

    int RunningProcesses();

    std::string Kernel();

    std::string OperatingSystem();

private:
    Processor m_cpu = {};
    std::vector<Process> m_processes = {};
    std::string m_kernal;
    std::string m_os;
    int m_totalProcess;
    int m_runningProcess;
    long m_upTime;
    float m_memory;

};

#endif