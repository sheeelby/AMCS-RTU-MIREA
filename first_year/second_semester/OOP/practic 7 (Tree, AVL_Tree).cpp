#include <iostream>
#include <string>

using namespace std;

class Schoolboy
{
public:
	string last_name, first_name, date_of_birth, address;
	int group;
	int gender;

	Schoolboy()
	{
		cout << "\nSchoolboy default constructor";
		group = 0;
		gender = 0;
	}

	Schoolboy(const char* l_n, const char* f_n, int g, int sex, const char* d_o_b, const char* add)
	{
		cout << "\nSchoolboy constructor";
		last_name = l_n;
		first_name = f_n;
		group = g;
		gender = sex;
		date_of_birth = d_o_b;
		address = add;
	}

	Schoolboy(const Schoolboy& S)
	{
		cout << "\nSchoolboy copy constructor";
		last_name = S.last_name;
		first_name = S.first_name;
		group = S.group;
		gender = S.gender;
		date_of_birth = S.date_of_birth;
		address = S.address;
	}

	// переопределение оператора "больше"
	bool operator>(const Schoolboy& other) const
	{
		if (last_name != other.last_name)
			return last_name > other.last_name;
		else
			return first_name > other.first_name;
	}

	// переопределение оператора "меньше"
	bool operator<(const Schoolboy& other) const
	{
		if (last_name != other.last_name)
			return last_name < other.last_name;
		else
			return first_name < other.first_name;
	}

	// переопределение оператора "равно"
	bool operator==(const Schoolboy& other) const
	{
		return last_name == other.last_name && first_name == other.first_name;
	}

	~Schoolboy() { cout << "\nSchoolboy destructor"; }

	friend ostream& operator<< (ostream& stream, Schoolboy& S);
	friend istream& operator>> (istream& stream, Schoolboy& S);
};

ostream& operator<< (ostream& stream, Schoolboy& S)
{
	return stream << "\nLast Name: " << S.last_name << "\nFirst Name: " << S.first_name << 
		"\nGroup: " << S.group << "\nGender: " << S.gender << "\nDate of Birth: " << S.date_of_birth <<
		"\nAddress: " << S.address;
}

istream& operator>> (istream& stream, Schoolboy& S)
{
	return stream >> S.last_name >> S.first_name >> S.group >> S.gender >> S.date_of_birth >> S.address;
}

template<class T>
class Node
{
protected:
	//закрытые переменные Node N; N.data = 10 вызовет ошибку
	T data;

	//не можем хранить Node, но имеем право хранить указатель
	Node* left;
	Node* right;
	Node* parent;

	//переменная, необходимая для поддержания баланса дерева
	int height;

	virtual Node<T>* Min(Node<T>* current)
	{
		while (current->getLeft() != NULL)
			current = current->getLeft();
		return current;
	}

	virtual Node<T>* Max(Node<T>* current)
	{
		while (current->getRight() != NULL)
			current = current->getRight();
		return current;
	}
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { data = d; }
	virtual T getData() { return data; }
	int getHeight() { return height; }

	virtual Node* getLeft() { return left; }
	virtual Node* getRight() { return right; }
	virtual Node* getParent() { return parent; }

	virtual void setLeft(Node* N) { left = N; }
	virtual void setRight(Node* N) { right = N; }
	virtual void setParent(Node* N) { parent = N; }

	//Конструктор. Устанавливаем стартовые значения для указателей
	Node<T>(T n)
	{
		data = n;
		left = right = parent = NULL;
		height = 1;
	}

	Node<T>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		data = 0;
		height = 1;
	}

	virtual void print()
	{
		cout << "\n" << data;
	}

	virtual void setHeight(int h)
	{
		height = h;
	}

	template<class T> friend ostream& operator<< (ostream& stream, Node<T>& N);

	Node* successor()
	{
		if (right != NULL)
			return Min(right);
		Node* current = parent;
		while (current != NULL && current->data < data) current = current->parent;
		return current;
	}

	Node* predecessor()
	{
		if (left != NULL)
			return Max(left);
		if (parent == NULL) return NULL;
		Node* current = this;
		while (current->parent != NULL && current->parent->left == current)
			current = current->parent;
		return current->parent;
	}

	// Перегрузка операторов ++ и --
	Node<T>* operator++()
	{
		return successor();
	}

	Node<T>* operator--()
	{
		return predecessor();
	}
};

