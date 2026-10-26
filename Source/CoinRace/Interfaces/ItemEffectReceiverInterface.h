#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "ItemEffectReceiverInterface.generated.h"

UINTERFACE(MinimalAPI)
class UItemEffectReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class COINRACE_API IItemEffectReceiverInterface
{
	GENERATED_BODY()

  public:
	// 코인 추가
	virtual void AddCoins(int32 Amount) = 0;

	// 추가 최대 속도 증가
	virtual void AddBonusMaxSpeed(float Amount) = 0;

	// 전진 가속 비활성화 (가속 페달 고장)
	virtual void DisableThrottle(float Duration) = 0;
};
