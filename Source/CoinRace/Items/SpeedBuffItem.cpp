#include "Items/SpeedBuffItem.h"

bool ASpeedBuffItem::Apply(IItemEffectReceiverInterface* Target)
{
	if (Target == nullptr)
	{
		return false;
	}

	// 코인 획득
	Target->AddBonusMaxSpeed(BonusMaxSpeed);

	return true;
}
