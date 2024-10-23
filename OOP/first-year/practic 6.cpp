#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

template<class T>
class Element
{
protected:
	Element* next;
	Element* prev;
	T info;
public:

	Element(T data)
	{
		next = prev = NULL;
		info = data;
	}

	Element(Element* Next, Element* Prev, T data)
	{
		next = Next;
		prev = Prev;
		info = data;
	}

	T getInfo() { return info; }
	void setInfo(T value) { info = value; }
	Element<T>* getNext() { return next; }
	void setNext(Element<T>* value) { next = value; }
	Element<T>* getPrev() { return prev; }
	void setPrev(Element<T>* value) { prev = value; }

	template<class T1>
	friend ostream& operator<<(ostream& s, Element<T1>& el);

};

template<class T1>
ostream& operator<<(ostream& s, Element<T1>& el)
{
	s << el.info;
	return s;
}

class Human
{
public:

	string last_name, first_name, date_of_birth, address, phone_number;
	bool gender;
	double height, weight;

	Human()
	{
		cout << "\nHuman default constructor";
		gender = 0;
		height = 0;
		weight = 0;
	}

	Human(const char* l_n, const char* f_n, char sex, double h, double w,
		const char* d_of_b, const char* num, const char* add)
	{
		cout << "\nHuman constructor";
		last_name = l_n;
		first_name = f_n;
		gender = sex;
		height = h;
		weight = w;
		date_of_birth = d_of_b;
		phone_number = num;
		address = add;
	}

	Human(const Human& H)
	{
		cout << "\nHuman copy constructor";
		last_name = H.last_name;
		first_name = H.first_name;
		gender = H.gender;
		height = H.height;
		weight = H.weight;
		date_of_birth = H.date_of_birth;
		phone_number = H.phone_number;
		address = H.address;
	}

	bool operator==(Human& H)
	{
		return last_name == H.last_name &&
			first_name == H.first_name &&
			gender == H.gender &&
			height == H.height &&
			weight == H.weight &&
			date_of_birth == H.date_of_birth &&
			phone_number == H.phone_number &&
			address == H.address;
	}

	~Human() { cout << "\nHuman destructor"; }

	friend ostream& operator<<(ostream& s, Human& h);
	friend istream& operator>>(istream& s, Human& h);
};

ostream& operator<<(ostream& s, Human& h)
{
	s << "\nLast name: " << h.last_name << "\nFirst name: " << h.first_name << "\nGender: " << h.gender
		<< "\nHeight: " << h.height << "\nWeight: " << h.weight << "\nDate of Birth: " << h.date_of_birth
		<< "\nPhone number: " << h.phone_number << "\nAddress: " << h.address;
	return s;
}

istream& operator>>(istream& s, Human& h)
{
	s >> h.last_name >> h.first_name >> h.gender >> h.height >> h.weight
		>> h.date_of_birth >> h.phone_number >> h.address;
	return s;
}

template<class T>
class LinkedList
{
protected:
	Element<T>* head;
	Element<T>* tail;
	int count;
public:
	//переместить в protected

	LinkedList()
	{
		head = tail = NULL;
		count = 0;
	}

	LinkedList(T* arr, int len)
	{
		for (int i = 0; i < len; i++)
			push(arr[i]);
	}

	virtual Element<T>* pop() = 0;
	virtual Element<T>* push(T value) = 0;

	virtual Element<T>* find(T value) // Поиск итеративный 
	{
		for (Element<T>* current = head; current != NULL; current = current->getNext())
			if (current->getInfo() == value)
				return current;
	}

	virtual Element<T>* find_recursive(T value, Element<T>* current = NULL) // Поиск рекурсивный
	{
		if (current == NULL) return find_recursive(value, head);

		if (current->getInfo() == value) return current;

		if (current->getNext() != NULL) return find_recursive(value, current->getNext());
	}

	virtual void Filter(LinkedList<T>* filtered_list, bool (*filter)(T)) // Фильтр итеративный
	{
		for (Element<T>* current = head; current != NULL; current = current->getNext())
			if (filter(current->getInfo()))
				filtered_list->push(current->getInfo());
	}

