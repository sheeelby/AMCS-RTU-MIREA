# Алгоритм Дейкстры вычисления минимального расстояния.

**Задача:** Составьте программу, осуществляющую реализацию алгоритма Дейкстры на основе двоичной кучи. Используйте ее для решения задачи о кратчайшем пути с единственным источником в разных неориентированных графах

**Формат входных данных.**

**Тестовый пример:** тестовый файл описывает неориентированный граф с 8 вершинами. Каковы кратчайшие расстояния от вершины 1 до любой другой вершины? (Ответ, для вершин с 1 по 8, в порядке: 0,1,2,3,4,4,3,2.)

**Набор данных задачи:** файл *data* содержит представление списка смежности неориентированного графа с 200 вершинами, помеченными от 1 до 200. В каждой строке указаны ребра, инцидентные данной вершине, а также их (неотрицательные) длины. Например, в шестой строке первой записью является «6», указывающая, что эта строка соответствует вершине 6. Следующая запись в этой строке «141,8200» указывает, что между вершиной 6 и вершиной 141 существует ненаправленное ребро, имеющее длина 8200. Остальные пары в этой строке указывают остальные вершины, смежные с вершиной 6, и длины соответствующих ребер. Вершина 1 является начальной вершиной.

**Дополнительные вопрос:** Каковы кратчайшие расстояния от вершины 1 до следующих десяти вершин: 7,37,59,82,99,115,133,165,188,197?
