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

	/** 값 변경 함수를 서버에서 호출하여 모든 클라이언트에 반영되도록 함 */
  public:
	// 서버에 PlayerName 변경 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCSetPlayerName(const FString& InPlayerName);

	// 서버에 Coins 증가 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCAddCoins(int32 Amount);
	// 서버에 Coins 감소 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCRemoveCoins(int32 Amount);

	// 서버에 BonusMaxSpeed 증가 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCAddBonusMaxSpeed(float Amount);

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
