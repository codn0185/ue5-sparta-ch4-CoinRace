#include "MineItem.h"

bool AMineItem::Apply(IItemEffectReceiverInterface* Target)
{
	if (Target == nullptr)
	{
		return false;
	}

	// 전진 가속 비활성화
	Target->DisableThrottle(Duration);

	return true;
}
