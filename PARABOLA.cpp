#include "PARABOLA.h"

void PARABOLA::plot(HDC hdc, int xView, int yView)
{
	HPEN hPen = NULL;
	int i, y;
	TCHAR Buf[10];

	double scale = 10.0;
	POINT pt;
	GetViewportOrgEx(hdc, &pt);
	SetViewportOrgEx(hdc, (int)(0.5 * xView), (int)(0.8 * yView), NULL);
	MoveToEx(hdc, 0, 0, NULL);

	// Рисуем оси координат
	Line(hdc, 0, (yView / 2 - 120) * 2 - 20, 0, -20); // ось Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

							   // Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	// Парабола
	for (i = 0; i < xView / 2 - 40; i++)
	{
		y = pow((double)i / scale, 2.0);
		if (y < (yView / 2 - 120) * 2 - 20)
			LineTo(hdc, i, (int)y);
	}
	MoveToEx(hdc, 0, 0, NULL);
	for (i = 0; i > -(xView / 2 - 40); i--)
	{
		y = pow((double)i / scale, 2.0);
		if (y < (yView / 2 - 120) * 2 - 20)
			LineTo(hdc, i, (int)y);
	}

	// Делаем перо снова черным
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// Наносим деления
	MoveToEx(hdc, 0, 0, NULL);
	// по Y
	for (i = (yView / 2 - 120) * 2 - 30; i > -10; i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf_s(Buf, L"%4.2f", (float)i);
		TextOut(hdc, -5, i, Buf, (int)_ftcslen(Buf));
	}
	// по X
	for (i = -(xView / 2 - 40) / 90 * 90; i < (xView / 2 - 40) / 90 * 90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf_s(Buf, L"%4.2f", (float)i / scale);
		TextOut(hdc, i - 5, -5, Buf, (int)_ftcslen(Buf));
	}

	DeleteObject(hPen);

	SetViewportOrgEx(hdc, pt.x, pt.y, NULL);
}
