#pragma once

#include "CoreMinimal.h"

#include "CoinLeaderboardEntry.generated.h"

// 코인 리더보드 항목
USTRUCT()
struct FCoinLeaderboardEntry
{
	GENERATED_BODY()

  public:
	// 순위
	UPROPERTY()
	int32 Rank = 0;

	// 플레이어 ID
	UPROPERTY()
	int32 PlayerId = INDEX_NONE;

	// 플레이어 이름
	UPROPERTY()
	FString PlayerName;

	// 코인 수
	UPROPERTY()
	int32 Coins = 0;
};
