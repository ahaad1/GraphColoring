cmake_minimum_required(VERSION 3.10)
project(GraphColoring)

set(CMAKE_CXX_STANDARD 26)

# Указываем пути к OpenMP на macOS (если установлен через Homebrew)
if(APPLE)
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
    set(OpenMP_C_LIB_NAMES "omp")
    set(OpenMP_CXX_LIB_NAMES "omp")
    set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
endif()

# Включаем поддержку OpenMP
find_package(OpenMP REQUIRED)

# Добавляем исходные файлы
set(SOURCES
        src/main.cpp
        src/ConsoleInterface.cpp
        src/GraphSolver.cpp
        src/PerformanceTimer.cpp
        include/ConsoleInterface.hpp
        include/GraphSolver.hpp
        include/PerformanceTimer.hpp
)

# Создаем исполняемый файл без проверки утечек
add_executable(GraphColoring ${SOURCES})

# Подключаем OpenMP к целевому исполняемому файлу
if(OpenMP_FOUND)
    target_link_libraries(GraphColoring PUBLIC OpenMP::OpenMP_CXX)
endif()

# Для macOS добавляем пути к библиотеке OpenMP
if(APPLE)
    include_directories(/opt/homebrew/opt/libomp/include)
    link_directories(/opt/homebrew/opt/libomp/lib)
    target_link_libraries(GraphColoring PUBLIC omp)
endif()

# Для Windows добавляем библиотеку для работы с консолью
if(WIN32)
    target_link_libraries(GraphColoring PUBLIC kernel32.lib)
endif()

# Для Linux (если OpenMP установлен через пакетный менеджер)
if(UNIX AND NOT APPLE)
    target_link_libraries(GraphColoring PUBLIC omp)
endif()

# Создаем второй исполняемый файл с проверкой утечек (AddressSanitizer)
add_executable(GraphColoring_ASan ${SOURCES})

# Подключаем OpenMP к целевому исполняемому файлу с проверкой утечек
if(OpenMP_FOUND)
    target_link_libraries(GraphColoring_ASan PUBLIC OpenMP::OpenMP_CXX)
endif()

# Добавляем флаги для AddressSanitizer
target_compile_options(GraphColoring_ASan PRIVATE -fsanitize=address -fno-omit-frame-pointer)
target_link_libraries(GraphColoring_ASan PRIVATE -fsanitize=address)

# Для macOS добавляем пути к библиотеке OpenMP
if(APPLE)
    include_directories(/opt/homebrew/opt/libomp/include)
    link_directories(/opt/homebrew/opt/libomp/lib)
    target_link_libraries(GraphColoring_ASan PUBLIC omp)
endif()

# Для Windows добавляем библиотеку для работы с консолью
if(WIN32)
    target_link_libraries(GraphColoring_ASan PUBLIC kernel32.lib)
endif()

# Для Linux (если OpenMP установлен через пакетный менеджер)
if(UNIX AND NOT APPLE)
    target_link_libraries(GraphColoring_ASan PUBLIC omp)
endif()