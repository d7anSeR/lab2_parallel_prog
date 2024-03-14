# Отчет по лабораторной работе № 2 по дисциплине "Параллельное программирование". Выполнила Куспанова Дания, гр. 6312-100503D
##### Задание:
 Модифицировать программу из л/р №1 для параллельной работы по технологии OpenMP.

##### Объемы задачи: 
матрицы 500х500, 1000х1000, 1500х1500;

##### Ход работы.
>_OpenMP_ реализует параллельные вычисления с помощью многопоточности, в которой ведущий поток создаёт набор ведомых потоков, и задача распределяется между ними. Предполагается, что потоки выполняются параллельно на машине с несколькими процессорами.

С помощью стандарта __OpenMP__ (Open Multi-Processing)  было реализовано распараллеливание функции умножения матриц
```
int** mulMatrix(int** matrix, int rows){
    //...
    #pragma omp parallel for shared(matrix, matrix_new) private(i, count, j)
    for (i = 0; i < rows; i++) {
        for (count = 0; count < rows; count++) {
            int sum = 0;
            for (j = 0; j < rows; j++) {
                sum += (matrix[i][j] * matrix[j][count]);
            }
            matrix_new[i][count] = sum;
        }
    }
    //...
}
```
Директива OpenMP
```
#pragma omp parallel for shared(matrix, matrix_new) private(i, count, j)
```
* указывает компилятору на параллельное выполнение цикла for. 
* shared(matrix, matrix_new) указывает, что переменные matrix и matrix_new будут общими для всех потоков, а private(i, count, j) указывает, что каждый поток будет иметь свои собственные копии переменных i, count и j.

Таким образом, происходит распараллеливание операции умножения матриц друг на друга.

###### Создание результатов и статистики такое же, как и в 1 лабораторной работе:
На языке С++ была реализована программа для основной работы с матрицами. Считанные из файлов матрицы один раз умножаются сами на себя, при этом данный опыт повторяется 10 раз и в отдельную матрицу фиксируется время выполнения операции умножения исходной матрицы. Результирующие матрицы записываются в csv-файлы matrix_res_500.csv, matrix_res_1000.csv, matrix_res_1500.csv, а матрица времени помещается в файл matrix_time.csv.
Далее на языке Python реализован скрип create_statistic.ipynb, реализующий отображение статистики результирующих файлов, в том числе зависимость времени перемножения матриц от их размера.
##### Вывод
 В результате полученных данных задания становится очевидно, что время реализации операции умножения матриц сильно зависит не только от размера матриц, но и от потоков и количества выполняемых на них операций. После распараллеливания входного потока я заметила, что время выполнения операции умножения матриц упало с 6 до 4 минут. 
 
 А по данным, отображающим статистику, видно, что время умножения матриц размеров 500х500, 1000х1000, 1500х1500 также снизилось. 
 
 Таким образом, я могу сделать вывод, что стандарт _OpenMP_ позволил ускорить исследуемую операцию. Каждый поток работает над своей частью результирующей матрицы, что позволяет эффективнее использовать вычислительные ресурсы и уменьшить время выполнения операции умножения.