#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Utilities/SpeedUnit.h"

#include "CRBonusMaxSpeedWidget.generated.h"

class UTextBlock;
class ACRPlayerState;

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

  protected:
	// UI에 표시할 속도 단위
	FSpeedUnit UISpeedUnit = SpeedUnit::KPH;
	// 현재 원본(cm/s) 보너스 최대 속도
	float CurrentBonusMaxSpeed = 0.f;

  protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

  public:
	// 속도 단위 설정
	void SetSpeedUnit(const FSpeedUnit& NewSpeedUnit);

	// 속도 값 설정
	void SetBonusMaxSpeed(float NewBonusMaxSpeed);

	/** OnBonusMaxSpeedUpdated 델리게이트 관리 */
  protected:
	FDelegateHandle BonusMaxSpeedUpdatedHandle;

	UFUNCTION()
	void HandleBonusMaxSpeedUpdated(const ACRPlayerState* PlayerState, float NewBonusMaxSpeed);
};
