#include "timer.h"

// 构造函数：记录开始时间
Timer::Timer() {
    start_time_point = std::chrono::high_resolution_clock::now();
}

// 析构函数：计算并打印持续时间
Timer::~Timer() {
    auto end_time_point = std::chrono::high_resolution_clock::now();
    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(start_time_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();
    auto duration = end - start;
    double ms = duration * 0.001;
    std::cout << "Duration: " << duration << "us (" << ms << "ms)" << std::endl;
}
