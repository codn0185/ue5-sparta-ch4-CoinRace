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

	// 부호
	const FString Sign = CurrentBonusMaxSpeed >= 0.f ? TEXT("+") : TEXT("-");

	// 표시 값
	const float DisplaySpeed = UISpeedUnit.ConvertSpeed(FMath::Abs(CurrentBonusMaxSpeed));

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMaximumFractionalDigits(2);                // 소수점 2자리 표시

	// 텍스트 UI 설정
	TextBlock_SpeedValue->SetText(FText::Format(
		FText::FromString(TEXT("{0} {1}")),
		FText::FromString(Sign),
		FText::AsNumber(DisplaySpeed, &Options)));
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
