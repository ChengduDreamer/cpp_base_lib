#pragma once

#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>

namespace yk
{

class TimeExt {
public:
    static uint64_t GetCurrentTimestamp() {
        std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp
                = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
        std::time_t timestamp = tp.time_since_epoch().count();
        return timestamp;
    }

    static std::string FormatTimestamp(uint64_t time) {
        std::time_t t(time);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%F %T");
        return ss.str();
    }

    static uint64_t GetCurrentTimePointUS() {
        auto now = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
        return microseconds;
    }
};

}


