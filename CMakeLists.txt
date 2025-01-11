cmake_minimum_required(VERSION 3.30)
project(grapth_coloring_proj)

set(CMAKE_CXX_STANDARD 26)

add_executable(grapth_coloring_proj src/main.cpp
        include/ConsoleInterface.hpp
        src/ConsoleInterface.cpp
        include/GraphSolver.hpp
        src/GraphSolver.cpp
        include/PerformanceTimer.hpp
        src/PerformanceTimer.cpp)
