#include "CRHUD.h"

#include "Components/TextBlock.h"

void UCRHUD::NativePreConstruct()
{
	Super::NativePreConstruct();

	const FText SpeedUnitText = FText::FromString(
		bIsMPH
			? TEXT("MPH")
			: TEXT("km/h"));

	TextBlock_Unit->SetText(SpeedUnitText);
}

void UCRHUD::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCRHUD::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UCRHUD::OnSpeedUpdate(float NewSpeed)
{
	FNumberFormattingOptions Options;
	Options.SetRoundingMode(ERoundingMode::HalfFromZero); // 반올림
	Options.SetMinimumIntegralDigits(3);                  // 정수 3자리 표시
	Options.SetMaximumFractionalDigits(0);                // 소수점 표시 X

	TextBlock_Speed->SetText(FText::AsNumber(NewSpeed, &Options));
}

void UCRHUD::OnGearUpdate(int32 NewGear)
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
