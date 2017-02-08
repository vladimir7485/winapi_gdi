#ifndef _GYPERBOLA_
#define _GYPERBOLA_

#include "CURVE.h"

class GYPERBOLA : public CURVE
{
public:
	void plot(HDC hdc, int xView, int yView);
};


#endif