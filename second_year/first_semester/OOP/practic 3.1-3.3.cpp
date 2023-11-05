#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

//------------------------------------------------------ Priority Queue. Customer. -----------------------------------------------------------//

class Customer
{
protected:
	string last_name, first_name, city, street, account_number;
	double house_number, average_check_amount;

public:
	Customer(string last_name = "NotStated", string first_name = "NotStated", string city = "NotStated", string street = "NotStated", string account_number = "NotStated", double house_number = 0, double average_check_amount = 0) :
		last_name(last_name), first_name(first_name), city(city), street(street), account_number(account_number), house_number(house_number), average_check_amount(average_check_amount) {};

	~Customer() { }

	Customer(const Customer& other)
	{
		last_name = other.last_name;
		first_name = other.first_name;
		city = other.city;
		street = other.street;
		account_number = other.account_number;
		house_number = other.house_number;
		average_check_amount = other.average_check_amount;
	}

	friend bool operator > (Customer cus1, Customer cus2);
	friend bool operator < (Customer cus1, Customer cus2);
	friend bool operator == (Customer cus1, Customer cus2);

	friend ostream& operator<<(ostream& s, const Customer& cus);
};

ostream& operator<< (ostream& stream, const Customer& C)
{
	stream << C.last_name << " " << C.first_name << " " << C.city << " " << C.street << " " << C.house_number << " " << C.account_number << " " << C.average_check_amount;
	return stream;
}

bool operator <(const Customer cus1, const Customer cus2)
{
	if (cus1.average_check_amount < cus2.average_check_amount) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number < cus2.account_number) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name < cus2.last_name) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name == cus2.last_name && cus1.first_name < cus2.first_name) return true;

	return false;
}

bool operator >(const Customer cus1, const Customer cus2)
{
	if (cus1.average_check_amount > cus2.average_check_amount) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number > cus2.account_number) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name > cus2.last_name) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name == cus2.last_name && cus1.first_name > cus2.first_name) return true;

	return false;
}

bool operator == (Customer cus1, Customer cus2)
{
	return cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name == cus2.last_name && cus1.first_name == cus2.first_name;
}

template<typename T>
void print_queue(T& q)
{
	while (!q.empty())
	{
		cout << q.top() << "\n";
		q.pop();
	}
}

//-------------------------------------------------------------- Heap. ---------------------------------------------------------------------//
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

//куча (heap)
template <class T>
class Heap
{
private:
	//массив
	T* arr;
	//сколько элементов добавлено
	int len;
	//сколько памяти выделено
	int size;
public:

	//доступ к вспомогательным полям кучи и оператор индекса
	int getCapacity() { return size; }
	int getCount() { return len; }

	T& operator[](int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to []: ", index);

		return arr[index];
	}
	//конструктор
	Heap<T>(int MemorySize = 100)
	{
		arr = new T[MemorySize];
		len = 0;
		size = MemorySize;
	}

	Heap<T>(const Heap<T>& other)
	{
		arr = new T[other.size];
		size = other.size;
		for (int i = 0; i < size;i++)
			arr[i] = other.arr[i];
		len = other.len;
	}

	~Heap()
	{
		if (arr != NULL)
			delete[] arr;
		len = 0;
		arr = NULL;
	}

	//поменять местами элементы arr[index1], arr[index2]
	void Swap(int index1, int index2)
	{
		if (index1 <= 0 || index1 >= len)
		{
			if (index1 < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index1);
			if (index1 >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index1);
		}
		if (index2 <= 0 || index2 >= len)
		{
			if (index2 < 0)
				throw IndexOutOfBoundsException("Index can't be less then zero", index2);
			if (index2 >= len)
				throw IndexOutOfBoundsException("Index can't be greater then heap length-1", index2);
		}

		T temp;
		temp = arr[index1];
		arr[index1] = arr[index2];
		arr[index2] = temp;
	}

	//функции получения левого, правого дочернего элемента, родителя или их индексов в массиве
	int GetLeftChildIndex(int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to GetLeftChildIndex:", index);

		return index * 2 + 1;
	}