	virtual void FilterRecursive(LinkedList<T>* filtered_list, bool (*filter)(T), Element<T>* current = NULL) // Фильтр рекурсивный
	{
		if (current == NULL)
		{
			FilterRecursive(filtered_list, filter, head);
			return;
		}

		if (filter(current->getInfo()))
			filtered_list->push(current->getInfo());

		if (current->getNext() != NULL)
			FilterRecursive(filtered_list, filter, current->getNext());
	}

	Element<T>& operator[](int index)
	{
		Element<T>* current = head;

		for (int i = 0; current != NULL && i < index; current = current->getNext(), i++);

		return *current;
	}

	virtual bool isEmpty() { return (LinkedList<T>::count == 0); }

	template<class T1>
	friend ostream& operator<<(ostream& s, LinkedList<T1>& el);

	virtual ~LinkedList()
	{
		cout << "\nBase class destructor";
		Element<T>* previous;
		for (Element<T>* current = head; current != NULL;)
		{
			previous = current;
			current = current->getNext();
			delete previous;
		}
	}

	virtual void load(ifstream& stream)
	{
		int s;

		stream >> s;

		for (int i = 0; i < s; i++)
		{
			T data;
			stream >> data;
			this->push(data);
		}
	}

	virtual void save(ofstream& stream)
	{
		stream << this->count << ' ';

		for (Element<T>* current = head; current != NULL; current = current->getNext())
		{
			T info = current->getInfo();
			stream << info << ' ';
		}
	}
};

ostream& my_manip(ostream& s)
{
	return s << setfill('0') << setw(5) << fixed << setprecision(2);
}

template<class T1>
ostream& operator<<(ostream& s, LinkedList<T1>& el)
{
	Element<T1>* current;

	// Собственный манипулятор
	
	for (current = el.head; current != NULL; current = current->getNext())
	{
		if (typeid(T1) == typeid(double) || typeid(T1) == typeid(float))
			s << my_manip << *current << "; ";
		else
			s << *current << "; ";
	}
	return s;
}


template<class T>
class SingleLinkedList : public LinkedList<T>
{
public:
	SingleLinkedList<T>() : LinkedList<T>() {}
	virtual ~SingleLinkedList() { cout << "\nSingleLinkedList class destructor"; }
	virtual Element<T>* pop()
	{
		Element<T>* res = LinkedList<T>::tail;

		if (LinkedList<T>::tail == NULL) return NULL;

		if (LinkedList<T>::head == LinkedList<T>::tail)
		{
			LinkedList<T>::count = 0;
			res = LinkedList<T>::tail;
			LinkedList<T>::head = LinkedList<T>::tail = NULL;
			return res;
		}

		Element<T>* current;

		for (current = LinkedList<T>::head; current->getNext() != LinkedList<T>::tail; current = current->getNext());

		current->setNext(NULL);
		LinkedList<T>::count--;
		LinkedList<T>::tail = current;

		return res;
	}

	virtual Element<T>* push(T value)
	{
		Element<T>* newElem = new Element<T>(value);
		newElem->setNext(LinkedList<T>::head);
		LinkedList<T>::head = newElem;
		if (LinkedList<T>::tail == NULL)
		{
			LinkedList<T>::tail = LinkedList<T>::head;
		}
		LinkedList<T>::count++;
		return LinkedList<T>::head;
	}

	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		if (previous == NULL || previous == LinkedList<T>::head) return push(value);

		Element<T>* inserted = new Element<T>(value);
		Element<T>* next = previous->getNext();

		previous->setNext(inserted);
		inserted->setNext(next);

		LinkedList<T>::count++;
		return inserted;
	}

	virtual Element<T>* remove(Element<T>* removed = NULL)
	{
		if (removed == NULL || removed == LinkedList<T>::tail) return pop();

		if (removed == LinkedList<T>::head)
		{
			LinkedList<T>::head = LinkedList<T>::head->getNext();
			LinkedList<T>::count--;
			return removed;
		}

		for (Element<T>* current = LinkedList<T>::head; current != NULL; current = current->getNext())
		{
			if ((current->getNext()) == removed)
			{
				current->setNext(removed->getNext());
				LinkedList<T>::count--;
				return removed;
			}
		}
	}
};

