#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "CRHUD.generated.h"

class UCRVehicleStatusWidget;

UCLASS()
class COINRACE_API UCRHUD : public UUserWidget
{
	GENERATED_BODY()

  protected:
	// UCRVehicleStatusWidget - 차량 정보 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCRVehicleStatusWidget> VehicleStatusWidget;

  public:
	// UCRVehicleStatusWidget의 속도 UI 변경
	void UpdateSpeed(float NewSpeed);

	// UCRVehicleStatusWidget의 기어 UI 변경
	void UpdateGear(int32 NewGear);
};
