#include "UI/CRGameResultWidget.h"

#include "Components/PanelWidget.h"
#include "Players/CRPlayerState.h"
#include "Systems/GameEventSubsystem.h"

void UCRGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			// 게임 결과 변경 델리게이트 바인딩
			GameEventSubsystem->OnGameResultUpdated.AddUObject(this, &ThisClass::HandleGameResultUpdated);
		}
	}

	// 초기 결과 화면 설정
	const ACRPlayerState* CRPS = GetOwningPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == true)
	{
		HandleGameResultUpdated(CRPS, CRPS->GetGameResult());
	}
}

void UCRGameResultWidget::NativeDestruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			// 게임 결과 변경 델리게이트 바인딩 해제
			GameEventSubsystem->OnGameResultUpdated.RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void UCRGameResultWidget::HandleGameResultUpdated(const ACRPlayerState* PlayerState, EGameResult NewGameResult)
{
	// 로컬 플레이어에만 반영
	if (PlayerState != GetOwningPlayerState<ACRPlayerState>())
	{
		return;
	}

	switch (NewGameResult)
	{
	case EGameResult::None:
		PanelWidget_VictoryUI->SetVisibility(ESlateVisibility::Hidden);
		PanelWidget_DefeatUI->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EGameResult::Win:
		PanelWidget_VictoryUI->SetVisibility(ESlateVisibility::Visible);
		PanelWidget_DefeatUI->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EGameResult::Lose:
		PanelWidget_VictoryUI->SetVisibility(ESlateVisibility::Hidden);
		PanelWidget_DefeatUI->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}
