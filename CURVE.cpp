#include "CURVE.h"

CURVE::CURVE() {}

CURVE::CURVE(CURVE&) {}

CURVE::~CURVE() {}

BOOL CURVE::Line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL); // сделать текущими координаты x1, y1
	return LineTo(hdc, x2, y2); // нарисовать линию
}

void CURVE::plot(HDC hdc, int xView, int yView) {}
