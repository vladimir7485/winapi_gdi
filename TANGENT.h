#ifndef _TANGENT_
#define _TANGENT_

#include "CURVE.h"

class TANGENT : public CURVE
{
public:
	void plot(HDC hdc, int xView, int yView);
};


#endif