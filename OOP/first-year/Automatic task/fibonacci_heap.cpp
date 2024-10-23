#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class Schoolboy
{
private:
	string last_name, first_name, address;
	int group, date_of_birth, gender;
public:
	Schoolboy()
	{
		//cout << "\nSchoolboy default constructor";
		group = 0;
		gender = 0;
	}

	Schoolboy(const char* l_n, const char* f_n, int g, int sex, int d_o_b, const char* add)
	{
		//cout << "\nSchoolboy constructor";
		last_name = l_n;
		first_name = f_n;
		group = g;
		gender = sex;
		date_of_birth = d_o_b;
		address = add;
	}

	Schoolboy(const Schoolboy& S)
	{
		//cout << "\nSchoolboy copy constructor";
		last_name = S.last_name;
		first_name = S.first_name;
		group = S.group;
		gender = S.gender;
		date_of_birth = S.date_of_birth;
		address = S.address;
	}

	// переопределение оператора "больше"
	bool operator>(const Schoolboy& other)
	{
		if (group > other.group)
			return true;
		else if (group < other.group)
			return false;

		if (date_of_birth > other.date_of_birth)
			return true;
		else if (date_of_birth < other.date_of_birth)
			return false;

		if (last_name > other.last_name)
			return true;
		else if (last_name < other.last_name)
			return false;

		if (first_name > other.first_name)
			return true;
		else if (first_name < other.first_name)
			return false;
	}

	// переопределение оператора "меньше"
	bool operator<(const Schoolboy& other)
	{
		if (group < other.group)
			return true;
		else if (group > other.group)
			return false;

		if (date_of_birth < other.date_of_birth)
			return true;
		else if (date_of_birth > other.date_of_birth)
			return false;

		if (last_name < other.last_name)
			return true;
		else if (last_name > other.last_name)
			return false;

		if (first_name < other.first_name)
			return true;
		else if (first_name > other.first_name)
			return false;
	}

	// переопределение оператора "равно"
	bool operator==(const Schoolboy& other)
	{
		return last_name == other.last_name && first_name == other.first_name && group == other.group && date_of_birth == other.date_of_birth;
	}

	~Schoolboy() { }

	friend ostream& operator<< (ostream& stream, const Schoolboy& S);
	friend istream& operator>> (istream& stream, Schoolboy& S);
};

ostream& operator<< (ostream& stream, const Schoolboy& S)
{
	return stream << "\nLast Name: " << S.last_name << "\nFirst Name: " << S.first_name <<
		"\nGroup: " << S.group << "\nGender: " << S.gender << "\nDate of Birth: " << S.date_of_birth <<
		"\nAddress: " << S.address;
}

istream& operator>> (istream& stream, Schoolboy& S)
{
	return stream >> S.last_name >> S.first_name >> S.group >> S.gender >> S.date_of_birth >> S.address;
}

class HeapException : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	HeapException(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	HeapException(const HeapException& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~HeapException()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "HeapException: " << str << "; " << what();
	}
};

class InvalidArgument : public HeapException
{
protected:
	//сообщение об ошибке
	char* str;
public:
	InvalidArgument(const char* s) : HeapException(s) { }
	InvalidArgument(const InvalidArgument& e) : HeapException(e) { }
	~InvalidArgument()
	{
		delete[] str;
	}
	virtual void print()
	{
		cout << "InvalidArgument: " << str << "; " << what();
	}
};

template<class T>
class FibMaxHeap;

// Узел фибоначчиевой кучи
template<class T>
class FibHeapNode
{
	// Дружественный класс для кучи для удобства обращения к полям
	friend class FibMaxHeap<T>;

private:
	T _data; // данные

	FibHeapNode<T>* _parent,	// указатель на родительский узел
		* _child,				// указатель на один из дочерних узлов
		* _left,				// указатель на левый узел того же предка
		* _right;				// указатель на правый узел того же предка

	int _degree;	// степень вершины
	bool _marked;	// был ли удален в процессе изменения ключа ребенок этой вершины
public:

	FibHeapNode<T>()
	{
		_parent = _child = nullptr; // По умолчанию родителей и детей нет

		_left = _right = this; // С самого начала зациклен сам на себе

		_degree = 0;
		_marked = false;
	}

	FibHeapNode<T>(T data) : FibHeapNode<T>() { _data = data; }

	T GetData() { return _data; }
	FibHeapNode<T>* GetChild() { return _child; }
	FibHeapNode<T>* GetLeft() { return _left; }
	FibHeapNode<T>* GetRight() { return _right; }
	FibHeapNode<T>* GetParent() { return _parent; }
	int GetDegree() { return _degree; }
	bool GetMarked() { return _marked; }

