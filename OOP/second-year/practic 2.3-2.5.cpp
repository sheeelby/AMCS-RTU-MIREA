#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>

using namespace std;

//------------------------- Exceptions. TreeException. TreeException/Exception in operator *. ---------------------------------------//

class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~Exception()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "Exception: " << str << "; " << what();
	}
};

class ExceptionForIterator : public Exception
{
public:

	ExceptionForIterator(const char* s) : Exception(s) { }
	ExceptionForIterator(const ExceptionForIterator& e) : Exception(e) { }

	virtual void print()
	{
		cout << "ExceptionForIterator: " << str << "; " << what();
	}
};

class TreeException : public Exception
{
public:
	TreeException(const char* s) : Exception(s) {}

	TreeException(const Exception& e) : Exception(e) {}

	virtual void print() { cout << "\nTreeException: " << str << "; " << what(); }
};

class TreeIteratorException : public TreeException
{
public:
	TreeIteratorException(const char* s) : TreeException(s) {}

	TreeIteratorException(const TreeIteratorException& e) : TreeException(e) {}

	virtual void print() { cout << "\nTreeIteratorException: " << str << "; " << what(); }
};

//---------------------------------------------------- Class Customer -------------------------------------------------------//

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

	friend ostream& operator<<(ostream& s, const Customer& cus);
	
	// Необходимо добавить сравнение элементов класса Customer по ключам:

	friend bool operator > (const Customer cus1, const Customer cus2);
	friend bool operator < (const Customer cus1, const Customer cus2);
	friend bool operator == (const Customer cus1, const Customer cus2);
	friend bool operator != (const Customer cus1, const Customer cus2);

	friend bool CustomerComparisonKey(Customer cus1, Customer cus2);
	friend bool CustomerPriority(Customer cus1, Customer cus2);
};

ostream& operator<< (ostream& stream, const Customer& C)
{
	return stream << C.last_name << " " << C.first_name <<
		" " << C.city << " " << C.street << " " << C.house_number <<
		" " << C.account_number << " " << C.average_check_amount;
}

bool operator > (const Customer cus1, const Customer cus2)
{
	if (cus1.last_name > cus2.last_name) return true;

	else if (cus1.last_name == cus2.last_name && cus1.first_name > cus2.first_name) return true;

	return false;
}

bool operator < (const Customer cus1, const Customer cus2)
{
	if (cus1.last_name < cus2.last_name) return true;

	else if (cus1.last_name == cus2.last_name && cus1.first_name < cus2.first_name) return true;

	return false;
}

bool operator == (const Customer cus1, const Customer cus2)
{
	return cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name == cus2.last_name && cus1.first_name == cus2.first_name;
}

bool operator != (const Customer cus1, const Customer cus2)
{
	return cus1.average_check_amount != cus2.average_check_amount && cus1.account_number != cus2.account_number && cus1.last_name != cus2.last_name && cus1.first_name != cus2.first_name;
}

bool CustomerComparisonKey(Customer cus1, Customer cus2)
{
	return cus1.first_name == cus2.first_name && cus1.last_name == cus2.last_name;
}

bool CustomerPriority(Customer cus1, Customer cus2)
{
	if (cus1.average_check_amount > cus2.average_check_amount) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number > cus2.account_number) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name > cus2.last_name) return true;

	else if (cus1.average_check_amount == cus2.average_check_amount && cus1.account_number == cus2.account_number && cus1.last_name == cus2.last_name && cus1.first_name > cus2.first_name) return true;

	return false;
}

//------------------------------------------------------------ Priority. Comparison Key -------------------------------------------------------//

template <class T>
bool Priority(T value1, T value2)
{
	return (value1 > value2);
}

bool Priority(Customer cust1, Customer cust2)
{
	return CustomerPriority(cust1, cust2);
}

template <class T>
bool ComparisonKey(T value1, T value2)
{
	return (value1 == value2);
}

bool ComparisonKey(Customer cust1, Customer cust2)
{
	return CustomerComparisonKey(cust1, cust2);
}

//----------------------------------------------------------- Решение задачи 1.3 - 1.5 -------------------------------------------------------------//

template <class T>
class Element
{
	//элемент связного списка
private:
	//указатель на предыдущий и следующий элемент
	Element* next;
	Element* prev;

	//информация, хранимая в поле
	T field;
public:
	//доступ к полю *next
	virtual Element* getNext() { return next; }
	virtual void setNext(Element* value) { next = value; }

	//доступ к полю *prev
	virtual Element* getPrevious() { return prev; }
	virtual void setPrevious(Element* value) { prev = value; }

	//доступ к полю с хранимой информацией field
	virtual T getValue() { return field; }
	virtual void setValue(T value) { field = value; }

	template<class T1> friend ostream& operator<< (ostream& ustream, Element<T1>& obj);

	Element(T value) { field = value; next = prev = NULL; }
};

template<class T>
ostream& operator << (ostream& ustream, Element<T>& obj)
{
	ustream << obj.field;
	return ustream;
}

template <class T>
class LinkedListParent
{
protected:
	//достаточно хранить начало и конец
	Element<T>* head;
	Element<T>* tail;
	//для удобства храним количество элементов
	int num;
public:
	virtual int Number() { return num; }

