#include "CRHUD.h"

#include "CRVehicleStatusWidget.h"

#include "Components/TextBlock.h"

void UCRHUD::UpdateSpeed(float NewSpeed)
{
	VehicleStatusWidget->UpdateSpeed(NewSpeed);
}

void UCRHUD::UpdateGear(int32 NewGear)
{
	VehicleStatusWidget->UpdateGear(NewGear);
}
