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
            return false; // Петля
        }
    }

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j] > 1 || adjacencyMatrix[j][i] > 1) {
                return false; // Кратное ребро
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
std::vector<bool> GraphSolver::getAvailableColors(const int vertex, int64_t* stepCnt) const {
    std::vector<bool> available(numColors, true);
    for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
        (*stepCnt)++;
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
    return static_cast<int>(uniqueColors.size());
}

void GraphSolver::saveColoredGraphToDot(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open output file " + filename);
    }
    const std::vector<std::string> prefined_colors = {
        "red", "blue", "green", "yellow", "purple", "orange", "pink",
        "cyan", "magenta", "lime", "brown", "gray", "gold", "violet", "teal"
    };

    std::map<int, std::string> colorMap;
    for (int color : vertexColors) {
        if (!colorMap.contains(color)) {
            if (color < static_cast<int>(prefined_colors.size())) {
                colorMap[color] = prefined_colors[color];
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

    file << "graph GeneratedGraph {\n";

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



/*std::tuple<bool, int64_t> GraphSolver::solveCustomAlgorithm() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        stepCount++; // Шаг: проверка доступных цветов

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[u] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (vertexColors[u] == -1) return {false, stepCount};
    }
    return {true, stepCount};
}*/
/*std::tuple<bool, int64_t> GraphSolver::solveDSATUR() {
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
    }

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        // Поиск вершины с максимальной насыщенностью (не учитываем в шагах)
        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && (saturation[v] > maxSaturation || (saturation[v] == maxSaturation && degree[v] > degree[maxSaturationVertex]))) {
                maxSaturation = saturation[v];
                maxSaturationVertex = v;
            }
        }

        // Выбор цвета для вершины (учитываем в шагах)
        std::vector<bool> available = getAvailableColors(maxSaturationVertex);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (vertexColors[maxSaturationVertex] == -1) return {false, stepCount};

        // Обновление насыщенности соседей (учитываем в шагах)
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                ++saturation[u];
                stepCount++; // Шаг: обновление насыщенности
            }
        }
    }
    return {true, stepCount};
}*/
/*std::tuple<bool, int64_t> GraphSolver::solveGreedy() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        stepCount++; // Шаг: проверка доступных цветов

        int color = -1;
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                color = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (color == -1) return {false, stepCount};

        vertexColors[u] = color;
    }
    return {true, stepCount};
}*/
/*std::tuple<bool, int64_t> GraphSolver::solveWelshPowell() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    std::vector<int> degrees(numVertices, 0);
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
    }

    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
    }
    std::ranges::sort(vertexList, [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    for (const auto& [degree, vertex] : vertexList) {
        std::vector<bool> available = getAvailableColors(vertex);
        stepCount++; // Шаг: проверка доступных цветов

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[vertex] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (vertexColors[vertex] == -1) {
            return {false, stepCount};
        }
    }
    return {true, stepCount};
}*/

std::tuple<bool, int64_t> GraphSolver::solveGreedy() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    // Шаг: инициализация
    stepCount++;

    for (int u = 0; u < numVertices; ++u) {
        // Шаг: проверка доступных цветов
        std::vector<bool> available = getAvailableColors(u, &stepCount);
        stepCount++;

        int color = -1;
        for (int c = 0; c < numColors; ++c) {
            // Шаг: проверка доступности цвета
            if (available[c]) {
                color = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
            stepCount++; // Шаг: проверка следующего цвета
        }

        if (color == -1) return {false, stepCount};

        // Шаг: назначение цвета вершине
        vertexColors[u] = color;
        stepCount++;
    }

    return {true, stepCount};
}
std::tuple<bool, int64_t> GraphSolver::solveDSATUR() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;
    std::vector<int> saturation(numVertices, 0);
    std::vector<int> degree(numVertices, 0);

    // Шаг: вычисление степеней вершин
    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j]) {
                ++degree[i];
                stepCount++; // Шаг: подсчет степени
            }
        }
    }

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        // Шаг: поиск вершины с максимальной насыщенностью
        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && (saturation[v] > maxSaturation || (saturation[v] == maxSaturation && degree[v] > degree[maxSaturationVertex]))) {
                maxSaturation = saturation[v];
                maxSaturationVertex = v;
                stepCount++; // Шаг: поиск вершины
            }
        }

        if (maxSaturationVertex == -1) return {false, stepCount};

        // Шаг: выбор цвета для вершины
        std::vector<bool> available = getAvailableColors(maxSaturationVertex, &stepCount);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
            stepCount++; // Шаг: проверка следующего цвета
        }

        if (vertexColors[maxSaturationVertex] == -1) return {false, stepCount};

        // Шаг: обновление насыщенности соседей
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                ++saturation[u];
                stepCount++; // Шаг: обновление насыщенности
            }
        }
    }

    return {true, stepCount};
}
std::tuple<bool, int64_t> GraphSolver::solveWelshPowell() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    // Шаг: вычисление степеней вершин
    std::vector<int> degrees(numVertices, 0);
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
        stepCount++; // Шаг: подсчет степени
    }

    // Шаг: сортировка вершин по убыванию степени
    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
        stepCount++; // Шаг: добавление вершины в список
    }
    std::ranges::sort(vertexList, [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    stepCount += numVertices * std::log2(numVertices); // Шаг: сортировка

    // Шаг: раскраска вершин
    for (const auto& [degree, vertex] : vertexList) {
        std::vector<bool> available = getAvailableColors(vertex, &stepCount);
        stepCount++; // Шаг: проверка доступных цветов

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[vertex] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
            stepCount++; // Шаг: проверка следующего цвета
        }

        if (vertexColors[vertex] == -1) {
            return {false, stepCount};
        }
    }

    return {true, stepCount};
}
std::tuple<bool, int64_t> GraphSolver::solveCustomAlgorithm() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    // Шаг: инициализация
    stepCount++;

    for (int u = 0; u < numVertices; ++u) {
        // Шаг: проверка доступных цветов
        std::vector<bool> available = getAvailableColors(u, &stepCount);
        stepCount++;

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[u] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
            stepCount++; // Шаг: проверка следующего цвета
        }

        if (vertexColors[u] == -1) return {false, stepCount};
    }

    return {true, stepCount};
}