	virtual Element<T>* getBegin() { return head; }

	virtual Element<T>* getEnd() { return tail; }

	LinkedListParent()
	{
		//конструктор без параметров
		head = NULL; tail = NULL;
		num = 0;
	}

	LinkedListParent(const LinkedListParent& other)
	{
		head = other.head;
		tail = other.tail;
		num = other.num;
	}

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
	virtual Element<T>* push(T value) = 0;

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
	virtual T pop() = 0;

	virtual ~LinkedListParent()
	{
		//деструктор - освобождение памяти
	}

	//получение элемента по индексу - какова асимптотическая оценка этого действия?
	virtual Element<T>* operator[](int i)
	{
		//индексация
		if (i<0 || i>num) return NULL;
		int k = 0;

		//ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
		Element<T>* cur = head;
		for (k = 0; k < i; k++)
		{
			cur = cur->getNext();
		}
		return cur;
	}

	virtual void Filter(LinkedListParent<T>* list, bool (*predict)(T))
	{
		for (Element<T>* current = head; current != nullptr; current = current->getNext())
			if (predict(current->getValue()))
				list->push(current->getValue());
	}

	template<class T1> friend ostream& operator<< (ostream& ustream, LinkedListParent<T1>& obj);
	template<class T1> friend istream& operator>> (istream& ustream, LinkedListParent<T1>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, LinkedListParent<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
			ustream << current->getValue() << " ";
		return ustream;
	}

	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
		ustream << "arr[" << i << "] = " << current->getValue() << "\n";

	return ustream;
}

template<class T>
istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
{
	//чтение из файла и консоли совпадают
	int len;
	ustream >> len;
	//здесь надо очистить память под obj, установить obj.num = 0
	double v = 0;
	for (int i = 0; i < len; i++)
	{
		ustream >> v;
		obj.push(v);
	}
	return ustream;
}

//дописать класс итератора по списку
template<typename ValueType>
class ListIterator : public iterator<input_iterator_tag, ValueType>
{
public:
	//конструкторы
	ListIterator() { ptr = NULL; }
	ListIterator(Element<ValueType>* p) { ptr = p; }
	ListIterator(const ListIterator& it) { ptr = it.ptr; }

	//методы работы с итераторами
	//присваивание
	ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
	ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }

	//проверка итераторов на равенство
	bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
	bool operator==(ListIterator const& other) const { return ptr == other.ptr; }
	//получить значение
	Element<ValueType>& operator*()
	{
		//Тут должен быть exception!
		if (ptr == NULL)
			throw ExceptionForIterator("An iterator cannot be associated with any element if there are none.");

		return *ptr;
	}
	//перемещение с помощью итераторов
	ListIterator& operator++() { ptr = ptr->getNext();  return *this; } //Префиксный ++
	ListIterator& operator++(int v) { ptr = ptr->getNext(); return *this; } //Постфиксный ++
	ListIterator& operator--() { ptr = ptr->getPrevious();  return *this; } //Префиксный --
	ListIterator& operator--(int v) { ptr = ptr->getPrevious(); return *this; } //Постфиксный --

	//текущий элемент
	Element<ValueType>* ptr;
};

//класс итерируемый список - наследник связного списка, родитель для Очереди и Стека
template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
	IteratedLinkedList() : LinkedListParent<T>() { }
	virtual ~IteratedLinkedList() { }

	ListIterator<T> begin() { ListIterator<T> it = LinkedListParent<T>::head; return it; }
	ListIterator<T> end() { ListIterator<T> it = LinkedListParent<T>::tail; return it; }

	template <class T1> friend ostream& operator<< (ostream& ustream, IteratedLinkedList<T1>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, IteratedLinkedList<T>& obj)
{
	if (typeid(ustream).name() == typeid(ofstream).name())
	{
		ustream << obj.num << "\n";
		for (ListIterator<T> current = obj.begin(); current != NULL; current++)
			ustream << (*current).getValue() << " ";
		return ustream;
	}

	ustream << "\nLength: " << obj.num << "\n";
	int i = 0;
	for (ListIterator<T> current = obj.begin(); current != NULL; current++, i++)
		ustream << "arr[" << i << "] = " << (*current).getValue() << "\n";

	return ustream;
}

template <class T>
class Queue : public IteratedLinkedList<T>
{
public:
	Queue() : IteratedLinkedList<T>() { }

	~Queue() { }

	Element<T>* push(T obj)
	{
		Element<T>* x = new Element<T>(obj);

		if (this->tail != NULL)
		{
			this->tail->setNext(x);
			x->setPrevious(this->tail);
			this->tail = x;
		}
		else
		{
			this->head = x;
			this->tail = this->head;
		}

		this->num++;

		return this->tail;
	}

	T pop()
	{
		if (this->head == NULL) { return T(); }

		T x = this->head->getValue();
		Element<T>* next = this->head->getNext();

		this->head->setNext(NULL);

		if (next != NULL) { next->setPrevious(NULL); }

		this->head = next;
		this->num--;
		return x;
	}

