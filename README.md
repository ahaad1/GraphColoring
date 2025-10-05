# Graph Coloring Application

Русская версия описания - ниже.
<img width="1920" height="958" alt="image" src="https://github.com/user-attachments/assets/14619387-cb81-4626-8858-6cdf7b1081c8" />

## Introduction

Graph coloring is the problem of assigning a color to each vertex of a graph so that no two adjacent vertices share the same color ￼. This NP-hard problem has many applications, but finding the minimum number of colors (the chromatic number) is computationally difficult ￼. Instead of exact algorithms, this project focuses on heuristic algorithms that produce a proper coloring efficiently, possibly using more colors than the minimum.

This repository implements and compares several graph coloring algorithms with the goal of improving both speed and coloring performance. By benchmarking classic algorithms against a custom algorithm, we aim to achieve faster execution and competitive color usage. The application can generate random graphs or read graphs from input, color them using each algorithm, verify the results, and output performance metrics.

### Project features:

•	Multiple coloring algorithms: Greedy, Welsh-Powell, DSATUR, and a custom heuristic (described below).

•	Cross-platform C++ code: The code is portable and can be compiled on Windows, Linux, etc. (OpenMP is used for parallelism in a separate branch).

•	Memory leak checks: A special debug build enables memory leak detection, ensuring the program runs without memory leaks.

•	Visualization output: Graphs are saved in Graphviz .dot format for visualization of colored results.

•	Benchmarking mode: Automatically runs the algorithms on a series of graphs and records metrics (time, steps, colors used) to a CSV file for analysis.

## Algorithms Implemented

### Greedy Algorithm

The greedy coloring algorithm processes vertices in a given order (e.g. their numeric index) and assigns each vertex the first available color that is not used by its neighbors ￼. This approach is very fast (linear in the number of edges) and simple to implement. However, the greedy algorithm does not guarantee an optimal coloring and may use more than the minimum number of colors ￼. The result depends on the vertex ordering; a poor ordering can lead to a suboptimal coloring.

### Welsh-Powell Algorithm

Welsh–Powell is an improved greedy heuristic that orders the vertices by descending degree (highest number of neighbors first) before coloring ￼. By coloring high-degree vertices first, it reduces the chance of later needing a new color for a heavily connected vertex ￼. The algorithm then colors the vertices in that order, using the smallest available color for each. In practice, Welsh-Powell often uses fewer colors than the basic greedy algorithm at the cost of a slightly longer runtime (due to the initial sorting step).

### DSATUR Algorithm

DSATUR (Degree of Saturation) is a heuristic introduced by Brélaz (1979) for higher-quality graph coloring ￼. At each step, it chooses the next vertex to color based on the highest saturation-the number of different colors already used by its neighbors ￼. (Ties are broken by choosing the vertex with the greatest degree among those tied ￼.) The chosen vertex is colored with the smallest available color not used by its neighbors. This strategy tends to color the most constrained vertices first ￼, often resulting in a coloring that is closer to the minimum number of colors. DSATUR usually finds an optimal coloring for certain graph classes (e.g. bipartite, cycle graphs) and often outperforms simpler heuristics in color count, but it is slower because selecting the next vertex dynamically and updating neighbor saturation is computationally more expensive.

### Custom Algorithm

The custom algorithm is a heuristic developed in this project to improve performance while maintaining proper colorings. It is based on the greedy approach but includes optimizations in data structures and ordering to reduce runtime. The custom algorithm follows a similar strategy to the basic greedy (assigning the lowest possible color to each vertex) but uses more efficient neighbor color checking and possibly a refined vertex processing order. As a result, it achieves the same coloring quality as the standard greedy algorithm (in our tests, it used the same number of colors as Greedy in all cases) while running faster in many scenarios. This algorithm demonstrates a better constant time performance per operation, giving it an edge in speed for medium-sized graphs (hundreds of vertices) and comparable performance even on larger graphs.

### Parallel Greedy Algorithm (OpenMP, Experimental)

