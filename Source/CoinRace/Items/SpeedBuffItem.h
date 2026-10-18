#pragma once

#include "CoreMinimal.h"

#include "Items/ItemBase.h"

#include "SpeedBuffItem.generated.h"

UCLASS()
class COINRACE_API ASpeedBuffItem : public AItemBase
{
	GENERATED_BODY()

  protected:
	// 증가할 최대 속도
	UPROPERTY(EditAnywhere, Category = "Item|Properties", meta = (ClampMin = "0", UIMin = "0"))
	float BonusMaxSpeed = 10.f;

  protected:
	// 아이템 효과 적용
	virtual void Apply(IItemEffectReceiverInterface* Target) override;
};