template<class T>
ostream& operator<< (ostream& stream, Node<T>& N)
{
	stream << "\nNode data: " << N.data << ", height: " << N.height;
	return stream;
}

template<class T>
void print(Node<T>* N) { cout << "\n" << N->getData(); }

template<class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	Node<T>* root;
public:
	//доступ к корневому элементу
	virtual Node<T>* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//функция добавления узла в дерево
	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, root);
	}

	//рекуррентная функция добавления узла. Интерфейс аналогичен (добавляется корень поддерева, 
	//куда нужно добавлять узел), но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		if (Current == NULL) Current = root;

		//пустое дерево - добавляем в корень
		if (root == NULL)
		{
			root = N;
			return root;
		}

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push_R(N, Current->getLeft());
			else
			{
				Current->setLeft(N);
				N->setParent(Current);
			}
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push_R(N, Current->getRight());
			else
			{
				Current->setRight(N);
				N->setParent(Current);
			}
		}
		return N;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T value)
	{
		Node<T>* N = new Node<T>(value);
		return push_R(N);
	}

	//функция удаления узла из дерева
	virtual void remove(Node<T>* N)
	{
		if (N == NULL) return; // Если не нашли, то выходим

		// Если удаляемый узел имеет двух потомков
		if (N->getLeft() != NULL && N->getRight() != NULL)
		{
			Node<T>* successor = N->successor(); // Находим преемника
			N->setData(successor->getData()); // Копируем данные преемника в удаляемый узел
			N = successor; // Теперь удаляемый узел - преемник
		}

		// Если удаляемый узел имеет только одного потомка или не имеет их вообще
		Node<T>* child = NULL;

		if (N->getLeft() != NULL)
			child = N->getLeft();
		else
		{
			if (N->getRight() != NULL)
				child = N->getRight();
		}

		if (child != NULL)
			child->setParent(N->getParent()); // Устанавливаем родителя для потомка

		if (N == root)
			root = child; // Если удаляем корень, то новый корень - потомок удаляемого узла
		else
		{
			if (N == N->getParent()->getLeft())
				N->getParent()->setLeft(child); // Если удаляемый узел - левый потомок, то переставляем указатель на потомка
			else
				N->getParent()->setRight(child); // Если удаляемый узел - правый потомок, то переставляем указатель на потомка
		}
		delete N; // Удаляем узел
	}

	//поиск минимума и максимума в дереве
	virtual Node<T>* Min(Node<T>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		Node<T>* current = root;

		while (current->getLeft() != NULL)
			current = current->getLeft();

		return current;
	}

	virtual Node<T>* Max(Node<T>* Current = NULL)
	{
		if (root == NULL)
			return NULL;

		Node<T>* current = root;

		while (current->getRight() != NULL)
			current = current->getRight();

		return current;
	}

	//поиск узла в дереве
	virtual Node<T>* Find(T data)
	{
		if (root == NULL) return NULL;
		return Find_R(data, root);
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current)
	{
		//база рекурсии
		if (Current == NULL || Current->getData() == data) return Current;
		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() < data) return Find_R(data, Current->getRight());
	}

	//три обхода дерева
	virtual void PreOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(Node<T>* N, void (*f)(Node<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}

	Node<T>* operator[](T value)
	{
		Node<T>* res = Find(value);
		if (res == NULL) { cout << "\Element not found"; }
		else return res;
	}

	template<class T> friend ostream& operator<<(ostream& stream, const Tree<T>& tree);
};

template<class T>
ostream& operator<<(ostream& stream, Tree<T>& tree)
{

	Node<T>* node = tree.Min();

	while (node != NULL)
	{
		T res = node->getData();
		stream << res << ' ';

		node = node->successor();
	}

	return stream;
}