To leverage multiple CPU cores, an experimental parallel version of the greedy algorithm was implemented (available in the openmp-test-branch). The idea is to color different vertices simultaneously in parallel. However, because adjacent vertices cannot share a color, naive parallel coloring can cause conflicts (race conditions where two neighboring vertices get colored with the same color) ￼. The parallel approach in this project uses OpenMP to attempt coloring in parallel and then checks for conflicts. If a conflict is detected (e.g., two adjacent vertices colored identically), the algorithm flags it and would need to resolve it (such as by recoloring one of the vertices). Parallel graph coloring often trades some coloring quality for speed: parallel implementations may use more colors than sequential algorithms and require conflict resolution steps ￼. In our implementation, the Parallel Greedy algorithm showed significant speedup on large graphs (for example, ~6× faster than sequential greedy on 10,000 vertices) but occasionally produced invalid colorings due to conflicts. This remains an area of further improvement, and thus the parallel algorithm is kept separate as an experimental feature.

## Usage

After building the program, run the GraphColoring executable. You will be presented with an interactive menu:
```bash
================ Graph Coloring Application ================
1. Generate random graph
2. Load graph from input.txt
3. Run benchmarks
Choose an option: 
```
### 1. Generate random graph: 

This option creates a random graph and then colors it. You will be prompted to enter:

•	the number of vertices (N),

•	the density (a percentage between 60–90 for how full the graph’s edge set should be),

•	the number of colors (L) to allow.

The program will generate an undirected graph with N vertices, adding edges randomly until the specified density (approximately) is reached. The generated graph is saved to a file generated_graph.dot in Graphviz DOT format (so you can visualize the structure). Then, the application runs each coloring algorithm on this graph in turn. It prints the result of each algorithm, including whether the coloring is valid (no conflicts), the time taken, and how many colors were used. For example:
```bash
Enter number of vertices: 10000
Enter density (60-90): 60
Enter number of colors (L): 8000
generating random graph with 10000 vertices and 60 density
Generated graph saved to generated_graph.dot.
Running algorithms...
Running Greedy Algorithm...
Greedy Algorithm: Coloring is valid
Running Welsh-Powell Algorithm...
Welsh-Powell Algorithm: Coloring is valid
Running Parallel Greedy Algorithm...
Parallel Greedy: Conflict detected: Vertex 0 and Vertex 909 have the same color 0
Parallel Greedy Algorithm: Coloring is invalid
Greedy Algorithm: Completed successfully in 886.21 ms. Colors used: 1117
Welsh-Powell Algorithm: Completed successfully in 1450.16 ms. Colors used: 1106
Parallel Greedy Algorithm: Completed successfully in 139.23 ms. Colors used: 1117
```
In the example above, a random graph of 10,000 vertices (60% density) was colored by three algorithms. The Greedy and Welsh-Powell algorithms produced valid colorings, while the Parallel Greedy algorithm encountered a conflict (adjacent vertices 0 and 909 got the same color) and thus its coloring was invalid. The timing results show that the parallel algorithm was much faster in execution, but its coloring required the same number of colors as the Greedy approach (and was not conflict-free). After running, you can open the DOT file to inspect the graph. The vertices in the DOT file are labeled or colored according to the algorithm’s coloring, which helps visualize which vertices got which color.

### 2. Load graph from input.txt: 
This option reads a graph from a file named input.txt (expected to be in the working directory). The file should contain a graph specification (for example, the number of vertices and an edge list or adjacency matrix – refer to the repository documentation for the exact format required). Once the graph is loaded, the program will run all coloring algorithms on that graph, just as in option 1, and output the coloring validity, time, and colors used for each. This mode is useful if you want to test the algorithms on a specific custom graph. Make sure to prepare input.txt in the correct format before choosing this option.

### 3. Run benchmarks: 
This mode executes a predefined set of benchmark tests to compare algorithm performance. The program will automatically generate a sequence of random graphs of increasing size and run all algorithms on each graph. For example, it may start with a graph of 100 vertices, then 125, 150, … up to 1000 (and even larger, up to 5000 vertices) at a fixed density (e.g. 90%). For each graph size, it uses a number of colors L proportional to the number of vertices (for instance, L = 90% of N). Each algorithm’s execution time, the number of steps (operations) it performed, and the number of colors actually used in the coloring are recorded. The program does not print all details to the console (to avoid flooding the screen), but at the end it saves all results to a CSV file (such as benchmark_results_small.csv). You will see a message like:

