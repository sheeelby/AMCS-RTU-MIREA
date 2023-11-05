#include <iostream>

using namespace std;

class MyArrayParent
{
protected:
    //������� ������ ��������?
    int capacity;
    //���������� ��������� - ������� ������ ����������
    int count;
    //������
    double* ptr;
public:
    //������������ � ����������
    MyArrayParent(int Dimension = 100)
    {
        cout << "\nMyArray constructor";
        ptr = new double[Dimension];
        capacity = Dimension;
        count = 0;
    }
    //����������� ��������� ������������ ������
    MyArrayParent(double* arr, int len)
    {
        cout << "\nMyArray constructor";
        count = len;
        capacity = len;
        ptr = new double[capacity];
        //��������� ������ ptr, ��������� ����
        for (int i = 0; i < count; i++)
            ptr[i] = arr[i];
    }
    //����������
    ~MyArrayParent()
    {
        cout << "\nMyArray destructor";
        if (ptr != NULL)
        {
            delete[] ptr;
            ptr = NULL;
        }
    }
    //��������� � �����
    int Capacity() { return capacity; }
    int Size() { return count; }
    double GetComponent(int index)
    {
        if (index >= 0 && index < count)
            return ptr[index];
        //������������� ����������, ���� ������ ������������
        return -1;
    }
    void SetComponent(int index, double value)
    {
        if (index >= 0 && index < count)
            ptr[index] = value;
        //������������� ����������, ���� ������ ������������
        else { return; }
    }
    //���������� � ����� ������ ��������
    void push(double value)
    {
        if (count < capacity)
        {
            ptr[count] = value;
            count++;
        }
        //��� ������, ���� ������ ��������?
        else
        {
            if (count == capacity)
            {
                double* arr = new double[count+1];
                for (int i = 0; i < count; i++)
                    arr[i] = ptr[i];
                arr[count] = value;
                count++;
                capacity++;
                delete[] ptr;
                ptr = arr;
            }
        }
    }
    //�������� �������� � �����
    void RemoveLastValue()
    {
        if (count >= 0)
            count--;
        //��� ������, ���� ����?
        if (count < 0)
            return;
    }
    double& operator[](int index)
    {
        //���������� ��������� []
        int i = index;
        return ptr[i];
    }
    MyArrayParent& operator=(const MyArrayParent& V)
    {
        cout << "\noperator = ";
        //�������� =
        //arr1 = arr2 = arr3; ��� arr_i - ������� ������ ������
        if (capacity < V.capacity)
        {
            delete[] ptr;
            capacity = V.capacity;
            ptr = new double[capacity];
        }
        count = V.count;
        for (int i; i < count; i++)
            ptr[i] = V.ptr[i];

        return *this;
    }
    MyArrayParent(const MyArrayParent& V)
    {
        cout << "\nCopy constructor";
        capacity = V.capacity;
        count = V.count;
        ptr = new double[capacity];
        for (int i = 0; i < count; i++)
            ptr[i] = V.ptr[i];
    }
    //����� ��������
    int IndexOf(double value, bool bFindFromStart = true)
    {
        if (bFindFromStart = true)
        {
            for (int i = 0; i < count; i++)
            {
                if (ptr[i] == value)
                    return i;
            }
        }
        else
        {
            for (int i = count - 1; i >= 0; i--)
            {
                if (ptr[i] == value)
                    return i;
            }
        }
        return -1;
    }

    void print()
    {
        cout << "\nMyArr, size: " << count << ", values: {";
        int i = 0;
        for (i = 0; i < count; i++)
        {
            cout << ptr[i];
            if (i != count - 1)
                cout << ", ";
        }
        cout << "}";
    }
};

class MyArrayChild : public MyArrayParent
{
public:
    //���������� ����������� ��������. ����� �� ���-�� ���?
    MyArrayChild(int Dimension = 100) : MyArrayParent(Dimension)
    {
        cout << "\nMyArrayChild constructor";
    }

    MyArrayChild(const MyArrayChild& ptr) : MyArrayParent(ptr) { };
    MyArrayChild(double* arr, int len) : MyArrayParent(arr, len) { };

    ~MyArrayChild() { cout << "\nMyArrayChild destructor\n"; }
    //�������� ��������
    void RemoveAt(int index)
    {
        if (count == 0)
            return;
        if (index == -1)
        {
            RemoveLastValue();
            return;
        }

        for (int i = index; i < count - 1; i++)
            ptr[i] = ptr[i + 1];

        count--;
    }

