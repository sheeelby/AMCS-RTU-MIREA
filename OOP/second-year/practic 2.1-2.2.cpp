//красно-черное (сблансированное) дерево map, есть интерфейс доступа к значению по ключу

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class MapExeption : public exception
{
protected:
	//сообщение об ошибке
	char* str;
public:
	MapExeption(const char* s)
	{
		str = new char[strlen(s) + 1];
		strcpy_s(str, strlen(s) + 1, s);
	}
	MapExeption(const MapExeption& e)
	{
		str = new char[strlen(e.str) + 1];
		strcpy_s(str, strlen(e.str) + 1, e.str);
	}
	~MapExeption()
	{
		delete[] str;
	}

	//функцию вывода можно будет переопределить в производных классах, когда будет ясна конкретика
	virtual void print()
	{
		cout << "MapExeption: " << str << "; " << what();
	}
};

class MapExeptionKey : public MapExeption
{
public:
	MapExeptionKey(const char* s) : MapExeption(s) {}

	MapExeptionKey(const MapExeptionKey& e) : MapExeption(e) {}

	virtual void print() { cout << "MapExeptionKey: " << str << "; " << what(); }
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

	bool operator>(const Customer& other) const
	{
		if (average_check_amount > other.average_check_amount)
			return true;
		if (average_check_amount == other.average_check_amount && last_name > other.last_name)
			return true;
		if (average_check_amount == other.average_check_amount && last_name == other.last_name && first_name > other.first_name)
			return true;

		return false;
	}

	bool operator<(const Customer& other) const
	{
		if (average_check_amount < other.average_check_amount)
			return true;
		if (average_check_amount == other.average_check_amount && last_name < other.last_name)
			return true;
		if (average_check_amount == other.average_check_amount && last_name == other.last_name && first_name < other.first_name)
			return true;

		return false;
	}

	bool operator==(const Customer& other) const
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
		"\nAccount number: " << C.account_number << "\nAverage Check Amount: " << C.average_check_amount;
}

istream& operator>> (istream& stream, Customer& C)
{
	return stream >> C.last_name >> C.first_name >> C.city >> C.street >> C.house_number >> C.account_number >> C.average_check_amount;
}

template <class K, class V> // Поиск элемента по ключу.
V findvalue(map<K, V> tree, K key)
{
	if (tree.find(key) != tree.end()) return tree.find(key)->second;

	return V();
}

