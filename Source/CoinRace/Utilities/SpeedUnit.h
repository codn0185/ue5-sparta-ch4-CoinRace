#pragma once

#include "CoreMinimal.h"

#include "SpeedUnit.generated.h"

USTRUCT()
struct FSpeedUnit
{
	GENERATED_BODY()

  public:
	// 기본 속도(cm/s) 변환 배율
	float ConversionMultiplier = 1.f;

	// UI 표시 문자열
	FString DisplayString;

  public:
	float ConvertSpeed(float Speed) const;
};

namespace SpeedUnit
{
// Kilometers Per Hour
inline const FSpeedUnit KPH{
	0.036f,
	FString("km/h")};

// Miles Per Hour
inline const FSpeedUnit MPH{
	0.02237f,
	FString("MPH")};

} // namespace SpeedUnit
