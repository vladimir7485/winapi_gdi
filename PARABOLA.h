#ifndef _PARABOLA_
#define _PARABOLA_

#include "CURVE.h"

class PARABOLA : public CURVE
{
public:
	void plot(HDC hdc, int xView, int yView);
};


#endif