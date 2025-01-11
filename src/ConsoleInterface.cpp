#include "../include/ConsoleInterface.hpp"

#include <fstream>
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
    std::ofstream outFile("results.txt");
    if (!outFile.is_open()) throw std::runtime_error("file open err \n");



    PerformanceTimer timer;
    bool success = false;

    std::cout << "Running algorithms..." << std::endl;
    outFile << "Running algorithms..." << std::endl;

    // Greedy Algorithm
    std::cout << "Running Greedy Algorithm..." << std::endl;
    outFile << "Running Greedy Algorithm..." << std::endl;
    timer.start();
    success = solver.solveGreedy();
    timer.stop();
    results.push_back({"greedy algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Greedy Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        outFile << "Greedy Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";

    }

    // Welsh-Powell Algorithm
    std::cout << "Running Welsh-Powell Algorithm..." << std::endl;
    outFile << "Running Welsh-Powell Algorithm..." << std::endl;
    timer.start();
    success = solver.solveWelshPowell();
    timer.stop();
    results.push_back({"Welsh-Powell Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Welsh-Powell Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        outFile << "Welsh-Powell Algorithm: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
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

    /*// Parallel Greedy
    std::cout << "Running Parallel Greedy Algorithm..." << std::endl;
    timer.start();
    success = solver.SolveParallelGreedy();
    timer.stop();
    results.push_back({"Parallel Greedy Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Parallel Greedy: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }*/

    // First Parallel Welsh-Powell
    std::cout << "Running Parallel Welsh-Powell First Algorithm..." << std::endl;
    outFile << "Running Parallel Welsh-Powell First Algorithm..." << std::endl;
    timer.start();
    success = solver.solveParallelWelshPowell_First();
    timer.stop();
    results.push_back({"Parallel Welsh-Powell First Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Parallel Welsh-Powell First: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        outFile << "Parallel Welsh-Powell First: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
    }


    // Sec Parallel Welsh-Powell
    std::cout << "Running Parallel Welsh-Powell Sec Algorithm..." << std::endl;
    outFile << "Running Parallel Welsh-Powell Sec Algorithm..." << std::endl;
    timer.start();
    success = solver.solveParallelWelshPowell_Sec();
    timer.stop();
    results.push_back({"Parallel Welsh-Powell Sec Algorithm", timer.getElapsedMilliseconds(), success, solver.getColorCount()});
    if (success) {
        std::cout << "Parallel Welsh-Powell Sec: Coloring is " << (solver.isColoringValid() ? "valid" : "invalid") << "\n";
        outFile << "Running Parallel Welsh-Powell Sec Algorithm..." << std::endl;
    }

    // Output results
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