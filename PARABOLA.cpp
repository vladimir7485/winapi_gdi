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

	// ������ ��� ���������
	Line(hdc, 0, (yView / 2 - 120) * 2 - 20, 0, -20); // ��� Y
	Line(hdc, -(xView / 2 - 40), 0, xView / 2 - 40, 0); // ��� X
	MoveToEx(hdc, 0, 0, NULL); // ������������ � ������ ���������

							   // �������� �������� ����
	hPen = CreatePen(1, 4, RGB(255, 25, 0));
	SelectObject(hdc, hPen);

	// ��������
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

	// ������ ���� ����� ������
	hPen = CreatePen(1, 1, RGB(0, 0, 0));
	SelectObject(hdc, hPen);

	// ������� �������
	MoveToEx(hdc, 0, 0, NULL);
	// �� Y
	for (i = (yView / 2 - 120) * 2 - 30; i > -10; i -= 50)
	{
		Line(hdc, -3, i, 3, i);
		_stprintf_s(Buf, L"%4.2f", (float)i);
		TextOut(hdc, -5, i, Buf, (int)_ftcslen(Buf));
	}
	// �� X
	for (i = -(xView / 2 - 40) / 90 * 90; i < (xView / 2 - 40) / 90 * 90; i += 90)
	{
		Line(hdc, i, 3, i, -3);
		_stprintf_s(Buf, L"%4.2f", (float)i / scale);
		TextOut(hdc, i - 5, -5, Buf, (int)_ftcslen(Buf));
	}

	DeleteObject(hPen);

	SetViewportOrgEx(hdc, pt.x, pt.y, NULL);
}
