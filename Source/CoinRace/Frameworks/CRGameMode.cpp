#include "CRGameMode.h"

#include "EngineUtils.h"

#include "Engine/World.h"
#include "Frameworks/CRGameState.h"
#include "GameFramework/PlayerStart.h"
#include "Players/CRPlayerController.h"
#include "Players/CRPlayerState.h"
#include "Systems/ChatWorldSubsystem.h"

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
	}

	Super::Logout(Exiting);
}

void ACRGameMode::OnPlayerInitialized(ACRPlayerController* Player)
{
	if (IsValid(Player) == false)
	{
		return;
	}

	// 이전 채팅 내역 동기화
	UChatWorldSubsystem* ChatWorldSubsystem = GetWorld()->GetSubsystem<UChatWorldSubsystem>();
	if (IsValid(ChatWorldSubsystem) == true)
	{
		const TArray<FChatData>& ChatLog = ChatWorldSubsystem->GetChatLog();
		Player->ClientRPCSyncChatLog(ChatLog);
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

void ACRGameMode::BroadcastChatMessage(ACRPlayerController* SenderController, const FString& ChatMessage)
{
	// 채팅 데이터 설정
	FChatData ChatData;

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
	UChatWorldSubsystem* ChatWorldSubsystem = GetWorld()->GetSubsystem<UChatWorldSubsystem>();
	if (IsValid(ChatWorldSubsystem) == true)
	{
		ChatWorldSubsystem->AddChatData(ChatData);
	}
}
