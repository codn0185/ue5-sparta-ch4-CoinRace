#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameStateBase.h"

#include "CRGameState.generated.h"

struct FChatEntry;
struct FCoinLeaderboardEntry;

UENUM()
enum class EGamePhase : uint8
{
	Waiting,  // 시작 전 대기
	Playing,  // 게임 플레이
	Finished, // 게임 종료
};

UCLASS()
class COINRACE_API ACRGameState : public AGameStateBase
{
	GENERATED_BODY()

  private:
	// 게임 이벤트 서브시스템
	TObjectPtr<class UGameEventSubsystem> GameEventSubsystem;

  protected:
	virtual void BeginPlay() override;

	// 레플리케이션 대상 등록
  protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ==================================================
	// 플레이어
  protected:
	// 입장한 플레이어 수
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 PlayerCount = 0;

  protected:
	UFUNCTION()
	void OnRep_PlayerCount();

  public:
	// PlayerCount 반환
	int32 GetPlayerCount() const;

	// PlayerCount 설정
	void SetPlayerCount(int32 NewPlayerCount);

	// PlayerCount 증가
	void AddPlayerCount(int32 Amount = 1);

	// PlayerCount 감소
	void RemovePlayerCount(int32 Amount = 1);

	// ==================================================
	// 게임 상태 머신
  protected:
	// 현재 GamePhase
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGamePhase)
	EGamePhase CurrentGamePhase = EGamePhase::Waiting;

  protected:
	UFUNCTION()
	void OnRep_CurrentGamePhase();

  public:
	// CurrentGamePhase 반환
	EGamePhase GetCurrentGamePhase() const;

	// 상태 변경
	void ChangeGamePhase(EGamePhase NewGamePhase);

	// ==================================================
	// 남은 대기 시간
  protected:
	// 남은 대기 시간
	UPROPERTY(ReplicatedUsing = OnRep_WaitingTimeRemaining)
	float WaitingTimeRemaining = 0.f;

  protected:
	UFUNCTION()
	void OnRep_WaitingTimeRemaining();

  public:
	// WaitingTimeRemaining 반환
	float GetWaitingTimeRemaining() const;

	// WaitingTimeRemaining 설정
	void SetWaitingTimeRemaining(float NewRemainingTime);

	// ==================================================
	// 남은 게임 시간
  protected:
	// 남은 게임 시간
	UPROPERTY(ReplicatedUsing = OnRep_PlayingTimeRemaining)
	float PlayingTimeRemaining = 0.f;

  protected:
	UFUNCTION()
	void OnRep_PlayingTimeRemaining();

  public:
	// PlayingTimeRemaining 반환
	float GetPlayingTimeRemaining() const;

	// PlayingTimeRemaining 설정
	void SetPlayingTimeRemaining(float NewRemainingTime);

	// ==================================================
	// 우승 플레이어
  protected:
	// 게임을 승리한 플레이어 ID 배열
	UPROPERTY(ReplicatedUsing = OnRep_WinnerPlayerIds)
	TArray<int32> WinnerPlayerIds;

  protected:
	UFUNCTION()
	void OnRep_WinnerPlayerIds();

  public:
	// WinnerPlayerIds 반환
	const TArray<int32>& GetWinnerPlayerIds() const;

	// WinnerPlayerIds 설정
	void SetWinnerPlayerIds(const TArray<int32>& NewWinnerPlayerIds);

	// WinnerPlayerIds 초기화
	void ClearWinnerPlayerIds();

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

	// 플레이어 순위 조회 (유효하지 않은 PlayerId면 -1 반환)
	int32 GetPlayerRank(int32 PlayerId);

  protected:
	// 리더보드 순위 갱신
	void RefreshCoinLeaderboardRanks();
};
