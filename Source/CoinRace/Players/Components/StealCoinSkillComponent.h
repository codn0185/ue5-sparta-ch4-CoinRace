#pragma once

#include "CoreMinimal.h"

#include "Players/Components/SkillComponentBase.h"

#include "StealCoinSkillComponent.generated.h"

class UParticleSystem;
class USoundBase;

UCLASS()
class COINRACE_API UStealCoinSkillComponent : public USkillComponentBase
{
	GENERATED_BODY()

  protected:
	// 사용 파티클
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
	TObjectPtr<UParticleSystem> ActivateParticle;
	// 사용 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects)
	TObjectPtr<USoundBase> ActivateSound;

  protected:
	// 빼앗는 코인 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties)
	int32 StealAmount = 5;

	// 추가 최대 속도 패널티 수치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Properties)
	float BonusMaxSpeedPenalty = 5.f;

  protected:
	virtual void ActivateSkill() override;

  protected:
	// 모든 클라이언트에게 보여야 하는 이펙트 재생
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCPlayEffect(const FVector& HitLocation, const FVector& HitNormal);
};