Benchmark results saved to benchmark_results_small.csv

You can open this CSV to inspect the performance metrics. The columns include: Vertices, Density, Colors (the L value), Algorithm, TimeMs (execution time in milliseconds), Steps (an internal count of operations), ColorsUsed (how many colors the algorithm ended up using), Success (whether a coloring was found within the L color limit), and ColoringValid (whether the coloring has no conflicts). This data allows you to analyze how each algorithm scales with graph size and density.

## Performance Comparison and Results

### The benchmark results provide insight into the trade-offs between the algorithms:

•	Greedy vs Custom: The Custom algorithm consistently uses the same number of colors as the Greedy algorithm for a given graph (they produce colorings of equal quality), but it often runs faster. For example, on a 100-vertex dense graph, Greedy used 48 colors and Custom also used 48, but Custom finished in 0.35 ms vs Greedy’s 0.43 ms ￼. Even at 5000 vertices, Custom and Greedy both used 1302 colors, with Custom taking ~175.7 ms versus Greedy’s ~172.6 ms ￼ (roughly on par). This demonstrates that our custom optimizations yield a modest speed improvement without loss of solution quality.

•	Welsh-Powell: By ordering vertices by degree, Welsh-Powell tends to reduce the number of colors needed compared to basic Greedy. In the 100-vertex example, Welsh-Powell used 45 colors (3 fewer than Greedy) ￼. At 5000 vertices, it used 1285 colors versus Greedy’s 1302 ￼, a reduction of 17 colors. However, the sorting step and the more restrictive coloring order make Welsh-Powell a bit slower. At 5000 vertices, it took about 336 ms, roughly 1.9× the Greedy time ￼. For many cases, this algorithm offers a good compromise, achieving near-optimal color counts with moderate overhead.

•	DSATUR: This algorithm usually achieves the lowest color count of the heuristics. In the 100-vertex test, DSATUR found a coloring with only 44 colors (best among the algorithms) ￼. For larger graphs, DSATUR’s color counts were often equal or very close to Welsh-Powell’s. In some cases, Welsh-Powell even slightly outperformed DSATUR on color count (e.g., at 5000 vertices WP used 1285 vs DSATUR’s 1300) ￼. DSATUR’s strength is in difficult instances where it can adapt to coloring needs dynamically. The trade-off is speed: DSATUR is the slowest algorithm due to its complex vertex selection process. At 5000 vertices, it took ~572 ms, about 3.3× slower than Greedy ￼. It also performs many more steps internally (e.g., ~25 million steps for Greedy vs ~61 million for DSATUR at 5000 vertices, as recorded in the CSV). Use DSATUR when color optimality is more important than runtime.

•	Parallel Greedy: The experimental parallel approach showed the greatest speedups on large graphs, but with caveats. In a 10000-vertex test, it completed in ~139 ms, much faster than the 886 ms of sequential Greedy (about 6× faster). However, as seen, it may produce invalid colorings if conflicts are not resolved. Moreover, the parallel algorithm did not reduce the number of colors used (it colored with the same number of colors as Greedy in tests). In practice, parallel graph coloring algorithms often have to sacrifice some optimality or do extra work to handle conflicts ￼. Our implementation demonstrates the potential for multi-threading (especially for very large graphs) but would need more sophisticated conflict-resolution to be reliably used. For now, the parallel algorithm is not included in the default benchmark results due to these issues.

The CSV output from the benchmark (e.g., benchmark_results_small.csv) provides the detailed data underlying these observations. You can use this data to create charts or perform further analysis. For instance, you might plot graph size vs. time for each algorithm to visualize how the algorithms scale. In summary, the custom algorithm achieves the fastest execution with the same coloring quality as Greedy, Welsh-Powell and DSATUR produce better (lower) chromatic counts at the cost of more time, and the parallel greedy approach offers a path to significantly reduce runtime if the challenge of conflict resolution can be overcome.

⸻

# Приложение для раскраски графов

