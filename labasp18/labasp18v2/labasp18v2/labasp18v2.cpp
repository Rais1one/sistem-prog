#include <windows.h>
#include <iostream>

int main() {
    // Объявляем структуры
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Инициализация
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL result;
    char appName[] = "C:\\Windows\\System32\\notepad.exe";

    // Получаем адрес функции CreateProcessA
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC pCreateProcess = GetProcAddress(hKernel32, "CreateProcessA");

    // Используем указатели на структуры
    STARTUPINFO* pSi = &si;
    PROCESS_INFORMATION* pPi = &pi;

    __asm {
        // Сохраняем адрес функции
        mov ebx, pCreateProcess

        // Адрес PROCESS_INFORMATION (через указатель)
        mov eax, pPi
        push eax

        // Адрес STARTUPINFO (через указатель)
        mov eax, pSi
        push eax

        // Остальные параметры
        push 0  // lpCurrentDirectory
        push 0  // lpEnvironment
        push 0  // dwCreationFlags
        push 0  // bInheritHandles
        push 0  // lpThreadAttributes
        push 0  // lpProcessAttributes
        push 0  // lpCommandLine

        // Адрес имени приложения
        lea eax, appName
        push eax

        // Вызов
        call ebx

        // Сохраняем результат
        mov result, eax
    }

    if (!result) {
        DWORD error = GetLastError();
        std::cout << "CreateProcess failed! Error: " << error << std::endl;
        return 1;
    }
    system("chcp 1251");
    std::cout << "Notepad запущен!" << std::endl;
    std::cout << "Process ID: " << pi.dwProcessId << std::endl;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

// 1. Процесс - экземпляр выполняемой программы с собственным адресным пространством
//    Поток - минимальная единица выполнения внутри процесса

// 2. Ресурсы процесса:
//    - Виртуальное адресное пространство
//    - Исполняемый код и данные
//    - Таблица открытых дескрипторов
//    - Переменные окружения
//    
//    Ресурсы потока:
//    - Стек потока
//    - Регистры процессора
//    - Счетчик команд
//    - Контекст выполнения

// 3. Параметры CreateProcess:
//    lpApplicationName, lpCommandLine, lpProcessAttributes,
//    lpThreadAttributes, bInheritHandles, dwCreationFlags,
//    lpEnvironment, lpCurrentDirectory, lpStartupInfo

// 4. Возвращаемые параметры (через PROCESS_INFORMATION):
//    hProcess, hThread, dwProcessId, dwThreadId

// 5. Функции завершения:
//    ExitProcess(), TerminateProcess(), return из main

// 6. Функции ожидания:
//    WaitForSingleObject(), WaitForMultipleObjects()

// 7. GetExitCodeProcess(hProcess, &exitCode)

// 8. Создается процесс Блокнота (notepad.exe)

// 9. Завершилось 2 потока:
//    - Поток 8312: главный поток нашей программы (завершился после main)
//    - Поток 19128: главный поток Блокнота (завершился при закрытии окна)
