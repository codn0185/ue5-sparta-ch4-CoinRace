#include "CRCoinLeaderboardItem.h"

FCoinLeaderboardEntry UCRCoinLeaderboardItem::GetCoinLeaderboardData() const
{
	return CoinLeaderboardData;
}

void UCRCoinLeaderboardItem::SetCoinLeaderboardData(FCoinLeaderboardEntry InCoinLeaderboardData)
{
	CoinLeaderboardData = InCoinLeaderboardData;
}