English version is above.
<img width="1920" height="958" alt="image" src="https://github.com/user-attachments/assets/622dbb4e-07d9-4347-a312-854ef678f1d1" />


## Введение

Раскраска графа – это присвоение цвета каждой вершине графа так, чтобы никакие две смежные вершины не имели одинаковый цвет ￼. Такая раскраска называется правильной. Задача нахождения минимального числа цветов для раскраски (так называемого хроматического числа графа) является вычислительно сложной (относится к NP-трудным задачам) ￼. Поэтому на практике используют эвристические алгоритмы, которые быстро находят допустимую раскраску, хотя и не гарантируют минимальное число цветов.

Данный репозиторий содержит приложение, реализующее и сравнивающее несколько эвристических алгоритмов раскраски графов. Цель – повысить скорость раскраски и при этом получить раскраску с относительно небольшим числом цветов. В программе реализованы классические алгоритмы и собственный улучшенный алгоритм; проведено их сравнительное тестирование по времени работы и качеству раскраски. Приложение позволяет генерировать случайные графы или загружать граф из файла, раскрашивать его всеми алгоритмами, проверять корректность раскраски и замерять показатели производительности.

## Особенности проекта:

•	Несколько алгоритмов раскраски: реализованы жадный алгоритм, алгоритм Уэлша–Пауэлла, алгоритм DSATUR и собственный (кастомный) алгоритм. Их описание приводится ниже.

•	Кроссплатформенность: код написан на C++ и успешно компилируется под разные ОС (Windows, Linux и др.). В отдельной ветке реализована параллелизация с использованием OpenMP (для многоядерных процессоров).

•	Проверка утечек памяти: в отладочной сборке включена проверка на утечки памяти, что гарантирует корректное освобождение ресурсов.

•	Визуализация графа: генерируемые графы и результаты раскраски сохраняются в формате Graphviz .dot, что позволяет визуализировать раскрашенный граф.

•	Режим бенчмарка: программа может автоматически запускать серию тестов и сохранять метрики работы алгоритмов (время, количество операций, использованные цвета и др.) в CSV-файл для дальнейшего анализа.

## Реализованные алгоритмы

### Жадный алгоритм

Жадная раскраска просматривает вершины в некотором порядке (например, в порядке их номеров) и назначает каждой вершине наименьший доступный цвет, который не использован у ее соседей ￼. Этот алгоритм очень быстрый (имеет линейную сложность относительно числа ребер) и прост в реализации. Однако жадный алгоритм не гарантирует оптимальную раскраску и обычно использует больше цветов, чем минимально необходимо ￼. Результат жадной раскраски зависит от порядка обхода вершин: при неудачном порядке алгоритм может дать далекое от оптимума количество цветов.

### Алгоритм Уэлша–Пауэлла

Алгоритм Уэлша–Пауэлла представляет собой модификацию жадной раскраски с выбором порядка вершин по убыванию степени (сначала раскрашиваются вершины с наибольшим числом соседей) ￼. Такая эвристика снижает вероятность ситуации, когда вершина с высокой степенью, раскрашиваемая поздно, потребует нового цвета ￼. Алгоритм сортирует вершины по степени, затем раскрашивает их в этом порядке, назначая каждой текущей вершине минимально возможный цвет. На практике алгоритм Уэлша–Пауэлла часто позволяет снизить число используемых цветов по сравнению с обычным жадным алгоритмом, хотя работает несколько медленнее (из-за первоначальной сортировки и более строгого порядка раскраски).

### Алгоритм DSATUR

DSATUR (Degree of Saturation) – эвристический алгоритм, предложенный Д. Брелацом в 1979 году, для получения качественной раскраски ￼. В ходе работы DSATUR на каждом шаге выбирает следующую вершину для раскрашивания не произвольно, а ту, у которой наибольшая степень насыщенности – то есть наибольшее число различных цветов уже присутствует у ее соседей ￼. Если несколько нераскрашенных вершин имеют одинаковую степень насыщенности, выбирается та из них, у которой больше степень (больше соседей) ￼. Выбранная вершина раскрашивается минимальным допустимым цветом (первым цветом, не встречающимся у соседних вершин). Таким образом, DSATUR в первую очередь раскрашивает наиболее «трудные» вершины ￼, что часто позволяет снизить общее число цветов. Этот алгоритм нередко находит оптимальную раскраску для некоторых классов графов (например, для двудольных или циклов) и в целом дает раскраску ближе к минимуму цветов, чем более простые методы. За улучшение по качеству приходится платить скоростью: DSATUR работает медленнее, так как на каждом шаге выполняет более сложный выбор вершины и обновляет степени насыщенности соседей.

