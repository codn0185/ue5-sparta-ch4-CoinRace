#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "CRPlayerState.generated.h"

// Coins 변경 시 실행되는 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCoinsChanged,
	int32);

// BonusMaxSpeed 변경 시 실행되는 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnBonusMaxSpeedChanged,
	float);

UCLASS()
class COINRACE_API ACRPlayerState : public APlayerState
{
	GENERATED_BODY()

  public:
	// Coins 변경 델리게이트
	FOnCoinsChanged OnCoinsChanged;
	// BonusMaxSpeed 변경 델리게이트
	FOnBonusMaxSpeedChanged OnBonusMaxSpeedChanged;

  protected:
	// Coins 변경 시 호출
	UFUNCTION()
	void OnRep_Coins();

	// BonusMaxSpeed 변경 시 호출
	UFUNCTION()
	void OnRep_BonusMaxSpeed();

  protected:
	// 코인 개수
	UPROPERTY(ReplicatedUsing = OnRep_Coins)
	int32 Coins = 0;

	// 보너스 속도
	UPROPERTY(ReplicatedUsing = OnRep_BonusMaxSpeed)
	float BonusMaxSpeed = 0.f;

  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  public:
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
