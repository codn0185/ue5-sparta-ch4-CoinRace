#include "CRPlayerController.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Frameworks/CRGameMode.h"
#include "Frameworks/CRGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Players/CRPlayerState.h"
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

void ACRPlayerController::ClientRPCReciveChatMessage_Implementation(const FChatData& ChatData)
{
	// 채팅 UI에 채팅 메시지 추가
	VehicleUI->AddChatMessage(ChatData);

	// GameState의 채팅 로그에 추가
	ACRGameState* CRGS = GetWorld()->GetGameState<ACRGameState>();
	if (IsValid(CRGS) == true)
	{
		CRGS->AddChatData(ChatData);
	}
}

void ACRPlayerController::ClientRPCSyncChatLog_Implementation(const TArray<FChatData>& ChatLog)
{
	// 채팅 UI에 채팅 로그 동기화
	for (const FChatData& ChatData : ChatLog)
	{
		VehicleUI->AddChatMessage(ChatData);
	}

	// CRGameMode의 채팅 로그 동기화
	ACRGameState* CRGS = GetWorld()->GetGameState<ACRGameState>();
	if (IsValid(CRGS) == true)
	{
		CRGS->SetChatLog(ChatLog);
	}
}

void ACRPlayerController::ServerRPCAddCoins_Implementation(const int32 Amount)
{
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	CRPS->AddCoins(Amount);
}

void ACRPlayerController::ServerRPCRemoveCoins_Implementation(const int32 Amount)
{
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	CRPS->TryRemoveCoins(Amount);
}

void ACRPlayerController::ServerRPCAddBonusMaxSpeed_Implementation(const float Amount)
{
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	CRPS->AddBonusMaxSpeed(Amount);
}

void ACRPlayerController::ServerRPCOnCoinsUpdated_Implementation(const int32 NewCoins)
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

	CRGameMode->UpdateCoinLeaderboard(this, NewCoins);
}

void ACRPlayerController::ClientRPCUpdateCoinLeaderboard_Implementation(const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
{
	VehicleUI->UpdateCoinLeaderboard(CoinLeaderboard);
}

void ACRPlayerController::ServerRPCNotifyPlayerInitialized_Implementation()
{
	// GameMode에 플레이어 초기화 완료 알림
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

	CRGameMode->OnPlayerInitialized(this);
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

	// 플레이어 준비됨을 서버에 알림
	ServerRPCNotifyPlayerInitialized();
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

void ACRPlayerController::OnRep_PlayerState()
{
	ACRPlayerState* CRPS = GetPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == false)
	{
		return;
	}

	// PlayerState의 델리게이트 바인딩
	TWeakObjectPtr<ACRPlayerController> WeakThis(this);

	CRPS->OnCoinsChanged.AddLambda(
		[WeakThis](int32 NewCoins)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->VehicleUI->SetCoins(NewCoins);
				WeakThis->ServerRPCOnCoinsUpdated(NewCoins);
			}
		});

	CRPS->OnBonusMaxSpeedChanged.AddLambda(
		[WeakThis](float NewBonusMaxSpeed)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->VehicleUI->SetBonusMaxSpeed(NewBonusMaxSpeed);
			}
		});
}

void ACRPlayerController::FocusChatInput()
{
	VehicleUI->FocusChatInput();
}
