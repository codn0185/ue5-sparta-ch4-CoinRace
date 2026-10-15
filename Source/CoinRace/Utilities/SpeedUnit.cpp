#include "Utilities/SpeedUnit.h"

float FSpeedUnit::ConvertSpeed(float Speed) const
{
	return Speed * ConversionMultiplier;
}
