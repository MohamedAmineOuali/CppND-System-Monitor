#include "linux_parser.h"
#include "processor.h"

float Processor::Utilization() {
    std::vector<float> current;
    auto cpuStat=LinuxParser::CpuUtilization();

    std::transform(cpuStat.begin(),cpuStat.end(),std::back_inserter(current),[](std::string const &str){
        return std::stof(str);
    });

    if(previous.empty()){
        previous=current;
        return 0;
    }

    float activeTime=getSysActiveCpuTime(current)-getSysActiveCpuTime(previous);
    float idleTime=getSysIdleCpuTime(current)-getSysIdleCpuTime(previous);
    previous=current;
    float totalTime=activeTime+idleTime;
    return activeTime/totalTime;
}

using namespace LinuxParser;

float Processor::getSysActiveCpuTime(std::vector<float> cpuStat) {
    return cpuStat[kUser_]+
            cpuStat[kNice_]+
            cpuStat[kSystem_]+
            cpuStat[kIRQ_]+
            cpuStat[kSteal_]+
            cpuStat[kGuest_]+
            cpuStat[kGuestNice_];
}

float Processor::getSysIdleCpuTime(std::vector<float> cpuStat) {
    return cpuStat[kIdle_]+
            cpuStat[kIOwait_];
}


