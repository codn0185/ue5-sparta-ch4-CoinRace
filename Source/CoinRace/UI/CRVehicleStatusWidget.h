#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

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
	// 속도 단위 설정 - "Km/h" or "MPH"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle)
	bool bIsMPH = false;

  protected:
	virtual void NativePreConstruct() override;

  public:
	// 속도 UI 업데이트
	void UpdateSpeed(float NewSpeed);

	// 기어 UI 업데이트
	void UpdateGear(int32 NewGear);
};
