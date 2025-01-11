/*#include "PerformanceTimer.hpp"*/
#include "../include/PerformanceTimer.hpp"

void PerformanceTimer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void PerformanceTimer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
}

double PerformanceTimer::getElapsedMilliseconds() const {
    return std::chrono::duration<double, std::milli>(endTime - startTime).count();
}


