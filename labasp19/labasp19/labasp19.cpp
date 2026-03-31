#include <windows.h>
#include <iostream>
using namespace std;

void CopyScreenToClipboard() {
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    SelectObject(hdcMem, hBitmap);

    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    DeleteObject(hBitmap);

    cout << "Скриншот скопирован в буфер обмена!" << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("chcp 1251");

    cout << "Программа для работы со скриншотами" << endl;
    cout << "===================================" << endl;

    CopyScreenToClipboard();

    // Параметры CreateProcessA через ASM
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    BOOL result;
    char cmdLine[] = "cmd.exe /c start mspaint.exe"; // безопасно для x86/x64

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC pCreateProcess = GetProcAddress(hKernel32, "CreateProcessA");

    STARTUPINFO* pSi = &si;
    PROCESS_INFORMATION* pPi = &pi;

    __asm {
        mov ebx, pCreateProcess
        lea eax, cmdLine

        push pPi; lpProcessInformation
        push pSi; lpStartupInfo
        push 0; lpCurrentDirectory
        push 0; lpEnvironment
        push 0; dwCreationFlags
        push 0; bInheritHandles
        push 0; lpThreadAttributes
        push 0; lpProcessAttributes
        push eax; lpCommandLine
        push 0; lpApplicationName = NULL для cmd.exe

        call ebx
        mov result, eax
    }

    if (!result) {
        DWORD error = GetLastError();
        cout << "CreateProcess failed! Error: " << error << endl;
        return 1;
    }

    cout << "Paint запущен!" << endl;
    cout << "Process ID: " << pi.dwProcessId << endl;

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}