#include "CRFrontWheel.h"

UCRFrontWheel::UCRFrontWheel()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}
