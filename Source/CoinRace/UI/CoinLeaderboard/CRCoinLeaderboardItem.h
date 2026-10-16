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
	FCoinLeaderboardEntry CoinLeaderboardData;

  public:
	FCoinLeaderboardEntry GetCoinLeaderboardData() const;

	void SetCoinLeaderboardData(FCoinLeaderboardEntry InCoinLeaderboardData);
};
