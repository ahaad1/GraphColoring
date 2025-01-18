import pandas as pd
import plotly.express as px
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# df = pd.read_csv('../build/benchmark_results.csv')
df = pd.read_csv('../build/benchmark_results_small.csv')

# # График времени выполнения алгоритмов в зависимости от количества вершин
# fig1 = px.line(df, x='Vertices', y='TimeMs', color='Algorithm',
#                title='Зависимость времени выполнения от количества вершин',
#                labels={'Vertices': 'Количество вершин', 'TimeMs': 'Время выполнения (мс)'})
# fig1.show()
#
# # График количества шагов алгоритмов в зависимости от количества вершин
# fig2 = px.line(df, x='Vertices', y='Steps', color='Algorithm',
#                title='Зависимость количества шагов от количества вершин',
#                labels={'Vertices': 'Количество вершин', 'Steps': 'Количество шагов'})
# fig2.show()
#
# # График количества использованных цветов в зависимости от количества вершин
# fig3 = px.line(df, x='Vertices', y='ColorsUsed', color='Algorithm',
#                title='Зависимость количества цветов от количества вершин',
#                labels={'Vertices': 'Количество вершин', 'ColorsUsed': 'Количество использованных цветов'})
# fig3.show()
#
# # График времени выполнения в зависимости от плотности графа
# fig4 = px.line(df, x='Density', y='TimeMs', color='Algorithm',
#                title='Зависимость времени выполнения от плотности графа',
#                labels={'Density': 'Плотность графа (%)', 'TimeMs': 'Время выполнения (мс)'})
# fig4.show()
#
# # График количества шагов в зависимости от плотности графа
# fig5 = px.line(df, x='Density', y='Steps', color='Algorithm',
#                title='Зависимость количества шагов от плотности графа',
#                labels={'Density': 'Плотность графа (%)', 'Steps': 'Количество шагов'})
# fig5.show()
#
# # График количества использованных цветов в зависимости от плотности графа
# fig6 = px.line(df, x='Density', y='ColorsUsed', color='Algorithm',
#                title='Зависимость количества цветов от плотности графа',
#                labels={'Density': 'Плотность графа (%)', 'ColorsUsed': 'Количество использованных цветов'})
# fig6.show()
#
# # Корреляция между временем выполнения и количеством шагов
# fig7 = px.scatter(df, x='TimeMs', y='Steps', color='Algorithm',
#                   title='Корреляция между временем выполнения и количеством шагов',
#                   labels={'TimeMs': 'Время выполнения (мс)', 'Steps': 'Количество шагов'})
# fig7.show()
#
# # Корреляция между временем выполнения и количеством использованных цветов
# fig8 = px.scatter(df, x='TimeMs', y='ColorsUsed', color='Algorithm',
#                   title='Корреляция между временем выполнения и количеством цветов',
#                   labels={'TimeMs': 'Время выполнения (мс)', 'ColorsUsed': 'Количество использованных цветов'})
# fig8.show()
#


# по итогу из количества увеличения шагов должна получиться горизонтальная асиптотта
# если так не получилось значит шаги считаются не правильно
df['TimePerStep'] = df['TimeMs'] / df['Steps']

# Строим график
fig9 = px.line(df, x='Vertices', y='TimePerStep', color='Algorithm',
               title='Зависимость времени выполнения на шаг от размера графа',
               labels={'Vertices': 'Количество вершин', 'TimePerStep': 'Время на шаг (мс/шаг)'})

#  горизонтальную линию для визуализации асимптоты
# Среднее значение времени на шаг по всем данным
mean_time_per_step = df['TimePerStep'].mean()
fig9.add_hline(y=mean_time_per_step, line_dash="dash", line_color="red",
               annotation_text=f"Среднее время на шаг: {mean_time_per_step:.4f} мс/шаг",
               annotation_position="bottom right")

fig9.update_layout(
    xaxis_title="Количество вершин",
    yaxis_title="Время на шаг (мс/шаг)",
    showlegend=True
)

fig9.show()