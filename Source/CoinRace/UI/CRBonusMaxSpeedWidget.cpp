#include "CRBonusMaxSpeedWidget.h"

#include "Components/TextBlock.h"

void UCRBonusMaxSpeedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetBonusMaxSpeed(0);
	TextBlock_SpeedUnit->SetText(FText::FromString(UISpeedUnit.DisplayString));
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

	// 보너스 없음 -> 숨기기
	if (CurrentBonusMaxSpeed <= 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// 현재 보너스 최대 속도 설정
	CurrentBonusMaxSpeed = NewBonusMaxSpeed;

	// 보너스 적용 -> 표시하기
	SetVisibility(ESlateVisibility::Visible);

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMaximumFractionalDigits(0);                // 소수점 표시 X

	TextBlock_SpeedValue->SetText(FText::AsNumber(
		UISpeedUnit.ConvertSpeed(FMath::Abs(CurrentBonusMaxSpeed)), // 단위에 맞춰 속도 값 변환
		&Options));
}
