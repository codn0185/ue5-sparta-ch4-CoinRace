#pragma once

#include "CoreMinimal.h"

#include "Items/ItemBase.h"

#include "CoinItem.generated.h"

UCLASS()
class COINRACE_API ACoinItem : public AItemBase
{
	GENERATED_BODY()

  protected:
	// 획득할 코인 수량
	UPROPERTY(EditAnywhere, Category = "Item|Properties")
	int32 Coins = 10;

  protected:
	// 아이템 효과 적용
	virtual void Apply(IItemEffectReceiverInterface* Target) override;
};
