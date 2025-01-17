#ifndef GRAPHSOLVER_HPP
#define GRAPHSOLVER_HPP

#include <vector>
#include <string>

/*
 * testing with threads
 */
#include <thread>
#include <mutex>


class GraphSolver {
private:
    int numVertices;
    int numColors;
    std::vector<std::vector<int>> adjacencyMatrix;
    std::vector<int> vertexColors;


    [[nodiscard]] std::vector<bool> getAvailableColors(int vertex) const;

public:
    GraphSolver();

    void loadFromFile(const std::string& filename);
    void generateRandomGraph(int vertices, int density);

    /*algos with deafault reaslisation without using multithreading*/
    bool solveCustomAlgorithm();
    bool solveDSATUR();
    bool solveGreedy();
    bool solveWelshPowell();

    /*algos which use multithreading*/
    bool solveParallelWelshPowell_First();
    bool solveParallelWelshPowell_Sec();

    [[nodiscard]] bool isColoringValid() const;
    [[nodiscard]] bool isGraphValid() const;

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
