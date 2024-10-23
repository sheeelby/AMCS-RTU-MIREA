#include <iostream>
#include <math.h>

using namespace std;

class Vector
{
private:
    // Координаты вектора: Координата X начала и конца вектора, координата Y начала и конца вектора
    double coord_x;
    double coord_y;
public:
    // Конструктор
    Vector(double x, double y)
    {
        cout << "\nConstructor is working";
        coord_x = x; coord_y = y;
    }

    // Деструктор
    ~Vector() { cout << "\nDestructor is working"; }

    // Геттеры и сеттеры

    double get_coord_x() { return coord_x; }
    double get_coord_y() { return coord_y; }

    void set_сoord_x(double value) { coord_x = value; }
    void set_coord_y(double value) { coord_y = value; }

    // Операции

    Vector operator+()
    {
        return Vector(coord_x, 0);
    }

    Vector operator-()
    {
        return Vector(0, coord_y);
    }

    double operator*(Vector V2)
    {
        double scalar = (coord_x * V2.coord_x) + (coord_y * V2.coord_y);
        double len_V1 = sqrt((coord_x * coord_x) + (coord_y * coord_y));
        double len_V2 = sqrt((V2.coord_x * V2.coord_x) + (V2.coord_y * V2.coord_y));
        double cos_vec = scalar / (len_V1 * len_V2);
        return cos_vec * len_V1;
    }

    friend Vector operator/(double a, Vector V);

    // Вывод

    void print() { cout << "(" << coord_x << ", " << coord_y << ")"; }
};

Vector operator/(double a, Vector V)
{
    return Vector(a / V.coord_x, a / V.coord_y);
}

int main()
{
    Vector V1(4, 2);
    Vector V2(6, 2);
    Vector V3 = -V1;
    Vector V4 = 5.6 / V1;
    cout << "\n"; V1.print();
    cout << "\nПроекция на произвольную ось: " << V1 * V2;
    return 0;
}
