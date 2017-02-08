#ifndef _CURVE_FACTORY_
#define _CURVE_FACTORY_

#include "globals.h"
#include "PARABOLA.h"

//class CURVE;

class FACTORY
{
public:
	// Использовал упрощенный (нестандартный) вариант фабрики
	CURVE* createCurve(CURVE_ID curve_id);
	~FACTORY();
};

#endif