#include "MineItem.h"

bool AMineItem::Apply(IItemEffectReceiverInterface* Target)
{
	if (Target == nullptr)
	{
		return false;
	}

	// Mine 설치자에게는 작동하지 않음
	AActor* TargetActor = Cast<AActor>(Target);
	if (TargetActor == GetOwner())
	{
		return false;
	}

	// 전진 가속 비활성화
	Target->DisableThrottle(Duration);

	return true;
}
