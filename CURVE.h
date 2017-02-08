#ifndef _CURVE_
#define _CURVE_

#include "globals.h"

class CURVE
{
protected:
	BOOL Line(HDC hdc, int x1, int y1, int x2, int y2);
public:
	CURVE();
	CURVE(CURVE&);

	virtual ~CURVE();

	virtual void plot(HDC hdc, int xView, int yView);
};

#endif