template<class T>
class DoubleLinkedList : public SingleLinkedList<T>
{
public:
	using LinkedList<T>::head;
	using LinkedList<T>::tail;
	using LinkedList<T>::count;

	DoubleLinkedList<T>() : SingleLinkedList<T>() {}
	virtual ~DoubleLinkedList() { cout << "\nDoubleLinkedList class destructor"; }

	virtual Element<T>* push(T value)
	{
		Element<T>* res = SingleLinkedList<T>::push(value);
		if (head != tail)
		{
			Element<T>* prehead = res->getNext();
			prehead->setPrev(head);
		}
		return res;
	}

	virtual Element<T>* pop()
	{
		if (head == tail) return SingleLinkedList<T>::pop();

		Element<T>* res = tail;
		Element<T>* previous = tail->getPrev();
		previous->setNext(NULL);
		tail->setPrev(NULL);
		count--;
		tail = previous;
		return res;
	}

	virtual Element<T>* insert(T value, Element<T>* previous = NULL)
	{
		Element<T>* inserted = SingleLinkedList<T>::insert(value, previous);
		if (head != tail)
		{
			if (inserted->getNext() != NULL)
			{
				inserted->getNext()->setPrev(inserted);
				inserted->setPrev(previous);
			}
		}
		return inserted;
	}

	virtual Element<T>* remove(Element<T>* current = NULL)
	{
		if (current == NULL || current == LinkedList<T>::tail) return pop();

		if (current == LinkedList<T>::head) return SingleLinkedList<T>::remove(current);

		Element<T>* removed = current; // Удаляемый элемент
		Element<T>* previous = current->getPrev();
		Element<T>* next = current->getNext();

		if (next != NULL)
			next->setPrev(previous); // Вместо current будет previous

		if (previous != NULL)
			previous->setNext(next); // Следующим после previous должен быть next

		LinkedList<T>::count--;
		current = NULL;
		return current;
	}

	template<class T>
	friend ostream& operator<<(ostream& s, DoubleLinkedList<T>& el);
};

template<class T>
ostream& operator<<(ostream& s, DoubleLinkedList<T>& el)
{
	Element<T>* current;

	for (current = el.head; current != NULL; current = current->getNext())
		s << *current << "; ";

	return s;
}

class HumanLinkedList : public DoubleLinkedList<Human>
{
public:
	virtual ~HumanLinkedList() { }

	virtual Element<Human>* FindHuman(const char* lastname) // Поиск по фамилии
	{
		for (Element<Human>* current = head; current != NULL; current = current->getNext())
			if (current->getInfo().last_name.compare(lastname) == 0)
				return current;

		return NULL;
	}

	virtual void FilterHuman(HumanLinkedList* filtered_list, const char* _address) // Фильтр по адресу
	{
		for (Element<Human>* current = head; current != NULL; current = current->getNext())
			if (current -> getInfo().address == _address)
				filtered_list->push(current->getInfo());
	}
};

bool filter_over_20(int x)
{
	return x > 20;
}