template <typename T>
void search_greater_and_less(Tree<T>& tree, T searchValue)
{
	Node<T>* node = tree.Find(searchValue);

	if (node == nullptr)
	{
		cout << "Element not found\n";
		return;
	}

	//cout << "\nNodes greater than " << searchValue << ":\n";

	Node<T>* greaternode = ++(*node);

	for (greaternode; greaternode != NULL; greaternode = ++(*greaternode))
	{
		T res_great = greaternode->getData();
		cout << res_great << "\n";
	}

	//cout << "Nodes less than " << searchValue << ":\n";

	Node<T>* lessnode = --(*node); // Предыдущий элемент

	for (lessnode; lessnode != NULL; lessnode = --(*lessnode))
	{ 
		T res_less = lessnode->getData();
		cout << res_less << "\n";
	}
}

template<class T>
class AVL_Tree : public Tree<T>
{
protected:
	//определение разности высот двух поддеревьев
	int bfactor(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(Node<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	Node<T>* RotateRight(Node<T>* p) // правый поворот вокруг p
	{
		Node<T>* q = p->getLeft();
		p->setLeft(q->getRight());
		q->setRight(p);

		if (p == Tree<T>::root) Tree<T>::root = q;
		else
		{
			//if(p->getData()<p->getParent()->getData())
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}
		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL) p->getLeft()->setParent(p);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	Node<T>* RotateLeft(Node<T>* q) // левый поворот вокруг q
	{
		Node<T>* p = q->getRight();
		q->setRight(p->getLeft());
		p->setLeft(q);

		if (q == Tree<T>::root) Tree<T>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}
		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL) q->getRight()->setParent(q);

		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	Node<T>* Balance(Node<T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0) RotateRight(p->getRight());
			/*{
				p->setRight(RotateRight(p->getRight()));
				p->getRight()->setParent(p);
			}*/
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			/*{
				p->setLeft(RotateLeft(p->getLeft()));
				p->getLeft()->setParent(p);
			}*/
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	//конструктор AVL_Tree вызывает конструктор базового класса Tree
	AVL_Tree<T>() : Tree<T>() {}

	virtual Node<T>* push_R(Node<T>* N)
	{
		return push_R(N, Tree<T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//вызываем функцию push_R из базового класса
		Node<T>* pushedNode = Tree<T>::push_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		return push_R(N);
	}

	//удаление узла
	virtual void Remove(Node<T>* N)
	{
		Node<T>* parent = N->getParent();

		Tree<T>::remove(N);

		if (parent != NULL) Balance(parent);

		return;
	}
};

int main()
{
	Tree<double> T;
	int arr[15];
	int i = 0;
	for (i = 0; i < 15; i++) arr[i] = (int)(i+1);
	for (i = 0; i < 15; i++) T.push(arr[i]);

	//T.remove(T.Find(10));
	//cout << T;

	Tree <Schoolboy> T1;

	Schoolboy S1("Lykov", "Danya", 11, 1, "14.09.2004", "Bryansk");
	Schoolboy S2("Lazarev", "Sasha", 11, 1, "14.02.2004", "Moscow");
	Schoolboy S3("Malyash", "Yarik", 11, 1, "05.12.2004", "Balashikha");
	Schoolboy S4("Pak", "Nastya", 11, 0, "02.09.2004", "Korea");
	Schoolboy S5("Kuslieva", "Vika", 11, 0, "07.08.2004", "Moscow");

	T1.push(S1); T1.push(S2); T1.push(S3); T1.push(S4); T1.push(S5);

	cout << T1;

	search_greater_and_less(T1, S4);

	//cout << *T1[S5];

	AVL_Tree<double> A;

	double s1 = 9.0; double s2 = -3.0; double s3 = 4.6; double s4 = 2.7; double s5 = 10.0; double s6 = 3.2;
	
	A.push(s1); A.push(s2); A.push(s3); A.push(s4); A.push(s5); A.push(s6);

	//cout << "\n" << A;

	A.remove(A[s3]);

	//cout << "\n" << A;

	cout << "\n-------------------------------------------------------------------------------------------";

	return 0;
}
