#define _USE_MATH_DEFINES
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
const int IDC_BTN_INTEGRATE = 1;
const int IDC_BTN_APPROXIMATE = 2;
const int IDC_BTN_EXIT = 3;

HWND hwndMain; // Главное окно
HWND hwndIntegral; // Окно для интегрирования
HWND hwndPolynom; // Окно для полиномиальной кого-то там
HWND hwndGraphics; // Окно для вывода графика функции
HWND hwndIntegrateButton, hwndLowerBound, hwndUpperBound, hwndPartitions,hwndFunctionList,hwndMethodList;
LRESULT CALLBACK WndProcMain(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcIntegral(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcPolynom(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ShowPolynomialWindow(HWND hwnd);
void ShowIntegrationWindow(HWND hwnd);
double FindingIntegral(double a, double b, int n, int choice_func, int choice_method);
void VisualizeIntegration(HWND hwnd, double a, double b, int n, int choice_func, int choice_method);
void CreatingGraphics(double a, double b, int choice_func, int choice_method, int n);
void DrawFunctionGraph(HWND hwnd, double a, double b, int choice_func);
double RectInt(double a, double b, int n, int choice_func);
double TrapInt(double a, double b, int n, int choice_func);
double SimsonInt(double a, double b, int n, int choice_func);
double f(double x, int choice_func);
double RectInt(double a, double b, int n, int choice_func) {
    double h = (b - a) / n;
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += h * f(a + i * h, choice_func);
    }
    return sum;
}
double TrapInt(double a, double b, int n, int choice_func) {
    double h = (b - a) / n;
    double sum = f(a, choice_func) + f(b, choice_func);
    for (int i = 0; i < n; i++) {
        sum += 2 * f(a + i * h, choice_func);
    }
    sum *= h / 2;
    return sum;
}
double SimsonInt(double a, double b, int n, int choice_func) {
    double h = (b - a) / n;
    double sum = f(a, choice_func) + f(b, choice_func);
    int k;
    for (int i = 0; i < n; i++) {
        k = 2 + 2 * (i % 2);
        sum += k * f(a + i * h, choice_func);
    }
    sum *= h / 3;
    return sum;
}
double f(double x, int choice_func) { //Подынтегральная функция
    switch (choice_func) {
    case 1:
        return sin(x);
        break;
    case 2:
        return cos(x);
        break;
    case 3:
        return sin(x + M_PI / 2);
        break;
    case 4:
        return sin(cos(x));
        break;
    case 5:
        return sin(x + 3 * M_PI / 2);
        break;
    case 6:
        return cos(x + 3 * M_PI / 2);
        break;
    default:
        return 0.0;
        break;
    }
}
double FindingIntegral(double a, double b, int n, int choice_func, int choice_method) {
    double sum = 0.0;
    switch (choice_method) {
        case 1:
            sum = RectInt(a, b, n, choice_func);
            break;
        case 2:
            sum = TrapInt(a, b, n, choice_func);
            break;
        case 3:
            sum = SimsonInt(a, b, n, choice_func);
            break;
    }
    return sum;
}
void DrawFunctionGraph(HWND hwnd, double a, double b, int choice_func) {
    HDC hdc = GetDC(hwnd);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, 0, 540, NULL);
    LineTo(hdc, 1920, 540);
    MoveToEx(hdc, 960, 0, NULL);
    LineTo(hdc, 960, 1080);
    TextOut(hdc, 10, 530, L"X", 1);
    TextOut(hdc, 970, 10, L"Y", 1);


    for (int x = 0; x < 1920; x++) {
        double xVal = (x - 960) / 100.0;
        int y = 540 - (int)(f(xVal, choice_func) * 100.0);
        SetPixel(hdc, x, y, RGB(0, 0, 250));
    }
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    ReleaseDC(hwnd, hdc);
}
void VisualizeIntegration(HWND hwnd, double a, double b, int n, int choice_func, int choice_method) {
    HDC hdc = GetDC(hwnd);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    double scale = 100.0;
    int centerX = 1920 / 2;
    int centerY = 1080 / 2;
    double h = (b - a) / n;
    double x = a;

    switch (choice_method) {
    case 1: // Метод прямоугольников
        for (int i = 0; i < n; i++) {
            int xStart = centerX + (int)(x * scale);
            int xEnd = centerX + (int)((x + h) * scale);
            int y = centerY - (int)(f(x, choice_func) * scale);
            MoveToEx(hdc, xStart, y, NULL);
            LineTo(hdc, xEnd, y);
            LineTo(hdc, xEnd, centerY);
            LineTo(hdc, xStart, centerY);
            LineTo(hdc, xStart, y);
            x += h;
        }
        break;
    case 2: // Метод трапеций
        for (int i = 0; i < n; i++) {
            int xStart = centerX + (int)(x * scale);
            int xEnd = centerX + (int)((x + h) * scale);
            int y1 = centerY - (int)(f(x, choice_func) * scale);
            int y2 = centerY - (int)(f(x + h, choice_func) * scale);
            MoveToEx(hdc, xStart, y1, NULL);
            LineTo(hdc, xEnd, y2);
            LineTo(hdc, xEnd, centerY);
            LineTo(hdc, xStart, centerY);
            LineTo(hdc, xStart, y1);
            x += h;
        }
        break;
    case 3: // Метод парабол
        for (int i = 0; i < n; i += 2) {
            int xStart = centerX + (int)(x * scale);
            int xMid = centerX + (int)((x + h) * scale);
            int xEnd = centerX + (int)((x + 2 * h) * scale);
            int y1 = centerY - (int)(f(x, choice_func) * scale);
            int y2 = centerY - (int)(f(x + h, choice_func) * scale);
            int y3 = centerY - (int)(f(x + 2 * h, choice_func) * scale);
            MoveToEx(hdc, xStart, y1, NULL);
            LineTo(hdc, xMid, y2);
            LineTo(hdc, xEnd, y3);
            LineTo(hdc, xEnd, centerY);
            LineTo(hdc, xStart, centerY);
            LineTo(hdc, xStart, y1);
            x += 2 * h;
        }
        break;
    }

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
    ReleaseDC(hwnd, hdc);
}
void CreatingGraphics(double a, double b, int choice_func, int choice_method, int n) {
    WNDCLASSA wcl;
    memset(&wcl, 0, sizeof(WNDCLASSA));
    wcl.lpszClassName = "My Window";
    wcl.lpfnWndProc = WndProc;
    RegisterClassA(&wcl);
    hwndGraphics = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        L"My Window",
        L"График заданной функции",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    ShowWindow(hwndGraphics, SW_SHOWNORMAL);
    DrawFunctionGraph(hwndGraphics, a, b, choice_func);
    VisualizeIntegration(hwndGraphics, a, b, n, choice_func, choice_method);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        DispatchMessage(&msg);
    }
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg{};
    WNDCLASSEX wcMain{};
    wcMain.cbSize = sizeof(WNDCLASSEX);
    wcMain.style = CS_HREDRAW | CS_VREDRAW;
    wcMain.lpfnWndProc = WndProcMain;
    wcMain.cbClsExtra = 0;
    wcMain.cbWndExtra = 0;
    wcMain.hInstance = hInstance;
    wcMain.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcMain.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcMain.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcMain.lpszMenuName = nullptr;
    wcMain.lpszClassName = L"MainWindowClass";
    wcMain.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcMain)) {
        return 0;
    }

    hwndMain = CreateWindowEx(
        0,
        wcMain.lpszClassName,
        L"Интегрирование и аппроксимация",
        WS_OVERLAPPEDWINDOW,
        300, 300, 326, 174,
        nullptr, nullptr, hInstance, nullptr
    );

    if (!hwndMain) {
        return 0;
    }

    ShowWindow(hwndMain, nCmdShow);
    UpdateWindow(hwndMain);

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (hwndIntegral && IsDialogMessage(hwndIntegral, &msg)) {
            continue;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        CreateWindow(
            L"BUTTON",
            L"Интегрирование",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            5, 0, 300, 30, hWnd, reinterpret_cast<HMENU>(IDC_BTN_INTEGRATE), nullptr, nullptr
        );

        CreateWindow(
            L"BUTTON",
            L"Полиномиальная аппроксимация",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            5, 35, 300, 30, hWnd, reinterpret_cast<HMENU>(IDC_BTN_APPROXIMATE), nullptr, nullptr
        );

        CreateWindow(
            L"BUTTON",
            L"Выход",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            5, 70, 300, 30, hWnd, reinterpret_cast<HMENU>(IDC_BTN_EXIT), nullptr, nullptr
        );
        break;
    }
    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_BTN_INTEGRATE:
            ShowIntegrationWindow(hWnd);
            break;
        case IDC_BTN_APPROXIMATE:
            ShowPolynomialWindow(hWnd);
            break;
        case IDC_BTN_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
