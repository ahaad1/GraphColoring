import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('../build/benchmark_results.csv')

# График времени выполнения алгоритмов в зависимости от количества вершин
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Vertices'], subset['TimeMs'], label=algorithm)

plt.xlabel('Количество вершин')
plt.ylabel('Время выполнения (мс)')
plt.title('Зависимость времени выполнения от количества вершин')
plt.legend()
plt.grid(True)
plt.show()

# График количества шагов алгоритмов в зависимости от количества вершин
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Vertices'], subset['Steps'], label=algorithm)

plt.xlabel('Количество вершин')
plt.ylabel('Количество шагов')
plt.title('Зависимость количества шагов от количества вершин')
plt.legend()
plt.grid(True)
plt.show()

#  График количества использованных цветов в зависимости от количества вершин
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Vertices'], subset['ColorsUsed'], label=algorithm)

plt.xlabel('Количество вершин')
plt.ylabel('Количество использованных цветов')
plt.title('Зависимость количества цветов от количества вершин')
plt.legend()
plt.grid(True)
plt.show()

# График времени выполнения в зависимости от плотности графа
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Density'], subset['TimeMs'], label=algorithm)

plt.xlabel('Плотность графа (%)')
plt.ylabel('Время выполнения (мс)')
plt.title('Зависимость времени выполнения от плотности графа')
plt.legend()
plt.grid(True)
plt.show()

# График количества шагов в зависимости от плотности графа
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Density'], subset['Steps'], label=algorithm)

plt.xlabel('Плотность графа (%)')
plt.ylabel('Количество шагов')
plt.title('Зависимость количества шагов от плотности графа')
plt.legend()
plt.grid(True)
plt.show()

# 6. График количества использованных цветов в зависимости от плотности графа
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Density'], subset['ColorsUsed'], label=algorithm)

plt.xlabel('Плотность графа (%)')
plt.ylabel('Количество использованных цветов')
plt.title('Зависимость количества цветов от плотности графа')
plt.legend()
plt.grid(True)
plt.show()

# Корреляция между временем выполнения и количеством шагов
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.scatter(subset['TimeMs'], subset['Steps'], label=algorithm)

plt.xlabel('Время выполнения (мс)')
plt.ylabel('Количество шагов')
plt.title('Корреляция между временем выполнения и количеством шагов')
plt.legend()
plt.grid(True)
plt.show()

# Корреляция между временем выполнения и количеством использованных цветов
plt.figure(figsize=(10, 6))
for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.scatter(subset['TimeMs'], subset['ColorsUsed'], label=algorithm)

plt.xlabel('Время выполнения (мс)')
plt.ylabel('Количество использованных цветов')
plt.title('Корреляция между временем выполнения и количеством цветов')
plt.legend()
plt.grid(True)
plt.show()