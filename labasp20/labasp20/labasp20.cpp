#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

//вставил код из задания 5.1
struct ScreenSize {
    int width;
    int height;
};
//вывел код из задания 5.2 в отдельную функцию
ScreenSize GetCurrentScreenSize() {
    return { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

HBITMAP CreateScreenBitmap(ScreenSize size) {
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, size.width, size.height);

    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, size.width, size.height, hdcScreen, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    return hBitmap;
}

void CopyScreenToClipboard() {
    ScreenSize screenSize = GetCurrentScreenSize();
    HBITMAP hBitmap = CreateScreenBitmap(screenSize);

    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    DeleteObject(hBitmap);
    cout << "Скриншот скопирован в буфер обмена!" << endl;
}

void SaveScreenshotToFile(const char* filename) {
    ScreenSize screenSize = GetCurrentScreenSize();
    HBITMAP hBitmap = CreateScreenBitmap(screenSize);

    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPFILEHEADER bfHeader = { 0 };
    BITMAPINFOHEADER biHeader = { 0 };

    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = bmp.bmWidth;
    biHeader.biHeight = bmp.bmHeight;
    biHeader.biPlanes = 1;
    biHeader.biBitCount = bmp.bmBitsPixel;
    biHeader.biCompression = BI_RGB;

    DWORD dwBmpSize = ((bmp.bmWidth * bmp.bmBitsPixel + 31) / 32) * 4 * bmp.bmHeight;

    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfHeader.bfSize = bfHeader.bfOffBits + dwBmpSize;
    bfHeader.bfType = 0x4D42;

    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD dwBytesWritten;
        WriteFile(hFile, &bfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
        WriteFile(hFile, &biHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);

        BYTE* pPixels = new BYTE[dwBmpSize];

        HDC hdcMem = CreateCompatibleDC(GetDC(NULL));
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = bmp.bmWidth;
        bmi.bmiHeader.biHeight = -bmp.bmHeight;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = bmp.bmBitsPixel;
        bmi.bmiHeader.biCompression = BI_RGB;

        GetDIBits(hdcMem, hBitmap, 0, bmp.bmHeight, pPixels, &bmi, DIB_RGB_COLORS);
        WriteFile(hFile, pPixels, dwBmpSize, &dwBytesWritten, NULL);

        delete[] pPixels;
        CloseHandle(hFile);
        DeleteDC(hdcMem);

        cout << "Скриншот сохранен в файл: " << filename << endl;
    }

    DeleteObject(hBitmap);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Программа для работы со скриншотами" << endl;
    cout << "====================================" << endl;

    char currentPath[MAX_PATH];
    GetModuleFileNameA(NULL, currentPath, MAX_PATH);

    string projectPath = currentPath;
    size_t lastSlash = projectPath.find_last_of("\\/");
    if (lastSlash != string::npos) {
        projectPath = projectPath.substr(0, lastSlash + 1);
    }

    string screenshotFile = projectPath + "screenshot.bmp";
    SaveScreenshotToFile(screenshotFile.c_str());
    CopyScreenToClipboard();

    cout << "Запуск Paint..." << endl;
    system("start mspaint.exe");

    cout << "\nСкриншот скопирован в буфер обмена!" << endl;
    cout << "В Paint нажмите Ctrl+V для вставки изображения." << endl;
    cout << "Файл скриншота сохранен в папке проекта." << endl;

    cout << "\nНажмите Enter для выхода...";
    cin.get();

    return 0;
}