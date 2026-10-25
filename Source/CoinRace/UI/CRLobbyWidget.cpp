#include "CRLobbyWidget.h"

#include "Components/TextBlock.h"
#include "Systems/GameEventSubsystem.h"

void UCRLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			// 남은 시간 변경 델리게이트 바인딩
			GameEventSubsystem->OnWaitingTimeRemainingUpdated.AddUObject(this, &ThisClass::UpdateWaitingTime);

			// 플레이어 수 변경 델리게이트 바인딩
			GameEventSubsystem->OnPlayerCountUpdated.AddUObject(this, &ThisClass::UpdatePlayerCount);
		}
	}
}

void UCRLobbyWidget::NativeDestruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			// 남은 시간 변경 델리게이트 바인딩 해제
			GameEventSubsystem->OnWaitingTimeRemainingUpdated.RemoveAll(this);

			// 플레이어 수 변경 델리게이트 바인딩 해제
			GameEventSubsystem->OnPlayerCountUpdated.RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void UCRLobbyWidget::UpdateWaitingTime(float WaitingTime)
{
	TextBlock_WaitingTime->SetText(FText::AsNumber(static_cast<int32>(WaitingTime)));
}

void UCRLobbyWidget::UpdatePlayerCount(int32 PlayerCount)
{
	TextBlock_PlayerCount->SetText(FText::AsNumber(PlayerCount));
}
