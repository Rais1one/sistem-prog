#include <iostream>
#include <windows.h>
extern "C" {
    int __stdcall _solve_quadratic(float a, float b, float c, float* x1, float* x2);
}
int main()
{
    setlocale(LC_ALL, "Russian");
    float a = 1.0f;
    float b = -5.0f;
    float c = 6.0f;
    float x1 = 0, x2 = 0;

    std::cout << "Уравнение: " << a << "x^2 + (" << b << ")x + (" << c << ") = 0\n";

    int result = _solve_quadratic(a, b, c, &x1, &x2);

    if (result == 2) {
        std::cout << "Корень 1: " << x1 << "\n";
        std::cout << "Корень 2: " << x2 << "\n";
    }
    else if (result == 0) {
        std::cout << "Нет действительных корней (D < 0).\n";
    }
    else if (result == -1) {
        std::cout << "Ошибка: уравнение не квадратное (a = 0).\n";
    }

    return 0;
}
