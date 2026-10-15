#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRBonusMaxSpeedWidget.generated.h"

class UTextBlock;

UCLASS()
class COINRACE_API UCRBonusMaxSpeedWidget : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// 속도 값 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SpeedValue;
	// 속도 단위 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SpeedUnit;

	// 속도 단위 설정 - "Km/h" or "MPH"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle)
	bool bIsMPH = false;

  protected:
	virtual void NativeConstruct() override;

  public:
	// 속도 값 설정
	void SetBonusMaxSpeed(float NewBonusMaxSpeed);
};
