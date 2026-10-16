#include "CRCoinLeaderboardItem.h"

FCoinLeaderboardEntry UCRCoinLeaderboardItem::GetCoinLeaderboardData() const
{
	return CoinLeaderboardData;
}

void UCRCoinLeaderboardItem::SetCoinLeaderboardData(FCoinLeaderboardEntry InCoinLeaderboardData)
{
	CoinLeaderboardData = InCoinLeaderboardData;
}

bool UCRCoinLeaderboardItem::IsOwner() const
{
	return bIsOwner;
}

void UCRCoinLeaderboardItem::SetIsOwner(bool bInIsOwner)
{
	bIsOwner = bInIsOwner;
}
