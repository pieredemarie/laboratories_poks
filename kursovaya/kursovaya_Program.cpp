#define _CRT_NON_CONFORMING_SWPRINTFS
#define _USE_MATH_DEFINES
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
const int IDC_BTN_INTEGRATE = 1;
const int IDC_BTN_APPROXIMATE = 2;
const int IDC_BTN_EXIT = 3;
const int nmax = 20;
int numPoints = 0;
typedef double arrtype[nmax];
int i, n; /// у нас же пользователь введет только 3 точки да?
arrtype a, b;
double* polynom;
double polynom1[nmax];
HWND hwndMain; // Главное окно
HWND hwndIntegral; // Окно для интегрирования
HWND hwndPolynom; // Окно для полиномиальной кого-то там
HWND hwndGraphics; // Окно для вывода графика функции
HWND hwndIntegrateButton, hwndLowerBound, hwndUpperBound, hwndPartitions, hwndFunctionList, hwndMethodList;
HWND hwndPolynomButton, hwndx1, hwndx2, hwndx3, hwndy1, hwndy2, hwndy3,hwndAddPoint;
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
///ПОЛИНОМИАЛЬНАЯ АППРОКСИМАЦИЯ СДЕЛАНА УРААААААААААААААААААААААААА
///начинается здесь.
double* Result(arrtype a, arrtype b, int n);
double Down(int k, int n, arrtype a);
double Multi(double p, double s, int k, int n, int jn, int jk);
void Up(int n, double* a, double y[nmax][nmax]);
double* Result(arrtype a, arrtype b, int n) {
    int i, j;
    double y[nmax][nmax];
    arrtype z;
    double Down(int k, int n, arrtype a);
    double Multi(double p, double s, int k, int n, int jn, int jk);
    void Up(int n, double* a, double y[nmax][nmax]);

    memset(z, 0, nmax * sizeof(double));
    Up(n, a, y);

    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            y[i][j] = y[i][j] * b[i];

    for (j = 0; j <= n; j++)
        for (i = 0; i <= n; i++)
            z[j] = z[j] + y[i][j];

    return z;
}
double Down(int k, int n, arrtype a) {
    int i;
    double q;
    q = 1;
    for (i = 0; i <= n; i++) {
        if (i != k) q = q * (a[k] - a[i]);
    }
    return q;
}
double Multi(double p, double s, int k, int n, int jn, int jk) {
    int j;
    double pr;
    for (j = jn; j <= jk; j++) {
        if (j != k)
            if (jk == (n - 1))
                s = s + p * a[j];
            else {
                pr = p;
                p = p * a[j];
                s = Multi(p, s, k, n, j + 1, jk + 1);
                p = pr;
            }
    }
    return s;
}
void Up(int n, double* a, double y[nmax][nmax]) {
    int i, k;
    double s, p, z;
    char mn;
    for (i = 0; i <= n; i++) {
        z = Down(i, n, a);
        mn = -1;
        for (k = 1; k <= n; k++) {
            p = 1, s = 0;
            s = Multi(p, s, i, n + 1, 0, n + 1 - k);
            y[i][k] = mn * s / z;
            mn = -1 * mn;
        }
        y[i][0] = 1 / z;
    }
}
///и заканчивается здесь.
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
double maxValue(double a, double b, int choice_func) {
    double maxVal = f(a, choice_func);
    for (double x = a + 0.1; x <= b; x += 0.1) {
        double val = f(x, choice_func);
        if (val > maxVal) {
            maxVal = val;
        }
    }
    return maxVal;
}

double minValue(double a, double b, int choice_func) {
    double minVal = f(a, choice_func);
    for (double x = a + 0.1; x <= b; x += 0.1) {
        double val = f(x, choice_func);
        if (val < minVal) {
            minVal = val;
        }
    }
    return minVal;
}
void DrawFunctionGraph(HWND hwnd, double a, double b, int choice_func) {
    HDC hdc = GetDC(hwnd);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Черный цвет для осей
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    // Определяем размеры окна
    int windowWidth = 1920;
    int windowHeight = 1080;

    // Вычисляем минимальное и максимальное значения функции на заданном интервале
    double minVal = minValue(a, b, choice_func);
    double maxVal = maxValue(a, b, choice_func);

    MoveToEx(hdc, windowWidth / 2, 0, NULL);
    LineTo(hdc, windowWidth / 2, windowHeight);
    MoveToEx(hdc, 0, windowHeight / 2, NULL);
    LineTo(hdc, windowWidth, windowHeight / 2);


    // Рисуем подписи для осей
    TextOut(hdc, windowWidth - 100, windowHeight / 2 - 20, L"X", 1);
    TextOut(hdc, windowWidth / 2 - 20, 50, L"Y", 1);

    // Рисуем график функции
    double xScale = (double)(windowWidth * 0.3) / (b - a);
    double yScale = (double)(windowHeight * 0.3) / (maxVal - minVal);

    MoveToEx(hdc, (int)(windowWidth / 2), (int)(windowHeight / 2), NULL);
    for (double x = a; x <= b; x += 0.01) {
        double funcVal = f(x, choice_func);
        int xPos = (int)(windowWidth / 2 + x * xScale);
        int yPos = (int)(windowHeight / 2 - funcVal * yScale);
        SetPixel(hdc, xPos, yPos, RGB(0, 0, 255)); // Рисуем синие пиксели
    }

    SelectObject(hdc, oldPen);
    ReleaseDC(hwnd, hdc);
}


