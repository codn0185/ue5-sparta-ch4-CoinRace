#include "CRBonusMaxSpeedWidget.h"

#include "Components/TextBlock.h"

void UCRBonusMaxSpeedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetBonusMaxSpeed(0);

	TextBlock_SpeedUnit->SetText(FText::FromString(
		bIsMPH
			? TEXT("MPH")
			: TEXT("km/h")));
}

void UCRBonusMaxSpeedWidget::SetBonusMaxSpeed(float NewBonusMaxSpeed)
{
	// 보너스 없음 -> 숨기기
	if (NewBonusMaxSpeed <= 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// 보너스 적용 -> 표시하기
	SetVisibility(ESlateVisibility::Visible);

	// 단위에 맞춰 속도 포맷
	float FormattedSpeed = FMath::Abs(NewBonusMaxSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMaximumFractionalDigits(0);                // 소수점 표시 X

	TextBlock_SpeedValue->SetText(FText::AsNumber(FormattedSpeed, &Options));
}
