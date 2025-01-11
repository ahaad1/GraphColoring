#ifndef GRAPHSOLVER_HPP
#define GRAPHSOLVER_HPP

#include <vector>
#include <string>

/*
 * testing with threads
 */
#include <thread>
#include <vector>
#include <mutex>


class GraphSolver {
private:
    int numVertices;
    int numColors;
    std::vector<std::vector<int>> adjacencyMatrix;
    std::vector<int> vertexColors;


    std::vector<bool> getAvailableColors(int vertex) const;

public:
    GraphSolver();

    void parallelGreedy(int start, int end, std::mutex &mtx);
    bool SolveParallelGreedy();

    bool solveParallelWelshPowell_First();
    bool solveParallelWelshPowell_Sec();

    void loadFromFile(const std::string& filename);
    void generateRandomGraph(int vertices, int density);
    bool solveCustomAlgorithm();
    bool solveDSATUR();
    bool solveGreedy();
    bool solveWelshPowell();

    [[nodiscard]] bool isColoringValid() const;
    bool isGrapthValid() const;

    void setNumColors(int colors);
    [[nodiscard]] int getColorCount() const;
    void saveColoredGraphToDot(const std::string& filename) const;
    void saveGeneratedGrapthToDot(const std::string& filename) const;

    struct AlgorithmResult {
        std::string name;
        double timeMs;
        bool success;
        int colorsUsed;
    };
};

#endif //GRAPHSOLVER_HPP