void VisualizeIntegration(HWND hwnd, double a, double b, int n, int choice_func, int choice_method) {
    HDC hdc = GetDC(hwnd);
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Черный цвет для осей
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    // Определяем размеры окна
    int windowWidth = 1920;
    int windowHeight = 1080;

    // Вычисляем минимальное и максимальное значения функции на заданном интервале
    double minVal = minValue(a, b, choice_func);
    double maxVal = maxValue(a, b, choice_func);

    // Рисуем подписи для осей
    TextOut(hdc, windowWidth - 100, windowHeight / 2 - 20, L"X", 1);
    TextOut(hdc, windowWidth / 2 - 20, 50, L"Y", 1);

    double h = (b - a) / n;
    double x = a;
    double xScale = (double)(windowWidth * 0.3) / (b - a);
    double yScale = (double)(windowHeight * 0.3) / (maxVal - minVal);

    switch (choice_method) {
    case 1: // Метод прямоугольников
        pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Красный цвет для прямоугольников
        SelectObject(hdc, pen);
        for (int i = 0; i < n; i++) {
            int xStart = (int)(windowWidth / 2 + x * xScale);
            int xEnd = (int)(windowWidth / 2 + (x + h) * xScale);
            int y = (int)(windowHeight / 2 - f(x, choice_func) * yScale);
            int rectHeight = (int)(f(x, choice_func) * yScale);
            MoveToEx(hdc, xStart, y, NULL);
            LineTo(hdc, xStart, y + rectHeight);
            LineTo(hdc, xEnd, y + rectHeight);
            LineTo(hdc, xEnd, y);
            LineTo(hdc, xStart, y);
            x += h;
        }
        DeleteObject(pen);
        break;
    case 2: // Метод трапеций
        pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0)); // Зеленый цвет для трапеций
        SelectObject(hdc, pen);
        for (int i = 0; i < n; i++) {
            int xStart = (int)(windowWidth / 2 + x * xScale);
            int xEnd = (int)(windowWidth / 2 + (x + h) * xScale);
            int y1 = (int)(windowHeight / 2 - f(x, choice_func) * yScale);
            int y2 = (int)(windowHeight / 2 - f(x + h, choice_func) * yScale);
            MoveToEx(hdc, xStart, y1, NULL);
            LineTo(hdc, xEnd, y2);
            LineTo(hdc, xEnd, (int)windowHeight / 2);
            LineTo(hdc, xStart, (int)windowHeight / 2);
            LineTo(hdc, xStart, y1);
            x += h;
        }
        DeleteObject(pen);
        break;
    case 3: // Метод парабол
        pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255)); // Синий цвет для парабол
        SelectObject(hdc, pen);
        for (int i = 0; i < n; i += 2) {
            int xStart = (int)(windowWidth / 2 + x * xScale);
            int xMid = (int)(windowWidth / 2 + (x + h) * xScale);
            int xEnd = (int)(windowWidth / 2 + (x + 2 * h) * xScale);

            int y1 = (int)(windowHeight / 2 - f(x, choice_func) * yScale);
            int y2 = (int)(windowHeight / 2 - f(x + h, choice_func) * yScale);
            int y3 = (int)(windowHeight / 2 - f(x + 2 * h, choice_func) * yScale);
            MoveToEx(hdc, xStart, y1, NULL);
            LineTo(hdc, xMid, y2);
            LineTo(hdc, xEnd, y3);
            LineTo(hdc, xEnd, (int)windowHeight / 2);
            LineTo(hdc, xStart, (int)windowHeight / 2);
            LineTo(hdc, xStart, y1);
            x += 2 * h;
        }
        DeleteObject(pen);
        break;
    }

    SelectObject(hdc, oldPen);
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
    HWND hwndLabelBound = CreateWindowEx(
        0, L"STATIC", L"Границы интегрирования", WS_CHILD | WS_VISIBLE,
        110, 10, 230, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    HWND hwndLabelA = CreateWindowEx(
        0, L"STATIC", L"a = ", WS_CHILD | WS_VISIBLE,
        60, 40, 20, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    hwndLowerBound = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        90, 40, 100, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    HWND hwndLabelB = CreateWindowEx(
        0, L"STATIC", L"b = ", WS_CHILD | WS_VISIBLE,
        210, 40, 20, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    hwndUpperBound = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        240, 40, 100, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    HWND hwndCountN = CreateWindowEx(
        0, L"STATIC", L"Число отрезков", WS_CHILD | WS_VISIBLE,
        30, 70, 130, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    hwndPartitions = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        30, 100, 100, 20, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    // Создание списка функций
    hwndFunctionList = CreateWindowEx(
        0, L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
        30, 140, 200, 200, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
    );
    hwndMethodList = CreateWindowEx(
        0, L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
        30, 180, 200, 200, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
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
        140, 220, 150, 30, hwndIntegral, nullptr, wcIntegral.hInstance, nullptr
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
    wcPolynom.lpfnWndProc = WndProcPolynom;
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
        600, 250, 400, 250,
        hWndParent, nullptr, wcPolynom.hInstance, nullptr
    );
    hwndx1 = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        40, 30, 100, 20, hwndPolynom, nullptr, wcPolynom.hInstance, nullptr
    );
    hwndy1 = CreateWindowEx(
        0, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
        200, 30, 100, 20, hwndPolynom, nullptr, wcPolynom.hInstance, nullptr
    );
    HWND hwndLabelX1 = CreateWindowEx(
        0, L"STATIC", L"x", WS_CHILD | WS_VISIBLE,
        80, 10, 20, 20, hwndPolynom, nullptr, wcPolynom.hInstance, nullptr
    );

    // Создание меток для полей ввода y1, y2, y3
    HWND hwndLabelY1 = CreateWindowEx(
        0, L"STATIC", L"y", WS_CHILD | WS_VISIBLE,
        240, 10, 20, 20, hwndPolynom, nullptr, wcPolynom.hInstance, nullptr
    );
    hwndAddPoint = CreateWindowEx(
        0, L"BUTTON", L"Добавить точку", WS_CHILD | WS_VISIBLE,
        30, 100, 300, 30, hwndPolynom, nullptr, wcPolynom.hInstance, nullptr
    );
    hwndPolynomButton = CreateWindowEx(
        0, L"BUTTON", L"Аппроксимизировать", WS_CHILD | WS_VISIBLE,
        30, 130, 300, 30, hwndPolynom, nullptr, wcPolynom.hInstance, nullptr
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
            int choice_func = (int)SendMessage(hwndFunctionList, CB_GETCURSEL, 0, 0) + 1;
            int methodChoice = (int)SendMessage(hwndMethodList, CB_GETCURSEL, 0, 0) + 1;
            ///MessageBoxA(hWnd, "NEGRETOS ONLINE!!!!!!!!!!!", "Результат интегрирования", MB_OK);
            double a = _wtof(lowerBoundstr);
            double b = _wtof(upperBoundstr);
            int n = _wtoi(nStr);
            if (wcslen(lowerBoundstr) == 0 || wcslen(upperBoundstr) == 0 || wcslen(nStr) == 0)
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
LRESULT CALLBACK WndProcPolynom(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY: {
        DestroyWindow(hwnd);
        hwndPolynom = nullptr;
        UnregisterClass(L"PolynomWindowClass", GetModuleHandle(nullptr));
        break;
    }
    case WM_COMMAND: {
        if (lParam == (LPARAM)hwndAddPoint && HIWORD(wParam) == BN_CLICKED) {
            // Получаем значения x и y из полей ввода
            wchar_t bufferX[100], bufferY[100];
            GetWindowText(hwndx1, bufferX, 100);
            GetWindowText(hwndy1, bufferY, 100);
            double x = _wtof(bufferX);
            double y = _wtof(bufferY);
            a[numPoints] = x;
            b[numPoints] = y;
            numPoints++;
            MessageBoxA(hwndPolynom, "Точка добавлена!", "Аппроксимация", MB_OK);
            SetWindowText(hwndx1, L"");
            SetWindowText(hwndy1, L"");
        }
        // Обработчик события для кнопки "Аппроксимизировать"
        if (lParam == (LPARAM)hwndPolynomButton && HIWORD(wParam) == BN_CLICKED) {
            // Вызываем функцию Result для вычисления полинома
            polynom = Result(a, b, numPoints);

            // Выводим результат в MessageBox
            wchar_t buffer[1000];
            swprintf_s(buffer, L"Коэффициенты полинома (по убыванию от старшей степени): %lf %lf %lf ...", polynom[0], polynom[1], polynom[2]);
            MessageBox(hwndPolynom, buffer, L"Результат", MB_OK);
            numPoints = 0;
        }
        break;
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
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
