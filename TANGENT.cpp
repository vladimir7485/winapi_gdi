#include "TANGENT.h"

void TANGENT::plot(HDC hdc, int xView, int yView)
{
	HPEN hPen = NULL;
	int i, y;
	TCHAR Buf[10];

	double scale = 50.0;

	// Рисуем оси координат
	Line(hdc, 0, yView / 2 - 120, 0, -(yView / 2 - 120)); // ось Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

							   // Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	double y_prev = 0.0;
	// Тангенс
	for (i = 0; i < xView / 2 - 40; i++)
	{
		y = (int)(tan((double)i / scale) * scale);
		if (y > -(yView / 2 - 120) && y < yView / 2 - 120)
		{
			if (y < 0 && y_prev > 0)
				MoveToEx(hdc, i, (int)y, NULL);
			LineTo(hdc, i, (int)y);
			y_prev = y;
		}
	}

	y_prev = 0.0;
	MoveToEx(hdc, 0, 0, NULL);
	for (i = 0; i > -(xView / 2 - 40); i--)
	{
		y = (int)(tan((double)i / scale) * scale);
		if (y > -(yView / 2 - 120) && y < yView / 2 - 120)
		{
			if (y > 0 && y_prev < 0)
				MoveToEx(hdc, i, (int)y, NULL);
			LineTo(hdc, i, (int)y);
			y_prev = y;
		}
	}

	// Делаем перо снова черным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// Наносим деления
	MoveToEx(hdc, 0, 0, NULL);
	for (i = yView / 2 - 120; i > -(yView / 2 - 120); i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf_s(Buf, L"%4.2f", (float)i / scale);
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