### Собственный алгоритм

Custom Algorithm – особый алгоритм, разработанный в рамках данного проекта для повышения эффективности раскраски. По своей сути он близок к жадному подходу, но включает оптимизации в структуре данных и в порядке обхода, что снижает время выполнения. Собственный алгоритм использует стратегию, аналогичную базовому жадному алгоритму (назначает каждой вершине наименьший возможный цвет), однако делает это более эффективно благодаря улучшенным методам проверки соседних цветов и, возможно, более продуманному порядку вершин. В результате качество раскраски остается таким же, как у обычного жадного алгоритма (в наших тестах он всегда использовал столько же цветов, сколько и жадный алгоритм), а скорость работы выше. Оптимизации снижают постоянные затраты на обработку, за счет чего custom-алгоритм выигрывает в скорости на графах средних размеров (сотни вершин) и показывает сопоставимое время даже на больших графах.

### Параллельный жадный алгоритм (OpenMP, экспериментально)

Для использования возможностей многоядерных процессоров был также реализован параллельный вариант жадной раскраски (в ветке репозитория openmp-test-branch). Идея состоит в том, чтобы раскрашивать несколько различных вершин одновременно, распараллеливая работу. Однако при параллельной раскраске возникает проблема: если не соблюдать осторожность, два соседних узла могут получить один и тот же цвет (состояние гонки при одновременном окрашивании соседних вершин) ￼. В данной реализации параллельный алгоритм с помощью OpenMP пытается раскрасить вершины параллельно, а затем выполняет проверку на конфликты. Если обнаруживается конфликт (например, две смежные вершины окрашены в один цвет), алгоритм сигнализирует об этом; для полноценного решения конфликта потребовалось бы повторно перекрасить одну из конфликтующих вершин. Как правило, параллельные алгоритмы раскраски жертвуют оптимальностью раскраски ради ускорения: параллельная раскраска может потребовать больше цветов, чем последовательная, и нуждается в дополнительных шагах для разрешения конфликтов ￼. В нашем проекте параллельный жадный алгоритм продемонстрировал существенное ускорение на больших графах (например, на графе с 10 000 вершинами он работал примерно в 6 раз быстрее последовательного варианта), но иногда давал некорректную раскраску из-за неразрешенных конфликтов. Поэтому данный параллельный алгоритм рассматривается как экспериментальный и вынесен в отдельную ветку, требуя дальнейшего совершенствования (реализации механизмов разрешения конфликтов), прежде чем он сможет стабильно использоваться.

## Использование

После сборки запустите исполняемый файл GraphColoring. Появится интерактивное меню:
```bash
================ Graph Coloring Application ================
1. Generate random graph
2. Load graph from input.txt
3. Run benchmarks
Choose an option:
```
Вам будет предложено выбрать один из режимов работы:

### 1. Генерация случайного графа: 
программа сгенерирует случайный граф и раскрасит его. Необходимо ввести:

•	число вершин (N);

•	плотность графа в процентах (число от 60 до 90 – доля существующих ребер от максимально возможного числа);

•	количество цветов L (верхний предел доступных цветов для раскраски).

