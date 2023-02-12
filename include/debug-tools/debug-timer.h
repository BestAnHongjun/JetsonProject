#ifndef __TIMER_H__
#define __TIMER_H__

#include "global_config.h"
#if TIMER_SWITCH == 1
    #define TIMER(ORDER) ORDER
#else
    #define TIMER(ORDER) {}
#endif


#if TIMER_SWITCH == 1
#include <stdint.h>
#include <string>
#include <iostream>
#include <chrono>

class Timer
{
private:
    double all_time = 0;
    double count = 0;
    size_t once_time = 0;
    std::string name;
    std::chrono::_V2::system_clock::time_point start_time, end_time;
    
public:
    Timer(std::string name)
    {
        this->name = name;
    }

    void start()
    {
        start_time = std::chrono::system_clock::now();
    }

    void end()
    {
        end_time = std::chrono::system_clock::now();
        once_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        count += 1;
        all_time += once_time;
        if (int(count) % TIMER_COUNT == 0) output();
    }

    void output()
    {
        double avg_time = all_time / count;
        std::cout << all_time << " " << count << std::endl;
        std::cout << this->name << "[" << int(count) << "]:" << avg_time << "ms, " << 1000.0 / avg_time << "FPS" << std::endl;
    }

    ~Timer()
    {
        double avg_time = all_time / count;
        std::cout << "[Summary-" << this->name << "] Average time:" << avg_time << "ms, " << 1000.0 / avg_time << "FPS" << std::endl;
    }
};
#endif

#endif  // __TIMER_H__
