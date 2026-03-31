#include <iostream>
#include <cstring>
#include <windows.h>

int main() {
    const int MAX_LENGTH = 100;
    char str1[MAX_LENGTH];
    char str2[MAX_LENGTH];
    system("chcp 1251");
    system("cls");
    std::cout << "Введите первую строку: ";
    std::cin.getline(str1, MAX_LENGTH);
    std::cout << "Введите вторую строку: ";
    std::cin.getline(str2, MAX_LENGTH);


    int result;

    __asm {
        lea esi, str1
        lea edi, str2

        xor ecx, ecx

        compare_loop :

        mov al, [esi]
            mov bl, [edi]

            cmp al, bl
            jne not_equal


            test al, al
            jz equal


            inc esi
            inc edi
            jmp compare_loop

            not_equal :
        mov result, 1
            jmp end_compare

            equal :
        mov result, 0

            end_compare :
    }


    if (result == 0) {
        std::cout << "Строки равны." << std::endl;
    }
    else {
        std::cout << "Строки не равны." << std::endl;
    }

    return 0;
}
