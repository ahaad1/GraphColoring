import pandas as pd
import plotly.express as px
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# Загрузка данных из CSV
df = pd.read_csv('../build/benchmark_results.csv')

# 1. График времени выполнения алгоритмов в зависимости от количества вершин
fig1 = px.line(df, x='Vertices', y='TimeMs', color='Algorithm',
               title='Зависимость времени выполнения от количества вершин',
               labels={'Vertices': 'Количество вершин', 'TimeMs': 'Время выполнения (мс)'})
fig1.show()

# 2. График количества шагов алгоритмов в зависимости от количества вершин
fig2 = px.line(df, x='Vertices', y='Steps', color='Algorithm',
               title='Зависимость количества шагов от количества вершин',
               labels={'Vertices': 'Количество вершин', 'Steps': 'Количество шагов'})
fig2.show()

# 3. График количества использованных цветов в зависимости от количества вершин
fig3 = px.line(df, x='Vertices', y='ColorsUsed', color='Algorithm',
               title='Зависимость количества цветов от количества вершин',
               labels={'Vertices': 'Количество вершин', 'ColorsUsed': 'Количество использованных цветов'})
fig3.show()

# 4. График времени выполнения в зависимости от плотности графа
fig4 = px.line(df, x='Density', y='TimeMs', color='Algorithm',
               title='Зависимость времени выполнения от плотности графа',
               labels={'Density': 'Плотность графа (%)', 'TimeMs': 'Время выполнения (мс)'})
fig4.show()

# 5. График количества шагов в зависимости от плотности графа
fig5 = px.line(df, x='Density', y='Steps', color='Algorithm',
               title='Зависимость количества шагов от плотности графа',
               labels={'Density': 'Плотность графа (%)', 'Steps': 'Количество шагов'})
fig5.show()

# 6. График количества использованных цветов в зависимости от плотности графа
fig6 = px.line(df, x='Density', y='ColorsUsed', color='Algorithm',
               title='Зависимость количества цветов от плотности графа',
               labels={'Density': 'Плотность графа (%)', 'ColorsUsed': 'Количество использованных цветов'})
fig6.show()

# 7. Корреляция между временем выполнения и количеством шагов
fig7 = px.scatter(df, x='TimeMs', y='Steps', color='Algorithm',
                  title='Корреляция между временем выполнения и количеством шагов',
                  labels={'TimeMs': 'Время выполнения (мс)', 'Steps': 'Количество шагов'})
fig7.show()

# 8. Корреляция между временем выполнения и количеством использованных цветов
fig8 = px.scatter(df, x='TimeMs', y='ColorsUsed', color='Algorithm',
                  title='Корреляция между временем выполнения и количеством цветов',
                  labels={'TimeMs': 'Время выполнения (мс)', 'ColorsUsed': 'Количество использованных цветов'})
fig8.show()