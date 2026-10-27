#include "CRWheeledVehiclePawn.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Players/CRPlayerController.h"
#include "Players/CRPlayerState.h"
#include "Players/Components/DeployMineSkillComponent.h"
#include "Players/Components/StealCoinSkillComponent.h"

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

	// 스킬 컴포넌트
	StealCoinSkillComponent = CreateDefaultSubobject<UStealCoinSkillComponent>(TEXT("StealCoin Skill"));
	DeployMineSkillComponent = CreateDefaultSubobject<UDeployMineSkillComponent>(TEXT("DeployMine Skill"));
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

		// focus chat input
		EnhancedInputComponent->BindAction(FocusChatAction, ETriggerEvent::Started, this, &ThisClass::FocusChatInput);

		// attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AttackInput);

		// deploy mine
		EnhancedInputComponent->BindAction(DeployMineAction, ETriggerEvent::Started, this, &ThisClass::DeployMine);
	}
	else
	{
		UE_LOG(LogTemplateVehicle, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACRWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CRPlayerState = GetPlayerState<ACRPlayerState>();
}

void ACRWheeledVehiclePawn::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	CRPlayerState = GetPlayerState<ACRPlayerState>();
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
	// 코인 스틸 스킬 사용
	StealCoinSkillComponent->ServerRPCUse();
}

void ACRWheeledVehiclePawn::DeployMine(const FInputActionValue& Value)
{
	// 지뢰 배치 스킬 사용
	DeployMineSkillComponent->ServerRPCUse();
}

bool ACRWheeledVehiclePawn::CanThrottle() const
{
	// CRPlayerState 검증
	if (IsValid(CRPlayerState) == false)
	{
		return true;
	}

	return CRPlayerState->CanThrottle();
}

int32 ACRWheeledVehiclePawn::StealCoins(int32 Amount)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return 0;
	}

	// CRPlayerState 검증
	if (IsValid(CRPlayerState) == false)
	{
		return 0;
	}

	// 코인 감소
	const bool bSuccess = CRPlayerState->TryRemoveCoins(Amount);

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

	// CRPlayerState 검증
	if (IsValid(CRPlayerState) == false)
	{
		return;
	}

	// 추가 최대 속도 감소
	CRPlayerState->ReduceBonusMaxSpeed(Amount);
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

	// CRPlayerState 검증
	if (IsValid(CRPlayerState) == false)
	{
		return;
	}

	// 코인 추가
	CRPlayerState->AddCoins(Amount);
}

void ACRWheeledVehiclePawn::AddBonusMaxSpeed(float Amount)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// CRPlayerState 검증
	if (IsValid(CRPlayerState) == false)
	{
		return;
	}

	// 추가 최대 속도 증가
	CRPlayerState->AddBonusMaxSpeed(Amount);
}

void ACRWheeledVehiclePawn::DisableThrottle(float Duration)
{
	// 서버 권한 검증
	if (HasAuthority() == false)
	{
		return;
	}

	// CRPlayerState 검증
	if (IsValid(CRPlayerState) == false)
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
	CRPlayerState->SetCanThrottle(false);

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
					// CRPlayerState 검증
					if (IsValid(CRPlayerState) == false)
					{
						return;
					}

					// 전진 가속 활성화
					CRPlayerState->SetCanThrottle(true);
				}),
			Duration,
			false);
	}
}

#undef LOCTEXT_NAMESPACE
