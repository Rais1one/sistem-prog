#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

HANDLE hSemaphore;
HANDLE hMutex;
int activeThreads = 0;

DWORD WINAPI WorkerThread(LPVOID lpParam) {
	int threadId = (int)lpParam;

	// Ожидаем семафор (максимум 3 одновременных потока)
	WaitForSingleObject(hSemaphore, INFINITE);

	// Безопасное обновление счетчика активных потоков
	WaitForSingleObject(hMutex, INFINITE);
	activeThreads++;
	printf("Поток %d запущен. Активных потоков: %d\n", threadId, activeThreads);
	ReleaseMutex(hMutex);

	// Имитация работы
	printf("Поток %d выполняет работу...\n", threadId);
	Sleep(2000);

	// Завершение
	WaitForSingleObject(hMutex, INFINITE);
	activeThreads--;
	printf("Поток %d завершен. Активных потоков: %d\n", threadId, activeThreads);
	ReleaseMutex(hMutex);

	// Освобождаем семафор
	ReleaseSemaphore(hSemaphore, 1, NULL);

	return 0;
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE hThreads[10];
	DWORD threadIds[10];

	// Создаем семафор (максимум 3 потока одновременно)
	hSemaphore = CreateSemaphore(NULL, 3, 3, NULL);
	hMutex = CreateMutex(NULL, FALSE, NULL);

	if (hSemaphore == NULL || hMutex == NULL) {
		printf("Ошибка создания объектов синхронизации!\n");
		return 1;
	}

	printf("Запуск 10 потоков (одновременно не более 3)...\n");
	printf("================================================\n");

	// Создаем 10 потоков
	for (int i = 0; i < 10; i++) {
		hThreads[i] = CreateThread(
			NULL, 0, WorkerThread, (LPVOID)(i + 1), 0, &threadIds[i]
		);

		if (hThreads[i] == NULL) {
			printf("Ошибка создания потока %d!\n", i + 1);
		}
	}

	// Ожидаем завершения всех потоков
	WaitForMultipleObjects(10, hThreads, TRUE, INFINITE);

	printf("\nВсе потоки завершены!\n");

	// Закрываем дескрипторы
	for (int i = 0; i < 10; i++) {
		CloseHandle(hThreads[i]);
	}
	CloseHandle(hSemaphore);
	CloseHandle(hMutex);

	return 0;
}