	int GetRightChildIndex(int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to GetRightChildIndex:", index);
		
		return index * 2 + 2;
	}

	int GetParentIndex(int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("An inadequate index was submitted to GetParentIndex:", index);

		if (index % 2 == 0)
			return index / 2 - 1;
		return index / 2;
	}

	//восстановление свойств кучи после удаления или добавления элемента
	void Heapify(int index = 0)
	{
		//то же, что и SiftDown
		int leftChild = 2 * index + 1;
		int rightChild = 2 * index + 2;

		if (len <= leftChild) return;

		int largestChild = index;

		if (arr[leftChild] > arr[largestChild])
			largestChild = leftChild;

		if (rightChild < len && (arr[rightChild] > arr[largestChild]))
			largestChild = rightChild;

		if (largestChild != index)
		{
			Swap(index, largestChild);
			Heapify(largestChild);
		}
	}

	//просеить элемент вверх
	void SiftUp(int index = -1)
	{
		if (index == -1) index = len - 1;

		if (index == 0) return;

		int parent_index = GetParentIndex(index);

		if (arr[index] > arr[parent_index])
		{
			Swap(index, parent_index);
			SiftUp(parent_index);
		}
	}

	//добавление элемента - вставляем его в конец массива и просеиваем вверх
	//удобный интерфейс для пользователя 
	void push(T v)
	{
		Node<T>* N = new Node<T>;
		N->setValue(v);
		push(N);
	}

	void push(Node<T>* N)
	{
		//добавить элемент и включить просеивание
		if (len < size)
		{
			arr[len] = N->getValue();
			len++;
			SiftUp();
		}
	}

	T ExtractMax()
	{
		//исключить максимум и запустить просеивание кучи
		T res = arr[0];
		Swap(0, len - 1);
		len--;
		if (len > 1)
			Heapify();
		return res;
	}

	T Remove(int index)
	{
		if (index < 0 || index >= len)
			throw IndexOutOfBoundsException("Index less or more then available was submitted to remove: ", index);

		T res = arr[index];

		if (index == len - 1)
			len--;
		else
		{
			Swap(index, len - 1);
			len--;

			if (arr[GetParentIndex(index)] < arr[index])
				SiftUp(index);

			else if (arr[GetParentIndex(index)] > arr[index])
				Heapify(index);
		}
		return res;
	}

	template <class T> friend ostream& operator<< (ostream& stream, Heap<T> heap);
};

template <class T>
ostream& operator<< (ostream& stream, Heap<T> heap)
{
	while (heap.getCount() > 0)
		stream << heap.ExtractMax() << ";\n";

	return stream;
}

//-------------------------------------------------------------- Fib Heap. ---------------------------------------------------------------------//

template <class T>
class FibHeap;

template <class T>
class FibNode
{
private:
	typename list<FibNode<T>*>::iterator current;
	typename list<FibNode<T>*>::iterator parent;

	list<FibNode<T>*> children; // Список содержащий указатели на дочерние узлы текущего узла.

	int degree;
	T value;

public:
	friend class FibHeap<T>;

	typename list<FibNode<T>*>::iterator getCurrent() { return current; }
	typename list<FibNode<T>*>::iterator getParent() { return parent; }
	list<FibNode<T>*> getChildren() { return children; }

	T getValue() { return value; }
};

template <class T>
class FibHeap
{
protected:
	list<FibNode<T>*> heap;
	typename list<FibNode<T>*>::iterator max;
public:

	FibHeap<T>()
	{
		heap.clear();
		max = heap.end();
	}

	virtual ~FibHeap()
	{
		if (!heap.empty())
		{
			for (typename list<FibNode<T>*>::iterator it = heap.begin(); it != heap.end(); ++it)
				delete* it;

			heap.clear();
		}
	}

	FibNode<T>* push(T value)
	{
		FibNode<T>* add = new FibNode<T>;

		add->value = value;
		add->degree = 0;
		add->children.clear();
		add->parent = add->children.end();
		add->current = heap.insert(heap.end(), add);

		if (heap.size() == 1 || (add->value > (*max)->value))
			max = add->current;

		return add;
	}

