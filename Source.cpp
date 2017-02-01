#include <Windows.h>
#include <stdio.h>
#include <math.h>

#define pi 3.14

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// Процедура рисовании линии
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);

char szProgName[] = "Graphs";

int i, xView, yView;
double y;
char Buf[5];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG lpMsg;
	WNDCLASS w;

	w.lpszClassName = (LPCWSTR)szProgName;
	w.hInstance = hInstance;
	w.lpfnWndProc = WndProc;
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hIcon = 0;
	w.lpszMenuName = 0;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	// Если не удается зарегистрировать класс окна - выходим
	if (!RegisterClass(&w))
		return 0;

	// Создадим окно в памяти
	hWnd = CreateWindow((LPCWSTR)szProgName, (LPCWSTR)szProgName, WS_OVERLAPPEDWINDOW, 100, 100, 500, 400, (HWND)NULL, (HMENU)NULL, (HINSTANCE)hInstance, (HINSTANCE)NULL);

	// Выводим окно из памяти на экран
	ShowWindow(hWnd, nCmdShow);
	// Обновим содержимое окна
	UpdateWindow(hWnd);

	// Цикл обработки сообщений
	while (GetMessage(&lpMsg, NULL, 0, 0)) // Получаем сообщение из очереди
	{
		TranslateMessage(&lpMsg); // Преобразует сообщения клавиш в символы
		DispatchMessage(&lpMsg); // Передает сообщение соответствующей функции окна
	}
	return lpMsg.wParam;
}

// Функция окна
LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc; // создаем контекст устройства
	PAINTSTRUCT ps; // создаём экземпляр структуры графического вывода
	HPEN hPen = NULL; // создаём перо
	
	// Цикл обработки сообщений
	switch (messg)
	{
	case WM_SIZE:
		xView = LOWORD(lParam);
		yView = HIWORD(lParam);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_ISOTROPIC); // логические единицы отображаем, как физические
		SetWindowExtEx(hdc, 500, 500, NULL); // длина осей
		SetViewportExtEx(hdc, xView, -yView, NULL); // определяем область вывода
		SetViewportOrgEx(hdc, xView / 6, yView / 2, NULL); // начало координат

		// Рисуем оси координат
		Line(hdc, 0, 220, 0, -220); // ось Y
		Line(hdc, -100, 0, 500, 0); // ось X
		MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

		// Создание красного пера
		hPen = CreatePen(1, 4, RGB(255, 25, 0));
		SelectObject(hdc, hPen);

		// Синусоида
		for (i = 0; i < 450; i++)
		{
			y = 180.0 * (exp(-i * 0.01)) * sin(pi * i * (200.0 / 400.0) / 180.0);
			LineTo(hdc, i, (int)y);
		}

		// Делаем перо снова черным
		hPen = CreatePen(1, 1, RGB(0, 0, 0));
		SelectObject(hdc, hPen);
		
		// Наносим деления
		for (i = -100; i < 500; i += 100)
		{
			Line(hdc, i, 3, i, -3);
			Line(hdc, -3, i, 3, i);
			sprintf_s(Buf, "%d", i);
			TextOut(hdc, i-5, -5, (LPCWSTR)Buf, strlen(Buf));
			TextOut(hdc, -5, i, (LPCWSTR)Buf, strlen(Buf));
		}

		ValidateRect(hWnd, NULL); // обновляем экран
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: // сообщение выхода - разрушаем окно
		DeleteObject(hPen); // не забываем уничтожить перья
		PostQuitMessage(0); // посылаем сообщение выхода с кодом 0 - нормальное завершение
		break;
	default:
		return DefWindowProc(hWnd, messg, wParam, lParam); // освобождаем очередь сообщений от неопознанных
		break;
	}
	return 0;
}

// Функция рисования линии
BOOL Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); // сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2); // нарисовать линию
}