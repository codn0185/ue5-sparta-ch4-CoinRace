#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "CRPlayerState.generated.h"

UENUM()
enum class EGameResult : uint8
{
	None, // 결과 미확정
	Win,  // 승리
	Lose, // 패배
};

UCLASS()
class COINRACE_API ACRPlayerState : public APlayerState
{
	GENERATED_BODY()

  private:
	// 게임 이벤트 서브시스템
	TObjectPtr<class UGameEventSubsystem> GameEventSubsystem;

  protected:
	virtual void BeginPlay() override;

  protected:
	// GameResult 변경 시 호출
	UFUNCTION()
	void OnRep_GameResult();

	// Coins 변경 시 호출
	UFUNCTION()
	void OnRep_Coins();

	// BonusMaxSpeed 변경 시 호출
	UFUNCTION()
	void OnRep_BonusMaxSpeed();

  protected:
	// 게임 결과
	UPROPERTY(ReplicatedUsing = OnRep_GameResult)
	EGameResult GameResult = EGameResult::None;

	// 코인 개수
	UPROPERTY(ReplicatedUsing = OnRep_Coins)
	int32 Coins = 0;

	// 보너스 속도
	UPROPERTY(ReplicatedUsing = OnRep_BonusMaxSpeed)
	float BonusMaxSpeed = 0.f;

  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

  public:
	// GameResult 반환
	EGameResult GetGameResult() const;
	// GameResult 설정
	void SetGameResult(EGameResult NewGameResult);

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