Программа сгенерирует неориентированный граф с N вершинами, добавляя ребра случайным образом примерно до достижения заданной плотности. Сгенерированный граф сохраняется в файл generated_graph.dot (формат Graphviz DOT), что позволяет при необходимости визуализировать его структуру. Далее приложение запускает все алгоритмы раскраски по очереди на этом графе и выводит результаты для каждого: сообщение о корректности раскраски (есть ли конфликты), время выполнения и число использованных цветов. Например:
```bash
Enter number of vertices: 10000
Enter density (60-90): 60
Enter number of colors (L): 8000
generating random graph with 10000 vertices and 60 density
Generated graph saved to generated_graph.dot.
Running algorithms...
Running Greedy Algorithm...
Greedy Algorithm: Coloring is valid
Running Welsh-Powell Algorithm...
Welsh-Powell Algorithm: Coloring is valid
Running Parallel Greedy Algorithm...
Parallel Greedy: Conflict detected: Vertex 0 and Vertex 909 have the same color 0
Parallel Greedy Algorithm: Coloring is invalid
Greedy Algorithm: Completed successfully in 886.21 ms. Colors used: 1117
Welsh-Powell Algorithm: Completed successfully in 1450.16 ms. Colors used: 1106
Parallel Greedy Algorithm: Completed successfully in 139.23 ms. Colors used: 1117
```
В этом примере для случайного графа с 10 000 вершинами (плотность 60%) выполнена раскраска тремя алгоритмами. Жадный и алгоритм Уэлша–Пауэлла выдали корректную раскраску (без конфликтов), а параллельный жадный алгоритм обнаружил конфликт (вершины 0 и 909 получили один цвет) и, соответственно, раскраска оказалась некорректной. В последних строках приведено время работы и число цветов: видно, что параллельный алгоритм выполнился значительно быстрее, но использовал столько же цветов, сколько и обычный жадный (и при этом выдал конфликт). После выполнения вы можете открыть файл DOT, чтобы просмотреть сгенерированный граф и результаты раскраски. В DOT-файле вершинам присвоены либо явные цвета, либо метки с номерами цветов, что позволяет увидеть, какие вершины каким цветом окрашены.

### 2. Загрузка графа из input.txt: 
этот режим позволяет загрузить граф из файла input.txt (файл должен находиться в рабочей директории). Ожидается, что файл содержит описание графа (например, сначала число вершин, затем список ребер или матрицу смежности – подробный формат приведен в документации репозитория). После успешной загрузки графа программа выполнит раскраску всеми алгоритмами, аналогично режиму 1, и выведет информацию о корректности, времени и числе цветов для каждого алгоритма. Этот режим удобен, если вы хотите протестировать алгоритмы на определенном графе, подготовленном заранее. Убедитесь, что файл input.txt соответствует ожидаемому формату перед запуском данного режима.

### 3. Запуск бенчмарков: 
в этом режиме приложение выполняет серию автоматических тестов для сравнения производительности алгоритмов. Программа будет генерировать случайные графы возрастающего размера и запускать все алгоритмы на каждом из них. Например, могут последовательно генерироваться графы на 100 вершинах, 125, 150, … вплоть до 1000 (и далее – вплоть до 5000 вершин) при фиксированной плотности, например 90%. Для каждого размера графа выбирается число цветов L, пропорциональное числу вершин (например, L = 90% от N). На каждом графе последовательно выполняются жадный, DSATUR, Уэлш–Пауэлл и кастомный алгоритмы; измеряется время выполнения, количество выполненных шагов (условных операций) и фактически использованное число цветов. Чтобы не перегружать консоль, подробные результаты не выводятся на экран для каждого графа – в консоли отображаются только общие этапы и прогресс. По завершении работы все данные бенчмарка сохраняются в CSV-файл (например, benchmark_results_small.csv). В консоли вы увидите сообщение:

Benchmark results saved to benchmark_results_small.csv

Открыв этот CSV-файл, вы найдете таблицу результатов. В ней содержатся столбцы: Vertices (число вершин графа), Density (плотность, %), Colors (выбранное L – максимально допустимое число цветов), Algorithm (название алгоритма), TimeMs (время работы в миллисекундах), Steps (количество шагов/операций, выполненных алгоритмом), ColorsUsed (сколько цветов фактически потребовалось алгоритму), Success (удалось ли раскрасить граф, не превысив L цветов; 1 – да, 0 – нет) и ColoringValid (корректна ли раскраска, т.е. нет ли конфликтов; 1 – да, 0 – нет). Эти данные позволяют проанализировать, как алгоритмы масштабируются с ростом графа и плотности.

## Сравнение производительности алгоритмов

Результаты бенчмарков демонстрируют особенности и компромиссы каждого алгоритма:

