#include "CRPlayerController.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Data/ChatEntry.h"
#include "Frameworks/CRGameMode.h"
#include "Frameworks/CRGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Players/CRPlayerState.h"
#include "Systems/GameEventSubsystem.h"
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

void ACRPlayerController::ClientRPCReciveChatMessage_Implementation(const FChatEntry& ChatData)
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

void ACRPlayerController::ClientRPCSyncChatLog_Implementation(const TArray<FChatEntry>& ChatLog)
{
	// 채팅 UI에 채팅 로그 동기화
	for (const FChatEntry& ChatData : ChatLog)
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

	// GameEventSubsystem 델리게이트 바인딩
	BindGameEventDelegates();

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

void ACRPlayerController::FocusChatInput()
{
	VehicleUI->FocusChatInput();
}

void ACRPlayerController::BindGameEventDelegates()
{
	// this 약한 참조
	TWeakObjectPtr<ACRPlayerController> WeakThis(this);

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == false)
	{
		return;
	}

	UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
	if (IsValid(GameEventSubsystem) == false)
	{
		return;
	}

	// OnCoinsUpdated 바인딩
	GameEventSubsystem->OnCoinsUpdated.AddLambda(
		[WeakThis](const ACRPlayerState* CRPS, int32 NewCoins)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->VehicleUI->SetCoins(NewCoins);
			}
		});
	// OnBonusMaxSpeedUpdated 바인딩
	GameEventSubsystem->OnBonusMaxSpeedUpdated.AddLambda(
		[WeakThis](const ACRPlayerState* CRPS, float NewBonusMaxSpeed)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->VehicleUI->SetBonusMaxSpeed(NewBonusMaxSpeed);
			}
		});
	// OnCoinLeaderboardUpdated 바인딩
	GameEventSubsystem->OnCoinLeaderboardUpdated.AddLambda(
		[WeakThis](const TArray<FCoinLeaderboardEntry>& CoinLeaderboard)
		{
			if (WeakThis.IsValid())
			{
				WeakThis->VehicleUI->UpdateCoinLeaderboard(CoinLeaderboard);
			}
		});
}
