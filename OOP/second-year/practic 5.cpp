#include <iostream>
#include <queue>
#include <stack>
#include <list>

using namespace std;

//------------------------------------------ DFS ----------------------------------------//

vector<int> dfs(const vector<vector<int>>& mat, int start) {

	vector<int> used(mat.size(), 0);
	vector<int> dist(mat.size(), -1);

	stack<int> s;
	s.push(start);
	dist[start] = 0;
	int step = 1;

	while (!s.empty()) {
		int cur_vertex = s.top();
		s.pop();

		for (int i = 0; i < 10 + 1; i++) {

			if (mat[cur_vertex][i] != 0 && used[i] == 0) {
				s.push(i);
				used[i] = 1;
			}

		}

		used[cur_vertex] = 2;
		dist[cur_vertex] = step++;
	}

	return dist;
}

//----------------- Поиск минимального пути в графе (Алгоритм Дейкстры). -------------------//

vector<int> Dijsktra_distances(const vector<vector<int>>& mat, int start)
{
	vector<int> dist(mat.size(), 10000);

	queue<int> q;
	q.push(start);
	dist[start] = 0;

	while (!q.empty())
	{
		int cur_vertex = q.front(); q.pop();

		for (int i = 0; i < 10 + 1; i++)
			if (mat[cur_vertex][i] != 0 && dist[i] > dist[cur_vertex] + mat[cur_vertex][i])
			{
				q.push(i); dist[i] = dist[cur_vertex] + mat[cur_vertex][i];
			}
	}

	return dist;
}

//------ BFS. Функция подсчета степени. Функция подсчета средней степени по всему дереву -----------//

void bfs(const vector<vector<int>>& mat, vector<bool>& visited, vector<vector<int>>& tree, int start) {

	int n = mat.size();

	queue<int> q;
	q.push(start);
	visited[start] = true;

	while (!q.empty()) {

		int current = q.front(); q.pop();

		for (int neighbor = 0; neighbor < n; ++neighbor) {
			if (mat[current][neighbor] != 0 && !visited[neighbor]) {

				tree[current][neighbor] = mat[current][neighbor];
				tree[neighbor][current] = mat[current][neighbor];

				visited[neighbor] = true;
				q.push(neighbor);
			}
		}
	}
}

vector<vector<int>> CreateTreeBFS(const vector<vector<int>>& mat) {

	vector<bool> visited(mat.size(), false);
	vector<vector<int>> tree(mat.size(), vector<int>(mat.size(), 0));

	for (int start = 0; start < mat.size(); ++start) {
		if (!visited[start]) {
			bfs(mat, visited, tree, start);
		}
	}

	return tree;
}

vector<int> calculateDegrees(const vector<vector<int>>& mat) {

	vector<int> degrees(mat.size(), 0);

	for (int i = 0; i < mat.size(); ++i)
		for (int j = 0; j < mat.size(); ++j)
			if (mat[i][j] != 0)
				degrees[i]++;

	return degrees;
}

double calculateAverageDegree(const vector<vector<int>>& mat) {

	int n = mat.size();
	int totalWeight = 0;
	int totalEdges = 0;

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (mat[i][j] != 0) {
				totalWeight += mat[i][j];
				totalEdges++;
			}
		}
	}

	if (totalEdges == 0) return 0.0;

	double averageDegree = totalWeight / totalEdges;

	return averageDegree;
}

//--------------- Алгоритм Прима. Создание матрицы смежности минимального остова. ------------------//

vector<int> Prime(const vector<vector<int>>& mat) {

	int n = mat.size();

	vector<int> parent(n, -1); // Массив для хранения родительских вершин в построенном остове
	vector<int> key(n, 10000); // Массив для хранения весов рёбер в минимальном остове
	vector<bool> mstSet(n, false); // Массив для отметки вершин, включенных в остов

	key[0] = 0;

	for (int count = 0; count < n - 1; count++) {
		int u = -1;

		// Выбираем вершину с минимальным ключом из множества вершин, еще не включенных в остов
		for (int v = 0; v < n; v++) {
			if (!mstSet[v] && (u == -1 || key[v] < key[u]))
				u = v;
		}

		mstSet[u] = true;

		// Обновляем значения ключей и родительских вершин для смежных вершин
		for (int v = 0; v < n; v++) {
			if (mat[u][v] != 0 && !mstSet[v] && mat[u][v] < key[v]) {
				parent[v] = u;
				key[v] = mat[u][v];
			}
		}
	}

	return parent;
}

vector<vector<int>> CreateMatrix(const vector<int>& parent, const vector<vector<int>>& matrix) {

	vector<vector<int>> incidenceMatrix(parent.size(), vector<int>(parent.size(), 0));

	for (int i = 1; i < parent.size(); i++) {
		incidenceMatrix[i][parent[i]] = matrix[i][parent[i]];
		incidenceMatrix[parent[i]][i] = matrix[i][parent[i]]; 
	}

	return incidenceMatrix;
}

int main() {

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

	vector<int> dist_dfs = dfs(matrix, 0);

	cout << "\nDFS order: ";
	for (int i = 0; i < matrix.size(); i++) cout << dist_dfs[i] << " ";

	vector<int> dijsk_dist = Dijsktra_distances(matrix, 0);

	cout << "\nDijsktra distances: ";
	for (int i = 0; i < matrix.size(); i++) cout << dijsk_dist[i] << " ";

	cout << "\nTree with BFS\n";

	vector<vector<int>> Tree = CreateTreeBFS(matrix);

	for (int i = 0; i < Tree.size(); i++) {
		for (int j = 0; j < Tree[i].size(); j++) {
			cout << Tree[i][j] << " ";
		}
		cout << endl;
	}

	cout << "\nCalculateDegrees: ";

	vector<int> tree_degrees = calculateDegrees(Tree);

	for (int i = 0; i < matrix.size(); i++) cout << tree_degrees[i] << " ";

	cout << "\nCalculateAverageDegree: " << calculateAverageDegree(Tree);

	vector<int> parent = Prime(matrix);

	cout << "\nPrime Ostov: \n";
	for (int i = 1; i < matrix.size(); i++) {
		cout << "Edge: (" << parent[i] << ", " << i << ") Weight: " << matrix[i][parent[i]] << endl;
	}
	
	vector<vector<int>> primematrix = CreateMatrix(parent, matrix); cout << endl;

	for (int i = 0; i < primematrix.size(); i++) {
		for (int j = 0; j < primematrix[i].size(); j++) {
			cout << primematrix[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}
