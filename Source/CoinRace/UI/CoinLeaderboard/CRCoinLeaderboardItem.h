#pragma once

#include "CoreMinimal.h"

#include "Data/CoinLeaderboardEntry.h"
#include "UObject/NoExportTypes.h"

#include "CRCoinLeaderboardItem.generated.h"

UCLASS()
class COINRACE_API UCRCoinLeaderboardItem : public UObject
{
	GENERATED_BODY()

  protected:
	// 코인 리더보드 항목
	FCoinLeaderboardEntry CoinLeaderboardData;

	// Owner 여부 (하이라이트 여부 결정)
	bool bIsOwner = false;

  public:
	FCoinLeaderboardEntry GetCoinLeaderboardData() const;

	void SetCoinLeaderboardData(FCoinLeaderboardEntry InCoinLeaderboardData);

	bool IsOwner() const;

	void SetIsOwner(bool bInIsOwner = true);
};
