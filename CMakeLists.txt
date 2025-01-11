cmake_minimum_required(VERSION 3.30)
project(grapth_coloring_proj)

set(CMAKE_CXX_STANDARD 26)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++26 -fopenmp")

# Находим OpenMP
find_package(OpenMP REQUIRED)

# Добавляем OpenMP к проекту
add_executable(grapth_coloring_proj src/main.cpp
        include/ConsoleInterface.hpp
        src/ConsoleInterface.cpp
        include/GraphSolver.hpp
        src/GraphSolver.cpp
        include/PerformanceTimer.hpp
        src/PerformanceTimer.cpp)

## Подключаем OpenMP
target_link_libraries(grapth_coloring_proj PRIVATE OpenMP::OpenMP_CXX)