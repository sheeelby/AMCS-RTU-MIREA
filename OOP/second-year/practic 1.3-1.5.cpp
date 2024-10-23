#pragma warning(disable : 4996) //Предупреждение про итераторы

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class Exception : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	Exception(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy(str, s);
	}
	Exception(const Exception& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy(str, e.str);
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
		cout << "\nParent constructor";
		head = NULL; tail = NULL;
		num = 0;
	}

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
	virtual Element<T>* push(T value) = 0;

	//чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
	virtual T pop() = 0;

	virtual ~LinkedListParent()
	{
		//деструктор - освобождение памяти
		cout << "\nParent destructor";

		Element<T>* cur = head;
		
		while (cur != NULL)
		{
			Element<T>* next = cur->getNext();
			delete cur;
			cur = next;
		}
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
	IteratedLinkedList() : LinkedListParent<T>() { cout << "\nIteratedLinkedList constructor"; }
	virtual ~IteratedLinkedList() { cout << "\nIteratedLinkedList destructor"; }

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
	Queue() : IteratedLinkedList<T>() { cout << "\nQueue constructor"; }

	~Queue() { cout << "\nQueue destructor"; }

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
	SortedQueue() : Queue<T>() { cout << "\nSortedQueue constructor"; }

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

class Customer
{
public:
	string last_name, first_name, city, street, account_number;
	double house_number, average_check_amount;

	Customer()
	{
		last_name = "NotStated"; first_name = "NotStated"; city = "NotStated";
		street = "NotStated"; house_number = 0.0; account_number = "NotStated";
		average_check_amount = 0.0;
	}

	Customer(string l_n, string f_n, string c, string s,
		double h_n, string acc_num, double a_c_a)
	{
		last_name = l_n; first_name = f_n; city = c;
		street = s; house_number = h_n; account_number = acc_num;
		average_check_amount = a_c_a;
	}

	Customer(const Customer& C)
	{
		cout << "\nCustomer copy constructor";
		last_name = C.last_name;
		first_name = C.first_name;
		city = C.city;
		street = C.street;
		house_number = C.house_number;
		account_number = C.account_number;
		average_check_amount = C.average_check_amount;
	}

	bool operator>(Customer other)
	{
		if (average_check_amount > other.average_check_amount)
			return true;
		else if (average_check_amount < other.average_check_amount)
			return false;

		if (account_number > other.account_number)
			return true;
		else if (account_number < other.account_number)
			return false;

		if (last_name > other.last_name)
			return true;
		else if (last_name < other.last_name)
			return false;

		if (first_name > other.first_name)
			return true;
		else if (first_name < other.first_name)
			return false;

		return false;
	}

	bool operator>=(Customer other)
	{
		if (average_check_amount >= other.average_check_amount)
			return true;
		else if (average_check_amount < other.average_check_amount)
			return false;

		if (account_number >= other.account_number)
			return true;
		else if (account_number < other.account_number)
			return false;

		if (last_name >= other.last_name)
			return true;
		else if (last_name < other.last_name)
			return false;

		if (first_name >= other.first_name)
			return true;
		else if (first_name < other.first_name)
			return false;

		return false;
	}

	bool operator<(Customer other)
	{
		if (average_check_amount < other.average_check_amount)
			return true;
		else if (average_check_amount > other.average_check_amount)
			return false;

		if (account_number < other.account_number)
			return true;
		else if (account_number > other.account_number)
			return false;

		if (last_name < other.last_name)
			return true;
		else if (last_name > other.last_name)
			return false;

		if (first_name < other.first_name)
			return true;
		else if (first_name > other.first_name)
			return false;

		return false;
	}

	bool operator<=(Customer other)
	{
		if (average_check_amount <= other.average_check_amount)
			return true;
		else if (average_check_amount > other.average_check_amount)
			return false;

		if (account_number <= other.account_number)
			return true;
		else if (account_number > other.account_number)
			return false;

		if (last_name <= other.last_name)
			return true;
		else if (last_name > other.last_name)
			return false;

		if (first_name <= other.first_name)
			return true;
		else if (first_name > other.first_name)
			return false;

		return false;
	}

	bool operator==(Customer other)
	{
		return average_check_amount == other.average_check_amount && account_number == other.account_number && last_name == other.last_name && first_name == other.first_name;
	}

	~Customer() { cout << "Destructor is working!"; }

	friend ostream& operator<<(ostream& s, const Customer& cus);
	friend istream& operator>>(istream& s, const Customer& cus);
};

ostream& operator<< (ostream& stream, const Customer& C)
{
	return stream << "\nLast Name: " << C.last_name << "\nFirst Name: " << C.first_name <<
		"\nCity: " << C.city << "\nStreet: " << C.street << "\nHouse Number: " << C.house_number <<
		"\nAccount number: " << C.account_number << "\nAverage Check Amount: " << C.average_check_amount << "\n";
}

istream& operator>> (istream& stream, Customer& C)
{
	return stream >> C.last_name >> C.first_name >> C.city >> C.street >> C.house_number >> C.account_number >> C.average_check_amount;
}

template <class T>
class SortedQueueIterator : public IteratedLinkedList<T>
{
public:
	SortedQueueIterator() : IteratedLinkedList<T>() { cout << "\nSortedQueueCustomer constructor"; }

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

		if (it == NULL) { return T(); }

		T x = (*it).getValue();

		(this->head->setNext(NULL));

		if (it != NULL) { (*it).setPrevious(NULL); }

		this->head = it.ptr;
		this->num--;
		return x;
	}
};