std::tuple<bool, int64_t> GraphSolver::solveParallelWelshPowell() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

    // Вычисление степеней вершин (параллельно)
    std::vector<int> degrees(numVertices, 0);
    #pragma omp parallel for
    for (int i = 0; i < numVertices; ++i) {
        degrees[i] = std::accumulate(adjacencyMatrix[i].begin(), adjacencyMatrix[i].end(), 0);
    }

    // Сортировка вершин по убыванию степени
    std::vector<std::pair<int, int>> vertexList;
    vertexList.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i) {
        vertexList.emplace_back(degrees[i], i);
    }
    std::ranges::sort(vertexList, [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    std::vector<int> saturation(numVertices, 0);

    for (int step = 0; step < numVertices; ++step) {
        int maxSaturationVertex = -1;
        int maxSaturation = -1;

        // Поиск вершины с максимальной насыщенностью (параллельно с редукцией)
        #pragma omp parallel for reduction(max:maxSaturation)
        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && saturation[v] > maxSaturation) {
                maxSaturation = saturation[v];
                maxSaturationVertex = v;
            }
        }

        if (maxSaturationVertex == -1) {
            return {false, stepCount};
        }

        // Выбор цвета для вершины (последовательно)
        std::vector<bool> available(numColors, true);
        for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
            if (adjacencyMatrix[maxSaturationVertex][neighbor] && vertexColors[neighbor] != -1) {
                available[vertexColors[neighbor]] = false;
            }
        }

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (vertexColors[maxSaturationVertex] == -1) {
            return {false, stepCount};
        }

        // Обновление насыщенности соседей (параллельно)
        #pragma omp parallel for
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                #pragma omp atomic
                saturation[u]++;
                stepCount++; // Шаг: обновление насыщенности
            }
        }
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveParallelGreedy() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

#pragma omp parallel for reduction(+:stepCount)
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        stepCount++; // Шаг: проверка доступных цветов

        int color = -1;
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                color = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (color == -1) {
#pragma omp critical
            {
                vertexColors[u] = -1; // Ошибка раскраски
            }
            continue;
        }

#pragma omp critical
        {
            vertexColors[u] = color;
        }
    }

    // Проверка, что все вершины раскрашены
    for (int u = 0; u < numVertices; ++u) {
        if (vertexColors[u] == -1) {
            return {false, stepCount};
        }
    }

    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveParallelDSATUR() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;
    std::vector<int> saturation(numVertices, 0);
    std::vector<int> degree(numVertices, 0);

    // Параллельное вычисление степеней вершин
    #pragma omp parallel for
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

        // Параллельный поиск вершины с максимальной насыщенностью
        #pragma omp parallel for reduction(max:maxSaturation)
        for (int v = 0; v < numVertices; ++v) {
            if (vertexColors[v] == -1 && (saturation[v] > maxSaturation || (saturation[v] == maxSaturation && degree[v] > degree[maxSaturationVertex]))) {
                maxSaturation = saturation[v];
                maxSaturationVertex = v;
            }
        }

        if (maxSaturationVertex == -1) {
            return {false, stepCount};
        }

        // Выбор цвета для вершины (последовательно)
        std::vector<bool> available = getAvailableColors(maxSaturationVertex);
        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                vertexColors[maxSaturationVertex] = c;
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (vertexColors[maxSaturationVertex] == -1) {
            return {false, stepCount};
        }

        // Параллельное обновление насыщенности соседей
        #pragma omp parallel for
        for (int u = 0; u < numVertices; ++u) {
            if (adjacencyMatrix[maxSaturationVertex][u] && vertexColors[u] == -1) {
                #pragma omp atomic
                saturation[u]++;
                stepCount++; // Шаг: обновление насыщенности
            }
        }
    }
    return {true, stepCount};
}

std::tuple<bool, int64_t> GraphSolver::solveParallelCustomAlgorithm() {
    vertexColors.assign(numVertices, -1);
    int64_t stepCount = 0;

#pragma omp parallel for
    for (int u = 0; u < numVertices; ++u) {
        std::vector<bool> available = getAvailableColors(u);
        stepCount++; // Шаг: проверка доступных цветов

        for (int c = 0; c < numColors; ++c) {
            if (available[c]) {
                // Синхронизация при выборе цвета
#pragma omp critical
                {
                    vertexColors[u] = c;
                }
                stepCount++; // Шаг: выбор цвета
                break;
            }
        }

        if (vertexColors[u] == -1) {
#pragma omp critical
            {
                vertexColors[u] = -1; // Ошибка раскраски
            }
        }
    }

    // Проверка, что все вершины раскрашены
    for (int u = 0; u < numVertices; ++u) {
        if (vertexColors[u] == -1) {
            return {false, stepCount};
        }
    }

    return {true, stepCount};
}


