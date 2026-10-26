#pragma once

#include "CoreMinimal.h"

#include "Items/ItemBase.h"

#include "MineItem.generated.h"

UCLASS()
class COINRACE_API AMineItem : public AItemBase
{
	GENERATED_BODY()

  protected:
	// 전진 가속 비활성화 기간
	UPROPERTY(EditAnywhere, Category = "Item|Properties", meta = (ClampMin = "0", UIMin = "0"))
	float Duration = 2.f;

  protected:
	// 아이템 효과 적용
	virtual bool Apply(IItemEffectReceiverInterface* Target) override;
};
