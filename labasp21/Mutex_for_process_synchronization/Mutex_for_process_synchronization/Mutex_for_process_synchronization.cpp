#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hMutex;
int counter = 0;

DWORD WINAPI ThreadFunc(LPVOID lpParam) {
    int threadNum = (int)lpParam;

    for (int i = 0; i < 10; i++) {
        // Ожидаем мьютекс
        WaitForSingleObject(hMutex, INFINITE);

        // Критическая секция
        counter++;
        printf("Поток %d: counter = %d\n", threadNum, counter);
        fflush(stdout);

        // Освобождаем мьютекс
        ReleaseMutex(hMutex);

        Sleep(100);
    }

    return 0;
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HANDLE hThreads[3];
    DWORD threadIds[3];

    // Создаем мьютекс
    hMutex = CreateMutex(NULL, FALSE, NULL);

    if (hMutex == NULL) {
        printf("Ошибка создания мьютекса!\n");
        return 1;
    }

    printf("Создание 3 потоков с мьютексом...\n");
    printf("==================================\n");

    // Создаем потоки
    for (int i = 0; i < 3; i++) {
        hThreads[i] = CreateThread(
            NULL, 0, ThreadFunc, (LPVOID)(i + 1), 0, &threadIds[i]
        );

        if (hThreads[i] == NULL) {
            printf("Ошибка создания потока %d!\n", i);
            CloseHandle(hMutex);
            return 1;
        }

        printf("Поток %d создан\n", i + 1);
    }

    // Ожидаем завершения
    WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);

    printf("\nВсе потоки завершены!\n");
    printf("Финальное значение счетчика: %d\n", counter);

    // Закрываем дескрипторы
    for (int i = 0; i < 3; i++) {
        CloseHandle(hThreads[i]);
    }
    CloseHandle(hMutex);

    return 0;
}
