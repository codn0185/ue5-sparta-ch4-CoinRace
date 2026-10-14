#include "CRVehicleStatusWidget.h"

#include "Components/TextBlock.h"

void UCRVehicleStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TextBlock_SpeedUnit->SetText(FText::FromString(
		bIsMPH
			? TEXT("MPH")
			: TEXT("km/h")));
}

void UCRVehicleStatusWidget::UpdateSpeed(float NewSpeed)
{
	// 단위에 맞춰 속도 포맷
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMinimumIntegralDigits(3);                  // 정수 3자리 표시
	Options.SetMaximumFractionalDigits(0);                // 소수점 표시 X

	TextBlock_SpeedValue->SetText(FText::AsNumber(FormattedSpeed, &Options));
}

void UCRVehicleStatusWidget::UpdateGear(int32 NewGear)
{
	switch (NewGear)
	{
	// 후진 기어
	case -1:
		TextBlock_Gear->SetText(FText::FromString("R"));
		break;
	// 중립 기어
	case 0:
		TextBlock_Gear->SetText(FText::FromString("N"));
		break;
	// 기어 단계
	default:
		TextBlock_Gear->SetText(FText::AsNumber(NewGear));
		break;
	}
}
