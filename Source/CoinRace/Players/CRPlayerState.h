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

	// 코인 개수
	UPROPERTY(Replicated)
	int32 Coins = 0;

	// 보너스 속도
	UPROPERTY(Replicated)
	float BonusMaxSpeed = 0.f;

  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  public:
	// 플레이어 이름 반환
	FString GetPlayerName() const;
	// 플레이어 이름 설정
	void SetPlayerName(const FString& InPlayerName);

	// 코인 개수 반환
	int32 GetCoins() const;
	// 코인 추가
	void AddCoins(int32 Amount);
	// 코인 차감 시도 - 성공 여부 반환
	bool TryRemoveCoins(int32 Amount);

	// 보너스 속도 반환
	float GetBonusMaxSpeed() const;
	// 보너스 속도 추가
	void AddBonusMaxSpeed(float Amount);
};
