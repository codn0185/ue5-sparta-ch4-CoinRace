#pragma once

#include "CoreMinimal.h"

#include "Data/CoinLeaderboardEntry.h"
#include "GameFramework/GameStateBase.h"

#include "CRGameState.generated.h"

UCLASS()
class COINRACE_API ACRGameState : public AGameStateBase
{
	GENERATED_BODY()

  protected:
	// 코인 리더보드 (내림차순 정렬)
	TArray<FCoinLeaderboardEntry> CoinLeaderboard;
	// 플레이어 ID -> 코인 리더보드 인덱스
	TMap<int32, int32> CoinLeaderboardIndexMap;

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
};
