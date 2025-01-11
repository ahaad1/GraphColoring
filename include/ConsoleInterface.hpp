#ifndef CONSOLEINTERFACE_HPP
#define CONSOLEINTERFACE_HPP

#include "GraphSolver.hpp"
#include "PerformanceTimer.hpp"


class ConsoleInterface {
private:
    GraphSolver solver;

    void runAlgorithms();

public:
    void run();
    static std::string getTimestampedFilename(const std::string& baseName);
};

#endif // CONSOLEINTERFACE_HPP