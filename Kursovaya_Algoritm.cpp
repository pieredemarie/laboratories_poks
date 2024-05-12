#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <locale.h>
#include <string.h>
double f(double x, int choice_func) { //Подынтегральная функция
	switch (choice_func) {
	case 1:
		return sin(x);
		break;
	case 2:
		return cos(x);
		break;
	case 3:
		return log(x);
		break;
	case 4:
		return exp(x);
		break;
	case 5:
		return sin(x + M_PI / 2);
		break;
	case 6:
		return cos(x + M_PI / 2);
		break;
	default:
		return 0.0;
		break;
	}
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

	if (a > b) {
		double temp = a;
		a = b;
		b = temp;
	}
	/*
	float xStart = 20 + (int)((a * 600) / (2 * M_PI));
	float xEnd = 20 + (int)((b * 600) / (2 * M_PI));

	for (float x = xStart; x <= xEnd; x++) {
		double xVal = (x - 20) * 6.28 / 600;
		int y = 240 - (int)(200 * f(xVal, choice_func));
		SetPixel(hdc, x, y, RGB(0, 0, 200));
	}
	*/
	for (int x = 0; x < 1920; x++) {
		double xVal = (x - 960) / 100.0;
		int y = 540 - (int)(f(xVal,choice_func) * 100.0);
		SetPixel(hdc, x, y, RGB(0, 0, 250));
	}
	int xa = 960 + (int)(a * 100.0);
	int xb = 960 + (int)(b * 100.0);
	for (int x = xa; x <= xb; x++) {
		double xVal = (x - 960) / 100.0;
		int y = 540 - (int)(f(xVal, choice_func) * 100.0);
		SetPixel(hdc, x, y, RGB(255, 0, 0));
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
void ChooseMethod(double a, double b, int n, int choice_func,int choice_method) {
	double sum = 0;
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
	default:
		printf("Неверный ввод!\n");
		return;
	}
	printf("Интеграл равен %f", sum);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_DESTROY)
		PostQuitMessage(0);
	else return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}
void CreatingMenu(double a,double b,int choice_func,int choice_method,int n) {
	WNDCLASSA wcl;
	memset(&wcl, 0, sizeof(WNDCLASSA));
	wcl.lpszClassName = "My Window";
	wcl.lpfnWndProc = WndProc;
	RegisterClassA(&wcl);
	HWND hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"My Window",
		L"График заданной функции",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	DrawFunctionGraph(hwnd, a, b, choice_func);
	VisualizeIntegration(hwnd, a, b, n, choice_func, choice_method);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
	}
}
int main() {
	setlocale(LC_ALL, "rus");
	printf("Табличные функции:\n1) sin(x)\n2) cos(x)\n3) ln(x)\n4) e^(x^2)\n5) sin(x+π/2)\n6) cos(x+π/2)\n");
	int choice;
	do {
		printf("\nВведите номер функции: \n");
		scanf_s("%d", &choice);
		int n;
		switch (choice) {
			case 1: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				printf("Введите шаг разбиения: \n");
				scanf_s("%d", &n);
				printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
				printf("Выберите метод: \n");
				int choice_method;
				scanf_s("%d", &choice_method);
				ChooseMethod(a, b, n, choice,choice_method);
				CreatingMenu(a,b,choice, choice_method, n);
				break;
			}
			case 2: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				printf("Введите шаг разбиения: \n");
				scanf_s("%d", &n);
				printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
				printf("Выберите метод: \n");
				int choice_method;
				scanf_s("%d", &choice_method);
				ChooseMethod(a, b, n, choice, choice_method);
				CreatingMenu(a, b, choice, choice_method, n);
				break;
			}
			case 3: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				printf("Введите шаг разбиения: \n");
				scanf_s("%d", &n);
				printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
				printf("Выберите метод: \n");
				int choice_method;
				scanf_s("%d", &choice_method);
				ChooseMethod(a, b, n, choice, choice_method);
				CreatingMenu(a, b, choice, choice_method, n);
				break;
			}
			case 4: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				printf("Введите шаг разбиения: \n");
				scanf_s("%d", &n);
				printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
				printf("Выберите метод: \n");
				int choice_method;
				scanf_s("%d", &choice_method);
				ChooseMethod(a, b, n, choice, choice_method);
				CreatingMenu(a, b, choice, choice_method, n);
				break;
			}
			case 5: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				printf("Введите шаг разбиения: \n");
				scanf_s("%d", &n);
				printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
				printf("Выберите метод: \n");
				int choice_method;
				scanf_s("%d", &choice_method);
				ChooseMethod(a, b, n, choice, choice_method);
				CreatingMenu(a, b, choice, choice_method, n);
				break;
			}
			case 6: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				printf("Введите шаг разбиения: \n");
				scanf_s("%d", &n);
				printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
				printf("Выберите метод: \n");
				int choice_method;
				scanf_s("%d", &choice_method);
				ChooseMethod(a, b, n, choice, choice_method);
				CreatingMenu(a, b, choice, choice_method, n);
				break;
			}
			default:
				printf("Неверный ввод!\n");
		}
	} while (choice != 0);
}