int main()
{
	SingleLinkedList<int> S; // Создание односвязного массива

	for (int i = 0; i < 7; i++)
		S.push(i); // Заполнение массива с начала

	cout << "\n" << S; // Вывод массива

	S.pop(); // Удаление последнего элемента массива

	cout << "\n" << S; // Вывод массива

	S.insert(8, &S[0]); // Добавление элемента 8 в начало
	S.insert(8, &S[6]); // Добавление элемента 8 в конец
	S.insert(8, &S[3]); // Добавление элемента 8 в середину

	cout << "\n" << S; // Вывод массива

	S.remove(&S[0]); // Удаление элемента 8 из начала
	S.remove(&S[7]); // Удаление элемента 8 из начала
	S.remove(&S[3]); // Удаление элемента 8 из середины

	cout << "\n" << S; // Вывод массива

	//--------------------------------------------------------------------------------------------------------//

	DoubleLinkedList<int> Q; // Создание двусвязного массива

	for (int i = 0; i < 7; i++)
		Q.push(i); // Заполнение массива с начала

	cout << "\n" << Q; // Вывод массива

	Q.pop(); // Удаление последнего элемента массива

	cout << "\n" << Q; // Вывод массива

	Q.insert(8, &Q[0]); // Добавление элемента 8 в начало
	cout << "\n" << Q; // Вывод массива
	Q.insert(8, &Q[6]); // Добавление элемента 8 в конец
	cout << "\n" << Q; // Вывод массива
	Q.insert(8, &Q[3]); // Добавление элемента 8 в середину

	cout << "\n" << Q; // Вывод массива

	Q.remove(&Q[0]); // Удаление элемента 8 из начала
	Q.remove(&Q[7]); // Удаление элемента 8 из начала
	Q.remove(&Q[3]); // Удаление элемента 8 из середины

	cout << "\n" << Q; // Вывод массива

	//-------------------------------------------------------------------------------------------------------//

	cout << "\n" << *Q.find(3); // Проверка работы итеративного поиска

	cout << "\n" << *Q.find_recursive(3); // Проверка работы рекурсивного поиска

	//-------------------------------------------------------------------------------------------------------//

	LinkedList<int>* W = new SingleLinkedList<int>(); // Указатель на базовый класс

	W->push(10); W->push(28); W->push(20); W->push(34); W->push(42); 

	SingleLinkedList<int> filtered_list_iterative, filtered_list_recursive; // Создание односвязных массивов

	W->FilterRecursive(&filtered_list_recursive, filter_over_20); // Рекурсивный фильтр (значения больше 20)

	W->Filter(&filtered_list_iterative, filter_over_20); // Итеративный фильтр (значения больше 20)

	cout << "\n" << filtered_list_recursive; // Вывод отфильтрованного списка через рекурсию

	cout << "\n" << filtered_list_iterative; // Вывод отфильтрованного итеративного списка 

	//-------------------------------------------------------------------------------------------------------//

	LinkedList<int>* list = new SingleLinkedList<int>(); // Указатель на базовый класс

	list->push(7); list->push(10); list->push(13); // Добавление элементов

	// Преобразование указателя на базовый класс в указатель типа производного класса SingleLinkedList

	SingleLinkedList<int>* converted_list = dynamic_cast<SingleLinkedList<int>*>(list);

	cout << "\n" << *converted_list; 

	delete converted_list; 

	//-------------------------------------------------------------------------------------------------------//

	HumanLinkedList humans, filtered_list;

	humans.push(Human("Lykov", "Danya", 'M', 173.0, 63.0, "14.09.2004", "+79103387832", "Bryansk"));
	humans.push(Human("Malysh", "Yarik", 'M', 180.0, 75.0, "05.12.2004", "+79167215733", "Balashikha"));
	humans.push(Human("Kuslieva", "Vika", 'W', 171.0, 57.0, "07.08.2004", "+79266721568", "Moscow"));
	humans.push(Human("Lazarev", "Sasha", 'M', 172.0, 62.0, "14.02.2004", "+79152755824", "Moscow"));
	humans.push(Human("Pak", "Nastya", 'W', 160.0, 46.0, "02.09.2004", "+79779790117", "Korea"));

	cout << "\n_________Friend :)_______________ " << "\n" << humans << "\n";

	humans.FilterHuman(&filtered_list, "Moscow"); // Фильтр по адресу Москва

	cout << "\nFilter Moscow: " << "\n" << filtered_list << "\n"; // Вывод отфильтрованного списка

	cout << "\nFind Lykov: " << "\n" << *humans.FindHuman("Lykov") << "\n"; // Результат поиска по фамилии Лыков (Lykov)

	//-------------------------------------------------------------------------------------------------------//

	ofstream fout("test.txt");

	S.save(fout); // Запись в файл

	ifstream fin("test.txt");

	S.load(fin); // Чтение из файла

	cout << S; // Вывод массива, прочтенного из файла

	//-------------------------------------------------------------------------------------------------------//

	SingleLinkedList<double> S1; // Тест манипулятора

	S1.push(1); S1.push(22.762232); S1.push(102.102232); S1.push(10202.032232); S1.push(2.032232);

	cout << "\n" << S1;
	//-------------------------------------------------------------------------------------------------------//


	return 0;
}
