#include "CRGameMode.h"

#include "EngineUtils.h"

#include "Data/ChatEntry.h"
#include "Engine/World.h"
#include "Frameworks/CRGameState.h"
#include "GameFramework/PlayerStart.h"
#include "Players/CRPlayerController.h"
#include "Players/CRPlayerState.h"

ACRGameMode::ACRGameMode()
{
	GameStateClass = ACRGameState::StaticClass();
	PlayerControllerClass = ACRPlayerController::StaticClass();
	PlayerStateClass = ACRPlayerState::StaticClass();
}

void ACRGameMode::OnPostLogin(AController* NewPlayer)
{
	// PlayerController 캐스팅
	ACRPlayerController* PlayerController = Cast<ACRPlayerController>(NewPlayer);
	if (IsValid(PlayerController) == true)
	{
		// 플레이어 추가
		AllPlayerControllers.Add(PlayerController);

		// PlayerState의 이름 설정
		ACRPlayerState* PlayerState = PlayerController->GetPlayerState<ACRPlayerState>();
		if (IsValid(PlayerState) == true)
		{
			FString PlayerName = FString::Printf(TEXT("Player %d"), AllPlayerControllers.Num());
			PlayerState->SetPlayerName(PlayerName);
		}
	}

	Super::OnPostLogin(NewPlayer);
}

void ACRGameMode::Logout(AController* Exiting)
{
	// PlayerController 캐스팅
	ACRPlayerController* PlayerController = Cast<ACRPlayerController>(Exiting);
	if (IsValid(PlayerController) == true)
	{
		// 플레이어 제거
		AllPlayerControllers.Remove(PlayerController);

		// 코인 리더보드에서 제거
		ACRPlayerState* PlayerState = PlayerController->GetPlayerState<ACRPlayerState>();
		if (IsValid(PlayerState) == true)
		{
			ACRGameState* CRGS = GetGameState<ACRGameState>();
			if (IsValid(CRGS) == true)
			{
				const int32 PlayerId = PlayerState->GetPlayerId();

				CRGS->RemovePlayerFromCoinLeaderboard(PlayerId);
			}
		}
	}

	Super::Logout(Exiting);
}

void ACRGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == false)
	{
		return;
	}

	// 대기 카운트다운 시작
	StartWaitingCountdown();
}

void ACRGameMode::StartWaitingCountdown()
{
	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == false)
	{
		return;
	}

	// Waiting 단계 설정
	CRGS->ChangeGamePhase(EGamePhase::Waiting);
	CRGS->SetWaitingTimeRemaining(WaitingTime); // 대기 시간 설정

	// 남은 대기 시간
	const float WaitingTimeRemaining = CRGS->GetWaitingTimeRemaining();

	// 남은 시간이 0 이하면 바로 게임 시작
	if (WaitingTimeRemaining <= 0.f)
	{
		StartGame();
		return;
	}

	// 대기 시간 감소 타이머 설정 - 1초 반복
	GetWorld()->GetTimerManager().SetTimer(
		WaitingTimerHandle,
		this,
		&ThisClass::UpdateWaitingCountdown,
		1.f,
		true);
}

void ACRGameMode::UpdateWaitingCountdown()
{
	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(WaitingTimerHandle);
		return;
	}

	// 남은 대기 시간
	const float WaitingTimeRemaining = CRGS->GetWaitingTimeRemaining();

	// 대기 카운트다운 지속
	if (WaitingTimeRemaining > 0.f)
	{
		// 대기 시간 1초 감소
		CRGS->SetWaitingTimeRemaining(WaitingTimeRemaining - 1.f);
	}
	// 대기 카운트다운 종료
	else
	{
		// 대기 타이머 제거
		GetWorld()->GetTimerManager().ClearTimer(WaitingTimerHandle);

		// 게임 시작
		StartGame();
	}
}

void ACRGameMode::StartGame()
{
	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == false)
	{
		return;
	}

	// Playing 단계 설정
	CRGS->ChangeGamePhase(EGamePhase::Playing);
	CRGS->SetPlayingTimeRemaining(PlayinigTime); // 게임 시간 설정

	// 남은 게임 시간
	const float PlayingTimeRemaining = CRGS->GetPlayingTimeRemaining();

	// 남은 시간이 0 이하면 바로 게임 종료
	if (PlayingTimeRemaining <= 0.f)
	{
		FinishGame();
		return;
	}

	// 남은 게임 시간 감소 타이머 설정 - 1초 반복
	GetWorld()->GetTimerManager().SetTimer(
		PlayingTimerHandle,
		this,
		&ThisClass::UpdatePlayingCountdown,
		1.f,
		true);
}

