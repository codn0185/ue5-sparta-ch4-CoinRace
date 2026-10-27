#include "StealCoinSkillComponent.h"

#include "Interfaces/AttackableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Players/CRPlayerState.h"
#include "Sound/SoundBase.h"

void UStealCoinSkillComponent::ActivateSkill()
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

	// 라인 트레이스의 시작 & 종료 위치
	const FVector StartLocation = Owner->GetActorLocation() + 50 * Owner->GetActorUpVector();
	const FVector EndLocation = StartLocation + Owner->GetActorForwardVector() * 700.f;

	// 콜리전 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Owner); // Owner 무시

	// 충돌 결과
	FHitResult HitResult;

	// 라인 트레이스 실행
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams);

	// 피격 여부 확인
	if (bHit == false)
	{
		return;
	}

	// Pawn 검증
	APawn* Pawn = GetOwner<APawn>();
	if (IsValid(Pawn) == false)
	{
		return;
	}

	// CRPlayerState 검증
	ACRPlayerState* CRPlayerState = Pawn->GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPlayerState) == false)
	{
		return;
	}

	// 공격 가능한 액터인지 확인
	if (IAttackableInterface* Attackable = Cast<IAttackableInterface>(HitResult.GetActor()))
	{
		// 코인 빼앗기
		const int32 StolenCoins = Attackable->StealCoins(StealAmount);

		// 피격 이펙트 재생
		MulticastRPCPlayEffect(HitResult.ImpactPoint, HitResult.ImpactNormal);

		// 빼앗은 코인 추가
		if (StolenCoins > 0)
		{
			CRPlayerState->AddCoins(StolenCoins);
		}

		// 피격 부위 확인
		EHitPart HitPart;
		if (Attackable->GetHitBody(HitResult, HitPart) == true)
		{
			// 바퀴
			if (HitPart == EHitPart::Wheel)
			{
				Attackable->ApplyBonusMaxSpeedPenalty(BonusMaxSpeedPenalty);
			}
		}
	}
}

void UStealCoinSkillComponent::MulticastRPCPlayEffect_Implementation(const FVector& HitLocation, const FVector& HitNormal)
{
	AActor* Owner = GetOwner();
	if (IsValid(Owner) == false ||
		Owner->HasAuthority() == true) // 클라이언트에서만 이펙트 재생
	{
		return;
	}

	// 파티클 재생
	if (IsValid(ActivateParticle) == true)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ActivateParticle,
			HitLocation,
			HitNormal.Rotation());
	}

	// 사운드 재생
	if (IsValid(ActivateSound) == true)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ActivateSound,
			HitLocation);
	}
}