	void Merge(FibHeap& other)
	{
		heap.splice(heap.end(), other.heap);

		if ((*other.max)->value > (*max)->value)
			max = other.max;
	}

	bool isEmpty() { return heap.empty(); }

	T ExtractMax()
	{
		if (isEmpty()) throw HeapException("Heap is empty! There is nothing to delete!");

		FibNode<T>* maxNode = *max;
		T maxValue = maxNode->value;

		if (!maxNode->children.empty())
			heap.splice(heap.end(), maxNode->children);

		heap.erase(maxNode->current);
		delete maxNode;

		if (!heap.empty())
			Consolidate();
		else
			max = heap.end();

		return maxValue;
	}
	
	void Consolidate()
	{
		vector<FibNode<T>*> degreeTable(heap.size() + 1, nullptr);

		typename list<FibNode<T>*>::iterator it = heap.begin();
		while (it != heap.end())
		{
			FibNode<T>* x = *it;
			int degree = x->degree;

			while (degreeTable[degree] != nullptr)
			{
				FibNode<T>* y = degreeTable[degree];
				if (x->value < y->value)
					swap(x, y);

				Link(x, y);
				
				degreeTable[degree] = nullptr;
				degree++;
			}
			degreeTable[degree] = x;
			++it;
		}

		heap.clear();
		max = heap.end();
		for (int i = 0; i < degreeTable.size(); ++i)
		{
			if (degreeTable[i] != nullptr)
			{
				degreeTable[i]->current = heap.insert(heap.end(), degreeTable[i]);
				if (max == heap.end() || degreeTable[i]->value > (*max)->value)
					max = degreeTable[i]->current;
			}
		}
	}
	
	void Link(FibNode<T>* parent, FibNode<T>* child)
	{
		parent->children.push_back(child);

		if (!child->children.empty())
			parent->children.splice(parent->children.end(), child->children);

		parent->degree += child->degree;
		child->children.clear();
		parent->degree++;
	}

	void print()
	{
		int Size = heap.size();

		for (int i = 0; i < Size; i++)
			cout << ExtractMax() << ";\n";
	}
};

int main()
{
	try
	{
		priority_queue<Customer> q;

		Customer Alex("Lazarev", "Sanya", "Moscow", "Fruktovia", "012313", 8.0, 124);
		Customer Danya("Lykov", "Danya", "Bryansk", "Garsia", "7777", 9.0, 124);
		Customer Danya_2("Lykov", "Danya", "Bryansk", "Garsia", "7777", 3.0, 122);
		Customer Nastya("Pak", "Nastya", "Korea", "Moskovskiy", "343", 3.0, 77);
		Customer Yarik("Malysh", "Yarik", "Balashikha", "Lyambda", "8882", 16.0, 102);
		Customer Vika("Kuslieva", "Vika", "Moscow", "Ryabinovaya", "666", 31.0, 323);

		q.push(Alex);
		q.push(Danya);
		q.push(Nastya);
		q.push(Yarik);
		q.push(Vika);

		cout << "Output of Customer queue items in descending order of priority." << "\n\n";

		print_queue(q);

		Heap<Customer> heap;

		heap.push(Alex);
		heap.push(Danya);
		heap.push(Nastya);
		heap.push(Yarik);
		heap.push(Vika);

		cout << "\nHeap output before deleting 3 element heap\n\n";

		cout << heap;

		heap.Remove(2);

		cout << "\nHeap output after deleting 3 elements heap\n\n";

		cout << heap;

		FibHeap<Customer> fibheap;

		fibheap.push(Alex);
		fibheap.push(Danya);
		fibheap.push(Nastya);
		fibheap.push(Yarik);
		fibheap.push(Vika);

		cout << "\nOutput of elements in the fibonacci heap using ExtractMax\n\n";

		fibheap.print();

		return 0;
	}
	catch (IndexOutOfBoundsException& e)
	{
		cout << "\nIndexOutOfBoundsException has been caught: "; e.print();
	}
	catch (HeapException& e)
	{
		cout << "\nMapExeptionKey has been caught: "; e.print();
	}
}