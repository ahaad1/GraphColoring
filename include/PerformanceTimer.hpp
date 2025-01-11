#ifndef PERFORMANCETIMER_HPP
#define PERFORMANCETIMER_HPP

#include <chrono>

class PerformanceTimer {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

public:
    void start();
    void stop();
    [[nodiscard]] double getElapsedMilliseconds() const;
};

#endif //PERFORMANCETIMER_HPP
