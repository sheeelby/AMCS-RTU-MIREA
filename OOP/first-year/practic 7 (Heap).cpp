#include <iostream>
#include <string>

using namespace std;

class Schoolboy
{
public:
	string last_name, first_name, address;
	int group, date_of_birth;
	int gender;

	Schoolboy()
	{
		cout << "\nSchoolboy default constructor";
		group = 0;
		gender = 0;
	}

	Schoolboy(const char* l_n, const char* f_n, int g, int sex, int d_o_b, const char* add)
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

	~Schoolboy() { cout << "\nSchoolboy destructor"; }

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

class IndexOutOfBoundsException : public HeapException
{
protected:
	int index;
public:
	IndexOutOfBoundsException(const char* s, int i) : HeapException(s) { index = i; }
	IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : HeapException(e) { index = e.index; }
	virtual void print()
	{
		cout << "IndexOutOfBoundsException: " << str << "; Index: " << index << "; " << what();
	}
};

template <class T>
class Node
{
private:
	T value;
public:
	//установить данные в узле
	T getValue() { return value; }
	void setValue(T v) { value = v; }

	//сравнение узлов
	int operator<(Node N)
	{
		return (value < N.getValue());
	}

	int operator>(Node N)
	{
		return (value > N.getValue());
	}

	//вывод содержимого одного узла
	void print()
	{
		cout << value;
	}
	template <class T1>
	friend ostream& operator<< (ostream& stream, const Node<T1>& N);
};

template<class T1>
ostream& operator<< (ostream& stream, const Node<T1>& N)
{
	stream << N.value;
	return stream;
}

template <class T>
void print(Node<T>* N)
{
	cout << N->getValue() << "\n";
}

template <class T>
class Heap //макс-куча (max-heap)
{
private:
	//массив
	Node<T>* arr;
	//сколько элементов добавлено
	int len;
	//сколько памяти выделено
	int size;
public:

	//доступ к вспомогательным полям кучи и оператор индекса
	int getCapacity() { return size; }
	int getCount() { return len; }

	Node<T>& operator[](int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to []: ", index);

		return arr[index];
	}

	//конструктор
	Heap<T>(int MemorySize = 100)
	{
		arr = new Node<T>[MemorySize];
		len = 0;
		size = MemorySize;
	}

	~Heap<T>()
	{
		delete[] arr;
	}

	//поменять местами элементы arr[index1], arr[index2]
	void Swap(int index1, int index2)
	{
		if (index1 < 0 || index1 >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to swap at the first object: ", index1);
		if (index2 < 0 || index2 >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to swap at the second object: ", index2);
		//здесь нужна защита от дурака

		Node<T> temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}

	//скопировать данные между двумя узлами
	void Copy(Node<T>* dest, Node<T>* source)
	{
		dest->setValue(source->getValue());
	}

	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве
	Node<T>* GetLeftChild(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0 || index >= len)
				throw IndexOutOfBoundsException("An inadequate index was submitted to GetLeftChild: ", index);
			if (index * 2 >= len)
				throw IndexOutOfBoundsException("There will be no left child to GetLeftChild: ", index);
		}
		//здесь нужна защита от дурака
		return &arr[index * 2 + 1];
	}

	Node<T>* GetRightChild(int index)
	{
		if (index < 0 || index * 2 >= len)
		{
			if (index < 0 || index >= len)
				throw IndexOutOfBoundsException("An inadequate index was submitted to GetRightChild: ", index);
			if (index * 2 >= len)
				throw IndexOutOfBoundsException("There will be no right child to GetRightChild: ", index);
		}
		//здесь нужна защита от дурака

		return &arr[index * 2 + 2];
	}