	Queue<T> filter(bool (*predict)(T))
	{
		Queue<T> filteredQueue;
		Element<T>* current = this->head;

		while (current != nullptr)
		{
			if (predict(current->getValue()))
				filteredQueue.push(current->getValue());

			current = current->getNext();
		}

		return filteredQueue;
	}
};

template <class T>
class SortedQueue : public Queue<T>
{
public:
	SortedQueue() : Queue<T>() { }

	Element<T>* push(T val)
	{
		Element<T>* cur = new Element<T>(val);

		Element<T>* current = this->head;
		Element<T>* previous = nullptr;

		while (current != nullptr && current->getValue() <= val)
		{
			previous = current;
			current = current->getNext();
		}

		if (previous == nullptr)
		{
			cur->setNext(this->head);
			this->head = cur;
		}
		else
		{
			cur->setNext(current);
			cur->setPrevious(previous);
			previous->setNext(cur);
		}

		if (current == nullptr)
			this->tail = cur;

		this->num++;
		return cur;
	}
};

template <class T>
class SortedQueueIterated : public IteratedLinkedList<T>
{
protected:
	bool(*PriorityKey)(T, T);

public:
	SortedQueueIterated(bool(*priority)(T, T) = Priority) : IteratedLinkedList<T>() { PriorityKey = priority; }

	SortedQueueIterated(const SortedQueueIterated& other) : IteratedLinkedList<T>(other) { PriorityKey = other.PriorityKey; }

	Element<T>* push(T val)
	{
		Element<T>* x = new Element<T>(val);

		ListIterator<T> it = this->begin();

		for (; it != NULL && (*it).getValue() > val; it++);

		if (it == NULL)
		{
			if (it == this->begin())
			{
				this->head = this->tail = x;
				this->num++;
				return x;
			}

			this->tail->setNext(x);
			x->setPrevious(this->tail);
			this->tail = x;
			this->num++;
			return x;

		}
		if (it == this->begin())
		{
			(*it).setPrevious(x);
			x->setNext(this->head);
			this->head = x;
			this->num++;
			return x;
		}

		x->setNext(it.ptr);
		x->setPrevious((--it).ptr);
		(*it).setNext(x);
		(it++)++;
		(*it).setPrevious(x);

		this->num++;
		return x;
	}

	T pop()
	{
		ListIterator<T> it = (this->begin());

		if (it == NULL) return T();

		ListIterator<T> next = ++it; it--;

		(*next).setPrevious(NULL);
		this->head = next.ptr;

		(*it).setNext(NULL); // Обрубаем связи у it

		this->num--;

		return (*it).getValue();
	}

	T remove(T value)
	{
		ListIterator<T> it = this->begin();

		for (; it != NULL && (*it).getValue() != value; it++);

		if (it != NULL)
		{
			if (it == this->begin()) return pop(); // Удаляем с начала

			else if (it == this->end()) // Удаляем с конца. Нужно обработать предпоследний элемент и хвост.
			{
				ListIterator<T> previous = --it; it++;

				(*previous).setNext(NULL);
				this->tail = previous.ptr;

				(*it).setPrevious(NULL); // Обрубаем связи у it

				this->num--;

				return (*it).getValue();
			}

			else
			{
				ListIterator<T> previous = --it; it++;
				ListIterator<T> next = ++it; it--;

				(*previous).setNext(next.ptr);
				(*next).setPrevious(previous.ptr);

				(*it).setNext(NULL);
				(*it).setPrevious(NULL);

				this->num--;

				return (*it).getValue();
			}
		}
		return T();
	}
};

template<class T>
ostream& operator << (ostream& stream, SortedQueueIterated<T> sqi)
{
	for (ListIterator<T> current = sqi.begin(); current != NULL; current++)
		stream << "\n" << (*current).getValue() << ";  ";

	return stream;
}

template<class T>
bool operator == (SortedQueueIterated<T> sqi1, SortedQueueIterated<T> sqi2)
{

	if (sqi1.Number() == sqi2.Number())
	{
		for (ListIterator<T> it1 = sqi1.begin(), it2 = sqi2.begin(); it1 != NULL && it2 != NULL; it1++, it2++)
			if ((*it1).getValue() != (*it2).getValue())
				return false;

		 return true;
	}

	return false;
}

template<class T>
bool operator > (SortedQueueIterated<T> sqi1, SortedQueueIterated<T> sqi2)
{
	if (sqi1.Number() == sqi2.Number())
	{
		for (ListIterator<T> it1 = sqi1.begin(), it2 = sqi2.begin(); it1 != NULL && it2 != NULL; it1++, it2++)
		{
			if ((*it1).getValue() > (*it2).getValue())
				return true;
			if ((*it1).getValue() < (*it2).getValue())
				return false;
		}
	}

	return false;
}

template<class T>
bool operator < (SortedQueueIterated<T> sqi1, SortedQueueIterated<T> sqi2)
{
	if (sqi1.Number() == sqi2.Number())
	{
		for (ListIterator<T> it1 = sqi1.begin(), it2 = sqi2.begin(); it1 != NULL && it2 != NULL; it1++, it2++)
		{
			if ((*it1).getValue() < (*it2).getValue())
				return true;
			if ((*it1).getValue() > (*it2).getValue())
				return false;
		}
	}

	return false;
}

