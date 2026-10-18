#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ItemSpawner.generated.h"

class UChildActorComponent;
class AItemBase;

UCLASS()
class COINRACE_API AItemSpawner : public AActor
{
	GENERATED_BODY()

  protected:
	// 아이템 미리보기
	UPROPERTY(VisibleAnywhere, Category = Components)
	TObjectPtr<UChildActorComponent> ItemPreviewComp;

  protected:
	// 소환할 아이템 클래스
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AItemBase> ItemClass;

	// 소환된 아이템 액터
	TObjectPtr<AItemBase> SpawnedItem;

	// 소환 간격
	UPROPERTY(EditAnywhere, Category = "Item", meta = (ClampMin = "0", UIMin = "0"))
	float SpawnInterval = 2.f;

	// 소환 타이머
	FTimerHandle SpawnTimerHandle;

  public:
	AItemSpawner();

  protected:
	virtual void BeginPlay() override;

  protected:
	// 아이템 소환
	void SpawnItem();

	// 제거 시 호출되는 콜백 메서드
	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedActor);
};
