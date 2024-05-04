#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <locale.h>
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
	}
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
void ChooseMethod(double a, double b, int n, int choice_func) {
	printf("1) Метод прямоугольников\n2) Метод трапеций\n3) Метод парабол\n");
	printf("Выберите метод: \n");
	int choice_method = 0;
	scanf_s("%d", &choice_method);
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
void DrawSinxGraph(HWND hwnd) {
	HDC hdc = GetDC(hwnd);
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	MoveToEx(hdc, 20, 240, NULL);
	LineTo(hdc, 620, 240);
	MoveToEx(hdc, 20, 20, NULL);
	LineTo(hdc, 20, 460);
	for (int x = 20; x < 640; x++) {
		double xVal = (x - 20) * 6.28 / 600;
		int y = 240 - (int)(200 * sin(xVal));
		SetPixel(hdc, x, y, RGB(0, 0, 200));
	}
	// Метод прямоугольников
	int n = 3; // Количество прямоугольников
	float a = 1.0f, b = 2.0f; // Пределы интегрирования
	float dx = (b - a) / n; // Шаг

	HPEN rectPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HGDIOBJ oldRectPen = SelectObject(hdc, rectPen);

	for (int i = 0; i < n; i++) {
		float x1 = a + i * dx;
		float x2 = x1 + dx;
		float y1 = sin(x1);
		float y2 = sin(x2);

		int x1Px = 20 + (int)((x1 * 600) / 6.28);
		int x2Px = 20 + (int)((x2 * 600) / 6.28);
		int y1Px = 240 - (int)(200 * y1);
		int y2Px = 240 - (int)(200 * y2);

		MoveToEx(hdc, x1Px, y1Px, NULL);
		LineTo(hdc, x1Px, y2Px);
		LineTo(hdc, x2Px, y2Px);
		LineTo(hdc, x2Px, y1Px);
		LineTo(hdc, x1Px, y1Px);
	}

	SelectObject(hdc, oldRectPen);
	DeleteObject(rectPen);

	SelectObject(hdc, oldPen);
	DeleteObject(pen);
	ReleaseDC(hwnd, hdc);
}
void DrawCosGraph(HWND hwnd) {
	HDC hdc = GetDC(hwnd);
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	MoveToEx(hdc, 20, 240, NULL);
	LineTo(hdc, 620, 240);
	MoveToEx(hdc, 20, 20, NULL);
	LineTo(hdc, 20, 460);
	for (int x = 20; x < 640; x++) {
		double xVal = (x - 20) * 6.28 / 600;
		int y = 240 - (int)(200 * cos(xVal));
		SetPixel(hdc, x, y, RGB(0, 0, 200));
	}
}
void DrawLogGraph(HWND hwnd) {
	HDC hdc = GetDC(hwnd);
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	MoveToEx(hdc, 20, 240, NULL);
	LineTo(hdc, 620, 240);
	MoveToEx(hdc, 20, 20, NULL);
	LineTo(hdc, 20, 460);
	for (int x = 20; x < 640; x++) {
		double xVal = (x - 20) * 6.28 / 600;
		int y = 240 - (int)(200 * log(xVal));
		SetPixel(hdc, x, y, RGB(0, 0, 200));
	}
}
void CreatingMenu(int choice) {
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
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	// здесь рисуем в общем.
	switch (choice) {
		case 1:
			DrawSinxGraph(hwnd);
			break;
		case 2:
			DrawCosGraph(hwnd);
			break;
		case 3:
			DrawLogGraph(hwnd);
			break;
		case 4:
			////
			printf("В процессе разработки...\n");
			break;
	}
	// рисуем прямоугольники

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
		int n = 1000;
		switch (choice) {
			case 1: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				ChooseMethod(a, b, n, choice);
				CreatingMenu(choice);
				break;
			}
			case 2: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				ChooseMethod(a, b, n, choice);
				CreatingMenu(choice);
				break;
			}
			case 3: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				ChooseMethod(a, b, n, choice);
				CreatingMenu(choice);
				break;
			}
			case 4: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				ChooseMethod(a, b, n, choice);
				break;
			}
			case 5: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				ChooseMethod(a, b, n, choice);
				break;
			}
			case 6: {
				printf("Введите пределы интегрирования: \n");
				float a, b;
				printf("a = ");
				scanf_s("%f", &a);
				printf("b = ");
				scanf_s("%f", &b);
				ChooseMethod(a, b, n, choice);
				break;
			}
			case 7: {
				
			}
		}
	} while (choice != 0);
}
