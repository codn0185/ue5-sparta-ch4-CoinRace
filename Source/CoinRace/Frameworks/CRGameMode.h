#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"

#include "CRGameMode.generated.h"

UCLASS()
class COINRACE_API ACRGameMode : public AGameModeBase
{
	GENERATED_BODY()

  public:
	ACRGameMode();

  protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

  private:
	bool IsPlayerStartAvailable(const APlayerStart* PlayerStart) const;

  private:
	UPROPERTY(EditDefaultsOnly)
	FVector VehicleSpawnCheckHalfExtent = FVector(250.f, 120.f, 80.f);

	UPROPERTY(EditDefaultsOnly)
	FVector VehicleSpawnCheckOffset = FVector(00.f, 00.f, 80.f);
};