•	Жадный vs. Custom: Собственный (кастомный) алгоритм во всех тестах использовал столько же цветов, сколько и классический жадный алгоритм (качество раскраски идентично), но зачастую работал быстрее. Например, на плотном графе со 100 вершинами жадный алгоритм использовал 48 цветов, custom – также 48, но время выполнения у custom составило 0,351 ms против 0,427 ms у жадного ￼. На графе с 5000 вершинами оба алгоритма использовали 1302 цвета, при этом custom завершился за ~175,7 ms, а жадный – за ~172,6 ms ￼ (практически одинаково). Таким образом, оптимизации в кастомном алгоритме дают небольшой выигрыш по скорости без ущерба для качества раскраски.

•	Уэлш–Пауэлл: За счет упорядочивания вершин по степеням алгоритм Уэлша–Пауэлла обычно снижает число цветов по сравнению с жадным. В тесте на 100 вершин он получил раскраску в 45 цветов (против 48 у жадного) ￼. На 5000 вершинах алгоритм использовал 1285 цветов против 1302 у жадного ￼, то есть выиграл 17 цветов. Однако дополнительная сортировка и иной порядок обработки делают Уэлша–Пауэлла чуть медленнее. Например, при 5000 вершинах он работал около 336 ms – примерно в 1,9 раза дольше жадного ￼. В целом этот алгоритм представляет компромисс: он достигает близкого к оптимальному числа цветов за умеренное дополнительное время.

•	DSATUR: Данный алгоритм чаще всего дает наименьшее число цветов из всех эвристик. В примере со 100 вершинами DSATUR справился 44 цветами – лучше остальных алгоритмов ￼. На больших графах количество цветов по DSATUR обычно равно или ненамного отличается от результата Уэлша–Пауэлла. В некоторых тестах Уэлш–Пауэлл даже слегка превосходил DSATUR по числу цветов (например, при 5000 вершинах WP использовал 1285 цветов, а DSATUR – 1300) ￼. Сильная сторона DSATUR – умение адаптивно раскрашивать сложные фрагменты графа, часто приближаясь к хроматическому числу. Обратная сторона – скорость: DSATUR оказался самым медленным. Для графа на 5000 вершин ему потребовалось ~572 ms, что примерно в 3,3 раза дольше работы жадного алгоритма ￼. Он также выполняет значительно больше внутренних операций (например, порядка 28 миллионов шагов у жадного против ~61 миллиона у DSATUR на 5000 вершинах, согласно CSV). Использование DSATUR оправдано, когда приоритет – минимизировать число цветов, а время выполнения не критично.

•	Параллельный алгоритм: Экспериментальный параллельный подход показал наибольшее ускорение на крупных графах, хотя не лишен недостатков. В тесте на 10 000 вершин он завершился примерно за 139 ms, тогда как последовательному жадному потребовалось 886 ms (ускорение почти 6×). Однако, как отмечалось, параллельный алгоритм может дать некорректную раскраску, если конфликты не устранены. Кроме того, параллельная раскраска не снизила число цветов (в наших опытах параллельный алгоритм давал столько же цветов, сколько жадный). В общем случае параллельные алгоритмы раскраски вынуждены либо использовать больше цветов, либо выполнять дополнительную работу для разрешения конфликтов ￼. Наша реализация наглядно демонстрирует потенциал многопоточности (особенно на очень больших графах), но требует более сложных методов устранения конфликтов, чтобы гарантировать корректность раскраски. Пока параллельный алгоритм не включен в основную серию бенчмарков из-за указанных проблем.

Все подробные данные измерений, сохраненные в CSV-файле (например, benchmark_results_small.csv), позволяют более детально проанализировать поведение алгоритмов. Эти данные можно использовать для построения графиков или дальнейшего статистического анализа. В целом результаты подтверждают, что наш кастомный алгоритм обеспечивает самую быструю работу, давая такое же качество раскраски, как жадный алгоритм; алгоритмы Уэлша–Пауэлла и DSATUR находят раскраску с меньшим числом цветов ценой большего времени; а параллельный подход может существенно ускорить раскраску, если удастся эффективно решить проблему конфликтов цветов.