	void SetData(T data) { _data = data; }
	void SetChild(FibHeapNode<T>* child) { _child = child; }
	void SetLeft(FibHeapNode<T>* left) { _left = left; }
	void SetRight(FibHeapNode<T>* right) { _right = right; }
	void SetParent(FibHeapNode<T>* parent) { _parent = parent; }
	void SetDegree(int deg) { _degree = deg; }
	void SetMarked(bool mark) { _marked = mark; }

	~FibHeapNode() { };

	template <class T>
	friend ostream& operator<< (ostream& stream, const FibHeapNode<T>& N);
};

template<class T>
ostream& operator<< (ostream& stream, FibHeapNode<T>& N)
{
	stream << N.GetData();
	return stream;
}

template<class T>
class FibMaxHeap
{
protected:
	// Корень кучи. Всегда максимум
	FibHeapNode<T>* _root;

	// Кол-во узлов в куче
	int _size;

	// объединение вершин (Справа: first, слева: second)
	virtual void Union(FibHeapNode<T>* first, FibHeapNode<T>* second);

	// объединение вершин, когда справа корень кучи (Удобно переносить узлы к корню)
	virtual void Union(FibHeapNode<T>* node) { if (_root != nullptr) { Union(_root, node); } };

	// Вставить вершину слева от корня
	virtual void Insert(FibHeapNode<T>* node);

	// Удаление всех связей у узла
	virtual void Remove(FibHeapNode<T>* node);

	// Присоеденение дочернего элемента к родительскому
	virtual void Link(FibHeapNode<T>* child, FibHeapNode<T>* parent);

	// Прорежение деревьев
	virtual void Consolidate();

	// Слияние двух куч
	void Join(FibHeapNode<T>* node);

	// Вырезание вершины (отнимаем у родителей >:P )
	virtual void Cut(FibHeapNode<T>* child, FibHeapNode<T>* parent);

	// Каскадное вырезание (С учётом метки)
	virtual void CascadingCut(FibHeapNode<T>* node);
public:

	FibMaxHeap<T>()
	{
		cout << "\nConstructor in FibMaxHeap is working!\n";
		_root = nullptr;
		_size = 0;
	}

	// Можно и _size == 0
	virtual bool IsEmpty() { return _root == nullptr; }

	// Добавление узла в кучу
	virtual FibHeapNode<T>* Push(T data) { return Add(new FibHeapNode<T>(data)); }

	// Добавление узла в кучу
	virtual FibHeapNode<T>* Add(FibHeapNode<T>* node);

	// Слияние куч
	virtual void Join(FibMaxHeap<T>* heap);

	virtual int GetSize() { return _size; }

	virtual FibHeapNode<T>* GetRoot() { return _root; }

	// В куче максимум всегда в корне
	virtual FibHeapNode<T>* GetMaximum() { return GetRoot(); }

	virtual FibHeapNode<T>* FindMaximum() { return GetMaximum(); }

	// Извлечение максимума
	virtual FibHeapNode<T>* ExtractMax();

	// Увеличение приоритета
	virtual FibHeapNode<T>* EncreaseData(FibHeapNode<T>* node, T data);

	// Поиск узла, который соответствует заданному квантилю
	FibHeapNode<T>* FindQuantile(double quantile);

	~FibMaxHeap<T>() { cout << "\nDestructor in FibMaxHeap is working!"; }
};

// Изменение приоритета некоторого элемента
template<class T>
FibHeapNode<T>* FibMaxHeap<T>::EncreaseData(FibHeapNode<T>* node, T data)
{
	if (data < node->_data)
		throw InvalidArgument("The data values in the nodes of the Fibonacci heap are monotonically increasing");

	node->_data = data; // Обновление значения данных в узле

	FibHeapNode<T>* parent = node->_parent;

	if (parent != nullptr && node->_data > parent->_data)
	{
		// Рекурсивно выполняем срезание родительского узла и его перемещение в корневой список кучи.

		Cut(node, parent);

		CascadingCut(parent);
	}

	if (node->_data > _root->_data)
	{
		_root = node;
	}

	return node;
}

template<class T>
FibHeapNode<T>* FibMaxHeap<T>::Add(FibHeapNode<T>* node)
{
	if (_root == nullptr) // Если в куче нет элементов, то только что добавленный максимальный.
		_root = node;
	else // Иначе аккуратно меняем указатели в списке, чтобы не перепутать указатели
	{
		Insert(node);

		if (node->_data > _root->_data) // Передвигаем указатель на новый корень
			_root = node;
	}

	_size++;

	return node;
}

