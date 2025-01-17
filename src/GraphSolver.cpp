/*#include "GraphSolver.hpp"*/
#include "../include/GraphSolver.hpp"
#include <random>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>
#include <omp.h>
#include <tuple>
#include <array>

GraphSolver::GraphSolver() : numVertices(0), numColors(0) {}

bool GraphSolver::isColoringValid() const {
    for (int u = 0; u < numVertices; ++u) {
        for (int v = 0; v < numVertices; ++v) {
            if (adjacencyMatrix[u][v] && vertexColors[u] != -1 && vertexColors[v] != -1 && vertexColors[u] == vertexColors[v]) {
                std::cerr << "Conflict detected: Vertex " << u << " and Vertex " << v << " have the same color " << vertexColors[u] << "\n";
                return false;
            }
        }
    }
    return true;
}

bool GraphSolver::isGraphValid() const {
    for (int i = 0; i < numVertices; ++i) {
        if (adjacencyMatrix[i][i] != 0) {
            return false; //петля
        }
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j] > 1 || adjacencyMatrix[j][i] > 1) {
                return false; // кратное ребро
            }
        }
    }

    return true;
}

std::vector<bool> GraphSolver::getAvailableColors(const int vertex) const {
    std::vector<bool> available(numColors, true);
    for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
        if (adjacencyMatrix[vertex][neighbor] && vertexColors[neighbor] != -1) {
            available[vertexColors[neighbor]] = false;
        }
    }
    return available;
}

void GraphSolver::setNumColors(const int colors) {
    if (colors <= 0) {
        throw std::invalid_argument("number of colors must be greater than 0");
    }
    numColors = colors;
}

int GraphSolver::getColorCount() const {
    const std::set<int> uniqueColors(vertexColors.begin(), vertexColors.end());
    return uniqueColors.size();
}

void GraphSolver::saveColoredGraphToDot(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open output file " + filename);
    }
    const std::vector<std::string> prefinedColors = {
        "red", "blue", "green", "yellow", "purple", "orange", "pink",
        "cyan", "magenta", "lime", "brown", "gray", "gold", "violet", "teal"
    };

    std::map<int, std::string> colorMap;
    for (int color : vertexColors) {
        if (!colorMap.contains(color)) {                   /*colorMap.find(color) == colorMap.end()*/
            if (color < static_cast<int>(prefinedColors.size())) {
                colorMap[color] = prefinedColors[color];
            }
            else {
                std::ostringstream oss;
                oss << "color" << color;
                colorMap[color] = oss.str();
            }
        }
    }

    file << "graph ColoredGraph {\n";
    file << "   node [style=filled];\n";

    for (int i = 0; i < numVertices; ++i) {
        file << "    " << i << " [fillcolor=\"" << colorMap[vertexColors[i]] << "\"];\n";
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j]) {
                file << "   " << i << " -- " << j << ";\n";
            }
        }
    }

    file << "}\n";
    file.close();
}

void GraphSolver::saveGeneratedGraphToDot(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open output file " + filename);
    }

    file << "graph GeneratedGrapth {\n";

    for (int i = 0; i < numVertices; ++i) {
        file << "   " << i<< ";\n";
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j]) {
                file << "   " << i << " -- " << j << ";\n";
            }
        }
    }

    file << "}\n";
    file.close();
}

void GraphSolver::loadFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open input file: " + filename);
    }

    file >> numVertices >> numColors;
    if (numVertices <= 0 || numColors <= 0) {
        throw std::invalid_argument("invalid graph parameters in file");
    }

    adjacencyMatrix.assign(numVertices, std::vector<int>(numVertices, 0));
    vertexColors.assign(numVertices, -1);

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if (!(file >> adjacencyMatrix[i][j] || adjacencyMatrix[i][j] < 0 || adjacencyMatrix[i][j] > 1)) {
                throw std::invalid_argument("invalid adjacency matrix format");
            }
        }
    }

    file.close();
}

