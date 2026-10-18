#include "ItemBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 레플리케이션 설정
	bReplicates = true;
	SetNetUpdateFrequency(1.f); // 초당 레플리케이션 횟수
	SetMinNetUpdateFrequency(1.f);

	// StaticMeshComp 생성
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	SetRootComponent(StaticMeshComp);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 콜리전 제거

	// SphereComp 생성
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(StaticMeshComp);
	SphereComp->SetSphereRadius(100.f);

	// SphereComp 콜리전 설정
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	// 서버에서 오버랩이 발생할 때만 이벤트 호출
	if (HasAuthority() == true)
	{
		// SphereComp 오버랩 이벤트 설정
		SphereComp->SetGenerateOverlapEvents(true);
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	}
}

void AItemBase::MulticastRPCPlayPickupEffect_Implementation()
{
	// 클라이언트에서만 재생
	if (HasAuthority() == true)
	{
		return;
	}

	UWorld* World = GetWorld();

	// Cascade 파티클 재생
	if (IsValid(PickupParticle) == true)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			World,
			PickupParticle,
			GetActorLocation());
	}

	// Niagara 시스템 재생
	if (IsValid(PickupNiagara) == true)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			World,
			PickupNiagara,
			GetActorLocation(),
			FRotator::ZeroRotator);
	}

	// 사운드 재생
	if (IsValid(PickupSound) == true)
	{
		UGameplayStatics::PlaySoundAtLocation(
			World,
			PickupSound,
			GetActorLocation());
	}
}

void AItemBase::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 서버에서만 처리
	if (HasAuthority() == false)
	{
		return;
	}

	// 아이템 효과 수신 인터페이스 확인
	if (IItemEffectReceiverInterface* Target = Cast<IItemEffectReceiverInterface>(Other))
	{
		// 아이템 효과 적용
		bool bSuccess = Apply(Target);

		// 획득 성공 후 처리
		if (bSuccess == true)
		{
			// 효과 재생
			MulticastRPCPlayPickupEffect();

			// 아이템 제거
			Destroy();
		}
	}
}
