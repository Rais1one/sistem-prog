#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


double asm_x_ln_x(double x) {
    double result;

    __asm {
        fld x; ST(0) = x
        fld st(0); ST(0) = x, ST(1) = x(дублируем x)
        fyl2x; ST(0) = x * log2(x), ST(1) = x
        fldln2; ST(0) = ln(2), ST(1) = x, ST(2) = x
        fmulp st(1), st(0); ST(0) = x * log2(x) * ln(2) = x * ln(x), ST(1) = x
        fstp result
    }

    return result;
}

double asm_x_log2_x(double x)
{
    double result;
    __asm {
        fld x; ST(0) = x
        fld st(0); ST(0) = x, ST(1) = x
        fyl2x; ST(0) = x * log2(x)
        fstp result; сохраняем результат
    }

    return result;
}

double asm_x_log10_x(double x)
{
    double result;
    __asm {
        fld x; ST(0) = x
        fld st(0); ST(0) = x, ST(1) = x
        fyl2x; ST(0) = x * log2(x)
        fldl2t; ST(0) = log2(10), ST(1) = x * log2(x)
        fdivp st(1), st(0); ST(0) = x * log2(x) / log2(10) = x * log10(x)
        fstp result
    }

    return result;
}

double asm_x2(double x)
{
    double result;
    __asm {
        fld x; ST(0) = x
        fld st(0); ST(0) = x, ST(1) = x
        fmulp st(1), st(0); ST(0) = x * x = x²
        fstp result
    }

    return result;
}

double asm_x3(double x)
{
    double result;
    __asm {
        fld x; ST(0) = x
        fld st(0); ST(0) = x, ST(1) = x
        fmul st(0), st(0); ST(0) = x * x = x², ST(1) = x
        fmulp st(1), st(0); ST(0) = x² * x = x³
        fstp result
    }

    return result;
}

double asm_sin_x_cos_x(double x)
{
    double result;
    __asm {
        fld x; ST(0) = x
        fsin; ST(0) = sin(x)
        fld x; ST(0) = x, ST(1) = sin(x)
        fcos; ST(0) = cos(x), ST(1) = sin(x)
        faddp st(1), st(0); ST(0) = sin(x) + cos(x)
        fstp result
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");

    double x;


    

    do {
        cout << "\nВведите x > 0 (0 - выход): ";
        cin >> x;

        if (x == 0) break;

        if (x < 0) {
            cout << "Ошибка: x должно быть положительным!" << endl;
            continue;
        }
        double result;
        result = asm_x_ln_x(x);

        cout << "Программа вычисления x * ln(x)" << endl;
        cout << "===============================" << endl;

        double cpp_result = x * log(x);

        cout << fixed << setprecision(15);
        cout << "\nРезультат (ассемблер): " << result << endl;
        cout << "Результат (C++ log):   " << cpp_result << endl;
        cout << "Разница: " << abs(result - cpp_result) << endl << endl;

        result = asm_x_log2_x(x);


        cpp_result = x * log2(x);
        cout << "Программа вычисления x * log(x)" << endl;
        cout << "===============================" << endl;

        cout << fixed << setprecision(15);
        cout << "\nРезультат (ассемблер): " << result << endl;
        cout << "Результат (C++ log2):   " << cpp_result << endl;
        cout << "Разница: " << abs(result - cpp_result) << endl << endl;

        result = asm_x_log10_x(x);


        cpp_result = x * log10(x);
        cout << "Программа вычисления x * log10(x)" << endl;
        cout << "===============================" << endl;
        cout << fixed << setprecision(15);
        cout << "\nРезультат (ассемблер): " << result << endl;
        cout << "Результат (C++ log10):   " << cpp_result << endl;
        cout << "Разница: " << abs(result - cpp_result) << endl << endl;

        result = asm_x2(x);


        cpp_result = x * x;
        cout << "Программа вычисления x^2" << endl;
        cout << "===============================" << endl;
        cout << fixed << setprecision(15);
        cout << "\nРезультат (ассемблер): " << result << endl;
        cout << "Результат (C++ x^2):   " << cpp_result << endl;
        cout << "Разница: " << abs(result - cpp_result) << endl << endl;

        result = asm_x3(x);


        cpp_result = x * x * x;
        cout << "Программа вычисления x^3" << endl;
        cout << "===============================" << endl;
        cout << fixed << setprecision(15);
        cout << "\nРезультат (ассемблер): " << result << endl;
        cout << "Результат (C++ x^3):   " << cpp_result << endl;
        cout << "Разница: " << abs(result - cpp_result) << endl << endl;

        result = asm_sin_x_cos_x(x);


        cpp_result = sin(x) + cos(x);
        cout << "Программа вычисления sin(x) + cos(x))" << endl;
        cout << "===============================" << endl;
        cout << fixed << setprecision(15);
        cout << "\nРезультат (ассемблер): " << result << endl;
        cout << "Результат (C++ sin(x) + cos(x)):   " << cpp_result << endl;
        cout << "Разница: " << abs(result - cpp_result) << endl<<endl;

    } while (true);

    cout << "\nПрограмма завершена." << endl;

    return 0;
}