//-------------------------------------------------- Node ----------------------------------------------------------------//

template<class T>
class Node
{
protected:
	T data;

	Node* left;
	Node* right;
	Node* parent;

	int height; //переменная, необходимая для поддержания баланса дерева

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
		height = 1;
	}

	virtual void print() { cout << "\n" << data;  }

	virtual void setHeight(int h) {	height = h; }

	template<class T> friend ostream& operator<< (ostream& stream, Node<T> N);

	Node* successor()
	{
		Node<T>* Current;
		if (right != NULL)
		{
			Current = right;
			while (Current->left != NULL)
				Current = Current->left;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data < data)
				Current = Current->parent;
		}
		return Current;
	}

	Node* predecessor()
	{
		Node<T>* Current;
		if (left != NULL)
		{
			Current = left;
			while (Current->right != NULL)
				Current = Current->right;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data > data)
				Current = Current->parent;
		}
		return Current;
	}
};

template<class T>
ostream& operator<< (ostream& stream, Node<T> N)
{
	stream << "\nNode data: " << N.data << ", height: " << N.height;
	return stream;
}

template<class T>
void print(Node<T>* N) { cout << "\n" << N->getData(); }

//---------------------------------------------------------- Tree ----------------------------------------------------------------//

template<class T>
class Tree
{
protected:
	Node<T>* root; //корень - его достаточно для хранения всего дерева

	virtual Node<T>* push_R(Node<T>* N, Node<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		//пустое дерево - добавляем в корень
		if (root == NULL) { root = N; return N; }

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push_R(N, Current->getLeft());
			else { Current->setLeft(N); N->setParent(Current); }
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push_R(N, Current->getRight());
			else { Current->setRight(N); N->setParent(Current); }
		}
		//if (Current->getData() == N->getData()) ?
		//вернуть добавленный узел
		return N;
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual Node<T>* Find_R(T data, Node<T>* Current)
	{
		//база рекурсии
		if (Current == NULL) return NULL;

		if (Current->getData() == data) return Current;

		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() < data) return Find_R(data, Current->getRight());
	}

