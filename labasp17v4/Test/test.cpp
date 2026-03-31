#include <iostream>
#include <windows.h>

extern "C" {
    int __stdcall _random_range(int min, int max);
}

int main()
{
    setlocale(LC_ALL, "Russian");

    int min = 1;
    int max = 100;

    std::cout << "Генерация случайных чисел:\n";

    for (int i = 0; i < 10; i++) {
        int r = _random_range(min, max);
        std::cout << r << std::endl;
    }

    return 0;
}