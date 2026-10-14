#include "CRRearWheel.h"

UCRRearWheel::UCRRearWheel()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}
