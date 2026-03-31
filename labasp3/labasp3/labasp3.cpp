#include <iostream>
#include "labasp4.cpp"
#include <cstdlib>
using namespace std;
extern "C" int MAS_FUNC(int*, int);
int main() {
    string a;
    int* mas, n, k;
    system("chcp 1251");
    system("cls");

    cout << "Лабораторная работа №3";
    cout << "\n5 вариант: Четвёртый элемент массива был умножен на 4";
    cout << "\nВведите размер массива: ";
    cin >> n;

    mas = new int[n];
    cout << "Введите элементы массива: " << endl;
    for (int i = 0; i < n; i++) {
        cout << "mas[" << i << "]= ";
        cin >> mas[i];  
    }

    k = MAS_FUNC(mas, n);
    cout << mas[3] << "*4= " << k << endl;  

    delete[] mas;  // Освобождение памяти
    cout << "Нажмите Enter для продолжения...";
    cin.get();
    cin.get();
    system("cls");
    cout << "Лабораторная работа №4";
    int x, y, result;

    cout << "\nВведите два целых числа: ";
    cin >> x >> y;
    int ix = x;
    unsigned int ux = x, uy = y;
    unsigned long long uresult;

    result = asm_add(x, y);
    cout << "Результат сложения (ассемблер): " << result << endl;
    cout << "Результат сложения (C++): " << x + y << endl<<endl;

    result = asm_sub(x, y);
    cout << "Результат вычитания (ассемблер): " << result << endl;
    cout << "Результат вычитания (C++): " << x - y << endl << endl;

    result = asm_dec(x);
    cout << "Результат декремент (ассемблер): " << result << endl;
    cout << "Результат декремент (C++): " << --ix << endl << endl;
    ix = x;
    result = asm_inc(x);
    cout << "Результат инкремент (ассемблер): " << result << endl;
    cout << "Результат инкремент (C++): " << ++ix << endl << endl;

    result = asm_neg(x);
    cout << "Результат изменения знака (ассемблер): " << result << endl;
    cout << "Результат изменения знака (C++): " << -x << endl << endl;

    uresult = asm_mul(ux, uy);
    cout << "Результат умножения (ассемблер): " << uresult << endl;
    cout << "Результат умножения (C++): " << ux * uy << endl << endl;

    uresult = asm_imul(ux, uy);
    cout << "Результат умножения со знаком (ассемблер): " << uresult << endl;
    cout << "Результат умножение со знаком (C++): " << ux * uy << endl << endl;

    if (uy != 0) 
    {
        uresult = asm_div(ux, uy);
        cout << "Результат деления (ассемблер): " << uresult << endl;
        cout << "Результат деления (C++): " << ux / uy << endl << endl;

        uresult = asm_idiv(ux, uy);
        cout << "Результат деления со знаком (ассемблер): " << uresult << endl;
        cout << "Результат деления со знаком (C++): " << ux / uy << endl << endl;
    }
    cin.get();
    cin.get();
    return 0;
}
extern "C" int MAS_FUNC(int* mas, int n) {
    __asm 
    {
        mov esi, mas          //Загружаем адрес массива в регистр
        mov eax, [esi + 12]   // Извлекаем четвертый элемент массива (mas[3]) и помещаем его в еах
        shl eax, 2           // Умножаем на 4, выполняя сдвиг влево
    }
}
