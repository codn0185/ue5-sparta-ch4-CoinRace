#include "CRVehicleStatusWidget.h"

#include "Components/TextBlock.h"

void UCRVehicleStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateSpeed(0);
	TextBlock_SpeedUnit->SetText(FText::FromString(UISpeedUnit.DisplayString));
}

void UCRVehicleStatusWidget::SetSpeedUnit(const FSpeedUnit& NewSpeedUnit)
{
	UISpeedUnit = NewSpeedUnit;

	// UI 업데이트
	TextBlock_SpeedUnit->SetText(FText::FromString(UISpeedUnit.DisplayString));
	UpdateSpeed(CurrentSpeed);
}

void UCRVehicleStatusWidget::UpdateSpeed(float NewSpeed)
{
	// 현재 속도 설정
	CurrentSpeed = NewSpeed;

	// 텍스트 변환 설정
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMinimumIntegralDigits(3);                  // 정수 3자리 표시
	Options.SetMaximumFractionalDigits(0);                // 소수점 표시 X

	TextBlock_SpeedValue->SetText(FText::AsNumber(
		UISpeedUnit.ConvertSpeed(FMath::Abs(CurrentSpeed)), // 단위에 맞춰 속도 값 변환
		&Options));
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
