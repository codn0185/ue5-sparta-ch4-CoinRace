#include "CRWheeledVehiclePawn.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Players/CRPlayerController.h"
#include "Players/CRPlayerState.h"
#include "Sound/SoundBase.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

DEFINE_LOG_CATEGORY(LogTemplateVehicle);

ACRWheeledVehiclePawn::ACRWheeledVehiclePawn()
{
	// construct the front camera boom
	FrontSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Front Spring Arm"));
	FrontSpringArm->SetupAttachment(GetMesh());
	FrontSpringArm->TargetArmLength = 0.0f;
	FrontSpringArm->bDoCollisionTest = false;
	FrontSpringArm->bEnableCameraRotationLag = true;
	FrontSpringArm->CameraRotationLagSpeed = 15.0f;
	FrontSpringArm->SetRelativeLocation(FVector(30.0f, 0.0f, 120.0f));

	FrontCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Front Camera"));
	FrontCamera->SetupAttachment(FrontSpringArm);
	FrontCamera->bAutoActivate = false;

	// construct the back camera boom
	BackSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	BackSpringArm->SetupAttachment(GetMesh());
	BackSpringArm->TargetArmLength = 650.0f;
	BackSpringArm->SocketOffset.Z = 150.0f;
	BackSpringArm->bDoCollisionTest = false;
	BackSpringArm->bInheritPitch = false;
	BackSpringArm->bInheritRoll = false;
	BackSpringArm->bEnableCameraRotationLag = true;
	BackSpringArm->CameraRotationLagSpeed = 2.0f;
	BackSpringArm->CameraLagMaxDistance = 50.0f;

	BackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	BackCamera->SetupAttachment(BackSpringArm);

	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// get the Chaos Wheeled movement component
	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
}

void ACRWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ThisClass::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &ThisClass::Steering);

		// throttle
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ThisClass::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ThisClass::Throttle);

		// break
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ThisClass::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &ThisClass::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ThisClass::StopBrake);

		// handbrake
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ThisClass::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ThisClass::StopHandbrake);

		// look around
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ThisClass::LookAround);

		// toggle camera
		EnhancedInputComponent->BindAction(ToggleCameraAction, ETriggerEvent::Triggered, this, &ThisClass::ToggleCamera);

		// reset the vehicle
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &ThisClass::ResetVehicle);

		// Focus Chat Input
		EnhancedInputComponent->BindAction(FocusChatAction, ETriggerEvent::Started, this, &ThisClass::FocusChatInput);

		// Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AttackInput);
	}
	else
	{
		UE_LOG(LogTemplateVehicle, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACRWheeledVehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	// realign the camera yaw to face front
	float CameraYaw = BackSpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	BackSpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));
}

void ACRWheeledVehiclePawn::Steering(const FInputActionValue& Value)
{
	// get the input magnitude for steering
	float SteeringValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetSteeringInput(SteeringValue);
}

void ACRWheeledVehiclePawn::Throttle(const FInputActionValue& Value)
{
	const float CurrentThrottle = ChaosVehicleMovement->GetThrottleInput();
	const bool bCanThrottle = CanThrottle();

	GEngine->AddOnScreenDebugMessage(
		1,
		0.f,
		bCanThrottle ? FColor::Green : FColor::Red,
		FString::Printf(
			TEXT("Current Throttle: %.2f | CanThrottle: %s"),
			CurrentThrottle,
			bCanThrottle ? TEXT("True") : TEXT("False")));

	if (CanThrottle() == false)
	{
		ChaosVehicleMovement->SetThrottleInput(0.f);
		return;
	}

	// get the input magnitude for the throttle
	float ThrottleValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
}

void ACRWheeledVehiclePawn::Brake(const FInputActionValue& Value)
{
	// get the input magnitude for the brakes
	float BreakValue = Value.Get<float>();

	// add the input
	ChaosVehicleMovement->SetBrakeInput(BreakValue);
}

