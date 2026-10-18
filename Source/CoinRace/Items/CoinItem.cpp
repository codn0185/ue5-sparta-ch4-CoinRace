#include "CoinItem.h"

void ACoinItem::Apply(IItemEffectReceiverInterface* Target)
{
	if (Target == nullptr)
	{
		return;
	}

	// 코인 획득
	Target->AddCoins(Coins);

	// 효과 재생
	MulticastRPCPlayPickupEffect();

	// 아이템 제거
	Destroy();
}