void ShowIntegrationWindow(HWND hWndParent) {
    if (hwndIntegral) {
        DestroyWindow(hwndIntegral);
        hwndIntegral = nullptr;
    }

    WNDCLASSEX wcIntegral{};
    wcIntegral.cbSize = sizeof(WNDCLASSEX);
    wcIntegral.style = CS_HREDRAW | CS_VREDRAW;
    wcIntegral.lpfnWndProc = WndProcIntegral;
    wcIntegral.cbClsExtra = 0;
    wcIntegral.cbWndExtra = 0;
    wcIntegral.hInstance = GetModuleHandle(nullptr);
    wcIntegral.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcIntegral.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcIntegral.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcIntegral.lpszMenuName = nullptr;
    wcIntegral.lpszClassName = L"IntegralWindowClass";
    wcIntegral.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    if (!RegisterClassEx(&wcIntegral)) {
        return;
    }

    hwndIntegral = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        wcIntegral.lpszClassName,
        L"Интегрирование",
        WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        600, 250, 426, 300,
        hWndParent, nullptr, wcIntegral.hInstance, nullptr
    );
    hwndLowerBound = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 10, 100, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );

    hwndUpperBound = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 10, 100, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );

    hwndPartitions = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        230, 10, 100, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );

    // Создание списка функций
    hwndFunctionList = CreateWindowEx(
        0, L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
        10, 40, 200, 200, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    hwndMethodList = CreateWindowEx(
        0, L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
        10, 80, 200, 200, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    ///функции
    SendMessage(hwndFunctionList, CB_ADDSTRING, 0, (LPARAM)L"sin(x)");
    SendMessage(hwndFunctionList, CB_ADDSTRING, 0, (LPARAM)L"cos(x)");
    SendMessage(hwndFunctionList, CB_ADDSTRING, 0, (LPARAM)L"sin(x+pi/2)");
    SendMessage(hwndFunctionList, CB_ADDSTRING, 0, (LPARAM)L"sin(cos(x))");
    SendMessage(hwndFunctionList, CB_ADDSTRING, 0, (LPARAM)L"sin(x+3*pi/2)");
    SendMessage(hwndFunctionList, CB_ADDSTRING, 0, (LPARAM)L"cos(x+3*pi/2)");
    SendMessage(hwndFunctionList, CB_SETCURSEL, 0, 0);
    /// методЫ
    SendMessage(hwndMethodList, CB_ADDSTRING, 0, (LPARAM)L"Метод прямоугольников");
    SendMessage(hwndMethodList, CB_ADDSTRING, 0, (LPARAM)L"Метод трапеций");
    SendMessage(hwndMethodList, CB_ADDSTRING, 0, (LPARAM)L"Метод Симсона");
    SendMessage(hwndMethodList, CB_SETCURSEL, 0, 0);
    hwndIntegrateButton = CreateWindowEx(
        0, L"BUTTON", L"Интегрировать", WS_CHILD | WS_VISIBLE,
        10, 130, 150, 30, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
}
void ShowPolynomialWindow(HWND hWndParent) {
    if (hwndPolynom) {
        DestroyWindow(hwndPolynom);
        hwndPolynom = nullptr;
    }
    WNDCLASSEX wcPolynom{};
    wcPolynom.cbSize = sizeof(WNDCLASSEX);
    wcPolynom.style = CS_HREDRAW | CS_VREDRAW;
    wcPolynom.lpfnWndProc = WndProcIntegral;
    wcPolynom.cbClsExtra = 0;
    wcPolynom.cbWndExtra = 0;
    wcPolynom.hInstance = GetModuleHandle(nullptr);
    wcPolynom.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcPolynom.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcPolynom.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcPolynom.lpszMenuName = nullptr;
    wcPolynom.lpszClassName = L"Аппроксимация";
    wcPolynom.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wcPolynom);
    hwndPolynom = CreateWindowEx(
        WS_EX_WINDOWEDGE,
        wcPolynom.lpszClassName,
        L"Аппроксимация",
        WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
        600,250,515,535,
        hWndParent,nullptr,wcPolynom.hInstance,nullptr
    );
    HWND hwndx1 = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        10, 10, 100, 20, hwndIntegral, nullptr, wcPolynom.hInstance, nullptr
    );

    HWND hwndy1 = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        120, 10, 100, 20, hwndIntegral, nullptr, wcPolynom.hInstance, nullptr
    );

    HWND hwndx2 = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        230, 10, 100, 20, hwndIntegral, nullptr, wcPolynom.hInstance, nullptr
    );
}
LRESULT CALLBACK WndProcIntegral(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            DestroyWindow(hWnd);
            hwndIntegral = nullptr;
            UnregisterClass(L"IntegralWindowClass", GetModuleHandle(nullptr));
            ///PostQuitMessage(0);
            break;
        case WM_COMMAND: {
            if (LOWORD(wParam) == BN_CLICKED && HWND(lParam) == hwndIntegrateButton) {
                wchar_t lowerBoundstr[255];
                wchar_t upperBoundstr[255];
                wchar_t nStr[255];
                GetWindowText(hwndLowerBound, lowerBoundstr, sizeof(lowerBoundstr) / sizeof(wchar_t));
                GetWindowText(hwndUpperBound, upperBoundstr, sizeof(upperBoundstr) / sizeof(wchar_t));
                GetWindowText(hwndPartitions, nStr, sizeof(nStr) / sizeof(wchar_t));
                int choice_func = (int)SendMessage(hwndFunctionList, CB_GETCURSEL, 0, 0)+1;
                int methodChoice = (int)SendMessage(hwndMethodList, CB_GETCURSEL, 0, 0) + 1;
                ///MessageBoxA(hWnd, "NEGRETOS ONLINE!!!!!!!!!!!", "Результат интегрирования", MB_OK);
                double a = _wtof(lowerBoundstr);
                double b = _wtof(upperBoundstr);
                int n = _wtoi(nStr);
                if (a == 0 || b == 0 || n == 0)
                    MessageBoxA(hwndIntegral, "Одно из полей не введено!", "Ошибка!", MB_OK);
                else {
                    double sum = FindingIntegral(a, b, n, choice_func, methodChoice);
                    char buffer[255];
                    sprintf_s(buffer, "Значение определенного интеграла: %.6f", sum);
                    MessageBoxA(hWnd, buffer, "Результат интегрирования", MB_OK);
                    int question = MessageBoxA(NULL, "Нужно ли вывести график?", "визуализация", MB_YESNO | MB_ICONQUESTION);
                    if (question == IDYES)
                        CreatingGraphics(a, b, choice_func, methodChoice, n);
                }

            }
            break;
        }
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
