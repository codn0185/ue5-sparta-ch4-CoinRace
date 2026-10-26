#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "AttackableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAttackableInterface : public UInterface
{
	GENERATED_BODY()
};

class COINRACE_API IAttackableInterface
{
	GENERATED_BODY()

  public:
	// 코인을 빼앗기고, 실제로 빼앗긴 수량을 반환
	virtual int32 StealCoins(int32 Amount) = 0;

	// 추가 최대 속도 감소 패널티 적용
	virtual void ApplyBonusMaxSpeedPenalty(float Amount) = 0;

	// 피격 이펙트 재생
	virtual void PlayHitEffect(const FVector& HitLocation, const FVector& HitNormal) = 0;
};
