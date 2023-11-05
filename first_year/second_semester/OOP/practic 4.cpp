#include <iostream>

using namespace std;

class BaseString
{
protected:
	char* p;
	int len;
	int capacity;
public:
	BaseString(char* ptr)
	{
		cout << "\nBase Constructor 1\n";

		int i = 0;
		while (ptr[i] != '\0')
			i++;

		len = i;
		capacity = (len > 127) ? len + 1 : 128;
		p = new char[capacity];

		for (i = 0; i <= len; i++)
			p[i] = ptr[i];
	}

	BaseString(const char* ptr)
	{
		cout << "\nBase Constructor 1\n";

		int i = 0;
		while (ptr[i] != '\0')
			i++;

		len = i;
		capacity = (len > 127) ? len + 1 : 128;
		p = new char[capacity];

		for (i = 0; i <= len; i++)
			p[i] = ptr[i];
	}

	BaseString(int Capacity = 128)
	{
		cout << "\nBase Constructor 0\n";
		capacity = Capacity;
		p = new char[capacity];
		len = 0;
	}

	~BaseString()
	{
		cout << "\nBase Destructor\n";
		if (p != NULL)
			delete[] p;
		len = 0;
	}

	int Length() { return len; }
	int Capacity() { return capacity; }
	//char* get() {return p;}
	char& operator[](int i) { return p[i]; }


	BaseString& operator=(BaseString& s)
	{
		cout << "\nBase Operator = \n";
		if (capacity < s.capacity)
		{
			delete[] p;
			capacity = s.capacity;
			p = new char[capacity];
		}
		len = s.len;
		for (int i = 0; p[i] != '\0'; i++)
			p[i] = s.p[i];
		return *this;
	}

	BaseString(BaseString& s)
	{
		cout << "\nBase Copy Constructor\n";
		len = s.len;
		capacity = s.capacity;
		p = new char[capacity];
		for (int i = 0; i <= len; i++)
			p[i] = s.p[i];
	}

	virtual void print()
	{
		int i = 0;
		while (p[i] != '\0')
		{
			cout << p[i];
			i++;
		}
	}

	virtual int IndexOf(char c, int start_index = 0)
	{
		if (len == 0 || start_index < 0 || start_index >= len) return -1;

		//for (int i = start_index; i < len; i++)
			//if (p[i] == c) return i;
		for (char* p1 = &p[start_index]; *p1 != '\0'; p1++)
			if (*p1 == c) return p1 - p;
		return -1;
	}

	bool IsPalindrome()
	{
		char* p1 = p;
		char* p2 = &p[len - 1];
		while (*p1 == *p2 && p1++ < p2--);
		return (p1 >= p2);
	}
};

class String : public BaseString
{
	//protected:
		//int newfield;
public:
	String(int Capacity = 128) : BaseString(Capacity) { cout << "\nString constructor int"; }
	String(char* ptr) : BaseString(ptr) { cout << "\nString constructor char*"; } //String s(ptr);
	String(const char* ptr) : BaseString(ptr) { cout << "\nString constructor const char*"; } //String s("test");
	String(String& s) : BaseString(s) { "\nString Copy constructor"; }
	~String() { cout << "\nString destructor"; }
	virtual int IndexOf(char c, int start_index = 0)
	{
		if (start_index == 0) start_index = len - 1;
		if (len == 0 || start_index < 0 || start_index >= len) return -1;
		for (char* p1 = &p[start_index]; p1 >= p; p1--)
			if (*p1 == c) return p1 - p;
		return -1;
	}

	int number_of_fives()
	{
		if (len < 5) { return 0; }

		double count_vowels = 0;
		for (char* p1 = p; *p1 != '\0'; p1++)
			if (*p1 == 'a' || *p1 == 'e' || *p1 == 'i' || *p1 == 'o' || *p1 == 'u')
				count_vowels++;
		double mean_count_vowels = count_vowels / len; // Подсчет среднего значения кол-ва гласных в строке
		
		//cout << "\n" << mean_count_vowels;
		//cout << "\n" << count_vowels;
		//cout << "\n" << len;

		double cnt = 0; // Количество гласных в строке
		int total_cnt = 0;

		for (char* p2 = p; *p2 != '\0'; p2++)
		{
			if (*p2 == 'a' || *p2 == 'e' || *p2 == 'i' || *p2 == 'o' || *p2 == 'u') // Если встретили гласную
				cnt++;
			//cout << "\nIndex: " << p2 - p << " cnt: " << cnt;
			if (((p2 - p) != 0) && ((p2 - p + 1) % 5) == 0) // Если мы просмотрели пятерку
			{
				//cout << "\n----------------";
				double cnt_of_fives = cnt / 5.0; // Подсчет среднего количества гласных в подстроке
				if (cnt_of_fives > mean_count_vowels)
					total_cnt++;
				cnt = 0.0;
			}
		}
		return total_cnt;
	}

};

int main()
{
	if (true)
	{
		String* ptr;
		String s("aaaaaacubcaaqfafafyttbtb\0"); ptr = &s;
		//"aaaaa.acubc.aaqfa.fafyt.tbtb\0"
		//f(s);
		//int index = ptr->IndexOf('t');
		//cout << "\nSearch: " << index << "\n";
		//s.print();
		//cout << "\n" << ptr->IsPalindrome();
		cout << "\nNumber of Fives result: " << ptr->number_of_fives();
	}
	char c; cin >> c;
	return 0;
}

