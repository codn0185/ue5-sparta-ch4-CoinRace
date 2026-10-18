#include "CoinItem.h"

bool ACoinItem::Apply(IItemEffectReceiverInterface* Target)
{
	if (Target == nullptr)
	{
		return false;
	}

	// 코인 획득
	Target->AddCoins(Coins);

	return true;
}
