#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Utilities/SpeedUnit.h"

#include "CRVehicleStatusWidget.generated.h"

class UTextBlock;

UCLASS()
class COINRACE_API UCRVehicleStatusWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// 속도 값
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SpeedValue;

	// 속도 단위
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SpeedUnit;

	// 기어
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Gear;

  protected:
	// UI에 표시할 속도 단위
	FSpeedUnit UISpeedUnit = SpeedUnit::KPH;
	// 현재 원본(cm/s) 속도
	float CurrentSpeed = 0.f;

  protected:
	virtual void NativePreConstruct() override;

  public:
	// 속도 단위 설정
	void SetSpeedUnit(const FSpeedUnit& NewSpeedUnit);

	// 속도 UI 업데이트
	void UpdateSpeed(float NewSpeed);

	// 기어 UI 업데이트
	void UpdateGear(int32 NewGear);
};
