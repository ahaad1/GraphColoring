#include "../include/ConsoleInterface.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

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
    std::ofstream outFile("results.txt");
    if (!outFile.is_open()) throw std::runtime_error("file open err \n");

    PerformanceTimer timer;
    bool success = false;

    std::cout << "\n";
    setConsoleColor(COLOR_YELLOW);
    std::cout << "================ Running Algorithms ================\n";
    resetConsoleColor();

    // Greedy Algorithm
    std::cout << "Running Greedy Algorithm...\n";
    outFile << "Running Greedy Algorithm...\n";
    timer.start();
    success = solver.solveGreedy();
    timer.stop();
    results.push_back({"Greedy Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "Greedy Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        resetConsoleColor();
        outFile << "Greedy Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    } else {
        setConsoleColor(COLOR_RED);
        std::cout << "Greedy Algorithm: Failed to color the graph.\n";
        outFile << "Greedy Algorithm: Failed to color the graph.\n";
        resetConsoleColor();
    }

    // DSATUR Algorithm
    std::cout << "Running DSATUR Algorithm...\n";
    outFile << "Running DSATUR Algorithm...\n";
    timer.start();
    success = solver.solveDSATUR();
    timer.stop();
    results.push_back({"DSATUR Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "DSATUR Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        resetConsoleColor();
        outFile << "DSATUR Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    } else {
        setConsoleColor(COLOR_RED);
        std::cout << "DSATUR Algorithm: Failed to color the graph.\n";
        outFile << "DSATUR Algorithm: Failed to color the graph.\n";
        resetConsoleColor();
    }

    // Custom Algorithm
    std::cout << "Running Custom Algorithm...\n";
    outFile << "Running Custom Algorithm...\n";
    timer.start();
    success = solver.solveCustomAlgorithm();
    timer.stop();
    results.push_back({"Custom Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "Custom Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        resetConsoleColor();
        outFile << "Custom Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    } else {
        setConsoleColor(COLOR_RED);
        std::cout << "Custom Algorithm: Failed to color the graph.\n";
        outFile << "Custom Algorithm: Failed to color the graph.\n";
        resetConsoleColor();
    }

    // Welsh-Powell Algorithm
    std::cout << "Running Welsh-Powell Algorithm...\n";
    outFile << "Running Welsh-Powell Algorithm...\n";
    timer.start();
    success = solver.solveWelshPowell();
    timer.stop();
    results.push_back({"Welsh-Powell Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "Welsh-Powell Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        resetConsoleColor();
        outFile << "Welsh-Powell Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    } else {
        setConsoleColor(COLOR_RED);
        std::cout << "Welsh-Powell Algorithm: Failed to color the graph.\n";
        outFile << "Welsh-Powell Algorithm: Failed to color the graph.\n";
        resetConsoleColor();
    }

    // First Parallel Welsh-Powell
    std::cout << "Running Parallel Welsh-Powell First Algorithm...\n";
    outFile << "Running Parallel Welsh-Powell First Algorithm...\n";
    timer.start();
    success = solver.solveParallelWelshPowell_First();
    timer.stop();
    results.push_back({"Parallel Welsh-Powell First Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "Parallel Welsh-Powell First: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        resetConsoleColor();
        outFile << "Parallel Welsh-Powell First: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    } else {
        setConsoleColor(COLOR_RED);
        std::cout << "Parallel Welsh-Powell First: Failed to color the graph.\n";
        outFile << "Parallel Welsh-Powell First: Failed to color the graph.\n";
        resetConsoleColor();
    }

    // Second Parallel Welsh-Powell
    std::cout << "Running Parallel Welsh-Powell Second Algorithm...\n";
    outFile << "Running Parallel Welsh-Powell Second Algorithm...\n";
    timer.start();
    success = solver.solveParallelWelshPowell_Sec();
    timer.stop();
    results.push_back({"Parallel Welsh-Powell Second Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        setConsoleColor(COLOR_GREEN);
        std::cout << "Parallel Welsh-Powell Second: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        resetConsoleColor();
        outFile << "Parallel Welsh-Powell Second: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    } else {
        setConsoleColor(COLOR_RED);
        std::cout << "Parallel Welsh-Powell Second: Failed to color the graph.\n";
        outFile << "Parallel Welsh-Powell Second: Failed to color the graph.\n";
        resetConsoleColor();
    }

    // Output results
    setConsoleColor(COLOR_YELLOW);
    std::cout << "\n================ Results ================\n";
    resetConsoleColor();
    for (const auto& [name, timeMs, success, colorsUsed] : results) {
        std::cout << name << ": "
                  << (success ? "Completed successfully" : "Failed")
                  << " in " << timeMs << " ms. "
                  << "Colors used: " << (success ? colorsUsed : -1) << "\n";
        outFile << name << ": "
                << (success ? "Completed successfully" : "Failed")
                << " in " << timeMs << " ms. "
                << "Colors used: " << (success ? colorsUsed : -1) << "\n";
    }
    outFile.close();
}

void ConsoleInterface::run() {
    while (true) {
        std::cout << "\n";
        setConsoleColor(COLOR_YELLOW);
        std::cout << "================ Graph Coloring Application ================\n";
        resetConsoleColor();
        std::cout << "1. Generate random graph\n";
        std::cout << "2. Load graph from input.txt\n";
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
        } else {
            setConsoleColor(COLOR_RED);
            std::cout << "Invalid choice. Try again.\n";
            resetConsoleColor();
        }
    }
}