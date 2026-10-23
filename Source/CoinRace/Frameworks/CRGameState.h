#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "CRGameState.generated.h"

struct FChatEntry;
struct FCoinLeaderboardEntry;

// 코인 리더보드 변경 알림 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(
	FOnCoinLeaderboardUpdated,
	const TArray<FCoinLeaderboardEntry>&);

UCLASS()
class COINRACE_API ACRGameState : public AGameStateBase
{
	GENERATED_BODY()

  protected:
	 virtual void BeginPlay() override;

	// 레플리케이션 대상 등록
  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ==================================================
	// 채팅 로그 관리
  protected:
	// 채팅 로그의 최대 보관 개수
	UPROPERTY(Replicated)
	int32 MaxChatLogCount = 20;

	// 채팅 로그
	TArray<FChatEntry> ChatLog;

  public:
	// 채팅 추가
	void AddChatData(const FChatEntry& NewChatData);

	// 채팅 로그 설정
	void SetChatLog(const TArray<FChatEntry>& InChatLog);

	// 채팅 로그 초기화
	void ClearChatLog();

	// 채팅 로그 반환
	const TArray<FChatEntry>& GetChatLog() const;

	// ==================================================
	// 코인 리더보드 관리
  protected:
	// 코인 리더보드 (내림차순 정렬)
	UPROPERTY(ReplicatedUsing = OnRep_CoinLeaderboard)
	TArray<FCoinLeaderboardEntry> CoinLeaderboard;
	// 플레이어 ID -> 코인 리더보드 인덱스
	TMap<int32, int32> CoinLeaderboardIndexMap;

  public:
	FOnCoinLeaderboardUpdated OnCoinLeaderboardUpdated;

  protected:
	UFUNCTION()
	void OnRep_CoinLeaderboard();

  public:
	// 코인 리더보드 반환
	const TArray<FCoinLeaderboardEntry>& GetCoinLeaderboard() const;
	// 코인 리더보드 초기화
	void ClearCoinLeaderboard();
	// 코인 리더보드에 플레이어 추가
	void AddPlayerToCoinLeaderboard(const int32 PlayerId, const FString& PlayerName);
	// 코인 리더보드에서 플레이어 제거
	void RemovePlayerFromCoinLeaderboard(const int32 PlayerId);
	// 코인 리더보드 업데이트
	void UpdateCoinLeaderboard(const int32 PlayerId, const int32 NewCoins);

  protected:
	// 리더보드 순위 갱신
	void RefreshCoinLeaderboardRanks();
};