void GraphSolver::generateRandomGraph(const int vertices, const int density) {
    std::cout << "generating random graph with " << vertices << " vertices and " << density << " density" << std::endl;
    numVertices = vertices;
    adjacencyMatrix.assign(numVertices, std::vector<int>(numVertices, 0));
    vertexColors.assign(numColors, 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    for (int i = 0; i < numVertices; ++i) {
        for (int j = i + 1; j < numVertices; ++j) {
            if (dis(gen) < density) {
                adjacencyMatrix[i][j] = 1;
                adjacencyMatrix[j][i] = 1;
            }
        }
    }
}



std::tuple<bool, int64_t> GraphSolver::solveCustomAlgorithm() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        stepCount++;

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[u] = c;
                break;
            }
            stepCount++;
        }

        if (vertexColors[u] == -1) return {false, -1};
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveDSATUR() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;
    std::vector<int> saturation(numVertices, 0);
    std::vector<int> degree(numVertices, 0);

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j]) {
                ++degree[i];
            }
        }
        stepCount++;
    }

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && (saturation[v] > maxSaturation || (saturation[v] == maxSaturation && degree[v] > degree[maxSaturationVertex]))) {
                maxSaturation = saturation[v];
                maxSaturationVertex = v;
            }
            stepCount++;
        }

        std::vector<bool> available = getAvailableColors(maxSaturationVertex);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                break;
            }
            stepCount++;
        }

        if (vertexColors[maxSaturationVertex] == -1) return {false, -1};

        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                ++saturation[u];
            }
            stepCount++;
        }
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveGreedy() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0; // Инициализация счетчика шагов
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        stepCount++;

        int color = -1;
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                color = c;
                break;
            }
            stepCount++;
        }

        if (color == -1) return {false, -1};

        vertexColors[u] = color;
        stepCount++;
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveWelshPowell() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    std::vector<int> degrees(numVertices, 0);
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
        stepCount++;
    }

    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
        stepCount++;
    }
    std::ranges::sort(vertexList, [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    for (const auto& vertexPair : vertexList) {
        const int vertex = vertexPair.second;
        std::vector<bool> available = getAvailableColors(vertex);
        stepCount++;

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[vertex] = c;
                break;
            }
            stepCount++;
        }

        if (vertexColors[vertex] == -1) {
            return {false, -1};
        }
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveParallelWelshPowell_First() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    std::vector<int> degrees(numVertices, 0);
    #pragma omp parallel for
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
        stepCount++;
    }

    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
        stepCount++;
    }
    std::ranges::sort(vertexList, [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    std::vector<int> saturation(numVertices, 0);

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        #pragma omp parallel for
        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && saturation[v] > maxSaturation) {
                #pragma omp critical
                {
                    if (saturation[v] > maxSaturation) {
                        maxSaturation = saturation[v];
                        maxSaturationVertex = v;
                    }
                }
            }
            stepCount++;
        }

        if (maxSaturationVertex == -1) {
            return {false, -1};
        }

        std::vector<bool> available(numColors, true);
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (adjacencyMatrix[maxSaturationVertex][neighbor] && vertexColors[neighbor] != -1) {
                available[vertexColors[neighbor]] = false;
            }
            stepCount++;
        }

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                break;
            }
            stepCount++; // Увеличение счетчика шагов
        }

        if (vertexColors[maxSaturationVertex] == -1) {
            return {false, -1};
        }

        #pragma omp parallel for
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                #pragma omp atomic
                saturation[u]++;
            }
            stepCount++; // Увеличение счетчика шагов
        }
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveParallelWelshPowell_Sec() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    std::vector<int> degrees(numVertices, 0);
    #pragma omp parallel for
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
        stepCount++;
    }

    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
        stepCount++;
    }
    std::ranges::sort(vertexList, [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    std::vector<int> saturation(numVertices, 0);

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        #pragma omp parallel for
        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && saturation[v] > maxSaturation) {
                #pragma omp critical
                {
                    if (saturation[v] > maxSaturation) {
                        maxSaturation = saturation[v];
                        maxSaturationVertex = v;
                    }
                }
            }
            stepCount++;
        }

        if (maxSaturationVertex == -1) {
            return {false, -1};
        }

        std::vector<bool> available(numColors, true);
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (adjacencyMatrix[maxSaturationVertex][neighbor] && vertexColors[neighbor] != -1) {
                available[vertexColors[neighbor]] = false;
            }
            stepCount++;
        }

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                break;
            }
            stepCount++;
        }

        if (vertexColors[maxSaturationVertex] == -1) {
            return {false, -1};
        }

        #pragma omp parallel for
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                #pragma omp atomic
                saturation[u]++;
            }
            stepCount++;
        }
    }
    return {true, stepCount};
}


