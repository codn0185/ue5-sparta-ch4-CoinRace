#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "CRPlayerState.generated.h"

UCLASS()
class COINRACE_API ACRPlayerState : public APlayerState
{
	GENERATED_BODY()

  protected:
	// 플레이어 이름
	UPROPERTY(Replicated)
	FString PlayerName;

  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  public:
	// 플레이어 이름 반환
	FString GetPlayerName() const;
	// 플레이어 이름 설정
	void SetPlayerName(const FString& InPlayerName);
};
