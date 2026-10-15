#include "CRBonusMaxSpeedWidget.h"

#include "Components/TextBlock.h"

void UCRBonusMaxSpeedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TextBlock_SpeedValue->SetText(FText::AsNumber(0));

	TextBlock_SpeedUnit->SetText(FText::FromString(
		bIsMPH
			? TEXT("MPH")
			: TEXT("km/h")));
}

void UCRBonusMaxSpeedWidget::SetBonusMaxSpeed(float NewBonusMaxSpeed)
{
	// 단위에 맞춰 속도 포맷
	float FormattedSpeed = FMath::Abs(NewBonusMaxSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMinimumIntegralDigits(3);                  // 정수 3자리 표시
	Options.SetMaximumFractionalDigits(0);                // 소수점 표시 X

	TextBlock_SpeedValue->SetText(FText::AsNumber(FormattedSpeed, &Options));
}
