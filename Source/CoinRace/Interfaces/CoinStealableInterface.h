#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "CoinStealableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCoinStealableInterface : public UInterface
{
	GENERATED_BODY()
};

class COINRACE_API ICoinStealableInterface
{
	GENERATED_BODY()

  public:
	// 코인을 빼앗기고, 실제로 빼앗긴 수량을 반환
	virtual int32 StealCoins(int32 Amount) = 0;

	// 코인을 빼앗길 때 이펙트 재생
	virtual void PlayCoinStolenEffect(const FVector& HitLocation, const FVector& HitNormal) = 0;
};