bool predict(double value)
{
	return abs(value) < 10.0 ? true : false;
}

int main()
{
	try
	{
		Queue<double> Q;

		Q.push(1.0); Q.push(7.0);
		Q.push(5.0); Q.push(12.0);
		Q.push(10.0); Q.push(7.4);

		cout << "\n" << Q << "\n";

		double e1 = Q.pop();

		cout << "\nPoped Element: " << e1 << "\n";

		SortedQueue<double>* filtered = new SortedQueue<double>;

		Q.Filter(filtered, predict);

		cout << "\nUniversalFilter" << *filtered << "\n";

		Queue<double> filteredQueue = Q.filter(predict);

		cout << "\nFilteredQueue" << filteredQueue << "\n";

		SortedQueue<double> Q_sort;

		Q_sort.push(1.0); Q_sort.push(7.0);
		Q_sort.push(5.0); Q_sort.push(12.0);
		Q_sort.push(10.0); Q_sort.push(7.4);

		cout << "\n" << Q_sort << "\n";

		SortedQueueIterator<Customer> Q_sort_customer;

		Customer Alex("Lazarev", "Sanya", "Moscow", "Fruktovia", 8.0, "012313", 124);
		Customer Danya("Lykov", "Danya", "Bryansk", "Garsia", 9.0, "7777", 124);
		Customer Nastya("Pak", "Nastya", "Korea", "Moskovskiy", 3.0, "343", 77);
		Customer Yarik("Malysh", "Yarik", "Balashikha", "Lyambda", 16.0, "8882", 102);
		Customer Vika("Kuslieva", "Vika", "Moscow", "Ryabinovaya", 31.0, "666", 323);

		Q_sort_customer.push(Alex); Q_sort_customer.push(Yarik);
		Q_sort_customer.push(Danya); Q_sort_customer.push(Vika);
		Q_sort_customer.push(Nastya);

		cout << "\n" << Q_sort_customer << "\n";

		Customer e2 = Q_sort_customer.pop();

		cout << "\nPoped Element: " << e2 << "\n";
	}
	catch (ExceptionForIterator& e)
	{
		cout << "\nExceptionForIterator has been caught: "; e.print();
	}
	return 0;
}