template<class T>
void FibMaxHeap<T>::Insert(FibHeapNode<T>* node)
{
	Remove(node); // Удаляем узел из списка, если он там уже присутствует.

	FibHeapNode<T>* rootLeft = _root->_left;

	node->_right = _root;
	_root->_left = node;

	if (rootLeft != nullptr)
	{
		node->_left = rootLeft;
		rootLeft->_right = node;
	}
}

template<class T>
void FibMaxHeap<T>::Remove(FibHeapNode<T>* node)
{
	FibHeapNode<T>* left = node->_left;
	FibHeapNode<T>* right = node->_right;

	left->_right = right;
	right->_left = left;

	// Обнуляем ссылки на левую и правую связи, чтобы node не указывал ни на какие другие узлы.
	node->_left = node; 
	node->_right = node;
}

// Связать две вершины

template<class T>
void FibMaxHeap<T>::Union(FibHeapNode<T>* first, FibHeapNode<T>* second) // Справа: first! , слева: second!
{
	// Аккуратно меняем указатели местами
	FibHeapNode<T>* firstLeft = first->_left, * secondRight = second->_right;

	second->_right = first;
	first->_left = second;

	firstLeft->_right = secondRight;
	secondRight->_left = firstLeft;
}

template<class T>
void FibMaxHeap<T>::Link(FibHeapNode<T>* child, FibHeapNode<T>* parent)
{
	// Убираем связи у дочерней вершины
	Remove(child);

	// Подвешиваем вершину
	child->_parent = parent;
	child->_marked = false;

	// Даём родителю указатель на дочерний узел, если его нет
	if (parent->_child == nullptr)
	{
		parent->_child = child;
		child->_left = child;
		child->_right = child;
	}
	else
	{
		// Добавляем новых соседей
		FibHeapNode<T>* parentChild = parent->_child;

		child->_left = parentChild->_left;
		child->_right = parentChild;

		parentChild->_left->_right = child;
		parentChild->_left = child;
	}

	parent->_degree++;
}

template<class T>
void FibMaxHeap<T>::Join(FibMaxHeap<T>* heap)
{
	if (heap->_root != nullptr)
		Join(heap->_root);

	_size += heap->_size;
}

template<class T>
void FibMaxHeap<T>::Join(FibHeapNode<T>* node)
{
	if (node == nullptr)
		throw HeapException("We can't connect to an empty heap");

	if (_root == nullptr) // если наша куча пуста, то результатом будет вторая куча
		_root = node;
	else // иначе объединяем два корневых списка
		Union(node);

	if (node->_data > _root->_data) // если максимум кучи изменился, то надо обновить указатель
		_root = node;
}

template<class T>
FibHeapNode<T>* FibMaxHeap<T>::ExtractMax()
{
	if (_root == nullptr) // Если нечего удалять
		return nullptr;

	FibHeapNode<T>* prev = _root;

	if (_root->_child != nullptr)
		Union(_root->_child); // Объединяем корневой список с дочерним списком, если у корневого узла есть дочерние узлы

	// Отсоединяем корневой узел от списка корней

	FibHeapNode<T>* left = _root->_left;
	FibHeapNode<T>* right = _root->_right;

	left->_right = right;
	right->_left = left;

	_size--;

	if (prev == prev->_right) // Если в куче остался только один узел
		_root = nullptr;
	else // В противном случае, если в куче остаются еще узлы, устанавливем новый корневой узел и выполняем процедуру консолидации кучи. 
	{
		_root = prev->_right;

		Consolidate();
	}

	return prev;
}

// Процесс консолидации, который гарантирует, что в куче не будет двух корневых узлов с одинаковыми степенями!
template<class T>
void FibMaxHeap<T>::Consolidate()
{
	int max_degree = _size; // Память для хранения всех возможных степеней корневых узлов.

	FibHeapNode<T>** array = new FibHeapNode<T>*[max_degree]; // Создаем массив и инициализируем его.

	for (int i = 0; i < max_degree; i++)
		array[i] = nullptr;

	array[_root->_degree] = _root;
	FibHeapNode<T>* current = _root->_right;

	while (array[current->_degree] != current)
	{
		if (array[current->_degree] == nullptr)
		{
			array[current->_degree] = current;
			current = current->_right;
		}
		else
		{
			FibHeapNode<T>* conflict = array[current->_degree], *addTo, *adding;

			if (conflict->_data > current->_data)
			{
				addTo = conflict;
				adding = current;
			}
			else
			{
				addTo = current;
				adding = conflict;
			}

			Link(adding, addTo);

			current = addTo;
		}

		if (_root->_data < current->_data)
			_root = current;
	}
}