	Node<T>* GetParent(int index)
	{
		if (index <= 0 || index >= len)
		{
			if (index < 0 || index >= len)
				throw IndexOutOfBoundsException("An inadequate index was submitted to GetParent: ", index);
			if (index == 0)
				throw IndexOutOfBoundsException("There will be no parent to GetParent: ", index);
		}
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return &arr[index / 2 - 1];
		return &arr[index / 2];
	}

	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index >= len)
		{
			if (index < 0 || index >= len)
				throw IndexOutOfBoundsException("An inadequate index was submitted to GetLeftChildIndex: ", index);
			//if (index * 2 >= len)
				//throw IndexOutOfBoundsException("There will be no left child to GetLeftChildIndex: ", index);
		}
		//здесь нужна защита от дурака
		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index >= len)
		{
			if (index < 0 || index >= len)
				throw IndexOutOfBoundsException("An inadequate index was submitted to GetRightChildIndex: ", index);
			//if (index * 2 >= len)
				//throw IndexOutOfBoundsException("There will be no right child to GetRightChildIndex: ", index);
		}
		//здесь нужна защита от дурака

		return index * 2 + 2;
	}

	int GetParentIndex(int index)
	{
		if (index <= 0 || index >= len)
		{
			if (index < 0 || index >= len)
				throw IndexOutOfBoundsException("An inadequate index was submitted to GetParentIndex: ", index);
			if (index == 0)
				throw IndexOutOfBoundsException("There will be no parent to GetParentIndex", index);
		}
		//здесь нужна защита от дурака

		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}

	//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		int left_child_index = GetLeftChildIndex(index);
		int right_child_index = GetRightChildIndex(index);

		if (len <= left_child_index) return;

		int max_index = index;

		if (arr[left_child_index] > arr[max_index])
		{
			max_index = left_child_index;
		}

		if (right_child_index < len && arr[right_child_index] > arr[max_index])
		{
			max_index = right_child_index;
		}

		if (max_index != index)
		{
			Swap(index, max_index);
			Heapify(max_index);
		}
	}

	//просеить элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;
		if (index == 0) return;
		int parent_index = GetParentIndex(index);
		//нужно сравнить элементы и при необходимости произвести просеивание вверх
		if (arr[index] > arr[parent_index])
		{
			Swap(index, parent_index);
			SiftUp(parent_index);
		}
	}

	//удобный интерфейс для пользователя 
	template <class T>
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setValue(v);
		push(N);
	}

	//добавление элемента - вставляем его в конец массива и просеиваем вверх
	template <class T>
	void push(Node<T>* N)
	{
		//добавить элемент и включить просеивание
		if (len < size)
		{
			arr[len] = *N;
			len++;
			SiftUp();
		}
	}

	/*
	Node<T>* ExtractMin()
	{
		//исключить максимум и запустить просеивание кучи
		//Node<T>* res = new Node<T>(arr[0]);
		Node<T>* res = new Node<T>; Copy(res, &arr[0]);
		Swap(0, len - 1);
		len--;
		Heapify();
		return res;
	}
	*/

	Node<T>* ExtractMax()
	{
		Node<T>* res = new Node<T>; Copy(res, &arr[0]);
		Swap(0, len - 1);
		len--;
		if (len > 1)
			Heapify();
		return res;
	}

	void Remove(int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("Index less or more available was submitted to Remove: ", index);

		if (index == len - 1)
		{
			len--;
		}
		else
		{ 
			Swap(index, len - 1);
			len--;
			if (arr[index] > *GetParent(index))
				SiftUp(index);
			else
				Heapify(index);
		}
	}

	//перечислить элементы кучи и применить к ним функцию
	void Straight(void(*f)(Node<T>*))
	{
		int i;
		for (i = 0; i < len; i++)
		{
			f(&arr[i]);
		}
	}

	//перебор элементов, аналогичный проходам бинарного дерева
	void PreOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}

	void InOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
	}

	void PostOrder(void(*f)(Node<T>*), int index = 0)
	{
		if (GetLeftChildIndex(index) < len)
			PreOrder(f, GetLeftChildIndex(index));
		if (GetRightChildIndex(index) < len)
			PreOrder(f, GetRightChildIndex(index));
		if (index >= 0 && index < len)
			f(&arr[index]);
	}

	template<class T>
	friend ostream& operator<<(ostream& stream, const Heap<T>& H);
};

template<class T>
ostream& operator<<(ostream& stream, const Heap<T>& H)
{
	for (int i = 0; i < H.len; i++)
		stream << H.arr[i] << "; ";

	return stream;
}

int main()
{
	Heap<int> Tree;

	Tree.push(1);
	Tree.push(-1);
	Tree.push(-2);
	Tree.push(2);
	Tree.push(5);

	Tree.push(6);
	Tree.push(-3);
	Tree.push(-4);
	Tree.push(4);
	Tree.push(3);

	cout << "\n" << Tree;

	Tree.ExtractMax();

	cout << "\n" << Tree;

	Tree.Remove(2);

	cout << "\n";

	while (Tree.getCount())
		cout << *(Tree.ExtractMax()) << "; ";

	Heap<Schoolboy> heap;

	Schoolboy S1("Lykov", "Danya", 11, 1, 2004, "Bryansk");
	Schoolboy S2("Lazarev", "Sasha", 11, 1, 2004, "Moscow");
	Schoolboy S3("Malyash", "Yarik", 11, 1, 2004, "Balashikha");
	Schoolboy S4("Pak", "Nastya", 11, 0, 2004, "Korea");
	Schoolboy S5("Kuslieva", "Vika", 11, 0, 2004, "Moscow");

	cout << S1;

	heap.push(S1); heap.push(S2); heap.push(S3); heap.push(S4); heap.push(S5);

	cout << "\n" << heap;

	heap.Remove(4);

	cout << "\n" << heap;

	while (heap.getCount())
		cout << *(heap.ExtractMax()) << "; ";

	char c; cin >> c;
	return 0;
}
