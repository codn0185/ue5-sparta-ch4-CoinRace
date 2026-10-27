#include "DeployMineSkillComponent.h"

#include "Items/MineItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UDeployMineSkillComponent::UDeployMineSkillComponent()
{
	SkillType = ESkillType::DeployMine;
	CooldownDuration = 2.f;
}

void UDeployMineSkillComponent::ActivateSkill()
{
	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		return;
	}

	AActor* Owner = GetOwner();

	if (IsValid(Owner) == false)
	{
		return;
	}

	// 서버 권한 검증
	if (Owner->HasAuthority() == false)
	{
		return;
	}

	// 지뢰 아이템 클래스 검증
	if (IsValid(MineItemClass) == false)
	{
		return;
	}

	// 지면 탐색 시작 위치와 방향
	const FVector StartLocation = Owner->GetActorLocation();
	const FVector EndLocation = StartLocation - FVector(0.f, 0.f, 100.f);

	// 콜리전 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner);

	// 충돌 결과
	FHitResult HitResult;

	// 아래 방향으로 라인 트레이스
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams);

	if (bHit == false)
	{
		return;
	}

	// 소환 Transform
	const FTransform SpawnTransform(
		UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal), // 소환 방향 (지면에 수직)
		HitResult.ImpactPoint);                                   // 소환 위치

	// Mine 지연 소환
	AMineItem* MineItem = World->SpawnActorDeferred<AMineItem>(
		MineItemClass,
		SpawnTransform,
		Owner,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (IsValid(MineItem) == false)
	{
		return;
	}

	// 소환 완료
	MineItem->FinishSpawning(SpawnTransform);
}
