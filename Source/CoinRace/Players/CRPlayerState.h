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

	// Lifecycles
  protected:
	virtual void BeginPlay() override;

	// 레플리케이션 대상 등록
  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ==================================================
	// GameResult
  protected:
	// 게임 결과
	UPROPERTY(ReplicatedUsing = OnRep_GameResult)
	EGameResult GameResult = EGameResult::None;

  protected:
	UFUNCTION()
	void OnRep_GameResult();

  public:
	// GameResult 반환
	EGameResult GetGameResult() const;

	// GameResult 설정
	void SetGameResult(EGameResult NewGameResult);

	// ==================================================
	// 전진 가속 활성화
  protected:
	// 전진 가속 활성화 여부
	UPROPERTY(ReplicatedUsing = OnRep_CanThrottle)
	bool bCanThrottle = true;

  protected:
	UFUNCTION()
	void OnRep_CanThrottle();

  public:
	// bCanThrottle 반환
	bool CanThrottle() const;

	// bCanThrottle 설정
	void SetCanThrottle(bool bNewCanThrottle);

	// ==================================================
	// Coins
  protected:
	// 코인 개수
	UPROPERTY(ReplicatedUsing = OnRep_Coins)
	int32 Coins = 0;

  protected:
	UFUNCTION()
	void OnRep_Coins();

  public:
	// Coins 반환
	int32 GetCoins() const;

	// Coins 추가
	void AddCoins(int32 Amount);

	// Coins 차감 시도 - 성공 여부 반환
	bool TryRemoveCoins(int32 Amount);

	// ==================================================
	// BonusMaxSpeed
  protected:
	// 보너스 최대 속도
	UPROPERTY(ReplicatedUsing = OnRep_BonusMaxSpeed)
	float BonusMaxSpeed = 0.f;

	// 회복 타이머 핸들
	FTimerHandle RecoveryBonusMaxSpeedTimerHandle;

  protected:
	UFUNCTION()
	void OnRep_BonusMaxSpeed();

  public:
	// BonusMaxSpeed 반환
	float GetBonusMaxSpeed() const;

	// BonusMaxSpeed 증가
	void AddBonusMaxSpeed(float Amount);

	// BonusMaxSpeed 감소
	void ReduceBonusMaxSpeed(float Amount);
};
