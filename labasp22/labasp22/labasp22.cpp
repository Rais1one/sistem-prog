#include <windows.h>
#include <stdio.h>

#define SERVICE_NAME L"SimpleService"
#define SERVICE_DISPLAY_NAME L"Simple Service"

SERVICE_STATUS g_ServiceStatus = { 0 };
SERVICE_STATUS_HANDLE g_ServiceStatusHandle = NULL;
HANDLE g_ServiceStopEvent = NULL;

// Прототипы
void WINAPI ServiceMain(DWORD argc, LPWSTR* argv);
void WINAPI ServiceControl(DWORD dwCtrl);
void ServiceReportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);
void InstallService(void);
void RemoveService(void);
void StartService(void);
void StopService(void);
void ShowMenu(void);

// Рабочий поток - просто ждет остановки
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam) {
    WaitForSingleObject(g_ServiceStopEvent, INFINITE);
    return 0;
}

// Регистрация обработчика
void WINAPI ServiceMain(DWORD argc, LPWSTR* argv) {
    g_ServiceStatusHandle = RegisterServiceCtrlHandlerW(SERVICE_NAME, ServiceControl);
    if (g_ServiceStatusHandle == NULL) return;

    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwWaitHint = 3000;
    ServiceReportStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL) {
        ServiceReportStatus(SERVICE_STOPPED, GetLastError(), 0);
        return;
    }

    HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);
    if (hThread == NULL) {
        ServiceReportStatus(SERVICE_STOPPED, GetLastError(), 0);
        return;
    }

    ServiceReportStatus(SERVICE_RUNNING, NO_ERROR, 0);

    WaitForSingleObject(g_ServiceStopEvent, INFINITE);

    ServiceReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
    WaitForSingleObject(hThread, 5000);
    CloseHandle(hThread);
    CloseHandle(g_ServiceStopEvent);

    ServiceReportStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

// Обработчик команд
void WINAPI ServiceControl(DWORD dwCtrl) {
    if (dwCtrl == SERVICE_CONTROL_STOP) {
        ServiceReportStatus(SERVICE_STOP_PENDING, NO_ERROR, 3000);
        SetEvent(g_ServiceStopEvent);
    }
}

// Отправка статуса
void ServiceReportStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint) {
    g_ServiceStatus.dwCurrentState = dwCurrentState;
    g_ServiceStatus.dwWin32ExitCode = dwWin32ExitCode;
    g_ServiceStatus.dwWaitHint = dwWaitHint;
    SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
}

// Установка службы
void InstallService(void) {
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager) {
        printf("Ошибка: запустите с правами администратора!\n");
        return;
    }

    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);

    SC_HANDLE hService = CreateServiceW(
        hSCManager,
        SERVICE_NAME,
        SERVICE_DISPLAY_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        szPath,
        NULL, NULL, NULL, NULL, NULL
    );

    if (hService) {
        printf("Служба установлена!\n");
        CloseServiceHandle(hService);
    }
    else {
        printf("Ошибка установки: %d\n", GetLastError());
    }

    CloseServiceHandle(hSCManager);
}

// Удаление службы
void RemoveService(void) {
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager) {
        printf("Ошибка: запустите с правами администратора!\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, SERVICE_NAME, DELETE | SERVICE_STOP);
    if (hService) {
        SERVICE_STATUS ss;
        ControlService(hService, SERVICE_CONTROL_STOP, &ss);
        Sleep(2000);

        if (DeleteService(hService)) {
            printf("Служба удалена!\n");
        }
        else {
            printf("Ошибка удаления: %d\n", GetLastError());
        }
        CloseServiceHandle(hService);
    }
    else {
        printf("Служба не найдена\n");
    }

    CloseServiceHandle(hSCManager);
}

// Запуск службы
void StartService(void) {
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Ошибка подключения\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, SERVICE_NAME, SERVICE_START);
    if (hService) {
        if (StartServiceW(hService, 0, NULL)) {
            printf("Служба запущена!\n");
        }
        else {
            printf("Ошибка запуска: %d\n", GetLastError());
        }
        CloseServiceHandle(hService);
    }
    else {
        printf("Служба не найдена\n");
    }

    CloseServiceHandle(hSCManager);
}

// Остановка службы
void StopService(void) {
    SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        printf("Ошибка подключения\n");
        return;
    }

    SC_HANDLE hService = OpenServiceW(hSCManager, SERVICE_NAME, SERVICE_STOP);
    if (hService) {
        SERVICE_STATUS ss;
        if (ControlService(hService, SERVICE_CONTROL_STOP, &ss)) {
            printf("Служба остановлена\n");
        }
        else {
            printf("Ошибка остановки: %d\n", GetLastError());
        }
        CloseServiceHandle(hService);
    }
    else {
        printf("Служба не найдена\n");
    }

    CloseServiceHandle(hSCManager);
}

// Меню
void ShowMenu(void) {
    system("cls");
    printf("\n");
    printf("========================================\n");
    printf("     УПРАВЛЕНИЕ СЛУЖБОЙ WINDOWS         \n");
    printf("========================================\n");
    printf("\n");
    printf("  [1] Установить службу (требуются права админа)\n");
    printf("  [2] Удалить службу (требуются права админа)\n");
    printf("  [3] Запустить службу\n");
    printf("  [4] Остановить службу\n");
    printf("  [0] Выход\n");
    printf("\n");
    printf("----------------------------------------\n");
    printf("Выберите действие: ");
}

// Точка входа
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Пытаемся запустить как службу
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        { (LPWSTR)SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
        { NULL, NULL }
    };

    if (StartServiceCtrlDispatcher(ServiceTable)) {
        // Запустились как служба - ничего не выводим
        return 0;
    }

    // Если не удалось запустить как службу, показываем меню
    int choice;
    do {
        ShowMenu();
        scanf_s("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            InstallService();
            printf("\nНажмите Enter...");
            getchar();
            break;
        case 2:
            RemoveService();
            printf("\nНажмите Enter...");
            getchar();
            break;
        case 3:
            StartService();
            printf("\nНажмите Enter...");
            getchar();
            break;
        case 4:
            StopService();
            printf("\nНажмите Enter...");
            getchar();
            break;
        case 0:
            printf("Выход...\n");
            break;
        default:
            printf("Неверный выбор!\n");
            printf("\nНажмите Enter...");
            getchar();
        }
    } while (choice != 0);

    return 0;
}
