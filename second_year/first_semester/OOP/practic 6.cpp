#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>

using namespace std;

#define INFINITY 10000

//---------------------------------------------------- Max Flow ---------------------------------------------------//

void FillVectorWith(vector<int>& v, int value, int size = 0)
{
	if (size != 0)
	{
		v.resize(0);
		for (int i = 0; i < size; i++)
			v.push_back(value);
	}
	for (int i = 0; i < v.size(); i++)
		v[i] = value;
}

void FillMatrixWith(vector<vector<int>>& matrix, int width = 0, int height = 0, int value = 0)
{
	if (width != 0 && height != 0)
	{
		matrix.resize(0);
		int i, j;
		for (i = 0; i < height; i++)
		{
			vector<int> v;
			for (j = 0; j < width; j++)
			{
				v.push_back(value);
			}
			matrix.push_back(v);
		}
	}
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
			matrix[i][j] = value;
	}
}

int FindPath(vector<vector<int> >& f, vector<vector<int> >& c, int source, int target, int vertices) // source - исток, target - сток
{
	int i, CurVertex;

	vector<int> Flow; FillVectorWith(Flow, 0, vertices); // Flow - значение потока через данную вершину на данном шаге поиска
	vector<int> Link; FillVectorWith(Link, -1, vertices); // Link[i] хранит номер предыдущей вешины на пути i -> исток

	Flow[source] = INFINITY;
	vector<int> used(vertices, 0);

	//queue<int> q; q.push(source);
	//stack<int> s; s.push(source);
	priority_queue<int> pq; pq.push(source);

	//while (!q.empty())
	//while (!s.empty())
	while (!pq.empty())
	{
		//int vertex = q.front();
		//int vertex = s.top();
		int vertex = pq.top();

		if (vertex == target) break;

		//s.pop();
		//q.pop();
		pq.pop();

		for (int i = 0; i < vertices; i++)
		{
			//есть ребро vertex->i, в i не заходили
			if (c[vertex][i] - f[vertex][i] > 0 && used[i] == 0) // Для каждой вершины, с которой есть непосещенное ребро в остаточной сети
			{
				pq.push(i);//s.push(i);//q.push(i);
				Flow[i] = (c[vertex][i] - f[vertex][i] < Flow[vertex]) ? c[vertex][i] - f[vertex][i] : Flow[vertex];
				Link[i] = vertex;
			}
		}
		used[vertex] = 1;
	}

	if (Link[target] == -1) return 0;

	CurVertex = target;
	while (CurVertex != source)
	{
		f[Link[CurVertex]][CurVertex] += Flow[target];
		CurVertex = Link[CurVertex];
	}
	cout << "\nFlow: " << Flow[target] << " Link: ";

	for (int i = 0; i < vertices; i++) cout << Link[i] << " ";

	return Flow[target];
}

// основная функция поиска максимального потока
int MaxFlow(vector<vector<int> >& f, vector<vector<int> >& c, int source, int target, int vertices) // source - исток, target - сток
{
	FillMatrixWith(f);
	int MaxFlow = 0;
	int AddFlow;
	do
	{
		AddFlow = FindPath(f, c, source, target, vertices);
		MaxFlow += AddFlow;
	} while (AddFlow > 0);
	return MaxFlow;
}

//----------------------------------------------------- Hamilton Cycle -----------------------------------------------------//

vector<int> HamiltonCycle(vector<vector<int>>& mat, vector<bool>& visited, vector<int>& path, int curr, bool flag = false)
{
	path.push_back(curr);

	if (flag) // Цикл или путь?
		if (path.size() == mat.size())
			return path;
		else
			if (mat[curr][path[0]] > 0)
				return path;

	visited[curr] = true;

	// Если размер пути равен количеству вершин и есть ребро от последней вершины к начальной, то нашли!!!

	if (path.size() == mat.size() && mat[path[0]][path[path.size() - 1]] > 0)
		return path;

	if (path.size() == mat.size() && mat[path[0]][path[path.size() - 1]] == 0)
	{
		visited[curr] = false;
		path.pop_back();
		return vector<int>();  // Не нашли =(
	}

	for (int i = 0; i < mat.size(); i++)
	{
		if (mat[curr][i] > 0 && !visited[i])
		{
			vector<int> result = HamiltonCycle(mat, visited, path, i, flag);
			if (!result.empty())
				return result;  // Урааа!! Нашли ;3
		}
	}

	visited[curr] = false;
	path.pop_back();

	return vector<int>();  // Не нашли =(
}

