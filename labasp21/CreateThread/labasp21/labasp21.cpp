#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
 
CRITICAL_SECTION cs;  // Критическая секция для синхронизации
int sharedCounter = 0;
int completedThreads = 0;
 
typedef struct {
	int threadId;
	int iterations;
} ThreadData;
 
DWORD WINAPI WorkerThread(LPVOID lpParam) {
	ThreadData* data = (ThreadData*)lpParam;
	int localSum = 0;
	
    for (int i = 0; i < data->iterations; i++) {
    	localSum += i;
    	Sleep(1);
	}
	
    // Критическая секция - безопасное обновление общего счетчика
	EnterCriticalSection(&cs);
	sharedCounter += localSum;
	completedThreads++;
	printf("Поток %d завершен. Локальная сумма: %d\n", data->threadId, localSum);
	LeaveCriticalSection(&cs);
	
    return 0;
}
 
int main() {
 
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
 
	HANDLE hThreads[5];
	ThreadData threadData[5];
	DWORD threadIds[5];
	
    // Инициализация критической секции
	InitializeCriticalSection(&cs);
	
    printf("Создание 5 потоков с синхронизацией...\n");
	printf("========================================\n");
	
    // Создаем потоки
	for (int i = 0; i < 5; i++) {
    	threadData[i].threadId = i + 1;
    	threadData[i].iterations = 1000 + i * 500;
    	
        hThreads[i] = CreateThread(
        	NULL, 0, WorkerThread, &threadData[i], 0, &threadIds[i]
    	);
    	
        if (hThreads[i] == NULL) {
        	printf("Ошибка создания потока %d!\n", i);
        	DeleteCriticalSection(&cs);
        	return 1;
    	}
    	
        printf("Поток %d создан (итераций: %d)\n",
               i + 1, threadData[i].iterations);
	}
	
    // Ожидаем завершения
	WaitForMultipleObjects(5, hThreads, TRUE, INFINITE);
	
    printf("\nВсе потоки завершены!\n");
	printf("Общий счетчик: %d\n", sharedCounter);
	printf("Завершилось потоков: %d\n", completedThreads);
	
    // Закрываем дескрипторы
	for (int i = 0; i < 5; i++) {
    	CloseHandle(hThreads[i]);
	}
	
    // Удаляем критическую секцию
	DeleteCriticalSection(&cs);
	
    return 0;
}
