#include <windows.h>
#include <iostream>
#include <tchar.h>

int main() {
    // Устанавливаем кодировку для вывода русских букв
    system("chcp 1251");

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Обнуляем структуры
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::cout << "Запускаю Блокнот..." << std::endl;

    // Создаем процесс (запускаем Блокнот)
    if (!CreateProcess(
        L"C:\\Windows\\System32\\notepad.exe",   // Путь к исполняемому файлу
        NULL,                                     // Командная строка
        NULL,                                     // Атрибуты безопасности процесса
        NULL,                                     // Атрибуты безопасности потока
        FALSE,                                    // Наследование дескрипторов
        0,                                        // Флаги создания
        NULL,                                     // Переменные окружения
        NULL,                                     // Текущая директория
        &si,                                      // Информация о запуске
        &pi))                                     // Информация о процессе
    {
        std::cout << "CreateProcess failed (" << GetLastError() << ")" << std::endl;
        return 1;
    }

    std::cout << "Процесс успешно создан!" << std::endl;
    std::cout << "ID процесса Блокнота: " << pi.dwProcessId << std::endl;
    std::cout << "ID потока Блокнота: " << pi.dwThreadId << std::endl;

    std::cout << "\nОжидаю закрытия Блокнота..." << std::endl;
    std::cout << "(Закройте окно Блокнота для продолжения)" << std::endl;

    // Ожидаем завершения процесса Блокнота
    WaitForSingleObject(pi.hProcess, INFINITE);

    std::cout << "\nБлокнот закрыт!" << std::endl;
    std::cout << "Завершаю программу..." << std::endl;

    // Закрываем дескрипторы
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;

}
