#include "CRPlayingTimeWidget.h"

#include "Components/TextBlock.h"
#include "Frameworks/CRGameState.h"
#include "Systems/GameEventSubsystem.h"

void UCRPlayingTimeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OnPlayingTimeRemainingUpdated 델리게이트 바인딩
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnPlayingTimeRemainingUpdated.AddUObject(this, &ThisClass::HandlePlayingTimeRemainingUpdated);
		}
	}

	// 초기 PlayingTime 설정
	const ACRGameState* CRGS = GetWorld()->GetGameState<ACRGameState>();
	if (IsValid(CRGS) == true)
	{
		UpdatePlayingTime(CRGS->GetPlayingTimeRemaining());
	}
	else
	{
		UpdatePlayingTime(0.f);
	}
}

void UCRPlayingTimeWidget::NativeDestruct()
{
	// OnPlayingTimeRemainingUpdated 델리게이트 바인딩 해제
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnPlayingTimeRemainingUpdated.RemoveAll(this);
		}
	}

	Super::NativeDestruct();
}

void UCRPlayingTimeWidget::UpdatePlayingTime(float PlayingTime)
{
	TextBlock_PlayingTime->SetText(FText::AsNumber(static_cast<int32>(PlayingTime)));
}

void UCRPlayingTimeWidget::HandlePlayingTimeRemainingUpdated(float NewRemainingTime)
{
	UpdatePlayingTime(NewRemainingTime);
}