void ACRGameMode::UpdatePlayingCountdown()
{
	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == false)
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayingTimerHandle);
		return;
	}

	// 남은 게임 시간
	const float PlayingTimeRemaining = CRGS->GetPlayingTimeRemaining();

	// 게임 카운트다운 지속
	if (PlayingTimeRemaining > 0.f)
	{
		// 게임 시간 1초 감소
		CRGS->SetPlayingTimeRemaining(PlayingTimeRemaining - 1.f);
	}
	// 게임 카운트다운 종료
	else
	{
		// 게임 타이머 제거
		GetWorld()->GetTimerManager().ClearTimer(PlayingTimerHandle);

		// 게임 시작
		FinishGame();
	}
}

void ACRGameMode::FinishGame()
{
	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == false)
	{
		return;
	}

	// Finished 단계 설정
	CRGS->ChangeGamePhase(EGamePhase::Finished);

	// TODO : 우승자들 확인 및 설정
}

void ACRGameMode::OnPlayerInitialized(ACRPlayerController* Player)
{
	if (IsValid(Player) == false)
	{
		return;
	}

	// PlayerState 검증
	ACRPlayerState* PlayerState = Player->GetPlayerState<ACRPlayerState>();
	if (IsValid(PlayerState) == false)
	{
		return;
	}

	// 플레이어 ID & 이름
	const int32 PlayerId = PlayerState->GetPlayerId();
	const FString& PlayerName = PlayerState->GetPlayerName();

	// ACRGameState 검증
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == true)
	{
		// 이전 채팅 내역 동기화
		const TArray<FChatEntry>& ChatLog = CRGS->GetChatLog();
		Player->ClientRPCSyncChatLog(ChatLog);

		// 코인 리더보드에 플레이어 추가
		CRGS->AddPlayerToCoinLeaderboard(PlayerId, PlayerName);
	}
}

AActor* ACRGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> AvailablePlayerStarts;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (IsPlayerStartAvailable(PlayerStart))
		{
			AvailablePlayerStarts.Add(PlayerStart);
		}
	}

	checkf(AvailablePlayerStarts.IsEmpty() == false, TEXT("There is no available Vehicle PlayerStart."));

	const int32 RandomIndex = FMath::RandHelper(AvailablePlayerStarts.Num());
	return Cast<AActor>(AvailablePlayerStarts[RandomIndex]);
}

bool ACRGameMode::IsPlayerStartAvailable(const APlayerStart* PlayerStart) const
{
	if (IsValid(PlayerStart) == false || IsValid(GetWorld()) == false)
	{
		return false;
	}

	const FTransform PlayerStartTransform = PlayerStart->GetActorTransform();
	const FVector CheckLocation = PlayerStartTransform.TransformPosition(VehicleSpawnCheckOffset);
	const FQuat CheckRotation = PlayerStartTransform.GetRotation();
	const FCollisionShape CheckShape = FCollisionShape::MakeBox(VehicleSpawnCheckHalfExtent);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);

	const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VehiclePlayerStartCheck), false);

	const bool bVehicleExists =
		GetWorld()->OverlapAnyTestByObjectType(
			CheckLocation,
			CheckRotation,
			ObjectQueryParams,
			CheckShape,
			QueryParams);

	return !bVehicleExists;
}

void ACRGameMode::BroadcastChatMessage(ACRPlayerController* SenderController, const FString& ChatMessage) const
{
	// 채팅 데이터 설정
	FChatEntry ChatData;

	ACRPlayerState* PlayerState = SenderController->GetPlayerState<ACRPlayerState>();
	if (IsValid(PlayerState) == true)
	{
		ChatData.SenderNameString = PlayerState->GetPlayerName();
	}
	ChatData.ChatMessageString = ChatMessage;

	// 등록된 플레이어 배열을 순회하여 채팅 전달
	for (ACRPlayerController* Controller : AllPlayerControllers)
	{
		if (IsValid(Controller) == true)
		{
			Controller->ClientRPCReciveChatMessage(ChatData);
		}
	}

	// 채팅 로그에 저장
	ACRGameState* CRGS = GetGameState<ACRGameState>();
	if (IsValid(CRGS) == true)
	{
		CRGS->AddChatData(ChatData);
	}
}
