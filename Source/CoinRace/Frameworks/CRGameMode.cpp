#include "CRGameMode.h"

#include "EngineUtils.h"

#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Players/CRPlayerController.h"
#include "Systems/ChatWorldSubsystem.h"

ACRGameMode::ACRGameMode()
{
	PlayerControllerClass = ACRPlayerController::StaticClass();
}

void ACRGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	// PlayerController 캐스팅
	ACRPlayerController* PlayerController = Cast<ACRPlayerController>(NewPlayer);
	if (IsValid(PlayerController) == true)
	{
		// 플레이어 추가
		AllPlayerControllers.Add(PlayerController);
	}
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
	// 등록된 플레이어 배열을 순회하여 채팅 전달
	for (ACRPlayerController* Controller : AllPlayerControllers)
	{
		if (IsValid(Controller) == false)
		{
			continue;
		}

		// 채팅 데이터 설정
		FChatData ChatData;

		ChatData.SenderNameString = GetNameSafe(SenderController);

		ChatData.ChatMessageString = ChatMessage;

		// 채팅 데이터 전달
		Controller->ClientRPCReciveChatMessge(ChatData);
	}
}
