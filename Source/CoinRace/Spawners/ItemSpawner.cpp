#include "Spawners/ItemSpawner.h"

#include "Components/ChildActorComponent.h"
#include "Items/ItemBase.h"

AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// ItemPreviewComp 생성
	ItemPreviewComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("PreviewComponent"));
	SetRootComponent(ItemPreviewComp);
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	// 아이템 미리보기 제거
	ItemPreviewComp->SetChildActorClass(nullptr);

	// 서버에서만 아이템 소환
	if (HasAuthority() == true)
	{
		SpawnItem();
	}
}

void AItemSpawner::SpawnItem()
{
	// 클라이언트에서 아이템 소환 방지
	if (HasAuthority() == false)
	{
		return;
	}

	// ItemClass 검증
	if (IsValid(ItemClass) == false)
	{
		return;
	}

	// 아이템 지연 소환
	SpawnedItem = GetWorld()->SpawnActorDeferred<AItemBase>(
		ItemClass,
		GetActorTransform(),
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (IsValid(SpawnedItem) == false)
	{
		return;
	}

	// OnDestroyed 델리게이트 바인딩
	SpawnedItem->OnDestroyed.AddDynamic(this, &ThisClass::OnItemDestroyed);

	// 소환 완료
	SpawnedItem->FinishSpawning(GetActorTransform());
}

void AItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	// 클라이언트에서 처리 방지
	if (HasAuthority() == false)
	{
		return;
	}

	// DestroyedActor 검증
	if (IsValid(DestroyedActor) == false)
	{
		return;
	}

	// 아이템 캐시 제거
	SpawnedItem = nullptr;

	// 아이템 소환 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ThisClass::SpawnItem,
		SpawnInterval,
		false);
}
