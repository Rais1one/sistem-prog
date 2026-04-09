#include <new>
#include <iostream> 
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::cout << "Сколько имен хотите ввести: ";
    int n;
    std::cin >> n;
    std::string* p = new (std::nothrow) std::string[n];

    if (p == nullptr) {
        std::cerr << "Ошибка выделения памяти!" << std::endl;
        return 1;
    }

    std::cout << "Введите имена:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cin >> p[i];
    }

    std::cout << "Все ваши имена:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << p[i] << " ";
    }
    std::cout << std::endl;

    delete[] p;
    return 0;
}