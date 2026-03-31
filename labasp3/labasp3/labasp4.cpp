#include <iostream>
using namespace std;

// Функция, выполняющая сложение через ассемблерную вставку
int static asm_add(int a, int b) 
{
    __asm 
    {
        mov eax, a; загружаем первое число в EAX
        add eax, b; прибавляем второе число
        ; результат в EAX, он будет возвращён
    }
}

int static asm_sub(int a, int b) 
{
    __asm 
    {
        mov eax, a
        sub eax, b

    }
}

int static asm_dec(int a) 
{
    __asm 
    {
        mov eax, a
        dec eax

    }
}

int static asm_inc(int a) 
{
    __asm 
    {
        mov eax, a
        inc eax
    }
}

int static asm_neg(int a) 
{
    __asm 
    {
        mov eax, a
        neg eax
    }
}

unsigned long long static asm_mul(unsigned int a, unsigned int b) {
    unsigned long long res;
    __asm {
        mov eax, a
        mul b; EDX:EAX = EAX * b
        mov dword ptr[res], eax
        mov dword ptr[res + 4], edx

    }
    return res;
}

unsigned int static asm_imul(unsigned int a, unsigned int b) {
    unsigned int res;
    __asm {
        mov eax, a
        imul b          
        mov res, eax
    }
    return res;
}

unsigned int static asm_div(unsigned int a, unsigned int b) {
    if (b == 0) {
        cout << "Ошибка: деление на ноль!" << endl;
        return 0;
    }
    unsigned int qdv;
    __asm {
        mov eax, a
        xor edx, edx; обнуляем EDX для беззнакового деления
        div b; EAX = EDX:EAX / b, остаток в EDX
        mov qdv, eax
    }
    return qdv;
}


unsigned int static asm_idiv(unsigned int a, unsigned int b) {
    if (b == 0) {
        cout << "Ошибка: деление на ноль!" << endl;
        return 0;
    }
    unsigned int qdv;
    __asm {
        mov eax, a
        cdq; расширяем знак EAX в EDX для знакового деления
        idiv b; EAX = EDX:EAX / b, остаток в EDX
        mov qdv, eax
    }
    return qdv;
}





