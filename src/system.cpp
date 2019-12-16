#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>


#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


System::System() {
    m_os=LinuxParser::OperatingSystem();
    m_kernal=LinuxParser::Kernel();
}

void System::refresh() {
    m_memory=LinuxParser::MemoryUtilization();
    m_runningProcess=LinuxParser::RunningProcesses() ;
    m_totalProcess=LinuxParser::TotalProcesses();
    m_upTime=LinuxParser::UpTime();

    auto&& pids=LinuxParser::Pids();

    m_processes.clear();
    for(auto& id:pids){
        m_processes.emplace_back(id,m_upTime);
    }



}

Processor& System::Cpu() { return m_cpu; }

vector<Process>& System::Processes() { return m_processes; }

std::string System::Kernel() { return m_kernal; }

float System::MemoryUtilization() { return m_memory; }

std::string System::OperatingSystem() { return m_os; }

int System::RunningProcesses() { return m_runningProcess;}

int System::TotalProcesses() { return m_totalProcess; }

long int System::UpTime() { return m_upTime; }