template<class T>
void FibMaxHeap<T>::Cut(FibHeapNode<T>* child, FibHeapNode<T>* parent)
{
	if (child->_parent != parent) // Родитель ребёнка не соотвествует переданным данным
		throw HeapException("The child's parent does not match the transmitted data");

	Remove(child); // Убираем связи child с parent

	parent->_degree--;

	if (parent->_child == child) // Проверка на то, является ли узел child первым ребенком узла parent
	{
		parent->_child = child->_left ? child->_left : child->_right;
	}

	Union(child); // Подвешиваем узел child к корневому списку кучи

	child->_parent = nullptr;
	child->_marked = false; // Так как у него не может быть дополнительных отрезаний
}

template<class T>
void FibMaxHeap<T>::CascadingCut(FibHeapNode<T>* node)
{
	FibHeapNode<T>* parent = node->_parent;

	if (parent != nullptr)
	{
		if (node->_marked) // Если узел не был отрезан от его родительского узла
		{
			Cut(node, parent);

			CascadingCut(parent);
		}
		else
		{
			node->_marked = true;
		}
	}
}

template<class T>
FibHeapNode<T>* FibMaxHeap<T>::FindQuantile(double quantile)
{
	if (quantile <= 0.0 || quantile > 1.0) // Значение квантиля должно находится в диапазоне(0, 1]!
		throw InvalidArgument("Quantile value should be in range (0, 1]");

	if (_root == nullptr) // Если куча пустая
		return nullptr;

	int n = static_cast<int>(quantile * _size);  // Количество элементов, которое должно быть в максимальном квантиле
	int currentSize = 0;

	FibHeapNode<T>* current = _root;

	do
	{
		int subtreeSize = current->GetDegree() + 1;  // Размер поддерева с корнем в текущей вершине

		if (currentSize + subtreeSize <= n) // Текущее поддерево может быть полностью включено в квантиль
		{
			currentSize += subtreeSize;
			if (currentSize == n)
				return current;

			current = current->GetRight();
		}
		else // Если размер текущего поддерева превышает n, это означает, что искомый квантиль должен находиться внутри текущего поддерева.
		{
			current = current->GetChild();  // Переходим к дочерней вершине
		}

	} while (current != _root); // Обход кучи, начиная с корневого узла.

	return nullptr;
}

int main()
{

	FibMaxHeap<Schoolboy> heap_sc;

	Schoolboy S1("Lykov", "Danya", 11, 1, 2004, "Bryansk");
	Schoolboy S2("Lazarev", "Sasha", 11, 1, 2004, "Moscow");
	Schoolboy S3("Malyash", "Yarik", 17, 1, 2004, "Balashikha");
	Schoolboy S4("Pak", "Nastya", 110, 0, 2004, "Korea");
	Schoolboy S5("Kuslieva", "Vika", 21, 0, 2004, "Moscow");

	heap_sc.Push(S1); heap_sc.Push(S4); heap_sc.Push(S3); heap_sc.Push(S5); FibHeapNode<Schoolboy>* node_sc = heap_sc.Push(S3);
	
	FibMaxHeap<Schoolboy> heap_sc_2;

	Schoolboy S1_("Kitkat", "Danya", 4, 1, 2004, "Bryansk");
	Schoolboy S2_("Twix", "Yarik", 17, 1, 2004, "Bryansk");
	Schoolboy S3_("Nuts", "Sanya", 23, 1, 2004, "Bryansk");
	Schoolboy S4_("MilkyWay", "Senya", 32, 1, 2004, "Penza");

	heap_sc_2.Push(S1_); heap_sc_2.Push(S2_); FibHeapNode<Schoolboy>* node_sc_2 = heap_sc_2.Push(S3_);
	
	cout << "\nRealization FindMaximum: " << *heap_sc.FindMaximum() << endl;

	cout << "\nRealization FindQuantile: " << *heap_sc.FindQuantile(0.25) << endl;

	cout << "\nRealization EncreaseData: " << endl; heap_sc.EncreaseData(node_sc, S4_);

	heap_sc.Join(&heap_sc_2);

	// Вывод по убыванию приоритета---------------------------------------------------------------------------------------------------------------------------

	while (!heap_sc.IsEmpty())
		cout << heap_sc.ExtractMax()->GetData() << endl;

	return 0;
}