public:
	//доступ к корневому элементу
	virtual Node<T>* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual Node<T>* push(Node<T>* N)
	{
		return push_R(N, root);
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual Node<T>* push(T n)
	{
		Node<T>* N = new Node<T>;
		N->setData(n);
		push(N);
		return N;
	}

	//функция удаления узла из дерева
	virtual Node<T>* RemoveNode(Node<T>* N)
	{
		if (N == NULL) return NULL; // Если удалять нечего, то выходим

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
		return N; // Удаляем узел
	}

	virtual Node<T>* Remove(T N)
	{
		return RemoveNode(Find(N));
	}

	virtual Node<T>* Min(Node<T>* Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (root == NULL) return NULL;

		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();
		return Current;
	}

	virtual Node<T>* Max(Node<T>* Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (root == NULL) return NULL;
		if (Current == NULL) Current = root;
		for (; Current->getRight() != NULL; Current = Current->getRight());
		return Current;
	}

	virtual Node<T>* Find(T data)
	{
		return Find_R(data, root);
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
};

//----------------------------------------------------------- AVL Tree ---------------------------------------------------------------//

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

		if (p == Tree<T>::root)
			Tree<T>::root = q;
		else
		{
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

		if (q == Tree<T>::root)
			Tree<T>::root = p;
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
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
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
	virtual Node<T>* Remove(Node<T>* N)
	{
		Node<T>* parent = N->getParent();

		Node<T>* removed = Tree<T>::Remove(N->getData());

		if (parent != NULL) Balance(parent);

		return removed;
	}
};

// -------------------------------------------------------- Tree Iterator ------------------------------------------------------------//

template<typename ValueType>
class TreeIterator : public std::iterator<std::input_iterator_tag, ValueType> // Итератор по дереву
{
private:

public:
	Node<ValueType>* ptr;

	TreeIterator() { ptr = NULL; }
	TreeIterator(Node<ValueType>* p) { ptr = p; }
	TreeIterator(const TreeIterator& it) { ptr = it.ptr; }

	TreeIterator& operator=(const TreeIterator& it) { ptr = it.ptr; return *this; }
	TreeIterator& operator=(Node<ValueType>* p) { ptr = p; return *this; }

	bool operator!=(TreeIterator const& other) const { return ptr != other.ptr; }
	bool operator==(TreeIterator const& other) const { return ptr == other.ptr; }

	Node<ValueType>& operator*() 
	{
		//if (ptr == NULL) { throw TreeIteratorException("\nAn iterator cannot be associated with any element if there are none."); }

		return *ptr;
	}

	TreeIterator& operator++() // Префиксный ++
	{
		if (ptr != NULL)
			ptr = ptr->successor();

		return *this;
	}

	TreeIterator operator++(int v) // Постфиксный ++
	{
		Node<ValueType>* p = ptr;

		if (ptr != NULL)
			ptr = ptr->successor();

		return TreeIterator<ValueType>(p);
	}

	TreeIterator& operator--() // Префиксный --
	{
		if (ptr != NULL)
			ptr = ptr->predecessor();

		return *this;
	}

	TreeIterator operator--(int v) // Постфиксный --
	{
		Node<ValueType>* p = ptr;

		if (ptr != NULL)
			ptr = ptr->predecessor();

		return TreeIterator<ValueType>(p);
	}
};

// -------------------------------------------------------- Iterated Tree ------------------------------------------------------------//

template<class T>
class IteratedTree : public AVL_Tree<T> // Переопределенные методы из AVL_Tree и Tree, переписанные под итераторы.
{
protected:
	virtual TreeIterator<T> push_R(TreeIterator<T> N, TreeIterator<T> Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return TreeIterator<T>(NULL);

		//пустое дерево - добавляем в корень
		if (Tree<T>::root == NULL) { Tree<T>::root = N.ptr; return N; }

		if ((*Current).getData() > (*N).getData())
		{
			//идем влево
			if ((*Current).getLeft() != NULL) return push(N, (*Current).getLeft());

			else { (*Current).setLeft(N.ptr); (*N).setParent(Current.ptr); }
		}
		if ((*Current).getData() < (*N).getData())
		{
			//идем вправо
			if ((*Current).getRight() != NULL) return push(N, (*Current).getRight());

			else { (*Current).setRight(N.ptr); (*N).setParent(Current.ptr); }
		}
		//вернуть добавленный узел
		return N;
	}

	virtual TreeIterator<T> Find_R(T data, TreeIterator<T> Current, bool(*comparison)(T, T))
	{
		//база рекурсии
		if (Current == NULL) return TreeIterator<T>(NULL);

		if (comparison((*Current).getData(), data)) return Current;

		//рекурсивный вызов
		if ((*Current).getData() > data) return Find_R(data, (*Current).getLeft(), comparison);

		return Find_R(data, (*Current).getRight(), comparison);
	}

	int bfactor(TreeIterator<T> p)
	{
		int hl = 0;
		int hr = 0;

		if ((*p).getLeft() != NULL)
			hl = (*p).getLeft()->getHeight();

		if ((*p).getRight() != NULL)
			hr = (*p).getRight()->getHeight();

		return (hr - hl);
	}

	void fixHeight(TreeIterator<T> p)
	{
		int hl = 0;
		int hr = 0;

		if ((*p).getLeft() != NULL)
			hl = (*p).getLeft()->getHeight();

		if ((*p).getRight() != NULL)
			hr = (*p).getRight()->getHeight();

		(*p).setHeight((hl > hr ? hl : hr) + 1);
	}

	TreeIterator<T> RotateRight(TreeIterator<T> p) // правый поворот вокруг p
	{
		TreeIterator<T> q = (*p).getLeft();

		(*p).setLeft((*q).getRight());
		(*q).setRight(p.ptr);

		if (p == Tree<T>::root)
			Tree<T>::root = q.ptr;
		else
		{
			if ((*p).getParent()->getLeft() == p.ptr)
				(*p).getParent()->setLeft(q.ptr);
			else
				(*p).getParent()->setRight(q.ptr);
		}

		(*q).setParent((*p).getParent());
		(*p).setParent(q.ptr);

		if ((*p).getLeft() != NULL) (*p).getLeft()->setParent(p.ptr);

		fixHeight(p);
		fixHeight(q);

		return q;
	}

	TreeIterator<T> RotateLeft(TreeIterator<T> q) // левый поворот вокруг q
	{
		TreeIterator<T> p = (*q).getRight();

		(*q).setRight((*p).getLeft());
		(*p).setLeft(q.ptr);

		if (q == Tree<T>::root)
			Tree<T>::root = p.ptr;
		else
		{
			if ((*q).getParent()->getLeft() == q.ptr)
				(*q).getParent()->setLeft(p.ptr);
			else
				(*q).getParent()->setRight(p.ptr);
		}

		(*p).setParent((*q).getParent());
		(*q).setParent(p.ptr);

		if ((*q).getRight() != NULL) (*q).getRight()->setParent(q.ptr);

		fixHeight(q);
		fixHeight(p);

		return p;
	}

	TreeIterator<T> Balance(TreeIterator<T> p) // балансировка узла p
	{
		fixHeight(p);

		if (bfactor(p) == 2)
		{
			if (bfactor((*p).getRight()) < 0) RotateRight((*p).getRight());
			return RotateLeft(p);
		}

		if (bfactor(p) == -2)
		{
			if (bfactor((*p).getLeft()) > 0) RotateLeft((*p).getLeft());
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	IteratedTree<T>() : AVL_Tree<T>() {}

	TreeIterator<T> begin() { return TreeIterator<T>(Tree<T>::Min()); }
	TreeIterator<T> end() { return TreeIterator<T>(Tree<T>::Max()); }

	virtual TreeIterator<T> push(TreeIterator<T> N, TreeIterator<T> Current)
	{
		//вызываем функцию push из базового класса
		TreeIterator<T> pushed = push_R(N, Current);

		//применяем к добавленному узлу балансировку
		if (Current != NULL) { return Balance(Current); }

		return pushed;
	}

	virtual TreeIterator<T> Push(T n)
	{
		TreeIterator<T> N = new Node<T>;
		(*N).setData(n);
		return push(N, TreeIterator<T>(Tree<T>::root));
	}

	virtual TreeIterator<T> remove(TreeIterator<T> N)
	{
		if (N == NULL) return TreeIterator<T>(NULL); // Если не нашли, то выходим

		TreeIterator<T> parent = (*N).getParent();

		// Если удаляемый узел имеет двух потомков
		if ((*N).getLeft() != NULL && (*N).getRight() != NULL)
		{
			TreeIterator<T> successor = N; successor++; // Находим преемника и ищем следующего

			T cur = (*N).getData();
			(*N).setData((*successor).getData()); // Копируем данные преемника в удаляемый узел
			(*successor).setData(cur);

			N = successor; // Теперь удаляемый узел - преемник
		}

		// Если удаляемый узел имеет только одного потомка или не имеет их вообще
		TreeIterator<T> child = TreeIterator<T>(NULL);

		if ((*N).getLeft() != NULL) { child = (*N).getLeft(); }

		else if ((*N).getRight() != NULL) { child = (*N).getRight(); }

		if (child != NULL) { (*child).setParent((*N).getParent()); } // Устанавливаем родителя для потомка

		if (N == Tree<T>::root)
			Tree<T>::root = child.ptr; // Если удаляем корень, то новый корень - потомок удаляемого узла
		else
		{
			if (N == (*N).getParent()->getLeft())
				(*N).getParent()->setLeft(child.ptr); // Если удаляемый узел - левый потомок, то переставляем указатель на потомка
			else
				(*N).getParent()->setRight(child.ptr); // Если удаляемый узел - правый потомок, то переставляем указатель на потомка
		}

		// Обрываем связи узла с деревом
		(*N).setLeft(NULL);
		(*N).setRight(NULL);
		(*N).setParent(NULL);
		(*N).setHeight(0);

		if (parent != NULL) { Balance(parent); }

		return N;
	}

	virtual TreeIterator<T> remove(T N) { return remove(find(N)); }

	virtual TreeIterator<T> Min(TreeIterator<T> Current = TreeIterator<T>(NULL))
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (Tree<T>::root == NULL) return TreeIterator<T>(NULL);

		if (Current == NULL) { Current = Tree<T>::root; }

		while ((*Current).getLeft() != NULL)
			Current = (*Current).getLeft();

		return Current;
	}

	virtual TreeIterator<T> Max(TreeIterator<T> Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (Tree<T>::root == NULL) return TreeIterator<T>(NULL);

		if (Current == NULL) Current = Tree<T>::root;

		for (; (*Current).getRight() != NULL; Current = (*Current).getRight());

		return Current;
	}

	virtual TreeIterator<T> find(T data, bool(*comprasion)(T, T) = ComparisonKey) { return Find_R(data, Tree<T>::root, comprasion); }

	//три обхода дерева
	virtual void PreOrder(TreeIterator<T> N, void (*f)(Node<T>*))
	{
		if (N != NULL)
			f(N.ptr);

		if (N != NULL && (*N).getLeft() != NULL)
			PreOrder((*N).getLeft(), f);

		if (N != NULL && (*N).getRight() != NULL)
			PreOrder((*N).getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(TreeIterator<T> N, void (*f)(Node<T>*))
	{
		if (N != NULL && (*N).getLeft() != NULL)
			InOrder((*N).getLeft(), f);

		if (N != NULL)
			f(N.ptr);

		if (N != NULL && (*N).getRight() != NULL)
			InOrder((*N).getRight(), f);
	}

	virtual void PostOrder(TreeIterator<T> N, void (*f)(Node<T>*))
	{
		if (N != NULL && (*N).getLeft() != NULL)
			PostOrder((*N).getLeft(), f);

		if (N != NULL && (*N).getRight() != NULL)
			PostOrder((*N).getRight(), f);

		if (N != NULL)
			f(N.ptr);
	}

	template<class T1> friend ostream& operator<< (ostream& stream, IteratedTree<T1>& N);
};

template<class T> ostream& operator<< (ostream& stream, IteratedTree<T>& N)
{
	for (TreeIterator<T> i = N.begin(); i != N.end(); ++i)
		stream << (*i).getData() << '\n';
	if (N.end() != NULL)
		stream << (*N.end()).getData();
	return stream;
}

// -------------------------------------------------------- Multi Iterated Tree ------------------------------------------------------------//

template <class T>
class IteratedMultiKeyTree : public IteratedTree<SortedQueueIterated<T>> // Узел из списка значений. Smile face)))
{
protected:
	bool(*comparisonKey)(T, T);

	virtual TreeIterator<SortedQueueIterated<T>> push_R(TreeIterator<SortedQueueIterated<T>> N, TreeIterator<SortedQueueIterated<T>> Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return TreeIterator<SortedQueueIterated<T>>(NULL);

		//пустое дерево - добавляем в корень
		if (Tree<SortedQueueIterated<T>>::root == NULL) { Tree<SortedQueueIterated<T>>::root = N.ptr; return N; }

		if (comparisonKey((*(*Current).getData().begin()).getValue(), (*(*N).getData().begin()).getValue()))  // много, много, много...
		{
			SortedQueueIterated<T> res = (*Current).getData();
			res.push((*(*N).getData().begin()).getValue());
			(*Current).setData(res);
			return Current;
		}
		if ((*Current).getData() > (*N).getData())
		{
			//идем влево
			if ((*Current).getLeft() != NULL) return push(N, (*Current).getLeft());
			else { (*Current).setLeft(N.ptr); (*N).setParent(Current.ptr); }
		}
		if ((*Current).getData() < (*N).getData())
		{
			//идем вправо
			if ((*Current).getRight() != NULL) return push(N, (*Current).getRight());
			else { (*Current).setRight(N.ptr); (*N).setParent(Current.ptr); }
		}
		//вернуть добавленный узел
		return N;
	}

	virtual TreeIterator<SortedQueueIterated<T>> Find_R(T data, TreeIterator<SortedQueueIterated<T>> Current, bool(*comprasion)(T, T))
	{
		//база рекурсии
		if (Current == NULL) return TreeIterator<SortedQueueIterated<T>>(NULL);

		for (ListIterator<T> it = (*Current).getData().begin(); it != NULL; it++)
			if (comprasion((*it).getValue(), data))
				return Current;

		//рекурсивный вызов
		if ((*(*Current).getData().begin()).getValue() > data) return Find_R(data, (*Current).getLeft(), comprasion);

		return Find_R(data, (*Current).getRight(), comprasion);
	}
public:
	IteratedMultiKeyTree(bool(*comp_key)(T, T) = NULL) : IteratedTree<SortedQueueIterated<T>>()
	{ 
		if (comp_key == NULL) comparisonKey = ComparisonKey;

		comparisonKey = comp_key;
	}

	virtual TreeIterator<SortedQueueIterated<T>> push(TreeIterator<SortedQueueIterated<T>> N, TreeIterator<SortedQueueIterated<T>> Current)
	{
		//вызываем функцию push из базового класса
		TreeIterator<SortedQueueIterated<T>> pushed = push_R(N, Current);

		//применяем к добавленному узлу балансировку
		if (Current != NULL) { return IteratedTree<SortedQueueIterated<T>>::Balance(Current); }

		return pushed;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual TreeIterator<SortedQueueIterated<T>> Push(T n)
	{
		TreeIterator<SortedQueueIterated<T>> N = new Node<SortedQueueIterated<T>>;

		SortedQueueIterated<T> sqi; sqi.push(n); (*N).setData(sqi);

		return push(N, TreeIterator<SortedQueueIterated<T>>(Tree<SortedQueueIterated<T>>::root));
	}

	virtual TreeIterator<SortedQueueIterated<T>> find(T key, bool(*comprasion)(T, T) = NULL)
	{
		if (comprasion == NULL)
			return Find_R(key, TreeIterator<SortedQueueIterated<T>>(Tree<SortedQueueIterated<T>>::root), ComparisonKey);

		return Find_R(key, TreeIterator<SortedQueueIterated<T>>(Tree<SortedQueueIterated<T>>::root), comprasion);
	}

	virtual SortedQueueIterated<T> operator [] (T key) { return (*find(key)).getData(); } // Урааааа, я добрааался

	virtual TreeIterator<SortedQueueIterated<T>> RemoveByKey(T N)
	{
		return IteratedTree<SortedQueueIterated<T>>::remove(find(N));
	}

	virtual T remove(T N)
	{
		SortedQueueIterated<T> queue = (*find(N)).getData();

		if (queue.begin() == queue.end()) { return (*(*RemoveByKey(N)).getData().begin()).getValue(); }

		T removed = queue.remove(N);
		(*find(N)).setData(queue);

		return removed;
	}

	template<class T> friend ostream& operator<< (ostream& stream, IteratedMultiKeyTree<T> tree);
};

template<class T>
ostream& operator<< (ostream& stream, IteratedMultiKeyTree<T> tree)
{
	for (TreeIterator<SortedQueueIterated<T>> it = tree.begin(); it != tree.end(); ++it)
	{
		SortedQueueIterated<T> current = (*it).getData();
		stream << current << '\n';
	}

	if (tree.end() != NULL)
	{
		SortedQueueIterated<T> current = (*tree.end()).getData();
		stream << current;
	}

	return stream;
}

//-------------------------------------------------------- Splay Tree -------------------------------------------------------------------------//

template<class T>
class SplayTree : public IteratedTree<T>
{
protected:
	TreeIterator<T> Splay(TreeIterator<T> node);

public:
	SplayTree<T>() : IteratedTree<T>() { }

	~SplayTree<T>() { }

	TreeIterator<T> begin() { return IteratedTree<T>::begin(); }
	TreeIterator<T> end() { return IteratedTree<T>::end(); }

	TreeIterator<T> Push(T data) { return Splay(Tree<T>::push(data)); }

	TreeIterator<T> find(T data, bool(*comprasion)(T, T) = ComparisonKey) { return Splay(IteratedTree<T>::find(data, comprasion)); }

	Node<T> operator [] (T data) { return *find(data); }
};

template<class T>
TreeIterator<T> SplayTree<T>::Splay(TreeIterator<T> node)
{
	if (node == NULL) { return TreeIterator<T>(NULL); }

	while ((*node).getParent() != NULL)
	{
		TreeIterator<T> parent = (*node).getParent();
		TreeIterator<T> grandparent = (*parent).getParent();

		if (grandparent == NULL)
		{
			if (node == (*parent).getLeft())
			{
				// Zig rotation (одиночный правый поворот)
				IteratedTree<T>::RotateRight(parent);
			}
			else
			{
				// Zag rotation (одиночный левый поворот)
				IteratedTree<T>::RotateLeft(parent);
			}
		}
		else if (node == (*parent).getLeft() && parent == (*grandparent).getLeft())
		{
			// Zig-Zig rotation (двойной правый поворот)
			IteratedTree<T>::RotateRight(grandparent);
			IteratedTree<T>::RotateRight(parent);
		}
		else if (node == (*parent).getRight() && parent == (*grandparent).getRight())
		{
			// Zag-Zag rotation (двойной левый поворот)
			IteratedTree<T>::RotateLeft(grandparent);
			IteratedTree<T>::RotateLeft(parent);
		}
		else if (node == (*parent).getLeft() && parent == (*grandparent).getRight())
		{
			// Zig-Zag rotation (правый поворот в родителе, затем левый поворот в дедушке)
			IteratedTree<T>::RotateRight(parent);
			IteratedTree<T>::RotateLeft(grandparent);
		}
		else
		{
			// Zag-Zig rotation (левый поворот в родителе, затем правый поворот в дедушке)
			IteratedTree<T>::RotateLeft(parent);
			IteratedTree<T>::RotateRight(grandparent);
		}
	}
	return node;
}

int main()
{
	try
	{
		cout << "---------------------------- SortedQueueIterated --------------------------\n";

		SortedQueueIterated<double> test;

		test.push(1.0);
		test.push(2.0);
		test.push(3.0);
		test.push(4.0);
		test.push(5.0);

		cout << test;
		
		cout << "\n\nremoved: " << test.remove(5.0) << "\n";

		cout << test;

		cout << "\n\n---------------------------- IteratedTree --------------------------\n";

		IteratedTree<Customer> it_tree;

		Customer Alex("Lazarev", "Sanya", "Moscow", "Fruktovia", "012313", 8.0, 124);
		Customer Danya("Lykov", "Danya", "Bryansk", "Garsia", "7777", 9.0, 124);
		Customer Danya_2("Lykov", "Danya", "Bryansk", "Garsia", "7777", 3.0, 122);
		Customer Nastya("Pak", "Nastya", "Korea", "Moskovskiy", "343", 3.0, 77);
		Customer Yarik("Malysh", "Yarik", "Balashikha", "Lyambda", "8882", 16.0, 102);
		Customer Vika("Kuslieva", "Vika", "Moscow", "Ryabinovaya", "666", 31.0, 323);

		it_tree.Push(Alex);
		it_tree.Push(Danya);
		it_tree.Push(Nastya);
		it_tree.Push(Yarik);
		it_tree.Push(Vika);

		cout << "\n\nSearch Alex:\n" << *(it_tree.find(Alex));

		cout << it_tree << "\n";

		cout << "\n\nRemoved:\n " << *it_tree.remove(Danya);

		cout << it_tree;

		cout << "\n\n---------------------------- IteratedMultyKeyTree --------------------------\n";

		IteratedMultiKeyTree<Customer> it_multi_tree(CustomerComparisonKey);

		it_multi_tree.Push(Alex);
		it_multi_tree.Push(Danya);
		it_multi_tree.Push(Nastya);
		it_multi_tree.Push(Yarik);
		it_multi_tree.Push(Vika);
		it_multi_tree.Push(Danya_2);

		cout << it_multi_tree;

		cout << "\n\nSearch Vika:\n" << *it_multi_tree.find(Customer("Kuslieva", "Vika"));
		
		cout << "\n\nOperator [] overload with searching by key:\n" << it_multi_tree[Customer("Lykov", "Danya")] << '\n';

		cout << "\n\nRemoving node with key Lazarev Sanya:" << (*it_multi_tree.RemoveByKey(Customer("Lazarev", "Sanya")));

		cout << it_multi_tree;

		cout << "\n\nRemoving Danya_2: " << it_multi_tree.remove(Danya_2);
		
		cout << it_multi_tree;

		cout << "\n\n---------------------------- SplayTree --------------------------\n";

		SplayTree<Customer> splay;

		void (*ptr)(Node<Customer>*);
		
		ptr = print;

		cout << *splay.Push(Alex);
		cout << *splay.Push(Danya);
		cout << *splay.Push(Nastya);
		cout << *splay.Push(Yarik);
		cout << *splay.Push(Vika);

		cout << "\n\nSearch Alex:\n" << *(splay.find(Alex));

		splay.InOrder(splay.getRoot(), ptr);

		cout << "\n";
	}
	catch (TreeIteratorException& e)
	{
		cout << "\nTreeIteratorException has been caught: "; e.print();
	}
	catch (ExceptionForIterator& e)
	{
		cout << "\nExceptionForIterator has been caught: "; e.print();
	}
	catch (TreeException& e)
	{
		cout << "\nTreeException has been caught: "; e.print();
	}
	catch (Exception& e)
	{
		cout << "\nException has been caught: "; e.print();
	}
	return 0;
}