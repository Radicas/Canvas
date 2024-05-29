#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>

class Timer {
   public:
    Timer();
    ~Timer();

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point;
};

#endif // TIMER_H
