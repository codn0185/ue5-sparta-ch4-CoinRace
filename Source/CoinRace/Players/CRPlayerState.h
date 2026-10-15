#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "CRPlayerState.generated.h"

UCLASS()
class COINRACE_API ACRPlayerState : public APlayerState
{
	GENERATED_BODY()

  protected:
	UPROPERTY(Replicated)
	FString PlayerName;

  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  public:
	FString GetPlayerName() const;
	void SetPlayerName(const FString& InPlayerName);
};
