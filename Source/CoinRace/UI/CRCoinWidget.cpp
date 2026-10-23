#include "UI/CRCoinWidget.h"

#include "Components/TextBlock.h"
#include "Players/CRPlayerState.h"
#include "Systems/GameEventSubsystem.h"

void UCRCoinWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OnCoinsUpdated 델리게이트 바인딩
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			CoinsUpdatedHandle = GameEventSubsystem->OnCoinsUpdated.AddUObject(this, &ThisClass::HandleCoinsUpdated);
		}
	}

	// 초기 Coins 설정
	const ACRPlayerState* CRPS = GetOwningPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == true)
	{
		SetCoins(CRPS->GetCoins());
	}
	else
	{
		SetCoins(0);
	}
}

void UCRCoinWidget::NativeDestruct()
{
	// OnCoinsUpdated 델리게이트 바인딩 제거
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnCoinsUpdated.Remove(CoinsUpdatedHandle);
			CoinsUpdatedHandle.Reset();
		}
	}

	Super::NativeDestruct();
}

void UCRCoinWidget::SetCoins(int32 Amount)
{
	TextBlock_Coins->SetText(FText::AsNumber(Amount));
}

void UCRCoinWidget::HandleCoinsUpdated(const ACRPlayerState* PlayerState, int32 NewCoins)
{
	SetCoins(NewCoins);
}
