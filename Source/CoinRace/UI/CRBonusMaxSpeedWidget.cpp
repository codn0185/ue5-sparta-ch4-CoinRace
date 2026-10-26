#include "CRBonusMaxSpeedWidget.h"

#include "Components/TextBlock.h"
#include "Players/CRPlayerState.h"
#include "Systems/GameEventSubsystem.h"

void UCRBonusMaxSpeedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OnBonusMaxSpeedUpdated 델리게이트 바인딩
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			BonusMaxSpeedUpdatedHandle = GameEventSubsystem->OnBonusMaxSpeedUpdated.AddUObject(this, &ThisClass::HandleBonusMaxSpeedUpdated);
		}
	}

	// 초기 BonusMaxSpeed & SpeedUnit 설정
	const ACRPlayerState* CRPS = GetOwningPlayerState<ACRPlayerState>();
	if (IsValid(CRPS) == true)
	{
		SetBonusMaxSpeed(CRPS->GetBonusMaxSpeed());
	}
	else
	{
		SetBonusMaxSpeed(0);
	}

	SetSpeedUnit(UISpeedUnit);
}

void UCRBonusMaxSpeedWidget::NativeDestruct()
{
	// OnBonusMaxSpeedUpdated 델리게이트 바인딩 제거
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance) == true)
	{
		UGameEventSubsystem* GameEventSubsystem = GameInstance->GetSubsystem<UGameEventSubsystem>();
		if (IsValid(GameEventSubsystem) == true)
		{
			GameEventSubsystem->OnBonusMaxSpeedUpdated.Remove(BonusMaxSpeedUpdatedHandle);
			BonusMaxSpeedUpdatedHandle.Reset();
		}
	}

	Super::NativeDestruct();
}

void UCRBonusMaxSpeedWidget::SetSpeedUnit(const FSpeedUnit& NewSpeedUnit)
{
	UISpeedUnit = NewSpeedUnit;

	// UI 업데이트
	TextBlock_SpeedUnit->SetText(FText::FromString(UISpeedUnit.DisplayString));
	SetBonusMaxSpeed(CurrentBonusMaxSpeed);
}

void UCRBonusMaxSpeedWidget::SetBonusMaxSpeed(float NewBonusMaxSpeed)
{
	// 현재 보너스 최대 속도 설정
	CurrentBonusMaxSpeed = NewBonusMaxSpeed;

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMaximumFractionalDigits(2);                // 소수점 2자리 표시

	TextBlock_SpeedValue->SetText(FText::AsNumber(
		UISpeedUnit.ConvertSpeed(FMath::Abs(CurrentBonusMaxSpeed)), // 단위에 맞춰 속도 값 변환
		&Options));

	// 0 이하면 숨기기
	SetVisibility(
		CurrentBonusMaxSpeed > 0.f
			? ESlateVisibility::Visible
			: ESlateVisibility::Hidden);
}

void UCRBonusMaxSpeedWidget::HandleBonusMaxSpeedUpdated(const ACRPlayerState* PlayerState, float NewBonusMaxSpeed)
{
	// 로컬 플레이어에만 반영
	if (PlayerState != GetOwningPlayerState<ACRPlayerState>())
	{
		return;
	}

	SetBonusMaxSpeed(NewBonusMaxSpeed);
}