template <class K, class V>
set<K> findkeys(map<K, V> tree, V value) // Поиск элемента по значению.
{
	set<K> keys;

	for (typename map<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		if (it->second == value) keys.insert(it->first);

	return keys;
}

template <class K, class V>
void print(map<K, V> tree) // Функция вывода содержимого дерева с помощью итераторов.
{
	for (typename map<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		cout << "\nKey: " << it->first << " Value: " << it->second << "\n";
}

// Функция добавление нового элемента. Исключение для добавления элемента с ключом, уже присутствующим в дереве.
template<class K, class V>
void push(map<K, V>& tree, K key, V value)
{
	if (tree.find(key) != tree.end()) throw MapExeptionKey("An element with such a key already exists.");

	tree[key] = value;
}

template <class V, class T> // Предикат для фильтрации.
bool predicate(V value, T threshold)
{
	return value > threshold;
}

template <class T> // Предикат для фильтрации своего класса.
bool predicateforcustomer(Customer client, T threshold)
{
	return client.average_check_amount > threshold;
}

template <class K, class V, class T> // Функция фильтрации.
map<K, V> filter(map<K, V> tree, bool (*predicate)(V, T), T threshold)
{
	map<K, V> filtered;

	for (typename map<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		if (predicate(it->second, threshold))
			push(filtered, it->first, it->second);

	return filtered;
}

// Функция, которая возвращает вектор из различных значений, которые встречаются в объекте класса map,
// заполненном при решении задачи (рекомендуется использовать класс set).
template <class K, class V>
set<V> unique(map<K, V> tree)
{
	set<V> result;

	for (typename map<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		result.insert(it->second);

	return result;
}

//Поиск элемента по ключу. Введите функцию, возвращающую все элементы дерева с одинаковыми ключами
// (ключ передаётся в функцию как параметр).
template <class K, class V>
vector<V> multifindvalue(multimap<K, V> tree, K key)
{
	vector<V> result;

	pair<typename multimap<K, V>::iterator, typename multimap<K, V>::iterator> found = tree.equal_range(key);

	for (typename multimap<K, V>::iterator it = found.first; it != found.second; it++)
		result.push_back(it->second);

	return result;
}

template <class K, class V>
vector<K> multifindkeys(multimap<K, V> tree, V value) // Поиск элемента по значению.
{
	vector<K> keys;

	for (typename multimap<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		if (it->second == value) keys.push_back(it->first);

	return keys;
}

template <class K, class V>
void multiprint(multimap<K, V> tree) // Функция вывода содержимого дерева с помощью итераторов.
{
	for (typename multimap<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		cout << "\nKey: " << it->first << " Value: " << it->second << "\n";
}

// Функция добавление нового элемента. Исключение для добавления элемента с ключом, уже присутствующим в дереве.
template<class K, class V>
void multipush(multimap<K, V>& tree, K key, V value)
{
	tree.insert(std::pair<K, V>(key, value));
}

template <class K, class V, class T> // Функция фильтрации.
multimap<K, V> multifilter(multimap<K, V> tree, bool (*predicate)(V, T), T threshold)
{
	multimap<K, V> filtered;

	for (typename multimap<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		if (predicate(it->second, threshold))
			multipush(filtered, it->first, it->second);

	return filtered;
}

// Функция, которая возвращает вектор из различных значений, которые встречаются в объекте класса map,
// заполненном при решении задачи (рекомендуется использовать класс set).
template <class K, class V>
set<V> multiunique(multimap<K, V> tree)
{
	set<V> result;

	for (typename multimap<K, V>::iterator it = tree.begin(); it != tree.end(); it++)
		result.insert(it->second);

	return result;
}

int main()
{
	try
	{
		map<string, int> marks;

		marks["Petrov"] = 5;
		marks["Ivanov"] = 4;
		marks["Sidorov"] = 5;
		marks["Nikolaev"] = 3;
		marks["Abramov"] = 4;
		cout << "\nMap:\n";

		//итератор пробегает по map
		map<string, int>::iterator it_m = marks.begin();

		while (it_m != marks.end())
		{
			//перемещение по списку с помощью итератора, нет операции [i]
			cout << "Key: " << it_m->first << ", value: " << it_m->second << "\n";
			it_m++;
		}

		//-------------------------------------------------------------------------------------------------------------------

		map<string, Customer> cust;

		Customer Alex("Lazarev", "Sanya", "Moscow", "Fruktovia", 8.0, "012313", 124);
		Customer Danya("Lykov", "Danya", "Bryansk", "Garsia", 9.0, "7777", 124);
		Customer Nastya("Pak", "Nastya", "Korea", "Moskovskiy", 3.0, "343", 77);
		Customer Yarik("Malysh", "Yarik", "Balashikha", "Lyambda", 16.0, "8882", 102);
		Customer Vika("Kuslieva", "Vika", "Moscow", "Ryabinovaya", 31.0, "666", 323);

		cust["Lazarev Sanya"] = Alex;
		cust["Lykov Danya"] = Danya;
		cust["Pak Nastya"] = Nastya;
		cust["Malysh Yarik"] = Yarik;
		cust["Kuslieva Vika"] = Vika;

		print(cust);

		Customer value = findvalue(cust, string("Lazarev Sanya"));

		cout << "\nFind value: " << value << "\n";

		set<string> keys = findkeys(cust, Customer(Vika));

		cout << "\nFind keys: ";

		for (set<string>::iterator it = keys.begin(); it != keys.end(); it++)
			cout << *it << "\n";

		map<string, Customer> res = filter(cust, predicateforcustomer, 101);

		cout << "\nFiltered map:" << "\n"; print(res);

		set<Customer> unique_map = unique(cust);

		cout << "\nUnique_map: " << "\n";

		for (set<Customer>::iterator it = unique_map.begin(); it != unique_map.end(); it++)
			cout << *it << "\n";

		//--------------------------------------------------------------------------------------------------------------------

		multimap<string, Customer> multicust;

		multipush(multicust, string("Lazarev Sanya"), Alex);
		multipush(multicust, string("Lazarev Sanya"), Vika);
		multipush(multicust, string("Malysh Yarik"), Yarik);
		multipush(multicust, string("Lykov Danya"), Danya);
		multipush(multicust, string("Pak Nastya"), Nastya);

		multiprint(multicust);

		vector<Customer> multivalues = multifindvalue(multicust, string("Lazarev Sanya"));

		cout << "\nFind values: \n"; // Возвращает все элементы дерева с одинаковыми ключами

		for (vector<Customer>::iterator it = multivalues.begin(); it != multivalues.end(); it++)
			cout << *it << "\n";

		vector<string> multikeys = multifindkeys(multicust, Customer(Vika));

		cout << "\nFind keys: ";

		for (vector<string>::iterator it = multikeys.begin(); it != multikeys.end(); it++)
			cout << *it << "\n";

		multimap<string, Customer> multires = multifilter(multicust, predicateforcustomer, 101);

		cout << "\nFiltered multimap:" << "\n"; multiprint(multires);

		set<Customer> unique_multimap = multiunique(multicust);

		cout << "\nUnique_multimap: " << "\n";

		for (set<Customer>::iterator it = unique_multimap.begin(); it != unique_multimap.end(); it++)
			cout << *it << "\n";
	}
	catch (MapExeptionKey& e)
	{
		cout << "\nMapExeptionKey has been caught: "; e.print();
	}
	return 0;
}
