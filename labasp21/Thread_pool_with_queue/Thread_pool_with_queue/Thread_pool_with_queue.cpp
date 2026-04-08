#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Task {
	int id;
	struct Task* next;
} Task;

Task* taskQueue = NULL;
HANDLE hMutex;
HANDLE hSemaphore;
int running = 1;

// Добавление задачи в очередь
void AddTask(int id) {
	Task* newTask = (Task*)malloc(sizeof(Task));
	newTask->id = id;
	newTask->next = NULL;

	WaitForSingleObject(hMutex, INFINITE);

	if (taskQueue == NULL) {
		taskQueue = newTask;
	}
	else {
		Task* temp = taskQueue;
		while (temp->next) temp = temp->next;
		temp->next = newTask;
	}

	ReleaseMutex(hMutex);
	ReleaseSemaphore(hSemaphore, 1, NULL);

	printf("Задача %d добавлена в очередь\n", id);
}

// Извлечение задачи из очереди
Task* GetTask() {
	Task* task = NULL;

	WaitForSingleObject(hMutex, INFINITE);

	if (taskQueue != NULL) {
		task = taskQueue;
		taskQueue = taskQueue->next;
	}

	ReleaseMutex(hMutex);
	return task;
}

// Рабочий поток
DWORD WINAPI WorkerThread(LPVOID lpParam) {
	int threadId = (int)lpParam;

	while (running) {
		// Ожидаем задачу
		WaitForSingleObject(hSemaphore, 1000);

		Task* task = GetTask();
		if (task != NULL) {
			printf("Поток %d выполняет задачу %d\n", threadId, task->id);
			Sleep(500); // Имитация работы
			printf("Поток %d завершил задачу %d\n", threadId, task->id);
			free(task);
		}
	}

	printf("Поток %d завершен\n", threadId);
	return 0;
}

int main() {
	system("chcp 1251");
	HANDLE hThreads[3];
	DWORD threadIds[3];

	// Создаем объекты синхронизации
	hMutex = CreateMutex(NULL, FALSE, NULL);
	hSemaphore = CreateSemaphore(NULL, 0, 10, NULL);

	printf("Создание пула из 3 потоков...\n");
	printf("==============================\n");

	// Создаем рабочие потоки
	for (int i = 0; i < 3; i++) {
		hThreads[i] = CreateThread(
			NULL, 0, WorkerThread, (LPVOID)(i + 1), 0, &threadIds[i]
		);
		printf("Поток %d создан\n", i + 1);
	}

	// Добавляем задачи
	printf("\nДобавление задач...\n");
	for (int i = 1; i <= 10; i++) {
		AddTask(i);
		Sleep(200);
	}

	Sleep(3000); // Даем время на выполнени
	// Завершаем работу
	running = 0;
	// Ждем завершения потоков
	WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);

	printf("\nВсе потоки завершены!\n");
	// Закрываем дескрипторы
	for (int i = 0; i < 3; i++) {
		CloseHandle(hThreads[i]);
	}
	CloseHandle(hMutex);
	CloseHandle(hSemaphore);

	return 0;
}
