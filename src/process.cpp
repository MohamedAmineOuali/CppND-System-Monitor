#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int mPid,float sysUptime) : m_pid(mPid) {
    m_command=LinuxParser::Command(m_pid);
    m_ram=LinuxParser::Ram(m_pid);
    m_user=LinuxParser::User(m_pid);

    auto values=LinuxParser::ParseStateFile(m_pid);
    float freq=sysconf(_SC_CLK_TCK);
    auto uptime=values["uptime"];
    auto stime=values["stime"];
    auto cutime=values["cutime"];
    auto cstime=values["cstime"];
    auto starttime=values["starttime"];

    m_upTime=uptime/freq;
    auto total_time=uptime+stime+cutime+cstime;
    auto seconds=sysUptime-(starttime/freq);
    m_cpuUsage=(total_time/freq)/seconds;
}

int Process::Pid() { return m_pid; }

float Process::CpuUtilization() { return m_cpuUsage; }

string Process::Command() { return m_command; }

string Process::Ram() { return m_ram; }

string Process::User() { return m_user; }

long int Process::UpTime() { return m_upTime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }


