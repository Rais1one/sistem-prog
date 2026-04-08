#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

HANDLE hDataReady;	// Событие "данные готовы"
HANDLE hBufferEmpty;  // Событие "буфер пуст"
HANDLE hMutex;    	// Мьютекс для доступа к буферу

int buffer[BUFFER_SIZE];
int head = 0, tail = 0;
int count = 0;

// Производитель
DWORD WINAPI ProducerThread(LPVOID lpParam) {
	int value = 1;

	while (value <= 20) {
		// Ждем, пока буфер не освободится
		WaitForSingleObject(hBufferEmpty, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);

		// Добавляем данные в буфер
		buffer[head] = value;
		head = (head + 1) % BUFFER_SIZE;
		count++;
		printf("Производитель: добавил %d (в буфере: %d)\n", value, count);
		value++;

		ReleaseMutex(hMutex);
		// Сигнализируем, что данные готовы
		SetEvent(hDataReady);

		Sleep(500);
	}

	return 0;
}

// Потребитель
DWORD WINAPI ConsumerThread(LPVOID lpParam) {
	int total = 0;

	while (total < 20) {
		// Ждем, пока появятся данные
		WaitForSingleObject(hDataReady, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);

		// Забираем данные из буфера
		int data = buffer[tail];
		tail = (tail + 1) % BUFFER_SIZE;
		count--;
		printf("Потребитель: получил %d (осталось: %d)\n", data, count);
		total++;

		ReleaseMutex(hMutex);
		// Сигнализируем, что буфер освободился
		SetEvent(hBufferEmpty);

		Sleep(1000);
	}

	return 0;
}

int main() {
	system("chcp 1251");
	HANDLE hProducer, hConsumer;
	DWORD producerId, consumerId;

	// Создаем объекты синхронизации
	hDataReady = CreateEvent(NULL, FALSE, FALSE, NULL);
	hBufferEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);
	hMutex = CreateMutex(NULL, FALSE, NULL);

	if (!hDataReady || !hBufferEmpty || !hMutex) {
		printf("Ошибка создания объектов синхронизации!\n");
		return 1;
	}

	printf("Запуск Producer-Consumer...\n");
	printf("============================\n");

	// Создаем потоки
	hProducer = CreateThread(NULL, 0, ProducerThread, NULL, 0, &producerId);
	hConsumer = CreateThread(NULL, 0, ConsumerThread, NULL, 0, &consumerId);

	if (!hProducer || !hConsumer) {
		printf("Ошибка создания потоков!\n");
		return 1;
	}

	// Ожидаем завершения
	WaitForSingleObject(hProducer, INFINITE);
	WaitForSingleObject(hConsumer, INFINITE);

	printf("\nВсе потоки завершены!\n");

	// Закрываем дескрипторы
	CloseHandle(hProducer);
	CloseHandle(hConsumer);
	CloseHandle(hDataReady);
	CloseHandle(hBufferEmpty);
	CloseHandle(hMutex);

	return 0;
}
