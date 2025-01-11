/*#include "GraphSolver.hpp"*/
#include "../include/GraphSolver.hpp"
#include <random>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <omp.h>

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

bool GraphSolver::isGrapthValid() const {
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


// TEST PARALLEL FUNC
void GraphSolver::parallelGreedy(const int start, const int end, std::mutex &mtx) {
    for (int u = start; u < end; ++u) {
        std::vector<bool> available(numColors, true);

        //захват мутеха для проверки доступных цветов
        {
            std::lock_guard<std::mutex> lock(mtx);
            for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
                if (adjacencyMatrix[u][neighbor] && vertexColors[neighbor] != -1) {
                    available[vertexColors[neighbor]] = false;
                }
            }
        }

        int color = -1;
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                color = c;
                break;
            }
        }
        if (color == -1) {
            std::lock_guard<std::mutex> lock(mtx);
            throw std::runtime_error("failed to color vertex " + std::to_string(u));
        }
        // захват мутеха для назначения цвета
        {
            std::lock_guard<std::mutex> lock(mtx);
            vertexColors[u] = color;
        }
    }
}

/*bool GraphSolver::SolveParallelGreedy() {
    vertexColors.assign(numVertices, -1);

    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mtx;  //это мьютекс для синканья

    int chunkSize = numVertices / numThreads; /*размер блока для каждого потока#1#

    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1 ) ? numVertices : start + chunkSize;
        threads.emplace_back(&GraphSolver::parallelGreedy, this, start, end, std::ref(mtx));
    }

    for (auto& t : threads) {
        t.join();
    }

    return true;
}*/
bool GraphSolver::SolveParallelGreedy() {
    vertexColors.assign(numVertices, -1); // Инициализация цветов

    // Параллельный цикл с использованием OpenMP
#pragma omp parallel for
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available(numColors, true);

        // Проверяем доступные цвета для вершины u
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (adjacencyMatrix[u][neighbor] && vertexColors[neighbor] != -1) {
                available[vertexColors[neighbor]] = false;
            }
        }

        // Назначаем первый доступный цвет
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[u] = c;
                break;
            }
        }

        if (vertexColors[u] == -1) {
            // Если цвет не найден, выбрасываем исключение
#pragma omp critical
            {
                throw std::runtime_error("Failed to color vertex " + std::to_string(u));
            }
        }
    }

    return true;
}


bool GraphSolver::solveParallelWelshPowell_First() {
    vertexColors.assign(numVertices, -1); // Инициализация цветов

    // Шаг 1: Вычисление степеней вершин (параллельно)
    std::vector<int> degrees(numVertices, 0);
    #pragma omp parallel for
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
    }

    // Шаг 2: Сортировка вершин по убыванию степени
    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
    }
    std::sort(vertexList.begin(), vertexList.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    // Шаг 3: Раскраска вершин
    std::vector<int> saturation(numVertices, 0); // Степень насыщенности

    for (int step = 0; step < numVertices; ++step) {
        // Находим вершину с максимальной степенью насыщенности
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
        }

        if (maxSaturationVertex == -1) {
            // Если вершина не найдена, граф не может быть раскрашен
            return false;
        }

        // Назначаем цвет вершине
        std::vector<bool> available(numColors, true);
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (adjacencyMatrix[maxSaturationVertex][neighbor] && vertexColors[neighbor] != -1) {
                available[vertexColors[neighbor]] = false;
            }
        }

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                break;
            }
        }

        if (vertexColors[maxSaturationVertex] == -1) {
            // Если цвет не найден, граф не может быть раскрашен
            return false;
        }

        // Обновляем степень насыщенности соседей
        #pragma omp parallel for
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                #pragma omp atomic
                saturation[u]++;
            }
        }
    }

    return true;
}

bool GraphSolver::solveParallelWelshPowell_Sec() {
    vertexColors.assign(numVertices, -1); // Инициализация цветов

    // Шаг 1: Вычисление степеней вершин (параллельно)
    std::vector<int> degrees(numVertices, 0);
    #pragma omp parallel for
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
    }

    // Шаг 2: Сортировка вершин по убыванию степени
    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
    }
    std::sort(vertexList.begin(), vertexList.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    // Шаг 3: Раскраска вершин
    std::vector<int> saturation(numVertices, 0); // Степень насыщенности

    for (int step = 0; step < numVertices; ++step) {
        // Находим вершину с максимальной степенью насыщенности
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
        }

        if (maxSaturationVertex == -1) {
            // Если вершина не найдена, граф не может быть раскрашен
            return false;
        }

        // Назначаем цвет вершине
        std::vector<bool> available(numColors, true);
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (adjacencyMatrix[maxSaturationVertex][neighbor] && vertexColors[neighbor] != -1) {
                available[vertexColors[neighbor]] = false;
            }
        }

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                break;
            }
        }

        if (vertexColors[maxSaturationVertex] == -1) {
            // Если цвет не найден, граф не может быть раскрашен
            return false;
        }

        // Обновляем степень насыщенности соседей
        #pragma omp parallel for
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                #pragma omp atomic
                saturation[u]++;
            }
        }
    }

    return true;
}

std::vector<bool> GraphSolver::getAvailableColors(int vertex) const {
    std::vector<bool> available(numColors, true);
    for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
        if (adjacencyMatrix[vertex][neighbor] && vertexColors[neighbor] != -1) {
            available[vertexColors[neighbor]] = false;
        }
    }
    return available;
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

void GraphSolver::generateRandomGraph(int vertices, int density) {
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

bool GraphSolver::solveCustomAlgorithm() {
    vertexColors.assign(numVertices, -1);
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[u] = c;
                break;
            }
        }
        if (vertexColors[u] == -1) return false;
    }
    return true;
}

bool GraphSolver::solveDSATUR() {
    vertexColors.assign(numVertices, -1);
    std::vector<int> saturation(numVertices, 0);
    std::vector<int> degree(numVertices, 0);

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j]) {
                ++degree[i];
            }
        }
    }

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && (saturation[v] > maxSaturation || (saturation[v] == maxSaturation && degree[v] > degree[maxSaturationVertex]))) {
                maxSaturation = saturation[v];
                maxSaturationVertex = v;
            }
        }

        std::vector<bool> available = getAvailableColors(maxSaturationVertex);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                break;
            }
        }

        if (vertexColors[maxSaturationVertex] == -1) return false;

        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                ++saturation[u];
            }
        }
    }
    return true;
}

bool GraphSolver::solveGreedy() {
    vertexColors.assign(numVertices, -1);
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);

        int color = -1;
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                color = c;
                break;
            }
        }

        if (color == -1) return false;

        vertexColors[u] = color;
    }
    return true;
}

bool GraphSolver::solveWelshPowell() {
    vertexColors.assign(numVertices, -1);

    std::vector<int> degrees(numVertices, 0);
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
    }

    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices); //added here after commented
    for (int i = 0; i < numVertices; ++i) {
        /*vertexList.push_back(std::make_pair(degrees[i], i));*/
        vertexList.emplace_back(degrees[i], i);
    }
    std::sort(vertexList.begin(), vertexList.end(), [](const std::pair<int, int>& a, const std::pair<int, int>&b){return a.first > b.first;});

    for (const auto& vertexPair : vertexList) {
        const int vertex = vertexPair.second;
        std::vector<bool> available = getAvailableColors(vertex);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[vertex] = c;
                break;
            }
        }
        if (vertexColors[vertex] == -1) {
            return false;
        }
    }
    return true;
}


void GraphSolver::setNumColors(int colors) {
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

void GraphSolver::saveGeneratedGrapthToDot(const std::string &filename) const {
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
