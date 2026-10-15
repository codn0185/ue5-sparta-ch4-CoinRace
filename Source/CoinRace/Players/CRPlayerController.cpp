#include "CRPlayerController.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Frameworks/CRGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/CRHUD.h"
#include "Vehicles/CRWheeledVehiclePawn.h"

void ACRPlayerController::ServerRPCSendChatMessage_Implementation(const FString& ChatMessage)
{
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (IsValid(GameMode) == false)
	{
		return;
	}

	ACRGameMode* CRGameMode = Cast<ACRGameMode>(GameMode);
	if (IsValid(CRGameMode) == false)
	{
		return;
	}

	CRGameMode->BroadcastChatMessage(this, ChatMessage);
}

void ACRPlayerController::ClientRPCReciveChatMessge_Implementation(const FCRChatData& ChatData)
{
	VehicleUI->AddChatMessage(ChatData);
}

void ACRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	// spawn the UI widget and add it to the viewport
	VehicleUI = CreateWidget<UCRHUD>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();

	// 채팅 입력 커밋 델리게이트 바인딩
	VehicleUI->OnChatMessageSubmitted.BindUObject(this, &ThisClass::ServerRPCSendChatMessage);
}

void ACRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		// optionally add the steering wheel context
		if (bUseSteeringWheelControls && SteeringWheelInputMappingContext)
		{
			Subsystem->AddMappingContext(SteeringWheelInputMappingContext, 1);
		}
	}
}

void ACRPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, VehiclePawn);
}

void ACRPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
	}
}

void ACRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<ACRWheeledVehiclePawn>(InPawn);
}

void ACRPlayerController::FocusChatInput()
{
	VehicleUI->FocusChatInput();
}