void ACRWheeledVehiclePawn::StartBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void ACRWheeledVehiclePawn::StopBrake(const FInputActionValue& Value)
{
	// call the Blueprint hook for the break lights
	BrakeLights(false);

	// reset brake input to zero
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void ACRWheeledVehiclePawn::StartHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(true);

	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void ACRWheeledVehiclePawn::StopHandbrake(const FInputActionValue& Value)
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(false);

	// call the Blueprint hook for the break lights
	BrakeLights(false);
}

void ACRWheeledVehiclePawn::LookAround(const FInputActionValue& Value)
{
	// get the flat angle value for the input
	float LookValue = Value.Get<float>();

	// add the input
	BackSpringArm->AddLocalRotation(FRotator(0.0f, LookValue, 0.0f));
}

void ACRWheeledVehiclePawn::ToggleCamera(const FInputActionValue& Value)
{
	// toggle the active camera flag
	bFrontCameraActive = !bFrontCameraActive;

	FrontCamera->SetActive(bFrontCameraActive);
	BackCamera->SetActive(!bFrontCameraActive);
}

void ACRWheeledVehiclePawn::ResetVehicle(const FInputActionValue& Value)
{
	// reset to a location slightly above our current one
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;

	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

	UE_LOG(LogTemplateVehicle, Error, TEXT("Reset Vehicle"));
}

void ACRWheeledVehiclePawn::FocusChatInput(const FInputActionValue& Value)
{
	ACRPlayerController* PlayerController = GetController<ACRPlayerController>();
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	PlayerController->FocusChatInput();
}

void ACRWheeledVehiclePawn::AttackInput(const FInputActionValue& Value)
{
	// 서버로 공격 입력함을 알림
	ServerRPCAttack();
}

void ACRWheeledVehiclePawn::MulticastRPCPlayHitEffect_Implementation(const FVector& HitLocation, const FVector& HitNormal)
{
	// 클라이언트만 이펙트 재생
	if (HasAuthority() == true)
	{
		return;
	}

	// 파티클 재생
	if (IsValid(HitParticle) == true)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			HitLocation,
			HitNormal.Rotation());
	}
	else
	{
		// 충돌 위치 표시
		DrawDebugSphere(
			GetWorld(),
			HitLocation,
			15.0f,
			12,
			FColor::Red,
			false,
			3.0f,
			0,
			2.0f);

		// 충돌 노멀 방향 표시
		DrawDebugDirectionalArrow(
			GetWorld(),
			HitLocation,
			HitLocation + HitNormal * 100.0f,
			20.0f,
			FColor::Yellow,
			false,
			3.0f,
			0,
			2.0f);
	}

	// 사운드 재생
	if (IsValid(HitSound) == true)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			HitSound,
			HitLocation);
	}
}

bool ACRWheeledVehiclePawn::CanThrottle() const
{
	// ACRPlayerState 검증
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return true;
	}

	return CRPS->CanThrottle();
}

int32 ACRWheeledVehiclePawn::StealCoins(int32 Amount)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return 0;
	}

	// ACRPlayerState 검증
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return 0;
	}

	// 코인 감소
	const bool bSuccess = CRPS->TryRemoveCoins(Amount);

	// 빼앗긴 코인 반환
	return bSuccess
			   ? Amount
			   : 0;
}

void ACRWheeledVehiclePawn::ApplyBonusMaxSpeedPenalty(float Amount)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// ACRPlayerState 검증
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	// 추가 최대 속도 감소
	CRPS->ReduceBonusMaxSpeed(Amount);
}

void ACRWheeledVehiclePawn::PlayHitEffect(const FVector& HitLocation, const FVector& HitNormal)
{
	MulticastRPCPlayHitEffect(HitLocation, HitNormal);
}

bool ACRWheeledVehiclePawn::GetHitBody(const FHitResult& HitResult, EHitPart& OutHitPart) const
{
	OutHitPart = EHitPart::None;

	// Bone 이름
	const FName BoneName = HitResult.BoneName;

	// Wheel
	if (HitResult.BoneName.ToString().Contains(TEXT("Wheel")))
	{
		OutHitPart = EHitPart::Wheel;
		return true;
	}

	// Body
	if (IsValid(HitResult.GetComponent()))
	{
		OutHitPart = EHitPart::Body;
		return true;
	}

	return false;
}

