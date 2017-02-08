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
	case GYPERBOLA_ID:
		return new GYPERBOLA();
		break;
	case SINUS_ID:
		return new SINUS();
		break;
	case TANGENT_ID:
		return new TANGENT();
		break;
	default:
		break;
	}
	return curve;
}