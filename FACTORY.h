#ifndef _CURVE_FACTORY_
#define _CURVE_FACTORY_

#include "globals.h"
#include "PARABOLA.h"
#include "GYPERBOLA.h"
#include "SINUS.h"
#include "TANGENT.h"

//class CURVE;

class FACTORY
{
public:
	// ����������� ���������� (�������������) ������� �������
	CURVE* createCurve(CURVE_ID curve_id);
	~FACTORY();
};

#endif