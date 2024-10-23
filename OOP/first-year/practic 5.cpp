#include <iostream>
#include <fstream>

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

class InvalidOperationException : public Exception
{
protected:
	int rows, cols;
public:
	InvalidOperationException(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	InvalidOperationException(const InvalidOperationException& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "InvalidOperationException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class IndexOutOfBoundsException : public Exception
{
protected:
	int row, column;
public:
	IndexOutOfBoundsException(const char* s, int Row, int Column) : Exception(s) { row = Row; column = Column; }
	IndexOutOfBoundsException(const IndexOutOfBoundsException& e) : Exception(e) { row = e.row; column = e.column; }
	virtual void print()
	{
		cout << "IndexOutOfBoundsException: " << str << "; Row: " << row << ", Column: " << column << "; " << what();
	}
};

class WrongSizeException : public Exception
{
protected:
	int rows, cols;
public:
	WrongSizeException(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	WrongSizeException(const WrongSizeException& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "WrongSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class NonPositiveSizeException : public WrongSizeException
{
public:
	NonPositiveSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	NonPositiveSizeException(const NonPositiveSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "NonPositiveSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class TooLargeSizeException : public WrongSizeException
{
public:
	TooLargeSizeException(const char* s, int Rows, int Columns) : WrongSizeException(s, Rows, Columns) { }
	TooLargeSizeException(const TooLargeSizeException& e) : WrongSizeException(e) { }
	virtual void print()
	{
		cout << "TooLargeSizeException: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

class EmptyMatrix : public Exception // Исключение для собственного класса (пустая матрица)
{
protected:
	int rows, cols;
public:
	EmptyMatrix(const char* s, int Rows, int Columns) : Exception(s) { rows = Rows; cols = Columns; }
	EmptyMatrix(const EmptyMatrix& e) : Exception(e) { rows = e.rows; cols = e.cols; }
	virtual void print()
	{
		cout << "EmptyMatrix: " << str << "; Rows: " << rows << ", Columns: " << cols << "; " << what();
	}
};

template<class T>
class BaseMatrix
{
protected:
	T** ptr;
	int height;
	int width;
public:
	BaseMatrix(int Height = 2, int Width = 2)
	{
		//конструктор
		if (Height <= 0 || Width <= 0)
			throw NonPositiveSizeException("Matrix size can't be negative or zero in constructor int, int", Height, Width);
		if (Height > 50 || Width > 50)
			throw TooLargeSizeException("Matrix size can't be more than 50 in constructor int, int", Height, Width);
		height = Height;
		width = Width;
		ptr = new T * [height];
		for (int i = 0; i < height; i++)
			ptr[i] = new T[width];
	}

	BaseMatrix(const BaseMatrix& M)
	{
		//конструктор копий
		height = M.height;
		width = M.width;
		ptr = new T * [height];

		for (int i = 0; i < height; i++)
		{
			ptr[i] = new T[width];
			for (int j = 0; j < width; j++)
				ptr[i][j] = M.ptr[i][j];
		}
	}

	virtual ~BaseMatrix()
	{
		//деструктор
		if (ptr != NULL)
		{
			for (int i = 0; i < height; i++)
				delete[] ptr[i];
			delete[] ptr;
			ptr = NULL;
		}
		cout << "\nBase Destructor";
	}

	void print()
	{
		//вывод
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << ptr[i][j] << " ";
			cout << "\n";
		}
	}

	T* operator[](int index)
	{
		if (index < 0 || index >= height)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", index, -1);
		return ptr[index];
	}

	T& operator()(int row, int column)
	{
		if (row < 0 || row >= height || column < 0 || column >= width)
			throw IndexOutOfBoundsException("Index out of bounds in operator[]", row, column);
		return ptr[row][column];
	}
};

template<class T>
class Matrix : public BaseMatrix<T>
{
public:
	Matrix<T>(int Height = 2, int Width = 2) : BaseMatrix<T>(Height, Width) { cout << "\nMatrix constructor is working!"; }

	Matrix<T>(double** arr, int Height = 2, int Width = 2) // Конструктор, принимающий массив типа double**
	{
		BaseMatrix<T>::height = Height;
		BaseMatrix<T>::width = Width;
		BaseMatrix<T>::ptr = new T * [BaseMatrix<T>::height];
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			BaseMatrix<T>::ptr[i] = new T[BaseMatrix<T>::width];
			for (int j = 0; j < BaseMatrix<T>::width; j++)
				BaseMatrix<T>::ptr[i][j] = arr[i][j];
		}
	}

	virtual ~Matrix() { cout << "\nDerived Destructor"; }

	Matrix<T>& operator=(const Matrix<T>& M) // Оператор присваивания
	{
		if (BaseMatrix<T>::height != M.height || BaseMatrix<T>::width != M.width)
		{
			for (int i = 0; i < BaseMatrix<T>::height; i++)
				delete[] BaseMatrix<T>::ptr[i];
			delete[] BaseMatrix<T>::ptr;
			BaseMatrix<T>::height = M.height;
			BaseMatrix<T>::width = M.width;
			BaseMatrix<T>::ptr = new T * [BaseMatrix<T>::height];
			for (int i = 0; i < BaseMatrix<T>::height; i++)
				BaseMatrix<T>::ptr[i] = new T[BaseMatrix<T>::width];
		}
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			for (int j = 0; j < BaseMatrix<T>::width; j++)
				BaseMatrix<T>::ptr[i][j] = M.ptr[i][j];
		}
		return *this;
	}

	void snake() // Заполнение массива змейкой
	{
		int counter = 0, j;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			if ((i % 2) == 0)
			{
				for (j = 0; j < BaseMatrix<T>::width; j++)
					BaseMatrix<T>::ptr[i][j] = counter;
			}
			else
			{
				for (j = BaseMatrix<T>::width - 1; j >= 0; j--)
					BaseMatrix<T>::ptr[i][j] = counter;
			}
			counter++;
		}
	}

	Matrix<T> my_func(double border) // Удалить из матрицы все строки и столбцы, среднее 
	{                                // арифметическое которых большие некоторого параметра border.
		bool* h = new bool[BaseMatrix<T>::height];
		bool* w = new bool[BaseMatrix<T>::width];

		int H = 0;
		int W = 0;

		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			double summa_elements_on_row = 0.0;
			for (int j = 0; j < BaseMatrix<T>::width; j++)
				summa_elements_on_row += BaseMatrix<T>::ptr[i][j];
			double mean_elements_on_row = summa_elements_on_row / BaseMatrix<T>::width;
			if (mean_elements_on_row <= border)
			{
				h[i] = true;
				H++;
			}
		}
		
		for (int j = 0; j < BaseMatrix<T>::width; j++)
		{
			double summa_elements_on_column = 0.0;
			for (int i = 0; i < BaseMatrix<T>::height; i++)
				summa_elements_on_column += BaseMatrix<T>::ptr[i][j];
			double mean_elements_on_column = summa_elements_on_column / BaseMatrix<T>::height;
			if (mean_elements_on_column <= border)
			{
				w[j] = true;
				W++;
			}
		}

		if ((H == 0) || (W == 0))
			throw EmptyMatrix("The matrix becomes empty because all rows and columns have been removed in my function", W, H);

		Matrix<T> res(H, W);
		int cur1 = 0;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			int cur2 = 0;
			if (h[i] == true)
			{
				for (int j = 0; j < BaseMatrix<T>::width; j++)
				{
					if (w[j] == true)
					{
						res.ptr[cur1][cur2] = BaseMatrix<T>::ptr[i][j];
						cur2++;
					}
				}
				cur1++;
			}
		}

		return res;
	}

	Matrix<T> func()
	{
		int* h = new int[BaseMatrix<T>::height];

		int H = 0;

		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{	
			for (int j = 0; j < BaseMatrix<T>::width; j++)
			{
				if (BaseMatrix<T>::ptr[i][j] == 0)
				{
					h[i] = 1;
					H--;
				}
			}
			H++;
		}


		Matrix<T> res(H, BaseMatrix<T>::width);

		int cur1 = 0;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			if (h[i] != 1)
			{
				for (int j = 0; j < BaseMatrix<T>::width; j++)
				{
					res.ptr[cur1][j] = BaseMatrix<T>::ptr[i][j];
				}
				cur1++;
			}
		}

		return res;
	}

	T operator*()
	{
		if (BaseMatrix<T>::width != BaseMatrix<T>::height)
			throw InvalidOperationException("Couldn't execute operation for rectangular matrix in operator*()", BaseMatrix<T>::height, BaseMatrix<T>::width);
		T p = 1;
		for (int i = 0; i < BaseMatrix<T>::height; i++)
		{
			p *= this->ptr[i][this->width - 1 - i];
		}
		return p;
	}

	template<class T>
	friend ostream& operator<<(ostream& s, Matrix<T> M);
	template<class T>
	friend istream& operator>>(istream& s, Matrix<T>& M);
};

template <class T>
ostream& operator<<(ostream& s, Matrix<T> M)
{
	if (typeid(s) == typeid(ofstream))
	{
		s << M.height << " " << M.width << " ";
		for (int i = 0; i < M.height; i++)
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
	}
	else
	{
		for (int i = 0; i < M.height; i++)
		{
			for (int j = 0; j < M.width; j++)
				s << M.ptr[i][j] << " ";
			s << "\n";
		}
	}
	return s;
}

template <class T>
istream& operator>>(istream& s, Matrix<T>& M)
{
	if (typeid(s) == typeid(ifstream))
	{
		int w, h;
		s >> h >> w;
		if ((h != M.height || w != M.width) && (!s.eof()))
		{
			for (int i = 0; i < M.height; i++)
				delete[] M.ptr[i];
			delete[] M.ptr;
			M.height = h;
			M.width = w;
			M.ptr = new T * [M.height];
			for (int i = 0; i < M.height; i++)
				M.ptr[i] = new T[M.width];
		}
	}
	for (int i = 0; i < M.height; i++)
		for (int j = 0; j < M.width; j++)
			s >> M.ptr[i][j];
	return s;
}


int main()
{
	try
	{
		double** ptr = new double * [2];

		for (int i = 0; i < 2; i++)
		{
			ptr[i] = new double[3] {10.0, 20.0, 10.0};
		}

		double** arr = new double* [3];

		for (int i = 0; i < 3; i++)
		{
			arr[i] = new double[3] {10.0, 0.0, -10.0};
		}

		Matrix<double> M1(ptr, 2, 3);
		Matrix<double> M2(arr, 3, 3);

		cout << "\n" << M1; // Вывод матрицы M1
		cout << "\n" << M2; // Вывод матрицы M1

		cout << "\n";
		(M1 = M2).print(); // Проверка работы оператора =

		Matrix<double> M3(10, 10);
		M3.snake(); // Проверка работы заполнения массива змейкой
		cout << "\n" << M3;

		Matrix<double> M4(5, 4);

		M4(0, 0) = 1; M4(0, 1) = 2; M4(0, 2) = 3; M4(0, 3) = 4;
		M4(1, 0) = 14; M4(1, 1) = 15; M4(1, 2) = 16; M4(1, 3) = 5;
		M4(2, 0) = 13; M4(2, 1) = 20; M4(2, 2) = 17; M4(2, 3) = 6;
		M4(3, 0) = 12; M4(3, 1) = 19; M4(3, 2) = 18; M4(3, 3) = 7;
		M4(4, 0) = 11; M4(4, 1) = 10; M4(4, 2) = 9; M4(4, 3) = 8;

		cout << "\n" << M4.my_func(13); // Проверка работы функции, удаляющей столбцы и строки матрицы, среднее арифметическое которых
		                                // больше некоторого параметра border
		
		Matrix<double>* massiv = new Matrix<double>[4]; // Массив, содержащий 4 матрицы

		ofstream fout("test.txt");

		if (fout) // Вывод в файл
		{
			massiv[0] = M1; massiv[2] = M3;
			massiv[1] = M2; massiv[3] = M4;
			for (int i = 0; i < 4; i++)
				fout << massiv[i] << "\n";
			fout.close();
		}
		
		ifstream fin("test.txt");

		if (fin) // Вывод из файла
		{

			Matrix<int> M1;

			while (!fin.eof())
			{

				try
				{
					fin >> M1;
					cout << "\n" << M1;
				}

				catch (exception e) { cout << "\nException is caught: " << e.what(); }
			}
			fin.close();
		}

		Matrix<double> M7(3, 3);

		M7(0, 0) = 1; M7(0, 1) = 2; M7(0, 2) = 3;
		M7(1, 0) = 14; M7(1, 1) = 15; M7(1, 2) = 0;
		M7(2, 0) = 13; M7(2, 1) = 20; M7(2, 2) = 17;

		cout << "\n" << M7;

		cout << "\n" << M7.func();
		//cout << "\n" << M7.func();

		Matrix<double> M6(5, 5);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
				M6[i][j] = 100;
		}
		cout << "\n" << M6.my_func(13); // Тест собственного исключения EmptyMatrix
	}
	catch (IndexOutOfBoundsException e)
	{
		cout << "\nIndexOutOfBoundsException has been caught: "; e.print();
	}
	catch (InvalidOperationException e)
	{
		cout << "\nInvalidOperationException has been caught: "; e.print();
	}
	catch (EmptyMatrix e) // Собственное исключение
	{
		cout << "\nEmptyMatrix has been caught: "; e.print();
	}
	catch (WrongSizeException e)
	{
		cout << "\nWrongSizeException has been caught: "; e.print();
	}
	catch (Exception e)
	{
		cout << "\nException has been caught: "; e.print();
	}
	catch (exception e)
	{
		cout << "\nexception has been caught: "; e.what();
	}

	return 0;
}
