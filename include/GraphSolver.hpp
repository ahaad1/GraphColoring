#ifndef GRAPHSOLVER_HPP
#define GRAPHSOLVER_HPP

#include <vector>
#include <string>
#include <tuple>

class GraphSolver {
private:
    int numVertices;
    int numColors;
    std::vector<std::vector<int>> adjacencyMatrix;
    std::vector<int> vertexColors;


    [[nodiscard]] std::vector<bool> getAvailableColors(int vertex) const;
    [[nodiscard]] std::vector<bool> getAvailableColors(int vertex, int64_t* stepCnt) const;

public:
    GraphSolver();

    void loadFromFile(const std::string& filename);
    void generateRandomGraph(int vertices, int density);

    /*algos with default realisation without using multithreading*/
    std::tuple<bool, int64_t> solveCustomAlgorithm();
    std::tuple<bool, int64_t>  solveDSATUR();
    std::tuple<bool, int64_t> solveGreedy();
    std::tuple<bool, int64_t> solveWelshPowell();

    /*algos which use multithreading*/
    std::tuple<bool, int64_t> solveParallelWelshPowell();
    std::tuple<bool, int64_t> solveParallelGreedy();
    std::tuple<bool, int64_t> solveParallelDSATUR();
    std::tuple<bool, int64_t> solveParallelCustomAlgorithm();


    [[nodiscard]] bool isColoringValid() const;
    [[nodiscard]] bool isGraphValid() const;

    void setNumColors(int colors);
    [[nodiscard]] int getColorCount() const;
    void saveColoredGraphToDot(const std::string& filename) const;
    void saveGeneratedGraphToDot(const std::string& filename) const;

    struct AlgorithmResult {
        std::string name;
        double timeMs;
        bool success;
        int colorsUsed;
        int64_t steps;
        bool coloringValid;
    };
};

#endif //GRAPHSOLVER_HPP
