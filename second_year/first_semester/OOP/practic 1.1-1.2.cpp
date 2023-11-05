#include <list>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

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
		"\nAccount number: " << C.account_number << "\nAverage Check Amount: " << C.average_check_amount;
}

istream& operator>> (istream& stream, Customer& C)
{
	return stream >> C.last_name >> C.first_name >> C.city >> C.street >> C.house_number >> C.account_number >> C.average_check_amount;
}

template<class T>
void push(list<T>& lst, T element)
{
	typename list<T>::iterator it = lst.begin();
	while (it != lst.end())
	{
		if (typeid(T) == typeid(Customer))
		{
			if (*it < element)
				break;
		}
		else
		{
			if (*it > element)
				break;
		}
		it++;
	}lst.insert(it, element);
}

template<class T>
void pop(list<T>& lst, T element)
{
	for (typename list<T>::iterator it = lst.begin(); it != lst.end(); it++)
	{
		if (*it == element)
		{
			it = lst.erase(it);
			break;
		}
	}
}

template<class T>
T pop(list<T>& lst)
{
	typename list<T>::iterator it_begin = lst.begin();
	typename list<T>::iterator it_end = lst.end();
	T begin = *it_begin; T end = *--it_end;
	if (*it_begin >= *it_end) { it_begin = lst.erase(it_begin); return begin; }
	it_end = lst.erase(it_end); return end;
}

template<class T>
bool predict(T it, double a)
{
	return abs(it) < a ? true : false;
}

template<class T>
list<T> filter(list<T>& lst, T a)
{
	list<T> res;

	for (typename list<T>::iterator it = lst.begin(); it != lst.end(); it++)
		if (predict(*it, a))
			push(res, *it);

	return res;
}

template<class T>
void print(list<T> lst)
{
	typename list<T>::iterator it = lst.begin();

	while (it != lst.end())
	{
		cout << *it << ' ';

		it++;
	}
}

int main()
{
	list<int> lst;

	push(lst, 12); push(lst, -12);
	push(lst, 14); push(lst, -1);
	push(lst, 3); push(lst, 44);
	push(lst, 11);

	pop(lst, 11);

	print(lst);

	cout << "\n";

	list<int> res = filter(lst, 13);

	print(res);

	list<Customer> cust;

	Customer Alex("Lazarev", "Sanya", "Moscow", "Fruktovia", 8.0, "012313", 124);
	Customer Danya("Lykov", "Danya", "Bryansk", "Garsia", 9.0, "7777", 124);
	Customer Nastya("Pak", "Nastya", "Korea", "Haina", 3.0, "343", 77);
	Customer Yarik("Malysh", "Yarik", "Balsikha", "Gricht", 16.0, "8882", 102);
	Customer Vika("Kuslieva", "Vika", "Moscow", "Mozaisk", 31.0, "666", 323);

	push(cust, Alex); push(cust, Yarik);
	push(cust, Danya); push(cust, Vika);
	push(cust, Nastya);

	print(cust);

	cout << "\nPopped: " << pop(cust) << "\n" << pop(cust);

	print(cust);

	return 0;
};
