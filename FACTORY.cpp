#include "FACTORY.h"

FACTORY::~FACTORY() {}

CURVE* FACTORY::createCurve(CURVE_ID curve_id)
{
	CURVE* curve = NULL;
	switch (curve_id)
	{
	case PARABOLA_ID:
		return new PARABOLA();
		break;
	default:
		break;
	}
	return curve;
}