//-------------------------------------------------------- Paint Graph ------------------------------------------------------//

void PaintGraph(vector<vector<int>>& matrix, vector<int>& color, int start = 0)
{
	int numVertices = matrix.size();

	vector<bool> usedColors(numVertices + 1, false); // Вектор для отслеживания использованных цветов.
	vector<int> weights(numVertices, INT_MAX); // Вектор для отслеживания весов ребер.

	priority_queue<int> pq;
	pq.push(start);
	weights[start] = 0;

	while (!pq.empty())
	{
		// На каждом шаге извлекаем вершину с минимальным весом (для этого нужна приоритетная очередь)

		int current = pq.top();
		pq.pop();

		if (color[current] != 0)
			continue;

		for (int next = 0; next < numVertices; next++)
		{
			int weight = matrix[current][next];
			if (weight > 0 && next != current && weights[next] > weight) // Если вес ребра меньше текущего веса этой вершины, обновить вес
			{
				weights[next] = weight;
				pq.push(next);
			}
		}

		int colorCounter = 1;
		while (usedColors[colorCounter])
			colorCounter++;

		color[current] = colorCounter;
		usedColors[colorCounter] = true;
	}
}

int main()
{
	vector<vector<int>> matrix =
	{
	{ 0, 9, 7, 9, 6, 9, 5, 5, 6, 3, 6 },
	{ 9, 0, 2, 3, 7, 6, 5, 6, 7, 7, 0 },
	{ 7, 2, 0, 5, 0, 0, 6, 8, 0, 5, 6 },
	{ 9, 3, 5, 0, 6, 2, 5, 1, 1, 2, 2 },
	{ 6, 7, 0, 6, 0, 0, 1, 0, 5, 8, 3 },
	{ 9, 6, 0, 2, 0, 0, 4, 2, 8, 3, 0 },
	{ 5, 5, 6, 5, 1, 4, 0, 5, 9, 7, 4 },
	{ 5, 6, 8, 1, 0, 2, 5, 0, 9, 2, 6 },
	{ 6, 7, 0, 1, 5, 8, 9, 9, 0, 1, 0 },
	{ 3, 7, 5, 2, 8, 3, 7, 2, 1, 0, 4 },
	{ 6, 0, 6, 2, 3, 0, 4, 6, 0, 4, 0 }
	};

	cout << "\nThe original graph: \n\n";

	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
			cout << matrix[i][j] << " ";
		cout << "\n";
	}

	cout << "\nSearch for the maximum flow using the Edmonds Carp algorithm\n";

	int source = 0;
	int target = matrix.size() - 1;
	int vertices = matrix.size();

	vector<vector<int>> f_matrix;
	FillMatrixWith(f_matrix, vertices, vertices, 0);

	cout << "\n\nMaxFlow: " << MaxFlow(f_matrix, matrix, source, target, vertices) << "\n";

	vector<bool> visited_matrix(matrix.size(), 0);

	vector<int> path_matrix;

	cout << "\nHamilton way: ";
	vector<int> way = HamiltonCycle(matrix, visited_matrix, path_matrix, 0, true);

	for (int i = 0; i < way.size(); i++)
		cout << way[i] << " ";

	cout << "\nHamilton cycle: ";
	vector<int> cycle = HamiltonCycle(matrix, visited_matrix, path_matrix, 0, false);

	for (int i = 0; i < cycle.size(); i++)
		cout << cycle[i] << " ";

	cout << "\n\nPainting Graph\n";

	vector<int> color(matrix.size(), 0);
	PaintGraph(matrix, color, 0);

	cout << "\nColors\n\n";

	for (int i = 0; i < matrix.size(); i++)
	{
		cout << "Vertex " << i << " is colored with " << color[i] << "\n";
	}
	cout << "\n";
	return 0;
}