void ACRWheeledVehiclePawn::AddCoins(int32 Amount)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// ACRPlayerState 검증
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	// 코인 추가
	CRPS->AddCoins(Amount);
}

void ACRWheeledVehiclePawn::AddBonusMaxSpeed(float Amount)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// ACRPlayerState 검증
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	// 추가 최대 속도 증가
	CRPS->AddBonusMaxSpeed(Amount);
}

void ACRWheeledVehiclePawn::DisableThrottle(float Duration)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// ACRPlayerState 검증
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	if (Duration <= 0.f)
	{
		return;
	}

	// 기존 가속 입력 초기화
	ChaosVehicleMovement->SetThrottleInput(0.f);

	// 전진 가속 비활성화
	CRPS->SetCanThrottle(false);

	// 남은 비활성화 시간
	const float RemainingTime = GetWorldTimerManager().GetTimerRemaining(DisableThrottleTimerHandle);

	// 기존 남은 시간보다 긴 경우 타이머 갱신
	if (Duration > RemainingTime)
	{
		GetWorldTimerManager().SetTimer(
			DisableThrottleTimerHandle,
			FTimerDelegate::CreateLambda(
				[this]()
				{
					// ACRPlayerState 검증
					ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
					if (IsValid(CRPS) == false)
					{
						return;
					}

					// 전진 가속 활성화
					CRPS->SetCanThrottle(true);
				}),
			Duration,
			false);
	}
}

void ACRWheeledVehiclePawn::ServerRPCAttack_Implementation()
{
	UWorld* World = GetWorld();

	// 공격 가능 여부 확인
	if (World->GetTimerManager().IsTimerActive(AttackCooldownTimerHandle) == true) // 공격 쿨다운
	{
		return;
	}

	// 공격 쿨다운 활성화
	World->GetTimerManager().SetTimer(
		AttackCooldownTimerHandle,
		AttackCooldown,
		false);

	// 시작 & 종료 위치
	const FVector StartLocation = GetActorLocation() + 50 * GetActorUpVector();
	const FVector EndLocation = StartLocation + GetActorForwardVector() * 700.f;

	// 콜리전 설정
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신 무시

	// 충돌 결과
	FHitResult HitResult;

	// 라인 트레이스 실행
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility,
		QueryParams);

	// 디버그 라인 그리기
	DrawDebugLine(
		World,
		StartLocation,
		EndLocation,
		bHit ? FColor::Green : FColor::Red, // 충돌 성공: 빨간색 / 충돌 실패: 초록색
		false,
		2.0f,
		0,
		2.0f);

	// 공격 성공
	if (bHit == true)
	{
		// ACRPlayerState 검증
		ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
		if (IsValid(CRPS) == false)
		{
			return;
		}

		if (IAttackableInterface* Attackable = Cast<IAttackableInterface>(HitResult.GetActor()))
		{
			// 코인 빼앗기
			const int32 StolenCoins = Attackable->StealCoins(5);

			// 피격 이펙트 재생
			Attackable->PlayHitEffect(HitResult.ImpactPoint, HitResult.ImpactNormal);

			// 빼앗은 코인 추가
			if (StolenCoins > 0)
			{
				CRPS->AddCoins(StolenCoins);
			}

			// 피격 부위 확인
			EHitPart HitPart;
			if (GetHitBody(HitResult, HitPart) == true)
			{
				// 바퀴
				if (HitPart == EHitPart::Wheel)
				{
					Attackable->ApplyBonusMaxSpeedPenalty(5.f);
				}
			}

			UE_LOG(LogTemplateVehicle, Warning, TEXT("Hit Part: %s"), *UEnum::GetValueAsString(HitPart));
		}
	}
}

#undef LOCTEXT_NAMESPACE
