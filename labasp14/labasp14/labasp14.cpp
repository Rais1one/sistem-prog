#include <stdio.h>
#include <locale.h>

extern "C" void Laba6(char* str);
extern "C" char VAL1[12] = "Hello world";
extern "C" void COPY();
extern "C" int VAL;  // Просто объявление, без инициализации
extern "C" void FUNC(int A1, int A2, int A3);
extern "C" int VAL2; // Просто объявление
extern "C" int VAL3; // Просто объявление
extern "C" int FUNCS(int A1, int A2, int A3);

// Определяем переменные здесь
int VAL = 0;
int VAL2 = 0;
int VAL3 = 0;

int main()
{
    setlocale(LC_ALL, "Russian");

    printf("Laba 6: \n");
    printf("До: %s\n", VAL1);

    Laba6(VAL1);

    printf("После: %s\n", VAL1);

    printf("Пример 1: \n");
    COPY();
    printf("VAL = %d\n", VAL);  

    printf("Пример 2: \n");
    int x = 10;
    FUNC(x, 20, 20 + 5);
    printf("VAL2 = %d\n", VAL2);

    printf("Пример 3: \n");
    int val;
    x = 10;
    val = FUNCS(x, 20, 20 + 5);
    printf("val = %d\n", val);

    return 0;
}