#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long minute=seconds/60;
    long hour=minute/60;
    minute%=60;
    seconds%=60;
    std::ostringstream out;
    out<<std::setw(2)<<std::setfill('0')<<hour<<":"<<
        std::setw(2)<<std::setfill('0')<<minute<<":"<<
        std::setw(2)<<std::setfill('0')<<seconds;
    return out.str();
}