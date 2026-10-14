#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRHUD.generated.h"

class UTextBlock;

UCLASS()
class COINRACE_API UCRHUD : public UUserWidget
{
	GENERATED_BODY()

  protected:
	/** Binding Wigets */
	// 속도 값
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Speed;
	// 속도 단위
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Unit;
	// 기어
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Gear;

  protected:
	/** Controls the display of speed in Km/h or MPH */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle)
	bool bIsMPH = false;

  protected:
	virtual void NativePreConstruct() override;

  public:
	/** Called to update the speed display */
	void UpdateSpeed(float NewSpeed);

	/** Called to update the gear display */
	void UpdateGear(int32 NewGear);

  protected:
	/** Display the new speed */
	void OnSpeedUpdate(float NewSpeed);

	/** Display the new gear */
	void OnGearUpdate(int32 NewGear);
};
