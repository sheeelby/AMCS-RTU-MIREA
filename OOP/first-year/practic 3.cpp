#include <iostream>

using namespace std;

class A1
{
protected:
	int a1;
public:
	A1() { a1 = 0; cout << "\nA1 default constructor " << a1 << "\n"; }
	A1(int B) { a1 = B; cout << "\nA1 constructor " << a1 << "\n"; }
	~A1() { cout << "\nA1 destructor"; }
	virtual void print() { cout << "\nVariable of A1 class"; }
	virtual void show() { cout << "\na1 = " << a1; }
};

class B1 : virtual public A1
{
protected:
	int b1;
public:
	B1() { b1 = 0; cout << "\nB1 default constructor " << b1 << "\n"; }
	B1(int D, int B) : A1(B)
	{
		b1 = D; cout << "\nB1 constructor " << b1 << "\n";
	}
	virtual void print() { cout << "\nVariable of B1 class"; }
	virtual void show() { cout << "\nb1 = " << b1 << "\ta1 =" << a1; }
};

class B2 : virtual public A1
{
protected:
	int b2;
public:
	B2() { b2 = 0; cout << "\nB2 default constructor " << b2 << "\n"; }
	B2(int D, int B) : A1(B)
	{
		b2 = D; cout << "\nB2 constructor " << b2 << "\n";
	}
	virtual void print() { cout << "\nVariable of B2 class"; }
	virtual void show() { cout << "\nb2 = " << b2 << "\ta1 = " << a1; }
};

class C1 : virtual public B1, virtual public B2
{
protected:
	int c1;
public:
	C1(int D, int B1, int B2) : B1(D, B1), B2(D, B2)
	{
		c1 = D; cout << "\nC1 constructor " << c1 << "\n";
	}
	virtual void print() { cout << "\nVariable of C1 class"; }
	virtual void show() { cout << "\nc1 = " << c1 << "\ta1 = " << a1 << "\tb1 = " << b1 << "\tb2 = " << b2; }
};

class D1 : virtual public C1
{
protected:
	int d1;
public:
	D1(int D, int C, int B1, int B2) : C1(C, B1, B2)
	{
		d1 = D; cout << "\nD1 constructor " << d1 << "\n";
	}
	virtual void print() { cout << "\nVariable of D1 class"; }
	virtual void show() { cout << "\nd1 = " << d1 << "\tc1 = " << c1 << "\tb1 = " << b1 << "\tb2 = " << b2 << "\ta1 = " << a1; }
};

class D2 : virtual public C1
{
protected:
	int d2;
public:
	D2(int D, int C, int B1, int B2) : C1(C, B1, B2)
	{
		d2 = D; cout << "\nD1 constructor " << d2 << "\n";
	}
	virtual void print() { cout << "\nVariable of D2 class"; }
	virtual void show() { cout << "\nd2 = " << d2 << "\tc1 = " << c1 << "\tb1 = " << b1 << "\tb2 = " << b2 << "\ta1 = " << a1; }
};

int main()
{
	//B1 b1(7, 6);
	D1 d1(5, 7, 8, 10);
	//b1.show();
	//b1.print();
	A1* ptr = &d1;
	ptr->show();
	ptr->print();
	char c; std::cin >> c;
	return 0;
}

