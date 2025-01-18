#include "../include/ConsoleInterface.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

// платформо-зависимые макросы
#ifdef _WIN32
#include <windows.h>
#define COLOR_RED FOREGROUND_RED | FOREGROUND_INTENSITY
#define COLOR_GREEN FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_YELLOW FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
#define COLOR_RESET FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#else
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RESET "\033[0m"
#endif

// Функция для установки цвета текста в терминале
#ifdef _WIN32
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#else
void setConsoleColor(const char* color) {
    std::cout << color;
}
#endif

// Функция для сброса цвета текста в терминале
void resetConsoleColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, COLOR_RESET);
#else
    std::cout << COLOR_RESET;
#endif
}

std::string ConsoleInterface::getTimestampedFilename(const std::string& baseName) {
    std::ostringstream oss;
    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);
    oss << baseName << "_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".dot";
    return oss.str();
}

void ConsoleInterface::runAlgorithms() {
    std::vector<GraphSolver::AlgorithmResult> results;
    std::ofstream outFile("results.csv");
    if (!outFile.is_open()) throw std::runtime_error("file open err \n");

    outFile << "Algorithm,TimeMs,Success,ColorsUsed,Steps,ColoringValid\n"; // Заголовок CSV файла

    PerformanceTimer timer;
    bool success = false;
    int64_t stepCount = 0;
    bool coloringValid = false;

    // Greedy Algorithm
    std::cout << "Running Greedy Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveGreedy();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Greedy Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Greedy Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Parallel Greedy Algorithm
    std::cout << "Running Parallel Greedy Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveParallelGreedy();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Parallel Greedy Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Parallel Greedy Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // DSATUR Algorithm
    std::cout << "Running DSATUR Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveDSATUR();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"DSATUR Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "DSATUR Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Parallel DSATUR Algorithm
    std::cout << "Running Parallel DSATUR Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveParallelDSATUR();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Parallel DSATUR Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Parallel DSATUR Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Welsh-Powell Algorithm
    std::cout << "Running Welsh-Powell Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveWelshPowell();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Welsh-Powell Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Welsh-Powell Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Parallel Welsh-Powell Algorithm
    std::cout << "Running Parallel Welsh-Powell Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveParallelWelshPowell();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Parallel Welsh-Powell Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Parallel Welsh-Powell Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Custom Algorithm
    std::cout << "Running Custom Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveCustomAlgorithm();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Custom Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Custom Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Parallel Custom Algorithm
    std::cout << "Running Parallel Custom Algorithm...\n";
    timer.start();
    std::tie(success, stepCount) = solver.solveParallelCustomAlgorithm();
    timer.stop();
    coloringValid = solver.isColoringValid();
    results.push_back({"Parallel Custom Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount(), stepCount, coloringValid});
    outFile << "Parallel Custom Algorithm," << timer.getElapsedMilliseconds() << "," << success << "," << solver.getColorCount() << "," << stepCount << "," << coloringValid << "\n";

    // Output results
    setConsoleColor(COLOR_YELLOW);
    std::cout << "\n================ Results ================\n";
    resetConsoleColor();
    for (const auto& [name, timeMs, success, colorsUsed, steps, coloringValid] : results) {
        std::cout << name << ": "
                  << (success ? "Completed successfully" : "Failed")
                  << " in " << timeMs << " ms. "
                  << "Colors used: " << (success ? colorsUsed : -1) << " "
                  << "Steps: " << steps << " "
                  << "Coloring valid: " << (coloringValid ? "Yes" : "No") << "\n";
    }
    outFile.close();
}

void ConsoleInterface::runBenchmarks() {
    std::ofstream outFile("benchmark_results_small.csv");
    if (!outFile.is_open()) throw std::runtime_error("file open err \n");

    outFile << "Vertices,Density,Colors,Algorithm,TimeMs,Steps,ColorsUsed,Success,ColoringValid\n";

    // Начальное количество вершин
    int vertices = 100;
    constexpr int maxVertices = 5000; // Максимальное количество вершин

    std::vector<std::tuple<int, int, int, std::string, double, int64_t, int, bool, bool>> benchmarkResults;

    while (vertices <= maxVertices) {
        const int densityMax = 90;
        const int densityMin = 90;
        const int densityStep = 10;
        for (int density = densityMin; density <= densityMax; density += densityStep) {
            int colors = static_cast<int>(vertices * 0.9); // Количество цветов = количество вершин - 10%

            setConsoleColor(COLOR_YELLOW);
            std::cout << "\nRunning benchmark for graph with " << vertices << " vertices, " << density << " density, and " << colors << " colors...\n";
            resetConsoleColor();

            solver.generateRandomGraph(vertices, density);
            solver.setNumColors(colors);

            if (!solver.isGraphValid()) {
                setConsoleColor(COLOR_RED);
                std::cerr << "Error: Generated graph is invalid.\n";
                resetConsoleColor();
                continue;
            }

            auto runAlgorithm = [&](const std::string& name, auto func) {
                PerformanceTimer timer;
                bool success = false;
                int64_t stepCount = 0;
                bool coloringValid = false;

                setConsoleColor(COLOR_GREEN);
                std::cout << "Running " << name << "...\n";
                resetConsoleColor();

                timer.start();
                std::tie(success, stepCount) = func();
                timer.stop();
                coloringValid = solver.isColoringValid();

                benchmarkResults.emplace_back(vertices, density, colors, name, timer.getElapsedMilliseconds(), stepCount, solver.getColorCount(), success, coloringValid);
            };

            runAlgorithm("Greedy Algorithm", [&]() { return solver.solveGreedy(); });
            /*runAlgorithm("Parallel Greedy Algorithm", [&]() { return solver.solveParallelGreedy(); });*/
            runAlgorithm("DSATUR Algorithm", [&]() { return solver.solveDSATUR(); });
            /*runAlgorithm("Parallel DSATUR Algorithm", [&]() { return solver.solveParallelDSATUR(); });*/
            runAlgorithm("Welsh-Powell Algorithm", [&]() { return solver.solveWelshPowell(); });
            /*runAlgorithm("Parallel Welsh-Powell Algorithm", [&]() { return solver.solveParallelWelshPowell(); });*/
            runAlgorithm("Custom Algorithm", [&]() { return solver.solveCustomAlgorithm(); });
            /*runAlgorithm("Parallel Custom Algorithm", [&]() { return solver.solveParallelCustomAlgorithm(); });*/
        }

        // Увеличиваем количество вершин в зависимости от текущего значения
        if (vertices < 1000) {
            vertices += 25;
        } else if (vertices < 10000) {
            vertices += 250;
        } else if (vertices < 100000) {
            vertices += 2500;
        } else {
            vertices += 250000;
        }
    }

    // Сортировка результатов по сложности (количество вершин, плотность, количество цветов)
    std::sort(benchmarkResults.begin(), benchmarkResults.end(), [](const auto& a, const auto& b) {
        return std::tie(std::get<0>(a), std::get<1>(a), std::get<2>(a)) < std::tie(std::get<0>(b), std::get<1>(b), std::get<2>(b));
    });

    // Запись отсортированных результатов в CSV
    for (const auto& [vertices, density, colors, name, timeMs, steps, colorsUsed, success, coloringValid] : benchmarkResults) {
        outFile << vertices << "," << density << "," << colors << "," << name << ","
                << timeMs << "," << steps << "," << colorsUsed << "," << success << "," << coloringValid << "\n";
    }

    outFile.close();

    setConsoleColor(COLOR_YELLOW);
    std::cout << "\nBenchmark results saved to benchmark_results_small.csv\n";
    resetConsoleColor();
}

void ConsoleInterface::run() {
    while (true) {
        std::cout << "\n";
        setConsoleColor(COLOR_YELLOW);
        std::cout << "================ Graph Coloring Application ================\n";
        resetConsoleColor();
        std::cout << "1. Generate random graph\n";
        std::cout << "2. Load graph from input.txt\n";
        std::cout << "3. Run benchmarks\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int vertices, density, numColors;
            std::cout << "Enter number of vertices: ";
            std::cin >> vertices;
            std::cout << "Enter density (60-90): ";
            std::cin >> density;
            std::cout << "Enter number of colors (L): ";
            std::cin >> numColors;

            solver.generateRandomGraph(vertices, density);
            solver.setNumColors(numColors);
            if (!solver.isGraphValid()) {
                setConsoleColor(COLOR_RED);
                std::cerr << "Error: Generated graph is invalid.\n";
                resetConsoleColor();
                continue;
            }
            std::cout << "Generated graph saved to generated_graph.dot.\n";
            /*solver.saveGeneratedGraphToDot("generated_graph.dot");*/

            runAlgorithms();
            break;
        }
        if (choice == 2) {
            try {
                solver.loadFromFile("input.txt");
                std::cout << "Graph loaded successfully from input.txt.\n";

                runAlgorithms();
                break;
            } catch (const std::exception& ex) {
                setConsoleColor(COLOR_RED);
                std::cerr << "Error: " << ex.what() << "\n";
                resetConsoleColor();
            }
        }
        if (choice == 3) {
            runBenchmarks();
            break;
        } else {
            setConsoleColor(COLOR_RED);
            std::cout << "Invalid choice. Try again.\n";
            resetConsoleColor();
        }
    }
}