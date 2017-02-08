#include "SINUS.h"

void SINUS::plot(HDC hdc, int xView, int yView)
{
	HPEN hPen = NULL;
	int i, y;
	TCHAR Buf[10];

	double scale = 40.0;

	// Рисуем оси координат
	Line(hdc, 0, yView / 2 - 120, 0, -(yView / 2 - 120)); // ось Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

							   // Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	// Синусоида
	for (i = 0; i < xView / 2 - 40; i++)
	{
		y = sin((double)i / scale) * (yView / 2 - 120);
		LineTo(hdc, i, (int)y);
	}
	MoveToEx(hdc, 0, 0, NULL);
	for (i = 0; i > -(xView / 2 - 40); i--)
	{
		//y = 180.0 * (exp(-i * 0.01)) * sin(pi * i * (200.0 / 400.0) / 180.0);
		y = sin((double)i / scale) * (yView / 2 - 120);
		LineTo(hdc, i, (int)y);
	}

	// Делаем перо снова черным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// Наносим деления
	MoveToEx(hdc, 0, 0, NULL);
	for (i = yView / 2 - 120; i > -(yView / 2 - 120); i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf_s(Buf, L"%4.2f", (float)i / (yView / 2 - 120));
		TextOut(hdc, -5, i, Buf, (int)_ftcslen(Buf));
	}
	for (i = -(xView / 2 - 40) / 90 * 90; i < (xView / 2 - 40) / 90 * 90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf_s(Buf, L"%4.2f", (float)i / scale / pi * 180);
		TextOut(hdc, i - 5, -5, Buf, (int)_ftcslen(Buf));
	}

	DeleteObject(hPen);
}
