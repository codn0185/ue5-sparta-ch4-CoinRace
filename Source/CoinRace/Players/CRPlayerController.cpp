#include "CRPlayerController.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Data/ChatEntry.h"
#include "Frameworks/CRGameMode.h"
#include "Frameworks/CRGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Systems/GameEventSubsystem.h"
#include "UI/CRGameResultWidget.h"
#include "UI/CRHUD.h"
#include "UI/CRLobbyWidget.h"
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

void ACRPlayerController::HandleGamePhaseUpdated(EGamePhase NewGamePhase)
{
	switch (NewGamePhase)
	{
	case EGamePhase::Waiting:
		LobbyUI->SetVisibility(ESlateVisibility::Visible);
		VehicleUI->SetVisibility(ESlateVisibility::Hidden);
		GameResultUI->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EGamePhase::Playing:
		LobbyUI->SetVisibility(ESlateVisibility::Hidden);
		VehicleUI->SetVisibility(ESlateVisibility::Visible);
		GameResultUI->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EGamePhase::Finished:
		LobbyUI->SetVisibility(ESlateVisibility::Hidden);
		VehicleUI->SetVisibility(ESlateVisibility::Hidden);
		GameResultUI->SetVisibility(ESlateVisibility::Visible);
		break;
	}
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

	// 대기실 UI 위젯
	LobbyUI = CreateWidget<UCRLobbyWidget>(this, LobbyUIClass);
	check(LobbyUI);
	LobbyUI->AddToViewport();

	// 게임 결과 UI 위젯
	GameResultUI = CreateWidget<UCRGameResultWidget>(this, GameResultUIClass);
	check(GameResultUI);
	GameResultUI->AddToViewport();

	// OnGamePhaseUpdated 델리게이트 바인딩
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnGamePhaseUpdated.AddUObject(this, &ThisClass::HandleGamePhaseUpdated);
		}
	}

	HandleGamePhaseUpdated(EGamePhase::Waiting);

	// 플레이어 준비됨을 서버에 알림
	ServerRPCNotifyPlayerInitialized();
}

void ACRPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// OnGamePhaseUpdated 델리게이트 바인딩 해제
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnGamePhaseUpdated.RemoveAll(this);
		}
	}

	Super::EndPlay(EndPlayReason);
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
