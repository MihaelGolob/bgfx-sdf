//
// Created by MihaelGolob on 12. 08. 2024.
//

#include "Timer.h"
#include <iostream>

Timer::Timer(const std::string &name, const std::function<void(double)> &callback, bool print) {
    name_ = name;
    print_ = print;
    callback_ = callback;
    
    start_timepoint_ = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    Stop();
}

void Timer::Stop() {
    auto end_timepoint = std::chrono::high_resolution_clock::now();
    auto start_time = std::chrono::time_point_cast<std::chrono::microseconds>(start_timepoint_).time_since_epoch().count();
    auto end_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch().count();
    
    auto duration = end_time - start_time;
    double ms = duration * 0.001;
    
    if (print_) std::cout << "\033[1;36mTimer-" << name_ << ": " << duration << "us (" << ms << "ms)" << "\033[0m" << std::endl;
    if (callback_) callback_(ms);
}
