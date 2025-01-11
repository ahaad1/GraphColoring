#include "../include/ConsoleInterface.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>


std::string ConsoleInterface::getTimestampedFilename(const std::string& baseName) {
    std::ostringstream oss;
    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);
    oss << baseName << "_" << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".dot";
    return oss.str();
}

void ConsoleInterface::runAlgorithms() {
    std::vector<GraphSolver::AlgorithmResult> results;

    PerformanceTimer timer;
    bool success = false;

    std::cout << "Running algorithms..." << std::endl;

    // Greedy Algorithm
    std::cout << "Running Greedy Algorithm..." << std::endl;
    timer.start();
    success = solver.solveGreedy();
    timer.stop();
    results.push_back({"greedy algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Greedy Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }

    // Welsh-Powell Algorithm
    std::cout << "Running Welsh-Powell Algorithm..." << std::endl;
    timer.start();
    success = solver.solveWelshPowell();
    timer.stop();
    results.push_back({"Welsh-Powell Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Welsh-Powell Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }

    /*
    // DSATUR Algorithm
    std::cout << "Running DSATUR Algorithm..." << std::endl;
    timer.start();
    success = solver.solveDSATUR();
    timer.stop();
    results.push_back({"DSATUR Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "DSATUR Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }


    // Custom Algorithm
    std::cout << "Running Custom Algorithm..." << std::endl;
    timer.start();
    success = solver.solveCustomAlgorithm();
    timer.stop();
    results.push_back({"Custom Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Custom Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }
    */

    // Parallel Greedy
    std::cout << "Running Parallel Greedy Algorithm..." << std::endl;
    timer.start();
    success = solver.SolveParallelGreedy();
    timer.stop();
    results.push_back({"Parallel Greedy Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Parallel Greedy: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }

    // Output results
    for (const auto& [name, timeMs, success, colorsUsed] : results) {
        std::cout << name << ": "
                  << (success ? "Completed successfully" : "Failed")
                  << " in " << timeMs << " ms. "
                  << "Colors used: " << (success ? colorsUsed : -1) << "\n";
    }
}

void ConsoleInterface::run() {
    while (true) {
        std::cout << "\nGraph Coloring Application\n";
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
            /*solver.saveGeneratedGrapthToDot("graph.dot");*/
            if (!solver.isGrapthValid()) {
                std::cerr << "err: generated graph is invalid" << std::endl;
                continue;
            }
            std::cout << "Generated graph saved to generated_graph.dot.\n";


            runAlgorithms();
            break;
        } else if (choice == 2) {
            try {
                solver.loadFromFile("input.txt");
                std::cout << "Graph loaded successfully from input.txt.\n";

                runAlgorithms();
                break;
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << "\n";
            }
        } else {
            std::cout << "Invalid choice. Try again.\n";
        }
    }
}