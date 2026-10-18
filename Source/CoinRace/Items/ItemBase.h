#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Interfaces/ItemEffectReceiverInterface.h"

#include "ItemBase.generated.h"

class UStaticMesh;
class USphereComponent;

UCLASS(Abstract)
class COINRACE_API AItemBase : public AActor
{
	GENERATED_BODY()

  protected:
	// 아이템 스태틱 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;

	// 아이템 획득 가능한 범위 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USphereComponent> SphereComp;

  protected:
	// 아이템 획득 Cascade 파티클
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	TObjectPtr<UParticleSystem> PickupParticle;

	// 아이템 획득 Niagara 시스템
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	TObjectPtr<class UNiagaraSystem> PickupNiagara;

	// 아이템 획득 사운드
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Effects")
	TObjectPtr<USoundBase> PickupSound;

  public:
	AItemBase();

  protected:
	virtual void BeginPlay() override;

  protected:
	// 아이템 효과 적용 (성공 여부 반환)
	virtual bool Apply(IItemEffectReceiverInterface* Target)
		PURE_VIRTUAL(ThisClass::Apply, return false;); // 순수가상함수

	// 모든 클라이언트에 아이템 획득 이펙트 재생
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCPlayPickupEffect();

  protected:
	// 오버랩 이벤트
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
