#include "GYPERBOLA.h"

void GYPERBOLA::plot(HDC hdc, int xView, int yView)
{
	HPEN hPen = NULL;
	int i, y;
	TCHAR Buf[10];

	double k = 10000.0;

	// Рисуем оси координат
	Line(hdc, 0, yView / 2 - 120, 0, -(yView / 2 - 120)); // ось Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ось X
	MoveToEx(hdc, 0, 0, NULL); // перемещаемся в начало координат

							   // Создание красного пера
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	bool flag = false;

	// Гипербола
	for (i = 1; i < xView / 2 - 40; i++)
	{
		y = (int)(k / i);
		if (y < (yView / 2 - 120))
		{
			if (!flag)
			{
				flag = true;
				MoveToEx(hdc, i, (int)y, NULL);
			}
			LineTo(hdc, i, (int)y);
		}
	}

	flag = false;
	for (i = -1; i > -(xView / 2 - 40); i--)
	{
		y = (int)(k / i);
		if (y > -(yView / 2 - 120))
		{
			if (!flag)
			{
				flag = true;
				MoveToEx(hdc, i, (int)y, NULL);
			}
			LineTo(hdc, i, (int)y);
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
		_stprintf_s(Buf, L"%4.2f", (float)i);
		TextOut(hdc, -5, i, Buf, (int)_ftcslen(Buf));
	}
	for (i = -(xView / 2 - 40) / 90 * 90; i < (xView / 2 - 40) / 90 * 90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf_s(Buf, L"%4.2f", (float)i);
		TextOut(hdc, i - 5, -5, Buf, (int)_ftcslen(Buf));
	}

	DeleteObject(hPen);
}