    //������� ��������
    void InsertAt(double value, int index = -1)
    {
        if (index == -1 || index == count)
        {
            push(value);
            return;
        }
        if (index<0 || index>count) return;

        for (int i = count; i > index; i--)
            ptr[i] = ptr[i - 1];

        ptr[index] = value;
        count++;
    }
    MyArrayChild IndexOfElements()
    {
        MyArrayChild res(count);
        for (int i = 0; i < count - 1; i++)
        {
            MyArrayChild cur(count);
            cur.push(i);
            int k = 1;
            for (int j = i + 1; j < count; j++)
            {
                if (ptr[i] == ptr[j])
                {
                    cur.push(j);
                    k += 1;
                }
            }
            if (k > 2)
            {
                for (int j = 0; j < k; j++)
                {
                    if (res.IndexOf(cur[j]) == -1)
                    {
                        res.push(cur[j]);
                    }
                    else { break; }
                }
            }
        }
        return res;
    }

    //��������� ���������������������
    MyArrayChild SubSequence(int StartIndex = 0, int Length = -1)
    {
        MyArrayChild res(Length);
        int EndIndex = StartIndex + Length;
        for (int i = StartIndex; i < EndIndex; i++)
            if (i < count)
                res.push(ptr[i]);

        return res;
    }

    //���������� �������� � �����
    MyArrayChild operator+(int value)
    {   
        MyArrayChild res(count);
        for (int i = 0; i < count; i++)
            res.push(ptr[i]);
        res.push(value);
        return res;
    }
};

class MySortedArray : public MyArrayChild
{
protected:
    int BinSearch(double value)
    {
        int low = 0, high = count - 1;
 
        while (low <= high)
        {
            int mid = (low + high) / 2;
            if (value == ptr[mid]) { return(mid); }
            else
            {
                if (value < ptr[mid]) { high = mid - 1; }
                else { low = mid + 1; }
            }
        }
    return -1;
    }

    int BinSearch_insert(double value)
    {
        int low = -1;
        int high = count;
        while (low + 1 < high)
        {
            int middle = (high + low) / 2;
            if (ptr[middle] >= value)
            { 
                high = middle; 
            }
            else 
            { 
                low = middle; 
            }
            if (ptr[middle] == value)
            {
                return middle;
            }
        }
        return high;
    }
public:
    //���������� ����������� ��������. ����� �� ���-�� ���?
    MySortedArray(int Dimension = 100) : MyArrayChild(Dimension) { cout << "\nMySortedArray constructor"; }
    ~MySortedArray() { cout << "\nMySortedArray destructor\n"; }
    int IndexOf(double value, bool bFindFromStart = true)
    {
        return BinSearch(value);
    }

    void push(double value)
    {
        if (count == 0)
        {
            MyArrayParent::push(value);
            return;
        }
        int index = BinSearch_insert(value);
        InsertAt(value, index);
    }

    MySortedArray IndexOfElements()
    {
        MySortedArray res(count);
        int i = 0;
        while (i < count)
        {
            int k = 0;
            double el = ptr[i];
            while (i < count && ptr[i] == el)
            {
                k += 1;
                i += 1;
            }
            if (k > 2)
            {
                for (int j = i - k; j < i; j++) { res.push(j); }
            }
        }
        return res;
    }
};

int main()
{
    if (true)
    {   
        
        double* a = new double[10] {1, 2, 41, 23, 23, 14, 22, 28, 23, 45};
        double* c = new double[10] {56, 13, 21, 5, 7, 10, 28, 32, 46, 100};
        //MyArrayParent arr_parent_1(c, 10) ;
        //MyArrayParent arr_parent(a, 10); // �����������, ����������� ������
        //MyArrayParent arr_copy(arr_parent); // ����������� �����
        double* b = new double[10] {1, 2, 2, 2, 3, 3, 3, 3, 3, 6};
        MyArrayChild arr_child(b, 10);
        //MySortedArray arr_sort(b, 10);
        MySortedArray p3;
        for (int i = 0; i < 10; i++)
        {
            p3.push(i + 1);
            p3.push(i + 1);
            p3.push(i + 1);
        }
        //p3.print();
        //cout << "\nIndexOf(41) = " << arr_parent.IndexOf(41, false); // ������ IndexOf
        // MyArrayChild
        //arr_child.RemoveAt(5); // �������� �������� � �������� 5
        //arr_child.InsertAt(100, 3); // ���������� �������� 100 �� 3 �����
        //arr_child.print();
        (arr_child.IndexOfElements()).print(); // ������� ������ � ������ ������� � ��������� ���������, ������������� ������ 2-� ��� � MyArrayChild
        // MySortedArray
        //p3.push(2); // ������� ���������� � ��������������� ������
        //cout << p3.IndexOf(6); // ������� ������ � ���������������� �������
        //p3.print();
        //p3.IndexOfElements().print(); // ������� ������ � ������ ������� � ��������� ���������, ������������� ������ 2-� ��� � MySortedArray
        arr_child.SubSequence(2, 4).print(); // ��������� ���������������������
        //(arr_child + 10).print(); // �������� +
        //arr_child.print();
    }
    char c; cin >> c;
    return 0;
}
