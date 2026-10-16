#pragma once

#include "CoreMinimal.h"

#include "Data/CoinLeaderboardEntry.h"
#include "Subsystems/WorldSubsystem.h"

#include "CoinLeaderboardWorldSubsystem.generated.h"

UCLASS()
class COINRACE_API UCoinLeaderboardWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

  protected:
	// 코인 리더보드 (내림차순 정렬)
	TArray<FCoinLeaderboardEntry> CoinLeaderboard;
	// 플레이어 ID -> 코인 리더보드 인덱스
	TMap<int32, int32> CoinLeaderboardIndexMap;

  public:
	// 서버에 코인 리더보드 초기화 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCClearCoinLeaderboard();
	// 서버에 코인 리더보드에 플레이어 추가 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCAddPlayerToCoinLeaderboard(const int32 PlayerId, const FString& PlayerName);
	// 서버에 코인 리더보드에 플레이어 추가 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCRemovePlayerFromCoinLeaderboard(const int32 PlayerId);
	// 서버에 코인 리더보드 업데이트 요청
	UFUNCTION(Server, Reliable)
	void ServerRPCUpdateCoinLeaderboard(const int32 PlayerId, const int32 NewCoins);

  public:
	// 서버로부터 코인 리더보드 초기화 요청받음
	UFUNCTION(Client, Reliable)
	void ClientRPCClearCoinLeaderboard();
	// 서버로부터 코인 리더보드에 플레이어 추가 요청받음
	UFUNCTION(Client, Reliable)
	void ClientRPCAddPlayerToCoinLeaderboard(const int32 PlayerId, const FString& PlayerName);
	// 서버로부터 코인 리더보드에 플레이어 추가 요청받음
	UFUNCTION(Client, Reliable)
	void ClientRPCRemovePlayerFromCoinLeaderboard(const int32 PlayerId);
	// 서버로부터 코인 리더보드 업데이트 요청받음
	UFUNCTION(Client, Reliable)
	void ClientRPCUpdateCoinLeaderboard(const int32 PlayerId, const int32 NewCoins);

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
